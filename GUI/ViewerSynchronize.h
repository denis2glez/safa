#ifndef VIEWERSYNCHRONIZE_H_
#define VIEWERSYNCHRONIZE_H_

#include "Viewer.h"

enum {MAGTYPE, FLOWTYPE};
class ViewerSynchronize {
public:
	ViewerSynchronize(QWidget *parent, vtkImageData *vtkImage = NULL, int type = 0);
	virtual ~ViewerSynchronize();

	void Delete();

	void SetImage(vtkImageData *vtkImage, int type);

	void Renderes();

	void SetRendererWindows(vtkRenderWindow *rendererWinXY,
			vtkRenderWindow *rendererWinYZ, vtkRenderWindow *rendererWinZX);

	int GetNumberSagitalSlices();
	int GetNumberAxialSlices();
	int GetNumberCoronalSlices();

	//Serían respectivamente los planos Sagital, Axial y Coronal.
	Viewer *viewerXY, *viewerYZ, *viewerZX;
	int type;
	std::string msg;
private:




};

#endif /* VIEWERSYNCHRONIZE_H_ */
