//#include <sstream>
#include <cstdio>
#include "itkImageFileReader.h"
#include "Segmentation/AF4DCenterLine.h"
#include "Segmentation/WriteImageRoutines.h"
#include "Segmentation/itkBinaryThinningImageFilter3D.h"

void CenterLineToFile(vector<AF4D::PointVector> &pv, const char *fileName){
//	std::ostringstream ss;
//	int size = pv.size();
//	for(int i=0; i < size; i++){
//		AF4D::CenterLine::PointsCloudToFile(pv[i], fileName);
//	}
	AF4D::CenterLine::PointsCloudToFile(pv[0], fileName);

}
void TestCenterLine() 
{
/*************************************************************************************/
	//Cantidad de puntos de la linea central que se desea.
	const int points_number = 1000;
/*************************************************************************************/
	typedef itk::ImageFileReader<Scalar3DImageType> ReaderType;

	ReaderType::Pointer maskReader = ReaderType::New();
//	maskReader->SetFileName("d:\\Data\\VTKs\\compositemask.vtk");
	//maskReader->SetFileName("d:\\Data\\VTKs\\mask3.vtk");
	maskReader->SetFileName("d:\\Data\\VTKs\\connected_frances.vtk");
	maskReader->Update();

//	typedef itk::BinaryThinningImageFilter3D
//		<Scalar3DImageType, Scalar3DImageType> FilterThinningType;
//
//	//Crear el filtro para obtener el skeleton
//	FilterThinningType::Pointer thinFilter =
//		FilterThinningType::New();
//
//	thinFilter->SetInput(maskReader->GetOutput());
//	thinFilter->Update();
//
//	//WriteScalar3DImageTypeToFile(thinFilter->GetThinning(), "d:\\Users\\Denis\\Trabajo\\connected_frances_scheleton.vtk");
//	//WriteScalar3DImageTypeToFile(thinFilter->GetThinning(),	"d:\\Users\\Denis\\Trabajo\\output_mask3.vtk");
//	WriteScalar3DImageTypeToFile(thinFilter->GetThinning(),	"d:\\Data\\VTKs\\connected_frances_output.vtk");

	AF4D::CenterLine::Pointer cl = AF4D::CenterLine::New();
	cl->SetInput(maskReader->GetOutput());

	printf("Comenzando FindCenterLine...\n");
	vector<AF4D::PointVector> pv = cl->FindCenterLine(points_number);
	printf("Terminando FindCenterLine...\n");

	CenterLineToFile(pv, "d:/Data/centerline/connected_frances1.txt");
//	CenterLineToFile(pv, "d:/Data/centerline/compositemask.txt");

}

//int main(){
//	TestCenterLine();
//
//	return 0;
//}
