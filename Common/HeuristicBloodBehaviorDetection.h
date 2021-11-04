#include <iostream>
#include <string>
#include <vector>

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkOffset.h"
#include "vnl/vnl_math.h"
#include "itkConstNeighborhoodIterator.h"
#include "Common/itkImageTypes.h"

struct MyVector 
{
  float X;
  float Y;
  float Z;
  float Magnitude;

  void CalculateMagnitude() 
  {
    Magnitude = GetMagnitude();
  }

  float GetMagnitude () 
  {
    return (vcl_sqrt(X*X + Y*Y + Z*Z));
  }

  MyVector operator-(const MyVector &v)
  {
    MyVector* result = new MyVector;

    result->X = X - v.X;
    result->Y = Y - v.Y;
    result->Z = Z - v.Z;
    result->CalculateMagnitude();

    return *result;
  }
  
  friend std::ostream & operator<<( std::ostream &out, const MyVector& v )
  {
    out << "(" << v.X << "," << v.Y << "," << v.Z << ")";
    return out;
  }

  float operator[](const unsigned int index) 
  {
    switch ( index ) {
      case 1: return X; break;
      case 2: return Y; break;
      case 3: return Z; break;
      default: return Magnitude;
    }
  }

  void ToArray(float a[]) 
  {
    a[0] = X;
    a[1] = Y;
    a[2] = Z;
    a[3] = Magnitude;

  }

};

struct MyVector2D 
{
  float X;
  float Y;

  MyVector2D() {}

  MyVector2D(float x, float y)
  {
    X = x; Y = y;
  }
};

using namespace itk;

typedef std::vector< std::string > SeriesIdContainer;
typedef std::vector< std::string > FileNamesContainer;

typedef std::vector< Scalar3DImageType::Pointer > Scalar3DImageTypeList;

/*
typedef itk::TileImageFilter < Scalar3DImageType, Scalar4DImageType >  TileFilterType;
typedef itk::ImageSeriesReader< Scalar3DImageType > ReaderType;
typedef itk::GDCMImageIO                            DICOMIOType;
typedef itk::GDCMSeriesFileNames                    NamesGeneratorType;
*/

Scalar3DImageType::Pointer MaximalComponentStDev(
  Scalar3DImageType::Pointer noiseMask, Vector4DImageType::Pointer input);


//Todas estas funciones se ván o se quedan en el .cpp
Scalar3DImageType::Pointer CreateScalar3DImageFromV4DImage(Vector4DImageType::Pointer input);
void CalculateLocalDeviation(float result[4], float a[4], float b[4]);
void ArrayMaximize(float a[4], float b[4]);
void ArrayAssing(float a[4], float b[4]); 
MyVector GetVectorFrom4DRayIndex(int i, ConstNeighborhoodIterator<Vector4DImageType> in);

Scalar3DImageType::Pointer CurveSmoothnes(
  Scalar3DImageType::Pointer noiseMask, Vector4DImageType::Pointer input);

Scalar3DImageType::Pointer MaximalComponentStDevWithCurveSmoothnes(
  Scalar3DImageType::Pointer noiseMask, Vector4DImageType::Pointer input);
