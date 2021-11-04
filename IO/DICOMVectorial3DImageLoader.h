#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkCompose3DVectorImageFilter.h" 
#include "Common/itkDICOMTags.h"
#include "Common/itkImageTypes.h"

typedef itk::Compose3DVectorImageFilter< Scalar3DImageType, Vector3DImageType >   Compose3DVectorImageType;
namespace itk{

	class DICOMVectorial3DImageLoader: public Object
	{

	public:
		/** Standard class typedefs. */
		typedef DICOMVectorial3DImageLoader		Self;
		typedef Object									Superclass;
		typedef SmartPointer<Self>						Pointer;
		typedef SmartPointer<const Self>				ConstPointer;

		typedef itk::ImageSeriesReader< Scalar3DImageType >                   ReaderType;
		typedef itk::GDCMImageIO                                              DICOMIOType;
		typedef itk::GDCMSeriesFileNames                                      NamesGeneratorType;
		typedef std::vector< std::string >                                    SeriesIdContainer;
		typedef std::vector< std::string >                                    FileNamesContainer;
		typedef itk::MetaDataDictionary                                       DictionaryType;
		typedef itk::MetaDataObject< std::string >                            MetaDataStringType;


		/** Method for creation through the object factory. */
		itkNewMacro( Self );  

		/** Run-time type information (and related methods). */
		itkTypeMacro( Self, Object );

		void SetDirectory(std::string const &dirname)
		{
			std::cout << "DIR: " << dirname << std::endl << std::endl;

			// A FileName Generator is needed in order to read the files in the right order
			NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
			nameGenerator->SetUseSeriesDetails( true );
			// First the acquisition type (component x = 1, y = 2, z = 3)
			//nameGenerator->AddSeriesRestriction("0020|0012"); // Acquision number (component)
			nameGenerator->AddSeriesRestriction( DCM_COMPONENT_TAG );
			//nameGenerator->AddSeriesRestriction("0008|0032"); // Time serie (iteration important)
			nameGenerator->AddSeriesRestriction( DCM_TIMESTEP_TAG );
			
			nameGenerator->SetInputDirectory( dirname );

			// Read in the directory given and get the number of series
			const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
	
			if ( !seriesUID.size() ) {
				std::cerr << "No series were found" << std::endl;
				return;
			}

			// Create a DICOM tagreader
			ReaderType::Pointer tagreader = ReaderType::New();
			DICOMIOType::Pointer dicomIO = DICOMIOType::New();
			tagreader->SetImageIO( dicomIO );

			//Dictionary
			const  DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
			DictionaryType::ConstIterator dictItr = dictionary.Begin();
			DictionaryType::ConstIterator dictEnd = dictionary.End();

			// Loop over the serieIdentifier
			SeriesIdContainer::const_iterator seriesItr = seriesUID.begin(); // Initialization of the loop
			SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
			int t1 = 0;   // Iteration number for x component.
			int t2 = 0;   // Iteration number for y component.
			int t3 = 0;   // Iteration number for z component.
			while (seriesItr != seriesEnd)
			{
				// Set file names of new iteration 
				std::string seriesIdentifier;
				seriesIdentifier = seriesItr->c_str();
				FileNamesContainer fileNames = nameGenerator->GetFileNames( seriesIdentifier );

				// Read the tag of the first image. Necessary in order to get the iteration number and component (xyz)
				FileNamesContainer::const_iterator fileNamesItr = fileNames.begin();
				tagreader->SetFileNames( fileNames );
				// read Image
				try {
					tagreader->Update();
				} catch (itk::ExceptionObject &ex) {
					std::cout << "exception catch: Cannot read image and/or image\n";
				}

				////////////////////////////////////////
				// Not needed only for debug purposes
				////////////////////////////////////////
				// Using the tagreader and dictionary, Get the component of the image serie (x,y or z)
				//std::string reftag = "0008|0032";
				std::string reftag = DCM_TIMESTEP_TAG;
				DictionaryType::ConstIterator tagItr = dictionary.Find( reftag );
				if( tagItr != dictEnd )
				{
					MetaDataStringType::ConstPointer entryvalue = dynamic_cast<const MetaDataStringType *>( tagItr->second.GetPointer() );
					if( entryvalue )
					{
						std::string tagvalue = entryvalue->GetMetaDataObjectValue();
					}
				}

				// Using the tagreader and dictionary, Get the time of image serie
				int component = 0;
				//reftag = "0020|0012";
				reftag = DCM_COMPONENT_TAG;
				tagItr = dictionary.Find( reftag );
				if( tagItr != dictEnd )
				{
					MetaDataStringType::ConstPointer entryvalue = dynamic_cast<const MetaDataStringType *>( tagItr->second.GetPointer() );
					if( entryvalue )
					{
						std::string tagvalue = entryvalue->GetMetaDataObjectValue();
						component = atoi(tagvalue.c_str());
					}
				}

				std::cout << "component = " << component << std::endl;

				if ((component == 1) && (t1 == 0))
				{
					// Create a reader for the actual serie
					ReaderType::Pointer reader = ReaderType::New();
					// Set file names of new iteration
					std::string seriesIdentifier = seriesItr->c_str();
					FileNamesContainer fileNames = nameGenerator->GetFileNames( seriesIdentifier );
					reader->SetFileNames( fileNames );
					reader->Register( );
					// Connect the reader with the tiler
					vectorizer->SetInput2( reader->GetOutput());
					t1++;
					seriesItr++;
				}
				else if ((component == 2)  && (t2 == 0))
				{
					// Create a reader for the actual serie
					ReaderType::Pointer reader = ReaderType::New();
					// Set file names of new iteration
					std::string seriesIdentifier = seriesItr->c_str();
					FileNamesContainer fileNames = nameGenerator->GetFileNames( seriesIdentifier );
					reader->SetFileNames( fileNames );
					reader->Register( );
					// Connect the reader with the tiler
					vectorizer->SetInput1( reader->GetOutput());
					t2++;
					seriesItr++;
				}
				else if ((component == 3) && (t3 == 0))
				{
					// Create a reader for the actual serie
					ReaderType::Pointer reader = ReaderType::New();
					// Set file names of new iteration
					std::string seriesIdentifier = seriesItr->c_str();
					FileNamesContainer fileNames = nameGenerator->GetFileNames( seriesIdentifier );
					reader->SetFileNames( fileNames );
					reader->Register( );
					// Connect the reader with the tiler
					vectorizer->SetInput3( reader->GetOutput());
					t3++;
					seriesItr++;
				}
				else 
				{
					std::cout << "The component tag was not found or it has a non legal value\n";
					std::cout << "This will probably lead to an error when reading the images.\n";
					seriesItr++;
				}
			}

			std::cout << "SetInput OK..." << std::endl;
		}

		Vector3DImageType::Pointer GetOutput()
		{
			return vectorizer->GetOutput( );
		}

		void Update( )
		{
			std::cout << "Entrando en Update\n";
			vectorizer->Update( );
			std::cout << "Saliendo de Update\n";
		}

	protected:
		DICOMVectorial3DImageLoader()
		{
			vectorizer = Compose3DVectorImageType::New();
		}

		~DICOMVectorial3DImageLoader() {};

	private:
		Compose3DVectorImageType::Pointer vectorizer;
	};
}
