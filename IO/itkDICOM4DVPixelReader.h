/*
  itkDICOM4DVPixelReader.h --

  Define una clase que encapsula un pipeline especifico para la
  lectura de DICOM 4D vectorial almacenado en una secuencia de slice
  2D.
 */

#ifndef ITK_DICOM4DVPIXELREADER_H
#define ITK_DICOM4DVPIXELREADER_H

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

class DICOM4DVPixelReader : public itk::Object
{
 public:
  /** Standard class typedefs. */
  typedef DICOM4DVPixelReader            Self;
  typedef itk::Object                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  typedef Vector4DImageType              OutputImageType;
  
  typedef itk::TileImageFilter < Scalar3DImageType, Scalar4DImageType >
    TileFilterType;
  typedef itk::Compose3DVectorImageFilter< Scalar4DImageType, Vector4DImageType >
    Compose3DVectorImageType;
  typedef itk::ImageSeriesReader< Scalar3DImageType >
    ReaderType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );  

  /** Run-time type information (and related methods). */
  itkTypeMacro( DICOM4DVPixelReader, itk::Object );

  itkSetMacro( NumberOfTimeStepsToRead, size_t );
  itkGetConstMacro( NumberOfTimeStepsToRead, size_t );
  
  void SetDirectory( std::string const &name );

  virtual void 	Update ();
  
  Vector4DImageType *GetOutput()
  {
    return m_Vectorizer->GetOutput();
  }

  float GetProgress () const { return m_progress; }

  bool PrintObservers( std::ostream& os )
  {
    return itk::Object::PrintObservers( os, ' ' );
  }
  
protected:
  DICOM4DVPixelReader ();
  ~DICOM4DVPixelReader() {};

private:
  // Tilers: apilan en un 4D las componentes
  // [0] --> 4D x-component
  // [1] --> 4D y-component
  // [2] --> 4D z-component
  TileFilterType::Pointer m_Tilers[3];

  // Readers: lectores de las secuencias 3D
  // [0] --> lista de readers 3D X uno para cada instante temporal
  // [1] --> lista de readers 3D Y uno para cada instante temporal
  // [2] --> lista de readers 3D Z uno para cada instante temporal
  std::list<ReaderType::Pointer> m_Readers[3];

  // Vectorizer: compone una imagen vectorial 4D a partir de 3
  // imagenes escalares 4D
  Compose3DVectorImageType::Pointer m_Vectorizer;
  size_t m_NumberOfTimeStepsToRead;
    
  float m_progress;
};

#endif
