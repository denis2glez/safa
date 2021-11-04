#include <GUI/Viewer.h>
#include "ui_Viewer.h"
#include <vtkInteractorStyleSwitch.h>

Viewer::Viewer(QWidget *parent, Orientation orient) {
	setupUi(this);
	setParent(parent);
	orientation = orient;
	viewer = vtkImageViewer2::New();
	if(orientation == SAGITAL){
		viewer->SetSliceOrientationToXY();
		label1->setText("Sagital");
	}
	else if(orientation == AXIAL){
		viewer->SetSliceOrientationToYZ();
		label1->setText("Axial");
	}
	else if(orientation == CORONAL){
		viewer->SetSliceOrientationToXZ();
		label1->setText("Coronal");
	}
	else
		std::cerr << "ERROR: Invalid orientation" << std::endl;

	qvtkWidget->GetInteractor()->SetInteractorStyle(vtkInteractorStyleSwitch::New());

	SetRenderWindow(qvtkWidget->GetRenderWindow());
	cornerAnnotation = vtkCornerAnnotation::New();
	legendScaleActor = vtkLegendScaleActor::New();
}

void Viewer::SetRenderWindow(vtkRenderWindow *rendererWin){
	viewer->SetRenderWindow(rendererWin);
}

void Viewer::Render(){
	viewer->Render();
}

Viewer::~Viewer() {
	// TODO Auto-generated destructor stub
	viewer->Delete();
	cornerAnnotation->Delete();
	legendScaleActor->Delete();
}

void Viewer::SetImage(vtkImageData *vtkImage) {
	viewer->SetInput(vtkImage);
	InitPopMenu();
	InitSlices();
	SetNumberSlices();
	InitSlider();
	SetCornerAnnotations();
	SetLeyendScaler();
	EstablishConnections();
	if(orientation == SAGITAL)
		FlipImage();
	Render();
}

void Viewer::SetNumberSlices(){
	int max, min;
	max = viewer->GetSliceMax();
	min = viewer->GetSliceMin();
	numSlices = max - min + 1;
}

void Viewer::InitSlider(){
	slider->setMinimum(1);
	slider->setMaximum(numSlices);
	slider->setValue(1);
	label3->setText("1");
}

void Viewer::SetCornerAnnotations() {
	std::ostringstream sstr;
	sstr << "Number of slices: " << GetNumberSlices();

	cornerAnnotation->SetLinearFontScaleFactor(2);
	cornerAnnotation->SetNonlinearFontScaleFactor(1);
	cornerAnnotation->SetMaximumFontSize(12);
	//	cornerAnnotationXY->SetText( 0, "lower left" );
	//	cornerAnnotationXY->SetText( 1, "lower right" );
	if(orientation == SAGITAL)
		cornerAnnotation->SetText(2, "XY");
	else if(orientation == AXIAL)
		cornerAnnotation->SetText(2, "ZX");
	else
		cornerAnnotation->SetText(2, "YZ");

	cornerAnnotation->SetText(3, sstr.str().c_str());
	cornerAnnotation->GetTextProperty()->SetColor(1, 1, 1);

	viewer->GetRenderer()->AddViewProp(cornerAnnotation);
}

void Viewer::SetLeyendScaler() {
	viewer->GetRenderer()->AddActor(legendScaleActor);
}

void Viewer::InitSlices(){
	viewer->SetSlice(1);
}

void Viewer::EnableCornerAnnotations(bool b){
	cornerAnnotation->SetVisibility(b);
	Render();
}

void Viewer::EnableLeyendScalers(bool b){
	legendScaleActor->SetVisibility(b);
	Render();
}

void Viewer::TakeSnapshot(){
	vtkWindowToImageFilter *filter = vtkWindowToImageFilter::New();
	filter->SetInput(viewer->GetRenderWindow());

	vtkPNGWriter *png = vtkPNGWriter::New();
	png->SetInput(filter->GetOutput());

	filter->Modified();
	png->SetFileName("snapshot.png");
	png->Write();

	filter->Delete();
	png->Delete();
}

int Viewer::GetNumberSlices(){
	return numSlices;
}

void Viewer::InitPopMenu(){
	popupMenu = new QMenu(qvtkWidget);
	QAction *action;
	action = popupMenu->addAction("Cornner annotation");
	action->setCheckable(true);
	action->setChecked(true);
	action = popupMenu->addAction("Leyend scaler");
	action->setCheckable(true);
	action->setChecked(true);
	popupMenu->addSeparator();
	popupMenu->addAction("Image restore");
	popupMenu->addAction("Flip image");
	popupMenu->addAction("Take snapshot");
}

void Viewer::EstablishConnections(){
	/**
	 * Conexiones originales de Qt
	 */
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateSlice(int)));
	connect(popupMenu, SIGNAL(triggered(QAction*)), this, SLOT(checkMenuAction(QAction*)));
	/**
	 * Conexiones entre Qt y VTK
	 */
	connections = vtkEventQtSlotConnect::New();
	connections->Connect(
			qvtkWidget->GetRenderWindow()->GetInteractor(),
			vtkCommand::RightButtonPressEvent,
			this,
			SLOT(popup( vtkObject*, unsigned long, void*, void*, vtkCommand*)),
			popupMenu, 1.0);
	connections->Connect(qvtkWidget->GetRenderWindow()->GetInteractor(),
			vtkCommand::MouseMoveEvent, this, SLOT(updateCoord(vtkObject*)));
}

void Viewer::popup(vtkObject * obj, unsigned long, void * client_data, void *, vtkCommand * command) {
	// A note about context menus in Qt and the QVTKWidget
	// You may find it easy to just do context menus on right button up,
	// due to the event proxy mechanism in place.

	// That usually works, except in some cases.
	// One case is where you capture context menu events that
	// child windows don't process.  You could end up with a second
	// context menu after the first one.

	// See QVTKWidget::ContextMenuEvent enum which was added after the
	// writing of this example.

	// get interactor
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(
			obj);
	// consume event so the interactor style doesn't get it
	command->AbortFlagOn();
	// get popup menu
	QMenu* popupMenu = static_cast<QMenu*> (client_data);
	// get event location
	int* sz = iren->GetSize();
	int* position = iren->GetEventPosition();
	// remember to flip y
	QPoint pt = QPoint(position[0], sz[1] - position[1]);
	// map to global
	QPoint global_pt = popupMenu->parentWidget()->mapToGlobal(pt);
	// show popup menu at global point
	popupMenu->popup(global_pt);
}

void Viewer::updateCoord(vtkObject* obj) {
	// get interactor
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(
			obj);
	// get event position
	int event_pos[2];
	iren->GetEventPosition(event_pos);
	// update label
	QString str;
	str.sprintf("(%d, %d)", event_pos[0], event_pos[1]);
	label2->setText(str);
}

void Viewer::checkMenuAction(QAction* menuAction) {
	QString text = menuAction->text();
	bool checked = menuAction->isChecked();
	menuAction->setChecked(checked);
	if (text == "Cornner annotation") {
		EnableCornerAnnotations(checked);
	} else if (text == "Leyend scaler") {
		EnableLeyendScalers(checked);
	} else if (text == "Image restore" && orientation == SAGITAL) {
		ImageRestore();
	} else if (text == "Flip image" && orientation == SAGITAL) {
		FlipImage();
	} else if (text == "Take snapshot") {
		TakeSnapshot();
	}
}

void Viewer::updateSlice(int value) {
	QString str;
	str.sprintf("   %d", value);
	viewer->SetSlice(value);
	label3->setText(str);
	//Por si el metodo es invocado por un visor externo
	slider->setValue(value);
}

void Viewer::SynchronizeWith(Viewer *otherViewer, bool sync){
	if(sync){
		connect(slider, SIGNAL(valueChanged(int)), otherViewer, SLOT(updateSlice(int)));
		connect(otherViewer->slider, SIGNAL(valueChanged(int)), this, SLOT(updateSlice(int)));
	}
	else{
		disconnect(slider, SIGNAL(valueChanged(int)), otherViewer, SLOT(updateSlice(int)));
		disconnect(otherViewer->slider, SIGNAL(valueChanged(int)), this, SLOT(updateSlice(int)));
	}
}

void Viewer::FlipImage(){
	vtkRenderer *renderer = viewer->GetRenderer();
	vtkCamera *camera = renderer->GetActiveCamera();
	camera->SetFocalPoint(0, 0, 1);
	camera->SetPosition(0, 0, 0);
	camera->SetViewUp(0, -1, 0);
	renderer->ResetCamera();
	viewer->Render();
}

void Viewer::ImageRestore(){
	vtkRenderer *renderer = viewer->GetRenderer();
	vtkCamera *camera = renderer->GetActiveCamera();
	camera->SetFocalPoint(0, 0, 1);
	camera->SetPosition(0, 0, 0);
	camera->SetViewUp(0, -1, 0);
	renderer->ResetCamera();
	viewer->Render();
}
