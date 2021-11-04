/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVelocityEncodingImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:37 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVelocityEncodingImageFilter_h
#define __itkVelocityEncodingImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace itk
{

// This functor class applies a scaling transformation A.x 
// to input values.
namespace Functor {  
 
template< typename TInput, typename  TOutput>
class VelocityEncodingTransform
{
public:
  typedef typename NumericTraits< typename TInput::ValueType >::RealType RealType;

  VelocityEncodingTransform() {}
  ~VelocityEncodingTransform() {}

  void SetVelocityAdquisition( RealType a ) { m_VelocityAdquisition = a; }

  void SetMaxGrayLevel( RealType a ) 
  { 
    m_MaxGrayLevel = a; 
    _grayDif = m_MaxGrayLevel - m_MinGrayLevel;
    _graySum = m_MaxGrayLevel + m_MinGrayLevel;
  }

  void SetMinGrayLevel( RealType a ) 
  { 
    m_MinGrayLevel = a; 
    _grayDif = m_MaxGrayLevel - m_MinGrayLevel;
    _graySum = m_MaxGrayLevel + m_MinGrayLevel;
  }

  itkStaticConstMacro(VectorDimension, unsigned int, TInput::Dimension);

  bool operator!=( const VelocityEncodingTransform & other ) const
  {
    if( m_VelocityAdquisition != other.m_VelocityAdquisition ||
       m_MaxGrayLevel != other.m_MaxGrayLevel ) {
      return true;
    }
    return false;
  }
  
  bool operator==( const VelocityEncodingTransform & other ) const
  {
    return !(*this != other);
  }
  
  inline TOutput operator()( const TInput & x ) const
  {
    TOutput  result;
    for( unsigned int i = 0; i < VectorDimension; i++ ) {
      RealType c = static_cast<RealType>(x[i]);
      const RealType encodedValue = m_VelocityAdquisition * (2 * c - _graySum) / _grayDif;
      result[i]= static_cast< typename TOutput::ValueType >( encodedValue );
    }
    return result;
  }

private:
  RealType m_VelocityAdquisition;
  RealType m_MaxGrayLevel;
  RealType m_MinGrayLevel;
  RealType _grayDif;
  RealType _graySum;
}; 

}  // end namespace functor


/** \class VelocityEncodingImageFilter
 * \brief Applies a linear transformation to the magnitude of pixel vectors in a
 * vector Image. 
 *
 * VelocityEncodingImageFilter applies pixel-wise a linear transformation
 * to the intensity values of input image pixels. The linear transformation is
 * defined by the user in terms of the maximum magnitude value of the vectors
 * in the pixels that the output image should have.
 * 
 * All computations are performed in the precison of the input pixel's 
 * RealType. Before assigning the computed value to the output pixel. 
 *
 * \sa RescaleIntensityImageFilter
 * 
 * \ingroup IntensityImageFilters  Multithreaded
 *
 */
template <typename  TInputImage, typename  TOutputImage=TInputImage>
class ITK_EXPORT VelocityEncodingImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Functor::VelocityEncodingTransform< 
  typename TInputImage::PixelType, 
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef VelocityEncodingImageFilter  Self;
  typedef UnaryFunctorImageFilter<
    TInputImage,TOutputImage, 
    Functor::VelocityEncodingTransform< 
      typename TInputImage::PixelType, 
      typename TOutputImage::PixelType> >    Superclass;
  typedef SmartPointer<Self>                 Pointer;
  typedef SmartPointer<const Self>           ConstPointer;

  typedef typename TOutputImage::PixelType                  OutputPixelType;
  typedef typename TInputImage::PixelType                   InputPixelType;
  typedef typename InputPixelType::ValueType                InputValueType;
  typedef typename OutputPixelType::ValueType               OutputValueType;
  typedef typename NumericTraits<InputValueType>::RealType  InputRealType;
  typedef typename NumericTraits<OutputValueType>::RealType OutputRealType;

  typedef typename Superclass::InputImageType      InputImageType;
  typedef typename Superclass::InputImagePointer   InputImagePointer;

  /** Run-time type information (and related methods).   */
  itkTypeMacro( VelocityEncodingImageFilter, UnaryFunctorImageFilter );

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Velocity of adquisition. */
  itkSetMacro( VelocityAdquisition, InputRealType );
  itkGetConstReferenceMacro( VelocityAdquisition, InputRealType );

  /** Max grey value of the input image. */
  itkSetMacro( MaxGrayLevel, InputRealType );
  itkGetConstReferenceMacro( MaxGrayLevel, InputRealType );

  /** Max grey value of the input image. */
  itkSetMacro( MinGrayLevel, InputRealType );
  itkGetConstReferenceMacro( MinGrayLevel, InputRealType );

  /** Process to execute before entering the multithreaded section */
  void BeforeThreadedGenerateData(void);

  /** Print internal ivars */
  void PrintSelf(std::ostream& os, Indent indent) const;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
                  (Concept::HasNumericTraits<InputValueType>));
  itkConceptMacro(OutputHasNumericTraitsCheck,
                  (Concept::HasNumericTraits<OutputValueType>));
  /** End concept checking */
#endif

protected:
  VelocityEncodingImageFilter();
  virtual ~VelocityEncodingImageFilter() {};

private:
  VelocityEncodingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputRealType m_VelocityAdquisition;
  InputRealType m_MaxGrayLevel;
  InputRealType m_MinGrayLevel;

};


  
} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVelocityEncodingImageFilter.cxx"
#endif
  
#endif
