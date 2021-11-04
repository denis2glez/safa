/*
DICOMVectorial4DImageLoader.h --

Define una clase que encapsula un pipeline especifico para la
lectura de DICOM 4D vectorial almacenado en una secuencia de slice
2D.
*/

#ifndef DICOMVectorial4DImageLoader_H
#define DICOMVectorial4DImageLoader_H

//#include "itkLightObject.h"
#include "Common/itkImageTypes.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkTileImageFilter.h"
#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"
#include "itkImageToVectorImageFilter.h"
#include "itkCompose3DVectorImageFilter.h"

//temporal
#include "qlabel.h"
#include "qprogressbar.h"

using namespace itk;
class DICOMVectorial4DImageLoader : public Object
{
public:
	/** Standard class typedefs. */
	typedef DICOMVectorial4DImageLoader            Self;
	typedef Object                    Superclass;
	typedef SmartPointer<Self>        Pointer;
	typedef SmartPointer<const Self>  ConstPointer;

	typedef Vector4DImageType              OutputImageType;

	typedef TileImageFilter < Scalar3DImageType, Scalar4DImageType >		    TileFilterType;
	typedef Compose3DVectorImageFilter< Scalar4DImageType, Vector4DImageType >	Compose3DVectorImageType;
	typedef ImageSeriesReader< Scalar3DImageType >								ReaderType;

	typedef GDCMImageIO DICOMIOType;

	typedef GDCMSeriesFileNames NamesGeneratorType;

	typedef std::vector< std::string > SeriesIdContainer;

	typedef std::vector< std::string > FileNamesContainer;

	typedef itk::MetaDataDictionary                                       DictionaryType;
	typedef itk::MetaDataObject< std::string >                            MetaDataStringType;

	/** Method for creation through the object factory. */
	itkNewMacro( Self );  

	/** Run-time type information (and related methods). */
	itkTypeMacro( DICOMVectorial4DImageLoader, Object );

	itkSetMacro( NumberOfTimeStepsToRead, size_t );
	itkGetConstMacro( NumberOfTimeStepsToRead, size_t );

	void SetDirectory( std::string const &dirName );

	void Update ()
	{
		vectorizer->Update();
	}

	Vector4DImageType::Pointer GetOutput()
	{
		return vectorizer->GetOutput();
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
	DICOMVectorial4DImageLoader( );
	~DICOMVectorial4DImageLoader() {};

public:
	TileFilterType::Pointer tilerx;
	TileFilterType::Pointer tilery;
	TileFilterType::Pointer tilerz;
	Compose3DVectorImageType::Pointer vectorizer;
	float m_progress;
	size_t m_NumberOfTimeStepsToRead;

	QLabel *progressMessage;
	QProgressBar *progressBar;
};

#endif
