#ifndef AORTAUTILITY_H_
#define AORTAUTILITY_H_

#include "Common/itkImageTypes.h"
#include "itkExtractImageFilter.h"
#include "itkVTKImageIO.h"
#include "itkImageFileWriter.h"

typedef itk::ImageFileWriter<Scalar3DImageType> WriterScalar3DType;

Scalar3DImageType::Pointer ExtractScalar3DImageFromScalar4DImage(
		const Scalar4DImageType *img4D, int it_idx);
Vector3DImageType::Pointer ExtractVector3DImageFromVector4DImage(
		const Vector4DImageType *img4D, int it_idx);

void VTKScalarImageWriter(const char* fileName,	const Scalar3DImageType::Pointer img3D);

#endif /* AORTAUTILITY_H_ */
