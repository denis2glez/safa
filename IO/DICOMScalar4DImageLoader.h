/*
itkDICOMSerie4DImageLoader.h --

Define una clase que encapsula un pipeline especifico para la
lectura de DICOM 4D escalar almacenado en una secuencia de slice
2D.
*/

#ifndef ITK_DICOMSerie4DImageLoader_H
#define ITK_DICOMSerie4DImageLoader_H

#include "Common/itkImageTypes.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkTileImageFilter.h"

#include "itkImageToVectorImageFilter.h"
#include "itkCompose3DVectorImageFilter.h"

//temporal
#include "qlabel.h"
#include "qprogressbar.h"

using namespace itk;
#define DICOMScalar4DImageLoaderException(x)                                       \
	throw ImageFileReaderException(  __FILE__, __LINE__, x, "DICOMScalar4DImageLoader" )

#endif
class DICOMScalar4DImageLoader : public Object
{
public:
	/** Standard class typedefs. */
	typedef DICOMScalar4DImageLoader  Self;
	typedef Object                    Superclass;
	typedef SmartPointer<Self>        Pointer;
	typedef SmartPointer<const Self>  ConstPointer;

	typedef Scalar4DImageType              OutputImageType;

	typedef TileImageFilter < Scalar3DImageType, Scalar4DImageType >
		TileFilterType;
	typedef ImageSeriesReader< Scalar3DImageType >
		ReaderType;
	typedef GDCMImageIO				DICOMIOType;

	typedef GDCMSeriesFileNames		NamesGeneratorType;

	typedef std::vector< std::string >		SeriesIdContainer;

	typedef std::vector< std::string >		FileNamesContainer;


	/** Method for creation through the object factory. */
	itkNewMacro( Self );  

	/** Run-time type information (and related methods). */
	itkTypeMacro( DICOMScalar4DImageLoader, Object );

	itkSetMacro( NumberOfTimeStepsToRead, size_t );
	itkGetConstMacro( NumberOfTimeStepsToRead, size_t );

	void SetDirectory( std::string const &dirName );

	virtual void Update ();

	OutputImageType *GetOutput()
	{
		return m_Tiler->GetOutput();
	}

	float GetProgress () const { return m_progress; }

	bool PrintObservers( std::ostream& os )
	{
		return Object::PrintObservers( os, ' ' );
	}
	void SetListener(QLabel *progressMessage, QProgressBar *progressBar){
		this->progressMessage = progressMessage;
		this->progressBar = progressBar;
	}

protected:
	DICOMScalar4DImageLoader();
	//Ver que se debe liberar
	~DICOMScalar4DImageLoader(){};

public:
	TileFilterType::Pointer m_Tiler;
	std::list<ReaderType::Pointer> m_Readers;
	size_t m_NumberOfTimeStepsToRead;
	float m_progress;

	QLabel *progressMessage;
	QProgressBar *progressBar;
};


