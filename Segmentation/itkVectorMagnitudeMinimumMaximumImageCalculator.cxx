/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVectorMagnitudeVectorMagnitudeMinimumMaximumImageCalculator.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-16 23:24:23 $
  Version:   $Revision: 1.23 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorMagnitudeVectorMagnitudeMinimumMaximumImageCalculator_txx
#define __itkVectorMagnitudeVectorMagnitudeMinimumMaximumImageCalculator_txx

#include "itkVectorMagnitudeMinimumMaximumImageCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
//#include "vnl/vnl_math.h"
#include <vcl_cmath.h>

namespace itk
{ 
    
/**
 * Constructor
 */
template<class TInputImage, class TMaskImage>
VectorMagnitudeMinimumMaximumImageCalculator<TInputImage, TMaskImage>
::VectorMagnitudeMinimumMaximumImageCalculator()
{
  m_Image = TInputImage::New();
  m_Maximum = NumericTraits<RealType>::NonpositiveMin();
  m_Minimum = NumericTraits<RealType>::max();
  m_IndexOfMinimum.Fill(0);
  m_IndexOfMaximum.Fill(0);
  m_RegionSetByUser = false;

  m_MaskImage = NULL;
}


/**
 * Compute Min and Max of m_Image
 */
template<class TInputImage, class TMaskImage>
void VectorMagnitudeMinimumMaximumImageCalculator<TInputImage, TMaskImage>
::Compute(void)
{
  m_Maximum = NumericTraits<RealType>::NonpositiveMin();
  m_Minimum = NumericTraits<RealType>::max();
  
  //if ( m_MaskImage == NULL ) { return; }

  ImageRegionConstIteratorWithIndex<TInputImage> 
    it(m_Image, m_Image->GetRequestedRegion());
  
  ImageRegionConstIteratorWithIndex<TMaskImage> 
    maskIt(m_MaskImage, m_MaskImage->GetRequestedRegion());
  
  RealType average = NumericTraits<RealType>::ZeroValue();
  int k = 0;

  while( !it.IsAtEnd() ) {
    const MaskImagePixelType maskValue = maskIt.Get();
    if ( maskValue > NumericTraits<MaskPixelType>::ZeroValue() ) {
      const PixelType value = it.Get();  
      RealType c = NumericTraits<RealType>::ZeroValue();
      for ( int i = 0; i < VectorDimension; i++ ) {
        c += value[i] * value[i];
      }

      c = vcl_sqrt(c);

      
      if ( c > m_Venc ) {
        average += c;
        k++;
        m_Maximum = average / k;
        m_IndexOfMaximum = it.GetIndex();
      }
      
      if (c < m_Minimum) {
        m_Minimum = c;
        m_IndexOfMinimum = it.GetIndex();
      }

      
      if ( maskIt.IsAtEnd() ) {
        maskIt.GoToBegin();
      }
    }
    ++it; ++maskIt;
  }

}

template<class TInputImage, class TMaskImage>
void VectorMagnitudeMinimumMaximumImageCalculator<TInputImage, TMaskImage>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Minimum: "
     << static_cast<typename NumericTraits<RealType>::PrintType>(m_Minimum)
     << std::endl;
  os << indent << "Maximum: "
     << static_cast<typename NumericTraits<RealType>::PrintType>(m_Maximum)
     << std::endl;
  os << indent << "Index of Minimum: " << m_IndexOfMinimum << std::endl;
  os << indent << "Index of Maximum: " << m_IndexOfMaximum << std::endl;
  os << indent << "Image: " << std::endl;
  m_Image->Print(os, indent.GetNextIndent());
  os << indent << "Region: " << std::endl;
  m_Region.Print(os,indent.GetNextIndent());
  os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}

} // end namespace itk

#endif
