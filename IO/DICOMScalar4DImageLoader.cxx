#include "IO/DICOMScalar4DImageLoader.h"
#include "Common/itkDICOMTags.h"

DICOMScalar4DImageLoader::DICOMScalar4DImageLoader() {
	TileFilterType::LayoutArrayType layout;
	layout[0] = 1;
	layout[1] = 1;
	layout[2] = 1;
	layout[3] = 0;
	this->m_Tiler = TileFilterType::New();
	this->m_Tiler->SetLayout(layout);
	this->m_progress = 0;
	this->m_NumberOfTimeStepsToRead = 0;
}

void DICOMScalar4DImageLoader::SetDirectory(std::string const &dirName) {
	// A FileName Generator is needed in order to read the files in the right order
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	//nameGenerator->AddSeriesRestriction("0008|0021");
	if (!this->GetNumberOfTimeStepsToRead()) {
		nameGenerator->SetUseSeriesDetails(true);
		nameGenerator->AddSeriesRestriction(DCM_DATE_TAG);
		// nameGenerator->AddSeriesRestriction("0018|1060");
		nameGenerator->AddSeriesRestriction(DCM_TIMESTEP_TAG);
		// nameGenerator->AddSeriesRestriction("0020|0012");
		nameGenerator->AddSeriesRestriction(DCM_COMPONENT_TAG);
	}
	nameGenerator->SetDirectory(dirName);

	// Type of image reader (DICOM Reader).
	DICOMIOType::Pointer dicomIO = DICOMIOType::New();
	// Array of pointer to readers
	//ReaderType::Pointer reader[seriesUID.size()];

	int value = 0;
	progressMessage->setText("Loading magnitude images...");
	progressBar->setValue(value);
	progressBar->setVisible(true);


	if (this->GetNumberOfTimeStepsToRead()) {
		// Read the full list of file names that will be used in case no
		// automatic series detection
		const FileNamesContainer& allFileNames =
				nameGenerator->GetFileNames("");
		size_t maxTimeStep = this->GetNumberOfTimeStepsToRead();
		size_t totalSeries = allFileNames.size();
		if (totalSeries % maxTimeStep) {
			DICOMScalar4DImageLoaderException( "Number of series does not match number of time step requested" );
		}
		size_t size3D = totalSeries / maxTimeStep;
		FileNamesContainer fileNames(size3D);
		for (size_t t = 0, firstSlice = 0; t < maxTimeStep; t++) {
			// std::copy does not work for me as expected
			for (int i = size3D - 1; i >= 0; i--) {
				fileNames[i] = allFileNames[firstSlice++];
			}
			ReaderType::Pointer reader = ReaderType::New();
			reader->SetFileNames(fileNames);
			this->m_Readers.push_back(reader);
			// Connect the reader with the tiler
			this->m_Tiler->SetInput(t, reader->GetOutput());

		}
	} else {
		// Read in the directory given and get the number of series
		const SeriesIdContainer &seriesUID = nameGenerator->GetSeriesUIDs();
		SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
		SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();

		if (!seriesUID.size()) {
			DICOMScalar4DImageLoaderException( "no series were found");
		}
		int t = 0; // Iteration number.
		while (seriesItr != seriesEnd) {
			// Create a reader for the actual serie
			ReaderType::Pointer reader = ReaderType::New();

			// Set file names of new iteration
			std::string seriesIdentifier = seriesItr->c_str();
			const FileNamesContainer &fileNames = nameGenerator->GetFileNames(
					seriesIdentifier);
			reader->SetFileNames(fileNames);
			this->m_Readers.push_back(reader);
			// Connect the reader with the tiler
			this->m_Tiler->SetInput(t, reader->GetOutput());

			// Change series and iteration number
			t++;
			seriesItr++;

			progressBar->setValue(33*t);
		}
	}
	progressBar->setVisible(false);
	progressMessage->setText("Ready");
}

void DICOMScalar4DImageLoader::Update() {
	this->m_Tiler->Update();
}
