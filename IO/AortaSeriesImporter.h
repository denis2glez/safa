#ifndef AORTASERIESIMPORTER_H_
#define AORTASERIESIMPORTER_H_

#include <string>
#include <cstdlib>
#include <strstream>           //stringstream ss;
#include <itksys/Directory.hxx>
#include "IO/DICOMScalar4DImageLoader.h"
#include "IO/DICOMHeaderReader.h"


using namespace std;
/**********************************************************************************************
 * Clase que se encarga de importar una serie de imágenes y convertirlas en un estudio.
 **********************************************************************************************/
class AortaSeriesImporter {
public:
	string _inputPath;
	string _outputPath;
	string _progressObj;
	int _minGray;
	int _maxGray;
	int _venc;
	int _timeStps;
	string _seriesHeader;
	int _vectorMagMin;
	int _vectorMagMax;

	AortaSeriesImporter();
	~AortaSeriesImporter();

	inline void _SetNumberOfTimeSteps(int value) {
		_timeStps = value;
	}
	inline void SetInputPath(const char* value) {
		_inputPath = string(value);
	}
	inline void SetOutputPath(const char* value) {
		_outputPath = string(value);
	}
	inline void SetVelocityEncoding(int value) {
		_venc = value;
	}
	inline void SetProgressObject(const char* value) {
		_progressObj = string(value);
	}

	Scalar3DImageType::Pointer _BuildMask (Scalar3DImageType::Pointer input);

	Scalar3DImageType::Pointer _ProcessMagnitudeImage (Scalar4DImageType::Pointer img, int nPhase);

	void _ProcessPhaseImage (Scalar3DImageType::Pointer mask, Vector4DImageType::Pointer img, int nPhase);

	void Update();
};


#endif /* AORTASERIESIMPORTER_H_ */
