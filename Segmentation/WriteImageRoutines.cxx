#include "WriteImageRoutines.h"
/**
 * Escribe una imagen a archivo
 */
void WriteScalar3DImageTypeToFile(Scalar3DImageType::Pointer outImg, char* fileName) 
{
  typedef itk::ImageFileWriter<Scalar3DImageType> WriterType;

  try {
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(outImg);
    writer->SetFileName(fileName);
    writer->Update();
  }
  catch( itk::ExceptionObject & excep ) {
    std::cerr << "Exception catched !" << std::endl;
    std::cerr << excep << std::endl;
  }
}

/**
 * Escribe una imagen a archivo
 */
void WriteInteger3DImageTypeToFile(Integer3DImageType::Pointer outImg, char* fileName) 
{
  typedef itk::ImageFileWriter<Integer3DImageType> WriterType;

  try {
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(outImg);
    writer->SetFileName(fileName);
    writer->Update();
  }
  catch( itk::ExceptionObject & excep ) {
    std::cerr << "Exception catched !" << std::endl;
    std::cerr << excep << std::endl;
  }
}