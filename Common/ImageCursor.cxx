#include <Common/ImageCursor.h>

ImageCursor::ImageCursor(){
	color[0] = color[1] = color[2] = 0;
	X = Y = Z = 0;
	I = J = K = 0;
	originX = originY = originZ = 0;
	sizeX = sizeY = sizeZ = 1;
	minCoordX = minCoordY = minCoordZ = 0;
	maxCoordX = maxCoordY = maxCoordZ = 0;
}
/**
 * Necesitamos liberar la memoria asignada a los objectos referenciados en
 * vtkObjects.
 */
ImageCursor::~ImageCursor() {
	std::map< std::string, vtkObject* >::iterator it;
	for(it = vtkObjects.begin(); it != vtkObjects.end(); it++)
		it->second->Delete();

}

void ImageCursor::_CreateActorGeometry(){
	std::cout << "X, Y, Z = " << X << ", " << Y << ", " << Z << endl;
	std::cout << "geometry.extent[0] = " << geometry.extent[0] << endl;

	vtkPoints *pts = vtkPoints::New();
	vtkObjects["Points"] = pts;
	pts->SetNumberOfPoints(6);

	vtkCellArray *lines = vtkCellArray::New();
	vtkObjects["Lines"] = lines;
//	Ejemplo de cómo usarlo.
//	vtkPoints *points = static_cast< vtkPoints * >(vtkObjects["Points"]);
//	std::cout << "Numero de ptos: " << points->GetNumberOfPoints() << std::endl;

	//Segmento paralelo al eje X
	lines->InsertNextCell(2);
	lines->InsertCellPoint(0);
	lines->InsertCellPoint(1);
	//Segmento paralelo al eje Y
	lines->InsertNextCell(2);
	lines->InsertCellPoint(2);
	lines->InsertCellPoint(3);
	//Segmento paralelo al eje Z
	lines->InsertNextCell(2);
	lines->InsertCellPoint(4);
	lines->InsertCellPoint(5);

	vtkPolyData *poly = vtkPolyData::New();
	vtkObjects["PolyData"] = poly;
	poly->SetPoints(pts);
	poly->SetLines(lines);

	vtkPolyDataMapper2D *crossMapper = vtkPolyDataMapper2D::New();
	vtkObjects["CrossMapper"] = crossMapper;
	crossMapper->SetInput(poly);

	vtkCoordinate *coord = vtkCoordinate::New();
	vtkObjects["Coord"] = coord;
	coord->SetCoordinateSystemToWorld();
	crossMapper->SetTransformCoordinate(coord);

	vtkActor2D *crossActor = vtkActor2D::New();
	vtkObjects["CrossActor"] = crossActor;
	crossActor->SetMapper(crossMapper);
	crossActor->GetProperty()->SetColor(color);
}

/*! \brief Este método devuelve la posición actual en coordenadas reales.
 *  \return Un arreglo de 3 elementos que representan las coordenadas (X, Y, Z)
 * Como se devulve un puntero este arreglo debe ser luego eliminado con delete.
 */
double* ImageCursor::GetPosition() const{
	double pos[] = {X, Y, Z};
	return pos;
}

/*! \brief Este método devuelve la posición actual en coordenadas discretas.
 *  \return Un arreglo de 3 elementos que representan las coordenadas (I, J, K)
 * Como se devulve un puntero este arreglo debe ser luego eliminado con delete.
 */
int* ImageCursor::GetDiscretePosition() const{
	int dis_pos[] = {I, J, K};
	return dis_pos;
}

void ImageCursor::SetInput(vtkImageData *imageData){
	imageData->UpdateInformation();
	int *extent = imageData->GetExtent();
	std::copy(extent, extent + 6, geometry.extent);
	double *spacing = imageData->GetSpacing();
	std::copy(spacing, spacing + 3, geometry.spacing);
	double *origin = imageData->GetOrigin();
	std::copy(origin, origin + 3, geometry.origin);
}
