#include "Common/itkImageTypes.h"
#include "itkImageFileWriter.h"

/**
 * Escribe una imagen a archivo
 */
void WriteScalar3DImageTypeToFile(Scalar3DImageType::Pointer outImg, char* fileName);

/**
 * Escribe una imagen a archivo
 */
void WriteInteger3DImageTypeToFile(Integer3DImageType::Pointer outImg, char* fileName);
