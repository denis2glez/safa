//#include "itkImageFileWriter.h"
//
//#include "DICOMVectorial3DImageLoader.h"
//#include "DICOMScalar4DImageLoader.h"
//#include "DICOMVectorial4DImageLoader.h"
//#include "itkReaderEvents.h"
//
//#include <iostream>
//#include <cstring>
//#include <string>
//#include <cmath>
//using namespace itk;
//
//template<class T>
//class ObserverReader : public itk::Command
//{
//  public:
//    typedef  ObserverReader   Self;
//    typedef  itk::Command             Superclass;
//    typedef  itk::SmartPointer<ObserverReader>  Pointer;
//    itkNewMacro( ObserverReader );
//  protected:
//    ObserverReader() {};
//  public:
//
//  void Execute(itk::Object *caller, const itk::EventObject & event)
//  {
//    Execute( (const itk::Object *)caller, event);
//  }
//
//  void Execute(const itk::Object * object, const itk::EventObject & event)
//  {
//    const T * reader =
//      dynamic_cast< const T * >( object );
//
//    std::cout << event.GetEventName() << ": " << reader->GetProgress() << "\n";
//  }
//};
//
//
//int TestVectorial3D(){
//	DICOMVectorial3DImageLoader::Pointer loader = DICOMVectorial3DImageLoader::New();
//	//loader->SetDirectory("d:\\Data\\Flow1x\\");
//	loader->SetDirectory("d:\\Data\\Michael_4D_MRI\\7DflowMRI_FreiburgtoBarcelona\\mag\\");
//	try{
//		loader->Update();
//	}catch(ExceptionObject &ex){
//		std::cerr << "ExceptionObject caught !" << std::endl;
//		std::cerr << ex << std::endl;
//		return 1;
//	}
//	return 0;
//}
//int TestSerie4D(){
//	DICOMScalar4DImageLoader::Pointer loader = DICOMScalar4DImageLoader::New();
//	ObserverReader<DICOMScalar4DImageLoader>::Pointer observer = ObserverReader<DICOMScalar4DImageLoader>::New();
//	loader->AddObserver( itk::StartScanUIDEvent(), observer );
//	loader->AddObserver( itk::EndScanUIDEvent(), observer );
//	loader->AddObserver( itk::ProgressUIDEvent(), observer );
//	loader->AddObserver( itk::StartReadEvent(), observer );
//	loader->AddObserver( itk::EndReadEvent(), observer );
//	loader->AddObserver( itk::ProgressReadEvent(), observer );
//
//	std::cout << "StartScanUIDEvent : " <<  loader->HasObserver( itk::StartScanUIDEvent() )
//              << "\n";
//	loader->PrintObservers( std::cout );
//
//	loader->SetDirectory("d:\\Data\\Flow1x\\");
//
//	try{
//		loader->Update();
//	}catch(ExceptionObject &ex){
//		std::cerr << "ExceptionObject caught !" << std::endl;
//		std::cerr << ex << std::endl;
//		return 1;
//	}
//
//	Scalar4DImageType::Pointer img4D = loader->GetOutput();
//	std::cout << "Update: BEGIN\n";
//	img4D->Update();
//	std::cout << "Update: END\n";
//	// Write image dimensions
//	Vector4DImageType::RegionType imgRegion = img4D->GetLargestPossibleRegion();
//	imgRegion.Print( std::cout );
//
//	return 0;
//}
//int TestVectorial4D(){
//	DICOMVectorial4DImageLoader::Pointer loader = DICOMVectorial4DImageLoader::New();
//	ObserverReader<DICOMVectorial4DImageLoader>::Pointer observer = ObserverReader<DICOMVectorial4DImageLoader>::New();
//	loader->AddObserver( itk::StartScanUIDEvent(), observer );
//	loader->AddObserver( itk::EndScanUIDEvent(), observer );
//	loader->AddObserver( itk::ProgressUIDEvent(), observer );
//	loader->AddObserver( itk::StartReadEvent(), observer );
//	loader->AddObserver( itk::EndReadEvent(), observer );
//	loader->AddObserver( itk::ProgressReadEvent(), observer );
//
//	std::cout << "StartScanUIDEvent : " <<  loader->HasObserver( itk::StartScanUIDEvent() )
//              << "\n";
//	loader->PrintObservers( std::cout );
//
//	loader->SetDirectory("d:\\Data\\Michael_4D_MRI\\7DflowMRI_FreiburgtoBarcelona\\flowliteOK\\");
//
//	try{
//		loader->Update();
//	}catch(ExceptionObject &ex){
//		std::cerr << "ExceptionObject caught !" << std::endl;
//		std::cerr << ex << std::endl;
//		return 1;
//	}
//
//	Vector4DImageType::Pointer img4D = loader->GetOutput();
//
//	std::cout << "Update: BEGIN\n";
//	img4D->Update();
//	std::cout << "Update: END\n";
//	// Write image dimensions
//	Vector4DImageType::RegionType imgRegion = img4D->GetLargestPossibleRegion();
//
//	imgRegion.Print( std::cout );
//	return 0;
//}
//
//int main() {
//
////	int result3D = TestVectorial3D();       // Siguen los problemas con las imagenes 3D
////	if(result3D == 0)
////		std::cout << "TestVectorial3D OK...\n" << std::endl;
//
////	int result4D = TestSerie4D();
////	if(result4D == 0)
////		std::cout << "TestSerie4D OK...\n" << std::endl;
//
//	int result4DV = TestVectorial4D();
//	if(result4DV == 0)
//		std::cout << "TestVectorial4D OK...\n" << std::endl;
//
//	return 0;
//}
