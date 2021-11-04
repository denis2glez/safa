#ifndef AORTAFLOWAPP_H_
#define AORTAFLOWAPP_H_

#include <QMainwindow>
#include "ui_AortaFlowApp.h"
#include <vtkEventQtSlotConnect.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkImageReslice.h>
#include <vtkPointData.h>
#include "vtkImageActor.h"
#include <vtkImageViewer2.h>
#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>
#include "itkImageFileWriter.h"
#include "IO/DICOMScalar4DImageLoader.h"
#include "IO/DICOMVectorial4DImageLoader.h"
#include "itkExtractImageFilter.h"
//VER ESTO
#include "itkVectorImageToImageAdaptor.h"
#include "Common/AortaUtility.h"
#include "ViewerSynchronize.h"

#include <vtkRendererCollection.h>

class AortaFlowApp: public QMainWindow, public Ui::AortaFlowApp {
Q_OBJECT

public:
	AortaFlowApp();
	~AortaFlowApp();
	void ProofsLoadScalar4D();
	void LoadDICOMScalar4DImage();
	void Set4DVectorialImageImageToVisor(const Vector4DImageType::Pointer img3DV);
	void FlipImage(vtkRenderer *renderer);

public slots:
	void updateCoord(vtkObject*);
	void updateSlice(int);

protected:

protected:
	vtkRenderer *renXY, *renXY_2;
	vtkRenderer *renYZ, *renYZ_2;
	vtkRenderer *renZX, *renZX_2;

	vtkEventQtSlotConnect *Connections;

	vtkImageData *vtkMagImage;
	vtkImageActor *actor;
	vtkImageViewer2 *imageViewer2;
	vtkImageReslice *imageReslice;

};

#endif /* AORTAFLOWAPP_H_ */
