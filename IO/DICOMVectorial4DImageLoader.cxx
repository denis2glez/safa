//#include "itkReaderEvents.h"
#include "IO/DICOMVectorial4DImageLoader.h"
#include "Common/itkDICOMTags.h"

DICOMVectorial4DImageLoader::DICOMVectorial4DImageLoader() {
	// We need to read the xyz coordinates of the velocity. Each component should be stored in a 4D image
	// Therefore we need: 3 tiler
	tilerx = TileFilterType::New();
	tilery = TileFilterType::New();
	tilerz = TileFilterType::New();

	// Prepare the all the objects necessary within the loop
	// Tiler attributes
	TileFilterType::LayoutArrayType layout;
	layout[0] = 1;
	layout[1] = 1;
	layout[2] = 1;
	layout[3] = 0;
	tilerx->SetLayout(layout);
	tilery->SetLayout(layout);
	tilerz->SetLayout(layout);

	vectorizer = Compose3DVectorImageType::New();
}

void DICOMVectorial4DImageLoader::SetDirectory(std::string const &dirName) {
	int value = 0;
	progressMessage->setText("Loading flow images...");
	progressBar->setValue(value++);
	progressBar->setVisible(true);
	// A FileName Generator is needed in order to read the files in the right order
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	nameGenerator->SetUseSeriesDetails(true);
	// First the acquisition type (component x = 1, y = 2, z = 3)
	nameGenerator->AddSeriesRestriction("0020|0012"); // Acquision number (component)
	nameGenerator->AddSeriesRestriction("0008|0032"); // Time serie (iteration important)
	nameGenerator->SetDirectory(dirName);

	// Read in the directory given and get the number of series
	const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
	if (!seriesUID.size()) {
		std::cerr << "No series were found\n";
		return;
	}

	// Create a DICOM tagreader
	ReaderType::Pointer tagreader = ReaderType::New();
	DICOMIOType::Pointer dicomIO = DICOMIOType::New();
	tagreader->SetImageIO(dicomIO);

	//Dictionary
	const DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
	DictionaryType::ConstIterator dictItr = dictionary.Begin();
	DictionaryType::ConstIterator dictEnd = dictionary.End();

	// Loop over the serieIdentifier
	SeriesIdContainer::const_iterator seriesItr = seriesUID.begin(); // Initialization of the loop
	SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
	int t1 = 0; // Iteration number for x component.
	int t2 = 0; // Iteration number for y component.
	int t3 = 0; // Iteration number for z component.
	while (seriesItr != seriesEnd) {
		// Set file names of new iteration
		std::string seriesIdentifier;
		seriesIdentifier = seriesItr->c_str();
		FileNamesContainer fileNames = nameGenerator->GetFileNames(
				seriesIdentifier);

		// Read the tag of the first image. Necessary in order to get the iteration number and component (xyz)
		FileNamesContainer::const_iterator fileNamesItr = fileNames.begin();
		tagreader->SetFileNames(fileNames);
		// read Image
		try {
			tagreader->Update();
		} catch (itk::ExceptionObject &ex) {
			std::cout << "exception catch: Cannot read image and/or image\n";
			return;
		}

		////////////////////////////////////////
		// Not needed only for debug purposes
		////////////////////////////////////////
		// Using the tagreader and dictionary, Get the component of the image serie (x,y or z)
		std::string reftag = "0008|0032";
		DictionaryType::ConstIterator tagItr = dictionary.Find(reftag);
		if (tagItr != dictEnd) {
			MetaDataStringType::ConstPointer
					entryvalue =
							dynamic_cast<const MetaDataStringType *> (tagItr->second.GetPointer());
			if (entryvalue) {
				std::string tagvalue = entryvalue->GetMetaDataObjectValue();
			}
		}

		// Using the tagreader and dictionary, Get the time of image serie
		int component = 0;
		reftag = "0020|0012";
		tagItr = dictionary.Find(reftag);
		if (tagItr != dictEnd) {
			MetaDataStringType::ConstPointer
					entryvalue =
							dynamic_cast<const MetaDataStringType *> (tagItr->second.GetPointer());
			if (entryvalue) {
				std::string tagvalue = entryvalue->GetMetaDataObjectValue();
				component = atoi(tagvalue.c_str());
			}
		}

		if (component == 1) {
			// Create a reader for the actual serie
			ReaderType::Pointer reader = ReaderType::New();
			// Set file names of new iteration
			std::string seriesIdentifier = seriesItr->c_str();
			FileNamesContainer fileNames = nameGenerator->GetFileNames(
					seriesIdentifier);
			reader->SetFileNames(fileNames);
			reader->Register();
			// Connect the reader with the tiler
			tilerx->SetInput(t1, reader->GetOutput());
			t1++;
			seriesItr++;
			progressBar->setValue(12*value++);
		} else if (component == 2) {
			// Create a reader for the actual serie
			ReaderType::Pointer reader = ReaderType::New();
			// Set file names of new iteration
			std::string seriesIdentifier = seriesItr->c_str();
			FileNamesContainer fileNames = nameGenerator->GetFileNames(
					seriesIdentifier);
			reader->SetFileNames(fileNames);
			reader->Register();
			// Connect the reader with the tiler
			tilery->SetInput(t2, reader->GetOutput());
			t2++;
			seriesItr++;
			progressBar->setValue(12*value++);
		} else if (component == 3) {
			// Create a reader for the actual serie
			ReaderType::Pointer reader = ReaderType::New();
			// Set file names of new iteration
			std::string seriesIdentifier = seriesItr->c_str();
			FileNamesContainer fileNames = nameGenerator->GetFileNames(
					seriesIdentifier);
			reader->SetFileNames(fileNames);
			reader->Register();
			// Connect the reader with the tiler
			tilerz->SetInput(t3, reader->GetOutput());
			t3++;
			seriesItr++;

			progressBar->setValue(12*value++);
		} else {
			std::cout
					<< "The component tag was not found or it has a non legal value\n";
			std::cout
					<< "This will probably lead to an error when reading the images.\n";
			seriesItr++;
		}
	}
	// Update all the images
	tilerx->Update();
	tilery->Update();
	tilerz->Update();

	vectorizer->SetInput2(tilerx->GetOutput());
	vectorizer->SetInput1(tilery->GetOutput());
	vectorizer->SetInput3(tilerz->GetOutput());

	progressBar->setVisible(false);
	progressMessage->setText("Ready");
}
