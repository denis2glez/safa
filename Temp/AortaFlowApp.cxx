#include "GUI/AortaFlowApp.h"

#include "Common/vtkKWImageIO.h"
#include "itkImage.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "vtkImageData.h"
#include "Common/vtkKWImage.h"
#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkPolyDataMapper.h"
#include "vtkInteractorStyleTrackballCamera.h"

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

class vtkImageInteractionCallback: public vtkCommand {
public:

	static vtkImageInteractionCallback *New() {
		return new vtkImageInteractionCallback;
	}
	;

	vtkImageInteractionCallback() {
		this->Slicing = 0;
		this->ImageReslice = 0;
		this->Interactor = 0;
	}
	;

	void SetImageReslice(vtkImageReslice *reslice) {
		this->ImageReslice = reslice;
	}
	;

	vtkImageReslice *GetImageReslice() {
		return this->ImageReslice;
	}
	;

	void SetInteractor(vtkRenderWindowInteractor *interactor) {
		this->Interactor = interactor;
	}
	;

	vtkRenderWindowInteractor *GetInteractor() {
		return this->Interactor;
	}
	;

	virtual void Execute(vtkObject *, unsigned long event, void *) {
		vtkRenderWindowInteractor *interactor = this->GetInteractor();

		int lastPos[2];
		interactor->GetLastEventPosition(lastPos);
		int currPos[2];
		interactor->GetEventPosition(currPos);

		if (event == vtkCommand::LeftButtonPressEvent) {
			this->Slicing = 1;
		} else if (event == vtkCommand::LeftButtonReleaseEvent) {
			this->Slicing = 0;
		} else if (event == vtkCommand::MouseMoveEvent) {
			if (this->Slicing) {
				vtkImageReslice *reslice = this->ImageReslice;

				// Increment slice position by deltaY of mouse
				int deltaY = lastPos[1] - currPos[1];

				reslice->GetOutput()->UpdateInformation();
				double sliceSpacing = reslice->GetOutput()->GetSpacing()[1];

				std::cout << "sliceSpacing = " << sliceSpacing << std::endl;
				vtkMatrix4x4 *matrix = reslice->GetResliceAxes();
				matrix->Print(std::cout);
				// move the center point that we are slicing through
				double point[4];
				double center[4];
				point[0] = 0.0;
				point[1] = 0.0;
				point[2] = sliceSpacing * deltaY;
				point[3] = 1.0;
				matrix->MultiplyPoint(point, center);
				matrix->SetElement(0, 3, center[0]);
				matrix->SetElement(1, 3, center[1]);
				matrix->SetElement(2, 3, center[2]);
				interactor->Render();
			} else {
				vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(
						interactor->GetInteractorStyle());
				if (style) {
					style->OnMouseMove();
				}
			}
		}
	}
	;

private:

	// Actions (slicing only, for now)
	int Slicing;

	// Pointer to vtkImageReslice
	vtkImageReslice *ImageReslice;

	// Pointer to the interactor
	vtkRenderWindowInteractor *Interactor;
};

AortaFlowApp::AortaFlowApp() {
	setupUi(this);

	// create a window to make it stereo capable and give it to QVTKWidget
	//	vtkRenderWindow* renwin = vtkRenderWindow::New();
	//	renwin->StereoCapableWindowOn();
	//	qvtkWidget1->SetRenderWindow(renwin);
	//	renwin->Delete();

	// add a renderer
	renXY = vtkRenderer::New();
	qvtkWidget_1->GetRenderWindow()->AddRenderer(renXY);
	renYZ = vtkRenderer::New();
	qvtkWidget_2->GetRenderWindow()->AddRenderer(renYZ);
	renZX = vtkRenderer::New();
	qvtkWidget_3->GetRenderWindow()->AddRenderer(renZX);
	renXY_2 = vtkRenderer::New();
	qvtkWidget_4->GetRenderWindow()->AddRenderer(renXY_2);
	renYZ_2 = vtkRenderer::New();
	qvtkWidget_5->GetRenderWindow()->AddRenderer(renYZ_2);
	renZX_2 = vtkRenderer::New();
	qvtkWidget_6->GetRenderWindow()->AddRenderer(renZX_2);

	vtkMagImage = vtkImageData::New();

	Connections = vtkEventQtSlotConnect::New();
	// update coords as we move through the window
	Connections->Connect(qvtkWidget_1->GetRenderWindow()->GetInteractor(),
			vtkCommand::MouseMoveEvent, this, SLOT(updateCoord(vtkObject*)));

	ProofsLoadScalar4D();

	QObject::connect(horizontalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(updateSlice(int)));
}
/**********************************************************************************************
 * Pruebas de la aplicación.
 **********************************************************************************************/
void AortaFlowApp::ProofsLoadScalar4D() {
	DICOMScalar4DImageLoader::Pointer loader = DICOMScalar4DImageLoader::New();
	loader->SetDirectory(
			"d:/Data/Michael_4D_MRI/7DflowMRI_FreiburgtoBarcelona/Mag1");
	try {
		loader->Update();
	} catch (ExceptionObject &ex) {
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << ex << std::endl;
		return;
	}
	Scalar4DImageType::Pointer img4D = loader->GetOutput();
	std::cout << "Update: BEGIN\n";
	img4D->Update();

	//	loader->Delete();

	std::cout << "Update: END\n";
	// Write image dimensions
	Scalar4DImageType::RegionType imgRegion = img4D->GetLargestPossibleRegion();

	imgRegion.Print(std::cout);

	int ext_iter = 0;
	Scalar3DImageType::Pointer img3D = ExtractScalar3DImageFromScalar4DImage(
			img4D, ext_iter);
	//	img4D->Delete();
	std::cout << "Extract iteration: " << ext_iter << std::endl;
	//	 Write image dimensions
	Scalar3DImageType::RegionType imgRegion1 =
			img3D->GetLargestPossibleRegion();
	imgRegion1.Print(std::cout);
	std::cout << "img3D image dimension: " << img3D->GetImageDimension()
			<< std::endl;
	std::cout << "TODO OK...\n";

	SetImageToVisor(img3D);

}
void AortaFlowApp::LoadDICOMScalar4DImage() {
	DICOMVectorial4DImageLoader::Pointer loader =
			DICOMVectorial4DImageLoader::New();
	loader->SetDirectory(
			"d:/Data/Michael_4D_MRI/7DflowMRI_FreiburgtoBarcelona/flowliteOK");
	try {
		loader->Update();
	} catch (ExceptionObject &ex) {
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << ex << std::endl;
		return;
	}
	Vector4DImageType::Pointer img4DV = loader->GetOutput();
	Set4DVectorialImageImageToVisor(img4DV);

}
AortaFlowApp::~AortaFlowApp() {
	renXY->Delete();
	renYZ->Delete();
	renZX->Delete();

	Connections->Delete();
}

void AortaFlowApp::updateCoord(vtkObject* obj) {
	// get interactor
	vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(
			obj);
	// get event position
	int event_pos[2];
	iren->GetEventPosition(event_pos);
	// update label
	QString str;
	str.sprintf("x=%d : y=%d", event_pos[0], event_pos[1]);
	//	label1->clear();
	label3_1->setText(str);

}
void AortaFlowApp::updateSlice(int value) {
	QString str;
	str.sprintf("   %d", value);
	label3_3->setText(str);

	imageViewer2->SetSlice(value);
}

void AortaFlowApp::Set4DVectorialImageImageToVisor(
		const Vector4DImageType::Pointer img4DV) {
	typedef itk::VectorIndexSelectionCastImageFilter<Vector4DImageType,
			Scalar4DImageType> VectorIndexSelectionFilterType;
	// Create Extract Component Filter
	VectorIndexSelectionFilterType::Pointer componentExtractor =
			VectorIndexSelectionFilterType::New();
	componentExtractor->SetInput(img4DV);
	componentExtractor->SetIndex(0);
	componentExtractor->Update();
	Scalar4DImageType::Pointer img4D = componentExtractor->GetOutput();

	Scalar3DImageType::Pointer img3D = ExtractScalar3DImageFromScalar4DImage(
			img4D, 1);

	// Create the pointer that will hold the image.
	vtkKWImage * kwImage = vtkKWImage::New();
	kwImage->SetITKImageBase(img3D);

	vtkMagImage = kwImage->GetVTKImage();

	std::cout << "<< vtkImage >>\n";
	vtkMagImage->Print(std::cout);
	/////////////////////////////////////////////////////////////////////////////
	//vtkDataArray *array = vtkMagImage->GetPointData()->GetScalars();
	//
	/////////////////////////////////////////////////////////////////////////////
	//FlipImage();

	/**
	 * Take it from ImageSlicing.cxx
	 */
	int extent[6];
	double spacing[3];
	double origin[3];
	vtkMagImage->GetWholeExtent(extent);
	vtkMagImage->GetSpacing(spacing);
	vtkMagImage->GetOrigin(origin);

	double center[3];
	center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
	center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
	center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

	/**
	 * Aqui pongo los valores del slider
	 */
	QString str;

	str.sprintf("Slices: %d", extent[1]);
	this->label2_3->setText(str);
	this->horizontalSlider_3->setMaxValue(extent[1] );
	this->horizontalSlider_3->setValue(extent[1]/2 );

	// Matrices for axial, coronal, sagittal, oblique view orientations
	static const double axialElements[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };

	//  static const double coronalElements[16] = {
	//           1, 0, 0, 0,
	//           0, 0, 1, 0,
	//           0,-1, 0, 0,
	//           0, 0, 0, 1 };

	//	   static const double sagittalElements[16] = {
	//	            0, 0,-1, 0,
	//	            1, 0, 0, 0,
	//	            0,-1, 0, 0,
	//	            0, 0, 0, 1 };

	//  static const double obliqueElements[16] = {
	//           1, 0, 0, 0,
	//           0, 0.866025, -0.5, 0,
	//           0, 0.5, 0.866025, 0,
	//           0, 0, 0, 1 };

	// Set the slice orientation
	vtkMatrix4x4 *resliceAxes = vtkMatrix4x4::New();
	resliceAxes->DeepCopy(axialElements);
	// Set the point through which to slice
	resliceAxes->SetElement(0, 3, center[0]);
	resliceAxes->SetElement(1, 3, center[1]);
	resliceAxes->SetElement(2, 3, center[2]);

	// Extract a slice in the desired orientation
	vtkImageReslice *reslice = vtkImageReslice::New();
	reslice->SetInput(vtkMagImage);
	reslice->SetOutputDimensionality(2);
	reslice->SetResliceAxes(resliceAxes);
	reslice->SetInterpolationModeToLinear();



	// Create a greyscale lookup table
	vtkLookupTable *table = vtkLookupTable::New();
	table->SetRange(0, 2000); // image intensity range
	table->SetValueRange(0.0, 1.0); // from black to white
	table->SetSaturationRange(0.0, 0.0); // no color saturation
	table->SetRampToLinear();
	table->Build();

	// Map the image through the lookup table
	vtkImageMapToColors *color = vtkImageMapToColors::New();
	color->SetLookupTable(table);
	color->SetInputConnection(reslice->GetOutputPort());

	// Display the image
	actor = vtkImageActor::New();
	actor->SetInput(color->GetOutput());

	vtkRenderer *renderer = vtkRenderer::New();
	renderer->AddActor(actor);

	/**
	 * Esto es para rotar la imagen
	 */
	vtkCamera *camera = renderer->GetActiveCamera();
	camera->SetFocalPoint(0, 0, 1);
    camera->SetPosition(0, 0, 0);
    camera->SetViewUp(0, -1, 0);
    renderer->ResetCamera();


	vtkRenderWindow *window = vtkRenderWindow::New();

	renZX = renderer;

	/******************************************************************************************//**
	 * Aqui es cuando le asignamos al QVTKWidget el RenderWindow
	 **********************************************************************************************/
	qvtkWidget_3->SetRenderWindow(window);

	window->AddRenderer(renderer);

	// Set up the interaction
	vtkInteractorStyleImage *imageStyle = vtkInteractorStyleImage::New();
	vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::New();

	interactor->SetInteractorStyle(imageStyle);

	window->SetInteractor(interactor);
	window->Render();

	vtkImageInteractionCallback *callback = vtkImageInteractionCallback::New();
	callback->SetImageReslice(reslice);
	callback->SetInteractor(interactor);

	imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
	imageStyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
	imageStyle->AddObserver(vtkCommand::LeftButtonReleaseEvent, callback);

	// Start interaction
	// The Start() method doesn't return until the window is closed by the user
	//	   interactor->Start();

	// Clean up
	//	   callback->Delete();
	//	   interactor->Delete();
	//	   imageStyle->Delete();
	////	   window->Delete();
	//	   renderer->Delete();
	//	   actor->Delete();
	//	   reslice->Delete();
	//	   resliceAxes->Delete();
	//	   color->Delete();
	//	   table->Delete();
	////	   reader->Delete();
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

