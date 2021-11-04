#ifndef Viewer_H_
#define Viewer_H_

#include <sstream>            // Para usar ostringstream
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkImageViewer2.h>
#include <vtkLegendScaleActor.h>
#include <vtkCornerAnnotation.h>
#include <vtkInteractorStyleImage.h>
#include <vtkSmartPointer.h>
#include <vtkPNGWriter.h>
#include <vtkWindowToImageFilter.h>
#include <qmenu.h>
#include "vtkEventQtSlotConnect.h"

#include <vtkPointWidget.h>
#include "ui_Viewer.h"

enum Orientation{SAGITAL, AXIAL, CORONAL};

class Viewer: public QWidget, public Ui::Viewer{
Q_OBJECT

public:
	Viewer(QWidget *parent, Orientation orient);
	virtual ~Viewer();

	void SetImage(vtkImageData *vtkImage);
	void SetRenderWindow(vtkRenderWindow *rendererWin);
	void Render();

	void SetNumberSlices();
	int  GetNumberSlices();
	void SetCornerAnnotations();
	void SetLeyendScaler();
	void InitSlider();
	void InitSlices();
	void EnableCornerAnnotations(bool b);
	void EnableLeyendScalers(bool b);
	void TakeSnapshot();
	void FlipImage();
	void ImageRestore();
	void InitPopMenu();
	void EstablishConnections();

	void SynchronizeWith(Viewer *otherViewer, bool sync);

	vtkImageViewer2 *viewer;
	vtkCornerAnnotation *cornerAnnotation;
	vtkLegendScaleActor *legendScaleActor;
	int numSlices;
	Orientation orientation;
	QMenu* popupMenu;
	vtkEventQtSlotConnect *connections;

public slots:
	void popup(vtkObject * obj, unsigned long, void * client_data, void *, vtkCommand * command);
	void updateCoord(vtkObject* obj);
	void checkMenuAction(QAction* menuAction);
	void updateSlice(int value);
};

#endif /* Viewer_H_ */
