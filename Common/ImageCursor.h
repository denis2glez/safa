#ifndef IMAGECURSOR_H_
#define IMAGECURSOR_H_

#include <string>
#include <map>
#include <algorithm>

#include <vtkImageIterator.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkCoordinate.h>
#include <vtkProperty2D.h>
#include <vtkActor2D.h>
#include <vtkImageData.h>

class ImageGeometry{
public:
	int extent[6];
	double spacing[3];
	double origin[3];
	ImageGeometry(){
		for(int i=0; i < 3; i++){
			spacing[i] = 1;
			origin[i] = 0;
		}
		for(int i=0; i < 6; i++)
			extent[i] = 0;
	}
};

class ImageCursor {
public:
	ImageCursor();
	virtual ~ImageCursor();

	void _CreateActorGeometry();
	double* GetPosition() const;
	int *   GetDiscretePosition() const;
	void SetInput(vtkImageData *imageData);

	double originX, originY, originZ;
	double sizeX, sizeY, sizeZ;
	double X, Y, Z;
	int I, J, K;
	double minCoordX, maxCoordX, minCoordY, maxCoordY, minCoordZ, maxCoordZ;
	double color[3]; 			//RGB
	ImageGeometry geometry;
	//vtkImageIterator interactor;

	std::map< std::string, vtkObject* > vtkObjects;
//	std::map< std::string, vtkObject* > observers;
};

#endif /* IMAGECURSOR_H_ */
