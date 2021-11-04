/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVelocityEncodingImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-17 20:50:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVelocityEncodingImageFilter_txx
#define __itkVelocityEncodingImageFilter_txx

#include "itkVelocityEncodingImageFilter.h"

namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
VelocityEncodingImageFilter<TInputImage, TOutputImage>
::VelocityEncodingImageFilter()
{
  m_VelocityAdquisition = static_cast<InputRealType>(150);
  m_MaxGrayLevel = static_cast<InputRealType>(4092);
  m_MinGrayLevel = static_cast<InputRealType>(-4096);
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
VelocityEncodingImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Velocity of adquisition: "
     << static_cast<typename NumericTraits<InputRealType>::PrintType>(m_VelocityAdquisition)
     << std::endl;
  os << indent << "Max grey value: "
     << static_cast<typename NumericTraits<InputRealType>::PrintType>(m_MaxGrayLevel)
     << std::endl;
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void 
VelocityEncodingImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  // set up the functor values
  this->GetFunctor().SetVelocityAdquisition(m_VelocityAdquisition);
  this->GetFunctor().SetMaxGrayLevel(m_MaxGrayLevel);
  this->GetFunctor().SetMinGrayLevel(m_MinGrayLevel);
}


} // end namespace itk

#endif
