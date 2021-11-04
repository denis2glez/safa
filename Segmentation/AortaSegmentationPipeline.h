#ifndef AORTASEGMENTATIONPIPELINE_H_
#define AORTASEGMENTATIONPIPELINE_H_

#include <string>
#include <qlabel.h>
#include <qprogressbar.h>

#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkScalarsToColors.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageMapToColors.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>

#include <Common/itkImageTypes.h>
#include <itkBinaryThresholdImageFilter.h>
#include "Common/vtkKWImage.h"

#define VTK_DECLARE_AND_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#define VTK_DECLARE(type, name) \
  vtkSmartPointer<type> name

#define VTK_CREATE(type, name) \
  name= vtkSmartPointer<type>::New()

class AortaSegmentationPipeline {
public:
	AortaSegmentationPipeline();
	virtual ~AortaSegmentationPipeline();

	void _BuildHeuristic(Scalar3DImageType::Pointer mask,	Vector4DImageType::Pointer img4DV);
	void _BuildMask(Scalar3DImageType::Pointer input);
	Scalar3DImageType::Pointer Update();
	void BuildMask();

	vtkSmartPointer<vtkImageData> _BuildTransferFunction(vtkSmartPointer<vtkImageData> img);
	Vector3DImageType::Pointer ApplyMaskToVectorialImage( Vector3DImageType::Pointer img, Scalar3DImageType::Pointer mask );
	void CalculateHeuristicBloodBehaviorValues();


	void AortaSegmentationPipelineTest();
	void RecalculateMinMaxValues();

	inline void SetMessage(const char *str = NULL, int value = 0){
		if(str != NULL){
			progressMessage->setText(str);
			progressBar->setVisible(true);
			progressBar->setValue(value);
		}else{
			progressMessage->setText("Ready");
			progressBar->setVisible(false);

		}
	}


	double _sigmaMax, _sigmaMin;
	double _minBloodRange, _maxBloodRange;
	double _smoothnessRatio, _smoothnessThreshold;
	double _vectorMagMin, _vectorMagMax;
	double _percent;

	int _progress;
	int timeStep;
	double velocityEncoding;

	//Imágenes ITK originales
	Scalar4DImageType::Pointer magImage;
	Vector4DImageType::Pointer flowImage;

	//Secuencia de imagenes escalares 3D.
	std::vector< Scalar3DImageType::Pointer >  magImages;

	//Secuencia de imagenes vectoriales 3D
	std::vector < std::vector< Scalar3DImageType::Pointer > >  flowImages;

	Scalar3DImageType::Pointer _mask;
	Scalar3DImageType::Pointer _preBloodHeuristicImg;

	std::string _inputDir;

	QLabel *progressMessage;
	QProgressBar *progressBar;
};



#endif /* AORTASEGMENTATIONPIPELINE_H_ */
