#include <GUI/Form3D.h>
#include <vtkProperty.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkCamera.h>

#include <vtkInteractorStyleSwitch.h>

Form3D::Form3D() {
	setupUi(this);
	volume = vtkVolume::New();
	renderer = vtkRenderer::New();

	qvtkWidget3D->GetRenderWindow()->AddRenderer(renderer);
	qvtkWidget3D->GetInteractor()->SetInteractorStyle(vtkInteractorStyleSwitch::New());
}

Form3D::~Form3D() {
	std::cout << "Entrando en ~Form3D..\n";
	renderer->Delete();
	volume->Delete();
}

void Form3D::Show3DView(bool b) {
	this->setVisible(b);
}

void Form3D::Set3DImage(vtkImageData *vtkImage) {
	vtkPiecewiseFunction *tfun = vtkPiecewiseFunction::New();
	tfun->AddPoint(0, 0.0);

	tfun->AddPoint(50, 0.0);

	tfun->AddPoint(255, 1.0);

	vtkColorTransferFunction *ctfun = vtkColorTransferFunction::New();
	ctfun->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
	ctfun->AddRGBPoint(1.0, 1.0, 1.0, 1.0);

	vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New();
	volumeProperty->SetColor(ctfun);

	volumeProperty->SetScalarOpacity(tfun);

	volumeProperty->SetInterpolationTypeToLinear();

	volumeProperty->ShadeOn();

//	vtkVolumeRayCastCompositeFunction *compositeFunction = vtkVolumeRayCastCompositeFunction::New();
	vtkFixedPointVolumeRayCastMapper *volumeMapper = vtkFixedPointVolumeRayCastMapper::New();
	volumeMapper->SetInput(vtkImage);
//	volumeMapper->SetVolumeRayCastFunction(compositeFunction);


	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	renderer->AddVolume(volume);

	FlipImage();

	//qvtkWidget3D->Render();
}

void Form3D::SetRenderWindow() {
	//viewer3D->SetRenderWindow(qvtkWidget3D->GetRenderWindow());
}

void Form3D::FlipImage(){
	vtkCamera *camera = renderer->GetActiveCamera();
	camera->SetFocalPoint(0, 0, 1);
	camera->SetPosition(0, 0, 0);
	camera->SetViewUp(0, -1, 0);
	renderer->ResetCamera();
}
