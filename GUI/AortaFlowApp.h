#ifndef AORTAFLOWAPP_H_
#define AORTAFLOWAPP_H_

#include <vector>

#include <QMainwindow>
#include <qfiledialog.h>
#include <qprogressbar.h>
#include <qframe.h>
#include <qmessagebox.h>

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

#include "itkVectorImageToImageAdaptor.h"
#include "Common/AortaUtility.h"
#include "Common/vtkKWImage.h"
#include "ViewerSynchronize.h"

#include <qmenu.h>

#include "ui_AortaFlowApp.h"
#include "GUI/Form3D.h"

#include "Segmentation/AortaSegmentationPipeline.h"

class AortaFlowApp: public QMainWindow, public Ui::AortaFlowApp {
Q_OBJECT

public:
	AortaFlowApp();
	~AortaFlowApp();

	void LoadDICOMScalar4DImage(std::string dirName);
	void LoadDICOMVectorial4DImage(std::string dirName);

	void FlipImage(vtkRenderer *renderer);


	void InitViewers();

	inline void SetSupFrameVisible(bool b);
	inline void SetInfFrameVisible(bool b);

	void EstablishConnections();

	void InitMagImages();
	void InitFlowImages();

	void InitProperties();
	void InitPipeline();
	void CleanImageSequences();
	void SetDocks(bool b);

	void Set2DCursor();


public slots:
	void syncronizeSupInf(bool sync);

	void LoadDICOMSeriesDialog();

	void closeCurrentSerie();

	void show3DView();

	void showSegment();

	void updateTimeStepView(int newTimeStep);
	void updateComponentView(int component);
	void updateProperties(int value);

public:
	QLabel *progressMessage;
	ViewerSynchronize *viewerSyncSup, *viewerSyncInf;
	bool syncSupInf;

	//Imágenes ITK originales
	Scalar4DImageType::Pointer magImage;
	Vector4DImageType::Pointer flowImage;

	int timeSteps;
	//Secuencia de imagenes escalares 3D.
	std::vector< vtkKWImage * >  magImages;

	//Secuencia de imagenes escalares 3D
	//flowImages[i][j] -> i paso de tiempo actual (0 <= i <= timeSteps) y j componente (0 <= j <= 2)
	std::vector < std::vector< vtkKWImage * > >  flowImages;

	Form3D *form;

	AortaSegmentationPipeline pipeline;

	Scalar3DImageType::Pointer result;
};

#endif /* AORTAFLOWAPP_H_ */
