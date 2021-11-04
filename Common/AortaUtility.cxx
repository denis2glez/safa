#include "Common/AortaUtility.h"

Scalar3DImageType::Pointer ExtractScalar3DImageFromScalar4DImage(
		const Scalar4DImageType *img4D, int it_idx //Extract iteration number
) {
	typedef itk::ExtractImageFilter<Scalar4DImageType, Scalar3DImageType>
			ExtractFilterType;
	// Create the a extracter,
	ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
	// Set Input
	extractFilter->SetInput(img4D);
	//  Find input image size
	Scalar4DImageType::RegionType inputRegion =
			img4D->GetLargestPossibleRegion();
	Scalar4DImageType::SizeType size = inputRegion.GetSize();

	// Collapse fourth dimension
	size[3] = 0;

	//  Set region for 3D extraction
	Scalar4DImageType::IndexType start = inputRegion.GetIndex();
	Scalar4DImageType::RegionType desiredRegion;
	desiredRegion.SetSize(size);

	// Set the iteration chosen
	start[3] = it_idx;
	desiredRegion.SetIndex(start);
	extractFilter->SetExtractionRegion(desiredRegion);

	try {
		extractFilter->Update();
	} catch (itk::ExceptionObject &ex) {
		std::cerr << "Exception catch: " << ex.GetDescription() << std::endl;
	}
	Scalar3DImageType *img3D = extractFilter->GetOutput();
	img3D->Update();

	return img3D;
}

Vector3DImageType::Pointer ExtractVector3DImageFromVector4DImage(
		const Vector4DImageType *img4D, int it_idx //Extract iteration number
) {
	typedef itk::ExtractImageFilter<Vector4DImageType, Vector3DImageType>
			ExtractFilterType;
	// Create the a extracter,
	ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
	// Set Input
	extractFilter->SetInput(img4D);
	//  Find input image size
	Vector4DImageType::RegionType inputRegion =
			img4D->GetLargestPossibleRegion();
	Vector4DImageType::SizeType size = inputRegion.GetSize();

	// Collapse fourth dimension
	size[3] = 0;

	//  Set region for 3D extraction
	Vector4DImageType::IndexType start = inputRegion.GetIndex();
	Vector4DImageType::RegionType desiredRegion;
	desiredRegion.SetSize(size);

	// Set the iteration chosen
	start[3] = it_idx;
	desiredRegion.SetIndex(start);
	extractFilter->SetExtractionRegion(desiredRegion);

	try {
		extractFilter->Update();
	} catch (itk::ExceptionObject &ex) {
		std::cerr << "Exception catch: " << ex.GetDescription() << std::endl;
	}
	Vector3DImageType *img3D = extractFilter->GetOutput();
	img3D->Update();

	return img3D;
}

void VTKScalarImageWriter(const char* fileName,
		const Scalar3DImageType::Pointer img3D) {
	WriterScalar3DType::Pointer writer = WriterScalar3DType::New();
	writer->SetInput(img3D);
	writer->SetFileName(fileName);
	writer->Update();
	writer->Delete();
}
