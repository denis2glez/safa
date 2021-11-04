#include <Segmentation/AortaSegmentationPipeline.h>
#include <Segmentation/AF4DCenterLine.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkBinaryErodeImageFilter.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkRelabelComponentImageFilter.h>
#include <itkVotingBinaryHoleFillingImageFilter.h>
#include <itkCompose3DVectorImageFilter.h>
#include <itkMeanImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <Segmentation/itkBinaryThinningImageFilter3D.h>

#include "IO/itkDICOM4DVPixelReader.h"
#include "itkVelocityEncodingImageFilter.h"
#include "itkVectorMagnitudeMinimumMaximumImageCalculator.h"
#include "Common/HeuristicBloodBehaviorDetection.h"
#include "WriteImageRoutines.h"

#define OUTPUT_MAXSTDEV "D:/Data/vtks/stdev_oliverMIA.vtk"
#define OUTPUT_DILATE "D:/Data/vtks/dilate_oliverMIA.vtk"
#define OUTPUT_CONNECTED_COMPONENT "D:/Data/vtks/connected_oliverMIA.vtk"
#define OUTPUT_THINNING "D:/Data/vtks/thinning_oliverMIA.vtk"
#define STEPS 2
#define INPUT "d:/Data/Michael_4D_MRI/7DflowMRI_FreiburgtoBarcelona/OK/flow"
#define INPUT_MASK "d:/Data/vtks/noisemask_oliver.vtk"
#define OUTPUT "d:/data/vtks/curvature_smoothnes_oliverMIA.vtk"

typedef itk::Compose3DVectorImageFilter< Scalar3DImageType, Vector3DImageType >    ComposeFilterType;
typedef itk::VectorMagnitudeMinimumMaximumImageCalculator< Vector3DImageType, Scalar3DImageType  >   VectorMagMinMaxCalculatorType;
typedef itk::MeanImageFilter< Scalar3DImageType ,Scalar3DImageType  >  MeanImageFilterType;
typedef itk::BinaryThresholdImageFilter<Scalar3DImageType, Scalar3DImageType>   BinaryThresholdFilterType;
typedef itk::MinimumMaximumImageCalculator<Scalar3DImageType>	MinMaxCalculatorType;
typedef itk::VotingBinaryHoleFillingImageFilter<Integer3DImageType,	Integer3DImageType> VotingBinaryHoleFillingImageFilterType;
typedef itk::BinaryThinningImageFilter3D< Scalar3DImageType, Scalar3DImageType >  ThinningFilterType;

AortaSegmentationPipeline::AortaSegmentationPipeline() {
	_progress = 0;
	_sigmaMax = 4.8;
	_sigmaMin = 4.0;

//	_minBloodRange = 120;
//	_maxBloodRange = 1024;

//	_minBloodRange = 50;
//	_maxBloodRange = 1000;

	_minBloodRange = 69;
	_maxBloodRange = 2643;

	_smoothnessRatio = _smoothnessThreshold = 0;
	_vectorMagMin = _vectorMagMax = -1;
	_percent = 0.025;

	velocityEncoding = 150;
}

AortaSegmentationPipeline::~AortaSegmentationPipeline() {

}

//Primero
void AortaSegmentationPipeline::_BuildMask(Scalar3DImageType::Pointer input) {
		SetMessage("Applying MinimumMaximumImageCalculator...", 10);
		MinMaxCalculatorType::Pointer minmax = MinMaxCalculatorType::New();
		minmax->SetImage(input);
		minmax->Compute();

		MinMaxCalculatorType::PixelType min = minmax->GetMinimum();
		MinMaxCalculatorType::PixelType max = minmax->GetMaximum();

		std::cout << "min = " << min << "\n" << "max = " << max << "\n";

//		minmax->Delete();
//
		SetMessage("Applying BinaryThresholdImageFilter...", 20);
		BinaryThresholdFilterType::Pointer threshold =	BinaryThresholdFilterType::New();
		threshold->SetInput(input);

		min = (max - min) * _percent + min;

		threshold->SetLowerThreshold(min);
		threshold->SetUpperThreshold(max);
		threshold->SetOutsideValue(0);
		threshold->SetInsideValue(1);
		threshold->Update();
//
		_mask = threshold->GetOutput();
		_mask->Update();
}

//Segundo
void AortaSegmentationPipeline::_BuildHeuristic(Scalar3DImageType::Pointer mask,
		Vector4DImageType::Pointer img4DV){
	_preBloodHeuristicImg = MaximalComponentStDev(mask, img4DV);

}
//Tercero
Scalar3DImageType::Pointer AortaSegmentationPipeline::Update() {
	SetMessage("Applying VelocityEncodingImageFilter...", 30);
	typedef itk::VelocityEncodingImageFilter< Vector4DImageType, Vector4DImageType  >  VelocityEncodingImageFilterType;
	VelocityEncodingImageFilterType::Pointer venc = VelocityEncodingImageFilterType::New();
	venc->SetInput(flowImage);
	//todo temporal
	venc->SetMaxGrayLevel(4092);
	venc->SetMinGrayLevel(-4096);
	venc->SetVelocityAdquisition(150);
	venc->Update();

	SetMessage("Applying MaximalComponentStDev...", 40);
	_BuildHeuristic(_mask, venc->GetOutput());


	SetMessage("Applying BinaryThresholdImageFilter...", 50);
	BinaryThresholdFilterType::Pointer binaryThreshold = BinaryThresholdFilterType::New();
	binaryThreshold->SetInput(_preBloodHeuristicImg);
	binaryThreshold->SetLowerThreshold(_minBloodRange);
	binaryThreshold->SetUpperThreshold(_maxBloodRange);
	binaryThreshold->SetOutsideValue(0);
	binaryThreshold->SetInsideValue(1);

	binaryThreshold->Update();

////	binaryThreshold->Delete();

	SetMessage("Applying BinaryErodeImageFilter...", 60);
	typedef itk::BinaryBallStructuringElement< PixelType, 3> StructuringElementType;
	typedef itk::BinaryErodeImageFilter<Scalar3DImageType, Scalar3DImageType,
			StructuringElementType> BinaryErodeImageFilterType;
	BinaryErodeImageFilterType::Pointer erode =	BinaryErodeImageFilterType::New();

	//Erode to denoise
	StructuringElementType kernel;
	kernel.SetRadius(1);
	kernel.CreateStructuringElement();

	erode->SetInput(binaryThreshold->GetOutput());
	erode->SetKernel(kernel);
	erode->SetErodeValue(0);
	erode->SetBackgroundValue(0);
	erode->SetForegroundValue(1);
	erode->Update();


////	Scalar3DImageType::Pointer result2 = erode->GetOutput();
////	img->DisconnectPipeline();
////
	SetMessage("Applying ConnectedComponentImageFilter...", 70);
	typedef itk::ConnectedComponentImageFilter<Scalar3DImageType, Integer3DImageType>
			ConnectedComponentImageFilterType;
	//Find maximal connected components
	ConnectedComponentImageFilterType::Pointer connected =
			ConnectedComponentImageFilterType::New();
	connected->SetInput(erode->GetOutput());
	connected->Update();


////	img = connected->GetOutput();
////	img->DisconnectPipeline();
//
	SetMessage("Applying RelabelComponentImageFilter...", 80);
	typedef itk::RelabelComponentImageFilter<Integer3DImageType, Scalar3DImageType>
			RelabelComponentImageFilterType;
	RelabelComponentImageFilterType::Pointer relabel =
			RelabelComponentImageFilterType::New();
	relabel->SetInput(connected->GetOutput());
	relabel->Update();


//
////	img->DisconnectPipeline();
////
	SetMessage("Applying BinaryThresholdImageFilter...", 90);
	BinaryThresholdFilterType::Pointer bigThreshold = BinaryThresholdFilterType::New();
	bigThreshold->SetInput(relabel->GetOutput());
	bigThreshold->SetLowerThreshold(1);
	bigThreshold->SetUpperThreshold(1);
	bigThreshold->SetOutsideValue(0);
	bigThreshold->SetInsideValue(1);
	bigThreshold->Update();


////	img = bigThreshold->GetOutput();
////	img->DisconnectPipeline();
////
	SetMessage("Applying BinaryDilateImageFilter...", 100);
	typedef itk::BinaryDilateImageFilter<Scalar3DImageType, Scalar3DImageType,
			StructuringElementType> BinaryDilateImageFilterType;
	BinaryDilateImageFilterType::Pointer dilate = BinaryDilateImageFilterType::New();
	dilate->SetInput(bigThreshold->GetOutput());
	dilate->SetKernel(kernel);
	dilate->SetDilateValue(1);
	dilate->SetBackgroundValue(0);
	dilate->SetForegroundValue(1);
	dilate->Update();

	this->_mask = dilate->GetOutput();

//	img->DisconnectPipeline();
//
//	result = img;
//
//	if(_smoothnessRatio){
//		MeanImageFilterType::Pointer mean = MeanImageFilterType::New();
//		mean->SetInput(img);
//
//		int radius = (int)_smoothnessRatio;
//	    Scalar3DImageType::SizeType indexRadius;
//	    indexRadius[0] = radius;
//	    indexRadius[1] = radius;
//	    indexRadius[2] = radius;
//
//		mean->SetRadius(indexRadius);
//
//		mean->Update();
//		img = mean->GetOutput();
//		img->DisconnectPipeline();
//
////		mean->Delete();
//
//		BinaryThresholdFilterType::Pointer smoothnesTrhld = BinaryThresholdFilterType::New();
//		smoothnesTrhld->SetInput(img);
//		smoothnesTrhld->SetLowerThreshold(_smoothnessThreshold);
//		smoothnesTrhld->SetUpperThreshold(1);
//		smoothnesTrhld->SetOutsideValue(0);
//		smoothnesTrhld->SetInsideValue(1);
//
//		smoothnesTrhld->Update();
//
//		img = smoothnesTrhld->GetOutput();
//		img->DisconnectPipeline();
//
//		result = img;
//	}

	SetMessage();
	return _mask;
}

void AortaSegmentationPipeline::BuildMask(){
	Update();
}

void AortaSegmentationPipeline::CalculateHeuristicBloodBehaviorValues() {
//	typedef itk::ImageFileReader< Scalar3DImageType > Reader3DType;
//
//	if(_preBloodHeuristicImg.IsNull()){
//		Reader3DType::Pointer reader = Reader3DType::New();
//		std::string filename = _inputDir + "/premask.vtk";
//		reader->SetFileName(filename);
//		reader->Update();
//		_preBloodHeuristicImg = reader->GetOutput();
//		_preBloodHeuristicImg->DisconnectPipeline();
//	}
}

void AortaSegmentationPipeline::RecalculateMinMaxValues(){
	if(_mask.IsNull())
		BuildMask();
	_vectorMagMin = 1000000000;
	_vectorMagMax = -1000000000;

	for(int i=0; i < timeStep; i++){
		ComposeFilterType::Pointer composer = ComposeFilterType::New();

		composer->SetInput1(flowImages[i][0]);
		composer->SetInput2(flowImages[i][1]);
		composer->SetInput3(flowImages[i][2]);

		composer->Update();

		VectorMagMinMaxCalculatorType::Pointer minmax = VectorMagMinMaxCalculatorType::New();
		minmax->SetVenc(velocityEncoding);
		minmax->SetImage(composer->GetOutput());
		minmax->SetMaskImage(_mask);
		minmax->Compute();

		double min = minmax->GetMinimum();
		double max = minmax->GetMaximum();

		if(min < _vectorMagMin)
			_vectorMagMin = min;
		if(max > _vectorMagMax)
			_vectorMagMax = max;

		minmax->Delete();
//		composer->Delete();
	}

}

void AortaSegmentationPipeline::AortaSegmentationPipelineTest() {
	std::cout << "Entrando en AortaSegmentationPipelineTest...\n";
	typedef itk::BinaryThresholdImageFilter<Scalar3DImageType, Scalar3DImageType>
			BinaryThresholdImageFilterType;

	typedef itk::BinaryBallStructuringElement<PixelType, 3> StructuringElementType;
	typedef itk::BinaryErodeImageFilter<Scalar3DImageType, Scalar3DImageType,
			StructuringElementType> BinaryErodeImageFilterType;
	typedef itk::BinaryDilateImageFilter<Scalar3DImageType, Scalar3DImageType,
			StructuringElementType> BinaryDilateImageFilterType;

	typedef itk::ConnectedComponentImageFilter<Scalar3DImageType, Integer3DImageType>
			ConnectedComponentImageFilterType;
	typedef itk::RelabelComponentImageFilter<Integer3DImageType, Scalar3DImageType>
			RelabelComponentImageFilterType;



	//  typedef BinaryThinningImageFilter3D< Scalar3DImageType, Scalar3DImageType >
	//    ThinningFilterType;

	DICOM4DVPixelReader::Pointer vectorReader = DICOM4DVPixelReader::New();
	vectorReader->SetNumberOfTimeStepsToRead(STEPS);
	vectorReader->SetDirectory(INPUT);
	vectorReader->Update();

	typedef ImageFileReader<Scalar3DImageType> ReaderType;
	ReaderType::Pointer maskReader = ReaderType::New();
	maskReader->SetFileName(INPUT_MASK);
	maskReader->Update();

	//Real encoding here
	VelocityEncodingImageFilter<Vector4DImageType, Vector4DImageType>::Pointer
			venc = VelocityEncodingImageFilter<Vector4DImageType,
					Vector4DImageType>::New();

	venc->SetInput(vectorReader->GetOutput());
	venc->Update();
	Vector4DImageType::Pointer vencImg = venc->GetOutput();
	vencImg->DisconnectPipeline();

	vectorReader->Delete();

	//TEST NEW ALGORITHM
	Scalar3DImageType::Pointer img3D = MaximalComponentStDev(
			maskReader->GetOutput(), vencImg);

	BinaryThresholdImageFilterType::Pointer threshold =
			BinaryThresholdImageFilterType::New();
	threshold->SetInput(img3D);
	/*//VALORES DE MICHAEL
	 threshold->SetLowerThreshold(120);
	 threshold->SetUpperThreshold(1024);
	 */
	/*
	 threshold->SetLowerThreshold(500);
	 threshold->SetUpperThreshold(7000);
	 */
	threshold->SetLowerThreshold(69);
	threshold->SetUpperThreshold(2643);
	threshold->SetOutsideValue(0);
	threshold->SetInsideValue(1);
	threshold->Update();

	//Erode to denoise
	StructuringElementType kernel;
	kernel.SetRadius(1);
	kernel.CreateStructuringElement();
	BinaryErodeImageFilterType::Pointer erode =
			BinaryErodeImageFilterType::New();
	erode->SetInput(threshold->GetOutput());
	erode->SetKernel(kernel);
	erode->SetErodeValue(0);
	erode->SetBackgroundValue(0);
	erode->SetForegroundValue(1);
	erode->Update();

	//Find maximal connected components
	ConnectedComponentImageFilterType::Pointer connected =
			ConnectedComponentImageFilterType::New();
	connected->SetInput(erode->GetOutput());
	connected->Update();
	RelabelComponentImageFilterType::Pointer relabel =
			RelabelComponentImageFilterType::New();
	relabel->InPlaceOn();
	relabel->SetInput(connected->GetOutput());
	relabel->Update();
	BinaryThresholdImageFilterType::Pointer bigThreshold =
			BinaryThresholdImageFilterType::New();
	bigThreshold->SetInput(relabel->GetOutput());
	bigThreshold->SetLowerThreshold(1);
	bigThreshold->SetUpperThreshold(1);
	bigThreshold->SetOutsideValue(0);
	bigThreshold->SetInsideValue(1);
	bigThreshold->Update();

	/*
	 VotingBinaryHoleFillingImageFilterType::Pointer holeFilling =
	 VotingBinaryHoleFillingImageFilterType::New();
	 holeFilling->SetInput(bigThreshold->GetOutput());
	 holeFilling->SetBackgroundValue(0);
	 holeFilling->SetForegroundValue(1);
	 holeFilling->Update();
	 */

	BinaryDilateImageFilterType::Pointer dilate =
			BinaryDilateImageFilterType::New();
	dilate->SetInput(bigThreshold->GetOutput());
	dilate->SetKernel(kernel);
	dilate->SetDilateValue(0);
	dilate->SetBackgroundValue(0);
	dilate->SetForegroundValue(1);
	dilate->Update();

	// Define the thinning filter

	//ThinningFilterType::Pointer thinningFilter = ThinningFilterType::New();
	//thinningFilter->SetInput( dilate->GetOutput() );
	//thinningFilter->Update();

	//  WriteScalar3DImageTypeToFile(dilate->GetOutput(), OUTPUT_DILATE);
	//WriteInteger3DImageTypeToFile(thinningFilter->GetOutput(), OUTPUT_THINNING);
	WriteScalar3DImageTypeToFile(dilate->GetOutput(), OUTPUT_CONNECTED_COMPONENT);
	WriteScalar3DImageTypeToFile(img3D, OUTPUT_MAXSTDEV);

	connected->Delete();
	threshold->Delete();
	erode->Delete();
	//  dilate->Delete();
	venc->Delete();
	img3D->Delete();

	std::cout << "Saliendo de AortaSegmentationPipelineTest...\n";
}

vtkSmartPointer<vtkImageData> AortaSegmentationPipeline::_BuildTransferFunction(vtkSmartPointer<vtkImageData> img){
	VTK_DECLARE(vtkImageData, result);

	vtkSmartPointer<vtkDataArray> data = img->GetPointData()->GetScalars();
	double range[2];
	data->GetRange(range);
	double min = range[0];
	double max = range[1];

	VTK_DECLARE_AND_CREATE(vtkColorTransferFunction, transferFn);

	transferFn->SetColorSpaceToHSV();
	transferFn->RemoveAllPoints();
	transferFn->AdjustRange(range);
	transferFn->AddHSVPoint(min, 0.0, 0.0, 0.0);
	transferFn->AddHSVPoint(max, 0.0, 0.0, 1.0);

	VTK_DECLARE_AND_CREATE(vtkImageMapToColors, mapColors);
	mapColors->SetLookupTable(transferFn);
	mapColors->SetOutputFormatToRGB();
	mapColors->SetInput(img);
	mapColors->Update();

	result = mapColors->GetOutput();

	return result;
}

Vector3DImageType::Pointer AortaSegmentationPipeline::ApplyMaskToVectorialImage ( Vector3DImageType::Pointer img, Scalar3DImageType::Pointer mask )
{
  typedef itk::MaskImageFilter< Vector3DImageType, Scalar3DImageType, Vector3DImageType >  MaskFilterType;
  MaskFilterType::Pointer maskFilter = MaskFilterType::New();
  maskFilter = MaskFilterType::New();
  maskFilter->SetInput1(img);
  maskFilter->SetInput2(mask);
  maskFilter->Update();
  Vector3DImageType::Pointer result = maskFilter->GetOutput();
  result->DisconnectPipeline();
  maskFilter->Delete();
  return result;
}

//int main(){
//	AortaSegmentationPipelineTest();
//	return 0;
//}
