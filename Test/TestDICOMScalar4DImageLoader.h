#ifndef TESTDICOMSCALAR4DIMAGELOADER_H_
#define TESTDICOMSCALAR4DIMAGELOADER_H_

#include "IO/DICOMScalar4DImageLoader.h"
#include "IO/ObserverReader.h"

int TestDICOMScalar4DImageLoader() {
	DICOMScalar4DImageLoader::Pointer loader = DICOMScalar4DImageLoader::New();
	ObserverReader<DICOMScalar4DImageLoader>::Pointer observer =
			ObserverReader<DICOMScalar4DImageLoader>::New();
	loader->AddObserver(itk::StartScanUIDEvent(), observer);
	loader->AddObserver(itk::EndScanUIDEvent(), observer);
	loader->AddObserver(itk::ProgressUIDEvent(), observer);
	loader->AddObserver(itk::StartReadEvent(), observer);
	loader->AddObserver(itk::EndReadEvent(), observer);
	loader->AddObserver(itk::ProgressReadEvent(), observer);

	std::cout << "StartScanUIDEvent : " << loader->HasObserver(
			itk::StartScanUIDEvent()) << "\n";
	loader->PrintObservers(std::cout);

	loader->SetDirectory("d:\\Data\\Flow1x\\");

	try {
		loader->Update();
	} catch (ExceptionObject &ex) {
		std::cerr << "ExceptionObject caught !" << std::endl;
		std::cerr << ex << std::endl;
		return 1;
	}

	Scalar4DImageType::Pointer img4D = loader->GetOutput();
	std::cout << "Update: BEGIN\n";
	img4D->Update();
	std::cout << "Update: END\n";
	// Write image dimensions
	Vector4DImageType::RegionType imgRegion = img4D->GetLargestPossibleRegion();
	imgRegion.Print(std::cout);

	return 0;
}
#endif /* TESTDICOMSCALAR4DIMAGELOADER_H_ */
