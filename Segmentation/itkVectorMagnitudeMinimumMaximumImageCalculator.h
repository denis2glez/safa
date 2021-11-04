/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVectorMagnitudeVectorMagnitudeMinimumMaximumImageCalculator.h,v $
  Language:  C++
  Date:      $Date: 2009-04-25 12:27:32 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorMagnitudeMinimumMaximumImageCalculator_h
#define __itkVectorMagnitudeMinimumMaximumImageCalculator_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkNumericTraits.h"
#include "Common/itkImageTypes.h"

namespace itk
{

/** \class VectorMagnitudeMinimumMaximumImageCalculator
 * This calculator computes the minimum and the maximum intensity values of
 * an image.  It is templated over input image type.  If only Maximum or
 * Minimum value is needed, just call ComputeMaximum() (ComputeMinimum())
 * otherwise Compute() will compute both.
 *
 * \ingroup Operators
 */
template <class TInputImage, class TMaskImage>
class ITK_EXPORT VectorMagnitudeMinimumMaximumImageCalculator : public Object 
{
public:
  /** Standard class typedefs. */
  typedef VectorMagnitudeMinimumMaximumImageCalculator Self;
  typedef Object                   Superclass;
  typedef SmartPointer<Self>       Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorMagnitudeMinimumMaximumImageCalculator, Object);

  /** Type definition for the input image. */
  typedef TInputImage ImageType;

  /** Type definition for the input image. */
  typedef TMaskImage MaskImageType;

  /** Pointer type for the image. */
  typedef typename TInputImage::Pointer  ImagePointer;
  
  /** Const Pointer type for the image. */
  typedef typename TInputImage::ConstPointer ImageConstPointer;
  typedef typename TMaskImage::ConstPointer  MaskImageConstPointer;

  typedef typename TMaskImage::PixelType MaskImagePixelType;

  /** Type definition for the input image pixel type. */
  typedef typename ImageType::PixelType PixelType;
  typedef typename PixelType::ValueType ValueType;

  /** Real type of the vector */
  typedef typename NumericTraits<ValueType>::RealType RealType;
  
  /** Type definition for the input image index type. */
  typedef typename TInputImage::IndexType IndexType;
  
  /** Type definition for the input image region type. */
  typedef typename TInputImage::RegionType RegionType;
  
  /** Set the input image. */
  itkSetConstObjectMacro(Image,ImageType);
  /** Set the input image. */
  itkSetConstObjectMacro(MaskImage,MaskImageType);

  /** Set the "inside" pixel value. The default value 
   * NumericTraits<OutputPixelType>::max() */
  itkSetMacro(Venc,RealType);
  
  /** Get the "inside" pixel value. */
  itkGetConstReferenceMacro(Venc,RealType);

  /** Compute the minimum value of intensity of the input image. */
  void ComputeMinimum(void);

  /** Compute the maximum value of intensity of the input image. */
  void ComputeMaximum(void);

  /** Compute the minimum and maximum values of intensity of the input image. */
  void Compute(void);

  /** Return the minimum intensity value. */
  itkGetConstMacro(Minimum,RealType);
  
  /** Return the maximum intensity value. */
  itkGetConstMacro(Maximum,RealType);

  /** Return the index of the minimum intensity value. */
  itkGetConstReferenceMacro(IndexOfMinimum,IndexType);

  /** Return the index of the maximum intensity value. */
  itkGetConstReferenceMacro(IndexOfMaximum,IndexType);

  /** Set the region over which the values will be computed */
  //void SetRegion( const RegionType & region );

protected:
  VectorMagnitudeMinimumMaximumImageCalculator();
  virtual ~VectorMagnitudeMinimumMaximumImageCalculator() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Vector dimention*/
  itkStaticConstMacro(VectorDimension, unsigned int, PixelType::Dimension);

private:
  VectorMagnitudeMinimumMaximumImageCalculator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  RealType m_Venc;
  RealType m_Minimum;
  RealType m_Maximum;
  
  ImageConstPointer     m_Image;
  MaskImageConstPointer m_MaskImage;

  IndexType m_IndexOfMinimum;
  IndexType m_IndexOfMaximum;

  RegionType m_Region;
  bool       m_RegionSetByUser;
};

} // end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVectorMagnitudeMinimumMaximumImageCalculator.cxx"
#endif

#endif /* __itkVectorMagnitudeVectorMagnitudeMinimumMaximumImageCalculator_h */
