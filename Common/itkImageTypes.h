#ifndef _ITK_IMAGE_TYPES_H
#define _ITK_IMAGE_TYPES_H

#include "itkVariableLengthVector.h"
#include "itkImage.h"
#include <itkImageToImageFilter.h>
#include <itkSymmetricSecondRankTensor.h>

// Dimension
const unsigned int  Dim3D = 3;
const unsigned int  Dim4D = 4;

// PixelTypes
typedef float                                  PixelType;
typedef unsigned char                          MaskPixelType;

// VectorTypes
typedef itk::VariableLengthVector< PixelType > VPixelType;
typedef itk::Vector< PixelType, 3 >            VPixel3Type;

// TensorTypes
typedef itk::SymmetricSecondRankTensor< float, Dim3D > TensorPixelType;

// ImageTypes
typedef itk::Image< int, Dim3D >             Integer3DImageType;
typedef itk::Image< PixelType, Dim3D >       Scalar3DImageType;
typedef itk::Image< PixelType, Dim4D >       Scalar4DImageType;
typedef itk::Image< VPixel3Type, Dim3D >     Vector3DImageType;
typedef itk::Image< VPixel3Type, Dim4D >     Vector4DImageType;
typedef itk::Image< MaskPixelType, Dim3D >   MaskScalar3DImageType;
typedef itk::Image< TensorPixelType, Dim3D > Tensor3DImageType;

// FilterType

//typedef itk::ImageToImageFilter<


#endif
