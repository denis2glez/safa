/** \file TestAll.cxx
 * Este es el archivo fuente que ejecuta todos las pruebas existentes en el directorio Test.
 */
#include "Test/TestDICOMVectorial3DImageLoader.h"
#include "Test/TestDICOMScalar4DImageLoader.h"
#include "Test/TestDICOMVectorial4DImageLoader.h"
#include "Test/TestAF4DCenterLine.h"

int main(int argc, char ** argv){
	TestDICOMScalar4DImageLoader();
	return 0;
}

