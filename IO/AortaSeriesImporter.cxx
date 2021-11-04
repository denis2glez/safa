#include <itkMinimumMaximumImageCalculator.h>
#include <itkBinaryThresholdImageFilter.h>
#include "IO/AortaSeriesImporter.h"

#include "Common/HeuristicBloodBehaviorDetection.h"
#include "Common/AortaUtility.h"
#include "itkVectorIndexSelectionCastImageFilter.h"

typedef itk::MinimumMaximumImageCalculator<Scalar3DImageType>
		MinMaxCalculatorType;
typedef itk::BinaryThresholdImageFilter<Scalar3DImageType, Scalar3DImageType>
		BinaryThresholdFilterType;
typedef itk::VectorIndexSelectionCastImageFilter<Vector3DImageType,
		Scalar3DImageType> VectorIndexSelectionFilterType;

AortaSeriesImporter::AortaSeriesImporter() {
	_minGray = -4096;
	_maxGray = 4092;
	_venc = 150;
	_timeStps = -1;
	_vectorMagMin = -1;
	_vectorMagMax = -1;
}
AortaSeriesImporter::~AortaSeriesImporter() {

}

Scalar3DImageType::Pointer AortaSeriesImporter::_BuildMask(
		Scalar3DImageType::Pointer input) {
	int _percent = 0.025;
	MinMaxCalculatorType::Pointer minmax = MinMaxCalculatorType::New();
	minmax->SetImage(input);
	minmax->Compute();

	MinMaxCalculatorType::PixelType min = minmax->GetMinimum();
	MinMaxCalculatorType::PixelType max = minmax->GetMaximum();

	minmax->Delete();

	BinaryThresholdFilterType::Pointer threshold = BinaryThresholdFilterType::New();
	threshold->SetInput(input);

	min = (max - min) * _percent + min;

	threshold->SetLowerThreshold(min);
	threshold->SetUpperThreshold(max);
	threshold->SetOutsideValue(0);
	threshold->SetInsideValue(1);
	threshold->Update();

	Scalar3DImageType::Pointer mask = threshold->GetOutput();
	mask->DisconnectPipeline();
	threshold->Delete();

	return mask;
}

Scalar3DImageType::Pointer AortaSeriesImporter::_ProcessMagnitudeImage(
		Scalar4DImageType::Pointer img, int nPhase) {
	Scalar3DImageType::Pointer mask;
	Scalar3DImageType::Pointer img3D;
	std::stringstream ss;
	for (int i = 0; i < nPhase; i++) {
		img3D = ExtractScalar3DImageFromScalar4DImage(img, i);
		if (i == 0)
			mask = _BuildMask(img3D);
		ss << _outputPath << "/mag_" << i << ".vtk";

		std::cout << "Escribiendo imagen: " << ss.str() << std::endl;
		VTKScalarImageWriter(ss.str().c_str(), img3D);

		ss.clear();
		img3D->Delete();
	}
	return mask;
}

void AortaSeriesImporter::_ProcessPhaseImage(Scalar3DImageType::Pointer mask,
		Vector4DImageType::Pointer img4DV, int nPhase) {
	VectorIndexSelectionFilterType::Pointer componentExtractor;
	Scalar3DImageType::Pointer preBloodHeuristicImg = MaximalComponentStDev(
			mask, img4DV);
	std::stringstream ss;
	ss << _outputPath << "/premask.vtk";

	std::cout << "Escribiendo imagen: " << ss.str() << std::endl;
	VTKScalarImageWriter(ss.str().c_str(), preBloodHeuristicImg);

	Vector3DImageType::Pointer img3DV;
	Scalar3DImageType::Pointer img3D;
	for (int i = 0; i < nPhase; i++) {
		img3DV = ExtractVector3DImageFromVector4DImage(img4DV, i);
		for (int j = 0; j < 3; j++) {
			componentExtractor = VectorIndexSelectionFilterType::New();
			componentExtractor->SetInput(img3DV);
			componentExtractor->SetIndex(j);
			img3D = componentExtractor->GetOutput();

			ss << _outputPath << "vct_" << i << j << ".vtk";
			std::cout << "Escribiendo imagen: " << ss.str() << std::endl;
			VTKScalarImageWriter(ss.str().c_str(), img3D);

			ss.clear();                 // TODO OJO con esto que no funciona como deberia (no vacia el string)
			img3D->Delete();
		}
		img3DV->Delete();
	}
}

void AortaSeriesImporter::Update(){
	//Garantizar que existan tanto los archivos de flujo como los de magnitud.
	std::string magDirName = _inputPath + "/mag";
	std::string flowDirName = _inputPath + "/flow";
	itksys::Directory dirMag, dirFlow;

	if(dirMag.Load(magDirName.c_str()) && dirFlow.Load(flowDirName.c_str())){
		if(dirMag.GetNumberOfFiles() > 2 && dirFlow.GetNumberOfFiles() > 2){
			const char *firstFileName = dirMag.GetFile(2);
			DICOMHeaderReader::Pointer headerReader = DICOMHeaderReader::New();
			headerReader->SetFileName(firstFileName);
			headerReader->Update();
			int nPhase;
			if(_timeStps == -1)
				nPhase =  atoi(headerReader->GetTag("0018|1090").c_str());
			else
				nPhase = _timeStps;
			string patientOrientation = headerReader->GetTag("0018|5100");
			headerReader->Delete();

			cout << "patientOrientation: " << patientOrientation << endl;

			// Leer la imagen de magnitud con DICOMScalar4DImageLoader
			DICOMScalar4DImageLoader::Pointer loader4D = DICOMScalar4DImageLoader::New();
			loader4D->SetNumberOfTimeStepsToRead(nPhase);
			loader4D->SetDirectory(magDirName.c_str());
			//itk::EventObject

			Scalar4DImageType::Pointer img4D = loader4D->GetOutput();



		}else{
			cerr << "Some of the directory:" << endl
				 << magDirName << endl
				 << flowDirName << endl
				 << "it's empty" << endl;
		}
	}else{
		cerr << "Don't exist some of the directories: " << endl
			 << magDirName << endl
			 << flowDirName << endl;
	}
}
