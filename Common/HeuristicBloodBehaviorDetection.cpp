
#include "Common/HeuristicBloodBehaviorDetection.h"

Scalar3DImageType::Pointer CreateScalar3DImageFromV4DImage(Vector4DImageType::Pointer input) 
{
  //printf("CreateScalar3DImageFromV4DImage BEGIN");
  // Allocate output
  Scalar3DImageType::Pointer output = Scalar3DImageType::New();

  Vector4DImageType::IndexType inIndex = input->GetRequestedRegion().GetIndex();
  Vector4DImageType::SizeType inSize = input->GetRequestedRegion().GetSize();

  Scalar3DImageType::IndexType outIndex;
  outIndex[0] = inIndex[0]; outIndex[1] = inIndex[1]; outIndex[2] = inIndex[2];

  //printf("CreateScalar3DImageFromV4DImage::InIndex %f, %f, %f, %f", inIndex[0], inIndex[1], inIndex[2], inIndex[3]);

  Scalar3DImageType::SizeType outSize;
  outSize[0] = inSize[0]; outSize[1] = inSize[1]; outSize[2] = inSize[2];

  //printf("CreateScalar3DImageFromV4DImage::InSize %f, %f, %f, %f", inSize[0], inSize[1], inSize[2], inSize[3]);

  Scalar3DImageType::RegionType region;
  region.SetIndex(outIndex);
  region.SetSize(outSize);
  output->SetRegions(region);

  //Spacing
  Scalar3DImageType::SpacingType outSpacing; 
  Vector4DImageType::SpacingType inSpacing = input->GetSpacing();
  outSpacing[0] = inSpacing[0]; outSpacing[1] = inSpacing[1]; outSpacing[2] = inSpacing[2];
  output->SetSpacing(outSpacing);

  //Origin
  Scalar3DImageType::PointType outOrigin; 
  Vector4DImageType::PointType inOrigin = input->GetOrigin();
  outOrigin[0] = inOrigin[0]; outOrigin[1] = inOrigin[1]; outOrigin[2] = inOrigin[2];
  output->SetOrigin(outOrigin);

  //Vamos a alocarnos!!!
  output->Allocate();

  //printf("CreateScalar3DImageFromV4DImage END");

  return output;
}


/*
Calcula el outlayer como la distancia entre 2 puntos y lo almacena en result.

Existen simplificaciones debido a que se conocen las cordenadas Y == 0 y 1
*/
void CalculateLocalDeviation(float result[4], float a[4], float b[4])
{
  result[0] = a[0] - b[0];
  result[0] = vcl_sqrt(result[0]*result[0] - 1);
  result[1] = a[1] - b[1];
  result[1] = vcl_sqrt(result[1]*result[1] - 1);
  result[2] = a[2] - b[2];
  result[2] = vcl_sqrt(result[2]*result[2] - 1);
  result[3] = a[3] - b[3];
  result[3] = vcl_sqrt(result[3]*result[3] - 1);
}


void ArrayMaximize(float a[4], float b[4]) 
{
  if (a[0] < b[0]) { a[0] = b[0]; }
  if (a[1] < b[1]) { a[1] = b[1]; }
  if (a[2] < b[2]) { a[2] = b[2]; }
  if (a[3] < b[3]) { a[3] = b[3]; }
}


void ArrayAssing(float a[4], float b[4]) 
{
  a[0] = b[0];
  a[1] = b[1];
  a[2] = b[2];
  a[3] = b[3];
}


MyVector GetVectorFrom4DRayIndex(int i, ConstNeighborhoodIterator<Vector4DImageType> in) 
{
  ConstNeighborhoodIterator<Vector4DImageType>::OffsetType offset;
  offset[0] = 0; offset[1] = 0; offset[2] = 0; offset[3] = i;
  Vector4DImageType::PixelType vector = in.GetPixel(offset);
  MyVector result;
  result.X = vector[0]; result.Y = vector[1];
  result.Z = vector[2]; result.CalculateMagnitude();
  
  /*
  ConstNeighborhoodIterator<Vector4DImageType>::IndexType index = in.GetIndex();
  
  if ( index[0] == 80 || index[1] == 82 || index[2] == 10 ) {
    itkGenericOutputMacro(<<"INDICE"<<i<<"  Vector="<<result.Magnitude<<","<<result.X << "," << result.Y <<"," << result.Z <<"\n");
  }*/

  return result;
}


//Area = abs((xB*yA-xA*yB)+(xC*yB-xB*yC)+(xA*yC-xC*yA))/2
float TriangleArea(const MyVector2D & A, 
  const MyVector2D & B, const MyVector2D & C)
{
  return abs((B.X * A.Y - A.X * B.Y)+ (C.X * B.Y - B.X * C.Y) + (A.X * C.Y - C.X * A.Y)) / 2;
}
/*

*/

//Devuelve la intersecciónd e dos segmentos
enum IntersectResult { PARALLEL, COINCIDENT, NOT_INTERESECTING, INTERESECTING };

IntersectResult Intersect(const MyVector2D & v1, const MyVector2D & v2,
  const MyVector2D & v3, const MyVector2D & v4, MyVector2D& intersection)
{
    float denom = ((v4.Y - v3.Y)*(v2.X - v1.X)) -
                  ((v4.X - v3.X)*(v2.Y - v1.Y));

    float nume_a = ((v4.X - v3.X)*(v1.Y - v3.Y)) -
                   ((v4.Y - v3.Y)*(v1.X - v3.X));

    float nume_b = ((v2.X - v1.X)*(v1.Y - v3.Y)) -
                   ((v2.Y - v1.Y)*(v1.X - v3.X));

    if(denom == 0.0f)
    {
        if(nume_a == 0.0f && nume_b == 0.0f)
        {
            return COINCIDENT;
        }
        return PARALLEL;
    }

    float ua = nume_a / denom;
    float ub = nume_b / denom;

    if(ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)
    {
        // Get the intersection point.
        intersection.X = v1.X + ua*(v2.X - v1.X);
        intersection.Y = v1.Y + ua*(v2.Y - v1.Y);

        return INTERESECTING;
    }

    return NOT_INTERESECTING;
}



Scalar3DImageType::Pointer MaximalComponentStDev(
  Scalar3DImageType::Pointer noiseMask, Vector4DImageType::Pointer input)
{
  Scalar3DImageType::Pointer stDev = CreateScalar3DImageFromV4DImage(input);
  
  //Ahora iteraremos sólo por el frame 0, o sea, necesitamos 
  //una región que recoja el 1er volumen nada más
  Vector4DImageType::IndexType inIndex = input->GetRequestedRegion().GetIndex();
  Vector4DImageType::SizeType inSize = input->GetRequestedRegion().GetSize();

  //itkGenericOutputMacro(<<"INPUT SIZE ="<<inSize[0]<<","<<inSize[1]<< ","<<inSize[2]<<","<<inSize[3]<<"\n");

  Vector4DImageType::SizeType volSize;
  volSize[0] = inSize[0]; volSize[1] = inSize[1]; volSize[2] = inSize[2]; volSize[3] = 1;
 
  Vector4DImageType::RegionType volume1;
  volume1.SetIndex(inIndex); 
  volume1.SetSize(volSize);

  //REGION DE SALIDA... que es igual al primer volumen de la imagen 4D
  Vector4DImageType::SizeType radius;
  radius[0] = 0; radius[1] = 0; radius[2] = 0; radius[3] = inSize[3];

  ConstNeighborhoodIterator<Vector4DImageType> in(radius, input, volume1);
  ImageRegionConstIterator<Scalar3DImageType> inMask(noiseMask, noiseMask->GetRequestedRegion());
  ImageRegionIterator<Scalar3DImageType> outStDev(stDev, stDev->GetRequestedRegion());

  in.GoToBegin(); outStDev.GoToBegin(); inMask.GoToBegin();

  while ( ! outStDev.IsAtEnd() ) {
    Scalar3DImageType::PixelType maskVal = inMask.Get();

    if ( maskVal > NumericTraits<Scalar3DImageType::ValueType>::Zero ) {
      float maxStDev = -1.0;

      //Media del rayo 4D
      float mean = 0;   float meanX = 0;   float meanY = 0;   float meanZ = 0;
      float error = 0;  float errorX = 0;  float errorY = 0;  float errorZ = 0;
      float sseX = 0;   float sseY = 0;    float sseZ = 0;    float sse  = 0;

      for (unsigned int i = 0; i < radius[3]; ++i) {
        MyVector v = GetVectorFrom4DRayIndex(i, in);
        mean  += v.Magnitude;
        meanX += v.X; meanY += v.Y; meanZ += v.Z;
  
      }
      
      mean /= radius[3];
      meanX /= radius[3];
      meanY /= radius[3];
      meanZ /= radius[3];

      for (unsigned int i = 0; i < radius[3]; ++i) {
        MyVector v = GetVectorFrom4DRayIndex(i, in);
        error = v.Magnitude - mean;
        errorX = v.X - meanX;
        errorY = v.Y - meanY;
        errorZ = v.Z - meanZ;
        sse += static_cast<float>( error * error );
        sseX += static_cast<float>( errorX * errorX );
        sseY += static_cast<float>( errorY * errorY );
        sseZ += static_cast<float>( errorZ * errorZ );
      }

      //Encontrar std Máxima
      sse /= radius[3]; sseX /= radius[3]; sseY /= radius[3]; sseZ /= radius[3];

      maxStDev = sse;
      if ( maxStDev < sseX ) { maxStDev = sseX; }
      if ( maxStDev < sseY ) { maxStDev = sseY; }
      if ( maxStDev < sseZ ) { maxStDev = sseZ; }

      outStDev.Set(static_cast<float>(maxStDev));
    } 
    else {
      outStDev.Set(0);
    }

    ++in; ++inMask; ++outStDev; 
  }

  return stDev;
}


//Calcular curvatura de la curva
float CalculateCurvatureSmoothnes(float * magArray, int size)
{

  float minMag = 0.0f;
  minMag = vnl_huge_val(minMag);
  float maxMag = -vnl_huge_val(minMag);
  for ( unsigned int i = 0; i < size - 1; i++ ) {
    if ( magArray[i] > maxMag ) { maxMag = magArray[i]; } 
    if ( magArray[i] < minMag ) { minMag = magArray[i]; } 
  }  

  float * normArr = new float[size];

  float dynRange = maxMag - minMag;
  for ( unsigned int i = 0; i < size - 1; i++ ) {
    normArr[i] = (magArray[i] - minMag) / dynRange;
  }

  float area = 0.0;
  for ( unsigned int i = 0; i < size - 3; i++ ) {
    area += TriangleArea(
      MyVector2D(normArr[i], i),
      MyVector2D(normArr[i + 1], i + 1),
      MyVector2D(normArr[i + 2], i + 2));
    if ( i > 0 ) {
    }
  }

  for ( unsigned int i = 0; i < size - 4; i++ ) {

    MyVector2D vIntersec;

    MyVector2D v1 = MyVector2D(normArr[i], i);
    MyVector2D v2 = MyVector2D(normArr[i + 1], i + 1);
    MyVector2D v3 = MyVector2D(normArr[i + 2], i + 2);
    MyVector2D v4 = MyVector2D(normArr[i + 3], i + 3);

    if ( Intersect(v1, v2, v3, v4, vIntersec) == INTERESECTING ) {
      area -= TriangleArea(v2, v3, vIntersec);
    }
  }

  return area;
}
/*

*/
Scalar3DImageType::Pointer CurveSmoothnes(
  Scalar3DImageType::Pointer noiseMask, Vector4DImageType::Pointer input)
{
  Scalar3DImageType::Pointer stDev = CreateScalar3DImageFromV4DImage(input);
  
  //Ahora iteraremos sólo por el frame 0, o sea, necesitamos 
  //una región que recoja el 1er volumen nada más
  Vector4DImageType::IndexType inIndex = input->GetRequestedRegion().GetIndex();
  Vector4DImageType::SizeType inSize = input->GetRequestedRegion().GetSize();

  Vector4DImageType::SizeType volSize;
  volSize[0] = inSize[0]; volSize[1] = inSize[1]; volSize[2] = inSize[2]; volSize[3] = 1;
 
  Vector4DImageType::RegionType volume1;
  volume1.SetIndex(inIndex); 
  volume1.SetSize(volSize);

  //REGION DE SALIDA... que es igual al primer volumen de la imagen 4D
  Vector4DImageType::SizeType radius;
  radius[0] = 0; radius[1] = 0; radius[2] = 0; radius[3] = inSize[3];

  ConstNeighborhoodIterator<Vector4DImageType> in(radius, input, volume1);
  ImageRegionConstIterator<Scalar3DImageType> inMask(noiseMask, noiseMask->GetRequestedRegion());
  ImageRegionIterator<Scalar3DImageType> outStDev(stDev, stDev->GetRequestedRegion());

  in.GoToBegin(); outStDev.GoToBegin(); inMask.GoToBegin();

  while ( ! outStDev.IsAtEnd() ) {
    Scalar3DImageType::PixelType maskVal = inMask.Get();

    if ( maskVal > NumericTraits<Scalar3DImageType::ValueType>::Zero ) {

      float * magArray;
      magArray = new float[radius[3]];

      for (unsigned int i = 0; i < radius[3]; ++i) {
        MyVector v = GetVectorFrom4DRayIndex(i, in);
        magArray[i] = v.Magnitude;
  
      }

      //Calcular la Dureza para este píxel
      float D = CalculateCurvatureSmoothnes(magArray, radius[3]);
      outStDev.Set(static_cast<float>(D));
    } 
    else {
      outStDev.Set(0);
    }

    ++in; ++inMask; ++outStDev; 
  }

  return stDev;
}


/*

*/
Scalar3DImageType::Pointer MaximalComponentStDevWithCurveSmoothnes(
  Scalar3DImageType::Pointer noiseMask, Vector4DImageType::Pointer input)
{
  Scalar3DImageType::Pointer stDev = CreateScalar3DImageFromV4DImage(input);
  
  //Ahora iteraremos sólo por el frame 0, o sea, necesitamos 
  //una región que recoja el 1er volumen nada más
  Vector4DImageType::IndexType inIndex = input->GetRequestedRegion().GetIndex();
  Vector4DImageType::SizeType inSize = input->GetRequestedRegion().GetSize();

  //itkGenericOutputMacro(<<"INPUT SIZE ="<<inSize[0]<<","<<inSize[1]<< ","<<inSize[2]<<","<<inSize[3]<<"\n");

  Vector4DImageType::SizeType volSize;
  volSize[0] = inSize[0]; volSize[1] = inSize[1]; volSize[2] = inSize[2]; volSize[3] = 1;
 
  Vector4DImageType::RegionType volume1;
  volume1.SetIndex(inIndex); 
  volume1.SetSize(volSize);

  //REGION DE SALIDA... que es igual al primer volumen de la imagen 4D
  Vector4DImageType::SizeType radius;
  radius[0] = 0; radius[1] = 0; radius[2] = 0; radius[3] = inSize[3];

  ConstNeighborhoodIterator<Vector4DImageType> in(radius, input, volume1);
  ImageRegionConstIterator<Scalar3DImageType> inMask(noiseMask, noiseMask->GetRequestedRegion());
  ImageRegionIterator<Scalar3DImageType> outStDev(stDev, stDev->GetRequestedRegion());

  in.GoToBegin(); outStDev.GoToBegin(); inMask.GoToBegin();

  while ( ! outStDev.IsAtEnd() ) {
    Scalar3DImageType::PixelType maskVal = inMask.Get();

    if ( maskVal > NumericTraits<Scalar3DImageType::ValueType>::Zero ) {
      float maxStDev = -1.0;

      //Media del rayo 4D
      float mean = 0;   float meanX = 0;   float meanY = 0;   float meanZ = 0;
      float error = 0;  float errorX = 0;  float errorY = 0;  float errorZ = 0;
      float sseX = 0;   float sseY = 0;    float sseZ = 0;    float sse  = 0;

      float * magArray;
      magArray = new float[radius[3]];

      for (unsigned int i = 0; i < radius[3]; ++i) {
        MyVector v = GetVectorFrom4DRayIndex(i, in);
        mean  += v.Magnitude;
        meanX += v.X; meanY += v.Y; meanZ += v.Z;
        magArray[i] = v.Magnitude;
  
      }
      
      mean /= radius[3];
      meanX /= radius[3];
      meanY /= radius[3];
      meanZ /= radius[3];

      for (unsigned int i = 0; i < radius[3]; ++i) {
        MyVector v = GetVectorFrom4DRayIndex(i, in);
        error = v.Magnitude - mean;
        errorX = v.X - meanX;
        errorY = v.Y - meanY;
        errorZ = v.Z - meanZ;
        sse += static_cast<float>( error * error );
        sseX += static_cast<float>( errorX * errorX );
        sseY += static_cast<float>( errorY * errorY );
        sseZ += static_cast<float>( errorZ * errorZ );
      }

      //Encontrar std Máxima
      sse /= radius[3]; sseX /= radius[3]; sseY /= radius[3]; sseZ /= radius[3];

      maxStDev = sse;
      if ( maxStDev < sseX ) { maxStDev = sseX; }
      if ( maxStDev < sseY ) { maxStDev = sseY; }
      if ( maxStDev < sseZ ) { maxStDev = sseZ; }

      //Calcular la Dureza para este píxel
      float D = CalculateCurvatureSmoothnes(magArray, radius[3]);
      outStDev.Set(static_cast<float>(maxStDev / D));
    } 
    else {
      outStDev.Set(0);
    }

    ++in; ++inMask; ++outStDev; 
  }

  return stDev;
}
