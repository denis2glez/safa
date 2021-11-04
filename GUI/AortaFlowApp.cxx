#include "GUI/AortaFlowApp.h"
#include "itkImage.h"
#include <itkSmartPointer.h>
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "vtkImageData.h"
#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkPolyDataMapper.h"
#include "vtkInteractorStyle.h"

#include "vtkImageReader2.h"
#include "vtkMatrix4x4.h"
#include "vtkImageReslice.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToColors.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkCommand.h"
#include "vtkImageData.h"
#include "vtkImageFlip.h"

#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <vtkSmartPointer.h>
#include <vtkAbstractMapper.h>

#include <vtkCursor2D.h>
#include <vtkProperty.h>
#include <vtkVolume.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeRayCastCompositeFunction.h>

#include "Common/ImageCursor.h"
#include "Segmentation/itkVelocityEncodingImageFilter.h"
#include "ui_AortaFlowApp.h"

#include <qspinbox.h>
#include <qdir.h>

AortaFlowApp::AortaFlowApp() {
	setupUi(this);

	SetDocks(false);

	progressMessage = new QLabel();
	statusBar()->addPermanentWidget(progressMessage);
	statusBar()->addPermanentWidget(progressBar);
	progressBar->setVisible(false);

	dockWidget_Properties_window->setEnabled(false);
	timeSteps = 2;
	std::stringstream ss;
	ss << timeSteps;
	labelTimeStepTotal->setText(ss.str().c_str());

	spinBox->setMinimum(1);
	spinBox->setMaximum(timeSteps);
	spinBox_2->setMinimum(1);
	spinBox_2->setMaximum(3);

	connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(updateTimeStepView(int)));
	connect(spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(updateComponentView(int)));
	connect(spinBox_3, SIGNAL(valueChanged(int)), this, SLOT(updateProperties(int)));
	connect(spinBox_4, SIGNAL(valueChanged(int)), this, SLOT(updateProperties(int)));
	connect(spinBox_5, SIGNAL(valueChanged(int)), this, SLOT(updateProperties(int)));

	syncSupInf = false;

	SetSupFrameVisible(false);
	SetInfFrameVisible(false);

	InitViewers();

	EstablishConnections();

	form = new Form3D();



	std::cout << "Terminando...\n";
}

void AortaFlowApp::InitViewers(){
	viewerSyncSup = new ViewerSynchronize(this);
	gridLayout_8->addWidget(viewerSyncSup->viewerXY, 0, 0);
	gridLayout_8->addWidget(viewerSyncSup->viewerZX, 0, 1);
	gridLayout_8->addWidget(viewerSyncSup->viewerYZ, 0, 2);

	viewerSyncInf = new ViewerSynchronize(this);
	gridLayout_12->addWidget(viewerSyncInf->viewerXY, 0, 0);
	gridLayout_12->addWidget(viewerSyncInf->viewerZX, 0, 1);
	gridLayout_12->addWidget(viewerSyncInf->viewerYZ, 0, 2);
}

void AortaFlowApp::EstablishConnections() {

	connect(actionLoad_DICOM_series, SIGNAL(triggered()), this,
			SLOT(LoadDICOMSeriesDialog()));

	connect(actionSyncronize_both_frames, SIGNAL(toggled(bool)), this,
			SLOT(syncronizeSupInf(bool)));
	connect(action3D_View, SIGNAL(triggered()), this, SLOT(show3DView()));
	connect(actionClose_current_serie, SIGNAL(triggered()), this,
			SLOT(closeCurrentSerie()));
	connect(actionSegment, SIGNAL(triggered()), this, SLOT(showSegment()));

	connect(actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}
/**
 * d:/Data/Michael_4D_MRI/7DflowMRI_FreiburgtoBarcelona/Mag1
 */
void AortaFlowApp::LoadDICOMScalar4DImage(std::string dirName) {
	if(form == NULL)
		new AortaFlowApp();

	std::string magDir = dirName + "/mag";

	DICOMScalar4DImageLoader::Pointer loader = DICOMScalar4DImageLoader::New();
	loader->SetListener(progressMessage, progressBar);

	loader->SetDirectory(magDir);
	try {
		loader->Update();
	} catch (ExceptionObject &ex) {
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << ex << std::endl;
		return;
	}
	magImage = loader->GetOutput();
	magImage->DisconnectPipeline();

	//////////////////////////////////////////////////////////////////////////////////////////////
	InitMagImages();
	//////////////////////////////////////////////////////////////////////////////////////////////

	LoadDICOMVectorial4DImage(dirName);
}

void AortaFlowApp::LoadDICOMVectorial4DImage(std::string dirName) {
	std::string flowDir = dirName + "/flow";
	DICOMVectorial4DImageLoader::Pointer loader =
			DICOMVectorial4DImageLoader::New();

	loader->SetListener(progressMessage, progressBar);
	loader->SetDirectory(flowDir);
	try {
		loader->Update();
	} catch (ExceptionObject &ex) {
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << ex << std::endl;
		return;
	}
	flowImage = loader->GetOutput();
	//flowImage->DisconnectPipeline();

	//////////////////////////////////////////////////////////////////////////////////////////////
	InitFlowImages();
	//////////////////////////////////////////////////////////////////////////////////////////////


	SetSupFrameVisible(true);
	SetInfFrameVisible(true);

	dockWidget_Properties_window->setEnabled(true);

	//Clean up
	//loader1->Delete();
}

AortaFlowApp::~AortaFlowApp() {
	std::cout << "Entrando en ~AortaFlowApp...\n";
	CleanImageSequences();

	if (viewerSyncSup != NULL)
		viewerSyncSup->Delete();
	if (viewerSyncInf != NULL)
		viewerSyncInf->Delete();
	if(form != NULL)
		delete form;
}

void AortaFlowApp::InitMagImages() {

	pipeline = AortaSegmentationPipeline();
	pipeline.timeStep = timeSteps;
	pipeline.progressMessage = progressMessage;
	pipeline.progressBar = progressBar;
	pipeline.magImage = magImage;

	progressMessage->setText("Extracting magnitude images...");
	progressBar->setValue(0);
	progressBar->setVisible(true);
	for (int i = 0; i < timeSteps; ++i) {
		Scalar3DImageType::Pointer img3D =
				ExtractScalar3DImageFromScalar4DImage(magImage, i);

		pipeline.magImages.push_back(img3D);

		vtkKWImage * kwMagImage = vtkKWImage::New();
		kwMagImage->SetITKImageBase(img3D);
		magImages.push_back(kwMagImage);
		progressBar->setValue((i + 1) * 100 / timeSteps);
		std::cout << "Current time step:  " << i << endl;
	}
	progressBar->setVisible(false);
	progressMessage->setText("Ready");

	viewerSyncSup->SetImage(pipeline._BuildTransferFunction(magImages[0]->GetVTKImage()), 0);
}

void AortaFlowApp::InitFlowImages() {
	typedef itk::VectorIndexSelectionCastImageFilter<Vector4DImageType,
			Scalar4DImageType> VectorIndexSelectionFilterType;
	// Create Extract Component Filter
	VectorIndexSelectionFilterType::Pointer componentExtractor = VectorIndexSelectionFilterType::New();
	componentExtractor->SetInput(flowImage);
	componentExtractor->SetIndex(0);
	componentExtractor->Update();
	Scalar4DImageType::Pointer flowImage_0 = componentExtractor->GetOutput();

	componentExtractor->SetInput(flowImage);
	componentExtractor->SetIndex(1);
	componentExtractor->Update();
	Scalar4DImageType::Pointer flowImage_1 = componentExtractor->GetOutput();

	componentExtractor->SetInput(flowImage);
	componentExtractor->SetIndex(2);
	componentExtractor->Update();
	Scalar4DImageType::Pointer flowImage_2 = componentExtractor->GetOutput();

	progressMessage->setText("Extracting flow images...");
	progressBar->setValue(0);
	progressBar->setVisible(true);

	pipeline.flowImages = std::vector < std::vector< Scalar3DImageType::Pointer > >(timeSteps);
	pipeline.flowImage = flowImage;
	flowImages = std::vector < std::vector< vtkKWImage * > >(timeSteps);

	for (int i = 0; i < timeSteps; ++i) {
		Scalar3DImageType::Pointer img3D_0 = ExtractScalar3DImageFromScalar4DImage(flowImage_0, i);
		Scalar3DImageType::Pointer img3D_1 = ExtractScalar3DImageFromScalar4DImage(flowImage_1, i);
		Scalar3DImageType::Pointer img3D_2 = ExtractScalar3DImageFromScalar4DImage(flowImage_2, i);

		pipeline.flowImages[i].push_back(img3D_0);
		pipeline.flowImages[i].push_back(img3D_1);
		pipeline.flowImages[i].push_back(img3D_2);

		vtkKWImage * kwFlowImage_0 = vtkKWImage::New();
		vtkKWImage * kwFlowImage_1 = vtkKWImage::New();
		vtkKWImage * kwFlowImage_2 = vtkKWImage::New();

		kwFlowImage_0->SetITKImageBase(img3D_0);
		kwFlowImage_1->SetITKImageBase(img3D_1);
		kwFlowImage_2->SetITKImageBase(img3D_2);

		flowImages[i].push_back(kwFlowImage_0);
		flowImages[i].push_back(kwFlowImage_1);
		flowImages[i].push_back(kwFlowImage_2);

		progressBar->setValue((i + 1) * 100 / timeSteps);
		std::cout << "Current time step:  " << i << endl;
	}
	progressBar->setVisible(false);
	progressMessage->setText("Ready");

	SetDocks(true);
	InitProperties();

	viewerSyncInf->SetImage(pipeline._BuildTransferFunction(flowImages[0][0]->GetVTKImage()), 1);
	/////////////////////////////////////////////////////////////////////////////////////
//	vtkKWImage::ImageBaseType * ItkImage = magImages[0]->GetITKImageBase();
//	Scalar3DImageType *itkImg = dynamic_cast< itk::Image< float, 3> * > ( ItkImage );
//
//	if(temp == itkImg)
//		std::cout << "Son realmente iguales?" << std::endl;
//	/////////////////////////////////////////////////////////////////////////////////////
//
//
}

void AortaFlowApp::syncronizeSupInf(bool sync) {
	viewerSyncSup->viewerXY->SynchronizeWith(viewerSyncInf->viewerXY, sync);
	viewerSyncSup->viewerYZ->SynchronizeWith(viewerSyncInf->viewerYZ, sync);
	viewerSyncSup->viewerZX->SynchronizeWith(viewerSyncInf->viewerZX, sync);
}

void AortaFlowApp::FlipImage(vtkRenderer *renderer) {
	/**
	 * Esto es para rotar la imagen
	 */
	vtkCamera *camera = renderer->GetActiveCamera();
	camera->SetFocalPoint(0, 0, 1);
	camera->SetPosition(0, 0, 0);
	camera->SetViewUp(0, -1, 0);
	renderer->ResetCamera();
}


void AortaFlowApp::LoadDICOMSeriesDialog() {
//	 QFileDialog dialog(this);
//	 dialog.setFileMode(QFileDialog::DirectoryOnly);
//	 dialog.setWindowTitle("Select a directory that contents the magnitude and flow DICOM series");
//	 dialog.exec();

	/**
	 * Usaremos por ahora los dialogos comunes nativos. Aunque podemos usar el anterior
	 * sin ningún problema.
	 */
	QString dirName = QFileDialog::getExistingDirectory(this, tr(
			"Select a directory that contents the magnitude and flow DICOM series"),
			"./Data");
	if (!dirName.isEmpty()){

		QDir dir(dirName);
		if (!dir.exists("mag")){
			QMessageBox::critical(this, "Directory don't found.","Don't found mag subdirectory in\n" + dirName );
			return;
		}

		if (!dir.exists("flow")){
			QMessageBox::critical(this, "Directory don't found.","Don't found flow subdirectory in\n" + dirName );
			return;
		}

		LoadDICOMScalar4DImage(dirName.toStdString());
	}
}

inline void AortaFlowApp::SetSupFrameVisible(bool b) {
	frameSup->setVisible(b);

	actionSuperior_frame->setEnabled(b);

	actionSuperior_frame->setCheckable(b);

	actionSuperior_frame->setChecked(b);
}

inline void AortaFlowApp::SetInfFrameVisible(bool b) {
	frameInf->setVisible(b);

	actionInferior_frame->setEnabled(b);

	actionInferior_frame->setCheckable(b);

	actionInferior_frame->setChecked(b);
}

void AortaFlowApp::Set2DCursor() {
	//	  vtkCursor2D *cursor = vtkCursor2D::New();
	//	  cursor->SetModelBounds(-10,10,-10,10,0,0);
	//	  cursor->AllOn();
	//	  cursor->OutlineOff();
	//	  cursor->Update();
	//
	//	  vtkPolyDataMapper *cursorMapper = vtkPolyDataMapper::New();
	//	  cursorMapper->SetInputConnection(cursor->GetOutputPort());
	//
	//	  vtkActor *cursorActor =  vtkActor::New();
	//	  cursorActor->GetProperty()->SetColor(1,0,0);
	//	  cursorActor->SetMapper(cursorMapper);
	//	  viewerSyncSup->GetImageViewerXY()->GetRenderer()->AddActor(cursorActor);
}

void AortaFlowApp::closeCurrentSerie() {
	frameSup->setVisible(false);
	frameInf->setVisible(false);
	magImage->Delete();
	flowImage->Delete();

	CleanImageSequences();

	viewerSyncSup->Delete();
	viewerSyncInf->Delete();

	labelTimeStepTotal->setText("");
	actionSuperior_frame->setCheckable(false);
	actionSuperior_frame->setEnabled(false);
	actionInferior_frame->setCheckable(false);
	actionInferior_frame->setEnabled(false);

	action3D_View->setEnabled(false);
	delete form;
}

void AortaFlowApp::CleanImageSequences(){
	for (int i = 0; i < magImages.size(); ++i){
		magImages[i]->Delete();
		flowImages[i][0]->Delete();
		flowImages[i][1]->Delete();
		flowImages[i][2]->Delete();
	}
	magImages.clear();
	flowImages.clear();

}

void AortaFlowApp::updateTimeStepView(int newTimeStep){
	viewerSyncSup->SetImage(pipeline._BuildTransferFunction(magImages[newTimeStep-1]->GetVTKImage()), 0);

	viewerSyncInf->SetImage(pipeline._BuildTransferFunction(flowImages[newTimeStep-1][0]->GetVTKImage()), 1);

	viewerSyncSup->Renderes();
	viewerSyncInf->Renderes();
}

void AortaFlowApp::updateComponentView(int component){
	viewerSyncInf->SetImage(pipeline._BuildTransferFunction(flowImages[spinBox->value()-1][component-1]->GetVTKImage()), 1);

	viewerSyncSup->Renderes();
	viewerSyncInf->Renderes();

}
void AortaFlowApp::show3DView() {
//	form->setParent(centralwidget);
//	gridLayout->addWidget(form, 0, 0, 1, 1);

	form->Set3DImage(magImages[0]->GetVTKImage());
//	form->setVisible(true);
	form->exec();
}

void AortaFlowApp::SetDocks(bool b){
	dockWidget_Properties_window->setVisible(b);
	dockWidget_Recent_studies->setVisible(b);
}

void AortaFlowApp::InitPipeline(){
	std::cout << "Entrando en _BuildHeuristic...\n";
	pipeline._BuildMask(pipeline.magImages[0]);
	std::cout << "Saliendo de _BuildHeuristic...\n";

	std::cout << "Entrando en Update...\n";
	result = pipeline.Update();
	std::cout << "Saliendo de Update...\n";
	std::cout.flush();
}

void AortaFlowApp::showSegment(){
	if(result.IsNull()){
		InitPipeline();
	}
	vtkKWImage * kwPreMaskImage = vtkKWImage::New();
	kwPreMaskImage->SetITKImageBase(result);
	viewerSyncInf->SetImage(pipeline._BuildTransferFunction(kwPreMaskImage->GetVTKImage()), 1);
}

void AortaFlowApp::InitProperties(){
	spinBox_3->setMinimum(1);
	spinBox_3->setMaximum(viewerSyncSup->GetNumberSagitalSlices());
	spinBox_3->setValue(1);

	spinBox_4->setMinimum(1);
	spinBox_4->setMaximum(viewerSyncSup->GetNumberAxialSlices());
	spinBox_4->setValue(1);

	spinBox_5->setMinimum(1);
	spinBox_5->setMaximum(viewerSyncSup->GetNumberCoronalSlices());
	spinBox_5->setValue(1);
}

void AortaFlowApp::updateProperties(int value){
	std::string name = sender()->name();
	if(name == "spinBox_3"){
		viewerSyncSup->viewerXY->updateSlice(value);
	}else if(name == "spinBox_4"){
		viewerSyncSup->viewerZX->updateSlice(value);
	}else{
		viewerSyncSup->viewerYZ->updateSlice(value);
	}

}
