#include "ViewerSynchronize.h"

ViewerSynchronize::ViewerSynchronize(QWidget *parent, vtkImageData *vtkImage, int type) {
	viewerXY = new Viewer(parent, SAGITAL);
	viewerZX = new Viewer(parent, AXIAL);
	viewerYZ = new Viewer(parent, CORONAL);

	if (vtkImage != NULL)
		SetImage(vtkImage, type);
}

ViewerSynchronize::~ViewerSynchronize() {
	this->Delete();
}

void ViewerSynchronize::Delete(){
	delete viewerXY;
	delete viewerYZ;
	delete viewerZX;
}


void ViewerSynchronize::SetRendererWindows(vtkRenderWindow *rendererWinXY,
		vtkRenderWindow *rendererWinZX, vtkRenderWindow *rendererWinYZ) {
	viewerXY->SetRenderWindow(rendererWinXY);
	viewerZX->SetRenderWindow(rendererWinZX);
	viewerYZ->SetRenderWindow(rendererWinYZ);
}

void ViewerSynchronize::SetImage(vtkImageData *vtkImage, int type) {
	viewerXY->SetImage(vtkImage);
	viewerZX->SetImage(vtkImage);
	viewerYZ->SetImage(vtkImage);
	this->type = type;
	if(type == MAGTYPE)
		msg = "This visor show the magnitude images of the ";
	else
		msg = "This visor show the flow images of the ";
	viewerXY->setToolTip((msg + "sagital plane").c_str());
	viewerZX->setToolTip((msg + "axial plane").c_str());
	viewerYZ->setToolTip((msg + "coronal plane").c_str());
}

int ViewerSynchronize::GetNumberSagitalSlices() {
	return viewerXY->GetNumberSlices();
}

int ViewerSynchronize::GetNumberAxialSlices() {
	return viewerZX->GetNumberSlices();
}

int ViewerSynchronize::GetNumberCoronalSlices() {
	return viewerYZ->GetNumberSlices();
}

void ViewerSynchronize::Renderes(){
	viewerXY->Render();
	viewerYZ->Render();
	viewerZX->Render();
}
