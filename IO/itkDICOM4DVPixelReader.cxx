#include "Common/itkReaderEvents.h"
#include "itkDICOM4DVPixelReader.h"
#include "Common/itkDICOMTags.h"

//#define TRACE_SLICES 1
//#define DO_PROFILE

typedef itk::GDCMImageIO
DICOMIOType;

typedef itk::GDCMSeriesFileNames
NamesGeneratorType;

typedef std::vector< std::string >
SeriesIdContainer;

typedef std::vector< std::string >
FileNamesContainer;

typedef itk::MetaDataDictionary
DictionaryType;

typedef itk::MetaDataObject< std::string >
MetaDataStringType;

#if defined( DO_PROFILE )
#include "itkTimeProbe.h"

#define INIT_PROFILE                            \
  itk::TimeProbe clock;                         \
  clock.Start()

#define ECHO_TIME( msg )                                        \
  do {                                                          \
    clock.Stop();                                               \
    double timeTaken = clock.GetMeanTime();                     \
    std::cout << msg << " elapsed " << timeTaken << "\n";       \
    clock.Start();                                              \
  } while( 0 ) 
#else
#define INIT_PROFILE 
#define ECHO_TIME( msg )
#endif

#define DO_TRACE 
#ifdef DO_TRACE
#define TRACE( msg )                            \
  std::cout << msg
#else
#define TRACE( MSG )
#endif

DICOM4DVPixelReader::
DICOM4DVPixelReader( )
{
  TileFilterType::LayoutArrayType layout;
  layout[0] = 1;
  layout[1] = 1;
  layout[2] = 1;
  layout[3] = 0;
  for ( int i = 0; i < 3; i++ ) {
    m_Tilers[ i ] = TileFilterType::New( );
    m_Tilers[ i ]->SetLayout( layout );
  }
  this->m_Vectorizer = Compose3DVectorImageType::New( );
  // por que esta permutacion?
  //#define __COMP_IDENT__ 1
#ifdef __COMP_IDENT__
  this->m_Vectorizer->SetInput1( m_Tilers[0]->GetOutput() );
  this->m_Vectorizer->SetInput2( m_Tilers[1]->GetOutput() );
  this->m_Vectorizer->SetInput3( m_Tilers[2]->GetOutput() );
#else
  this->m_Vectorizer->SetInput2( m_Tilers[0]->GetOutput() );
  this->m_Vectorizer->SetInput1( m_Tilers[1]->GetOutput() );
  this->m_Vectorizer->SetInput3( m_Tilers[2]->GetOutput() );
#endif
  this->m_progress = 0;
  this->m_NumberOfTimeStepsToRead = 0;
}

void DICOM4DVPixelReader::
SetDirectory( std::string const &dirName )
{
  INIT_PROFILE;

  m_progress = 0;
  // inicializo el pipeline
  
  // limpio la lista de readers, esto destruye los objetos readers
  for ( int i = 0; i < 3; i++ ) {
    m_Readers[ i ].clear( );
  }
  // OJO: no se como quedan los inputs de los Tilers, creo que hay que
  // limpiarlos, por lo pronto esta clase solo se puede usar con un
  // directorio, hay que hacer delete despues de usarla

  NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New( );
  TRACE( "Initializing nameGenerator: BEGIN\n" );
  nameGenerator->SetUseSeriesDetails( true );
  // First the acquisition type (component x = 1, y = 2, z = 3)
  // Acquision number (component)
  // nameGenerator->AddSeriesRestriction("0020|0012");
  if ( !this->GetNumberOfTimeStepsToRead( ) ) {
    nameGenerator->AddSeriesRestriction( DCM_COMPONENT_TAG );
    /*
      (0008|0032) Acquisition Time: The time the acquisition of data
      that resulted in this image started
    */
    //nameGenerator->AddSeriesRestriction("0008|0032");
    /*
      (0018,1060) Trigger Time: Time interval measured in msec from the
      start of the R-wave to the beginning of the data taking.
    */
    // nameGenerator->AddSeriesRestriction("0018|1060");
    nameGenerator->AddSeriesRestriction( DCM_TIMESTEP_TAG );
    
  }    
  TRACE( "Initializing nameGenerator: END\n" );
  TRACE( "nameGenerator->SetDirectory: BEGIN\n" );
  
  ECHO_TIME( "Before nameGenerator->SetDirectory" );
  nameGenerator->SetDirectory( dirName );
  
  TRACE( "nameGenerator->SetDirectory: END\n" );
  ECHO_TIME( "SetDirectory" );

#ifdef TRACE_SLICES
    const size_t timeStepToTrace = 1;
#endif
  if ( this->GetNumberOfTimeStepsToRead( ) ) {
    // Read the full list of file names that will be used in case no
    // automatic series detection
    const FileNamesContainer&
      allFileNames = nameGenerator->GetFileNames( "" );
    size_t maxTimeStep = this->GetNumberOfTimeStepsToRead( );
    size_t totalSeries = allFileNames.size();
    // should be multiple of 3 * TS
    size_t totalVolumes = 3*maxTimeStep;
    if ( totalSeries % totalVolumes ) {
      // here we should throw an exception
      TRACE( "number of series does not match number of time "
             "step requested and components\n" );
      return;
    }

    // este valor 0.124 debemos colocarlo en una constante
    m_progress = 0.124;
    // este valor 0.138 deberia ir en una constante
    float pstep = 0.138 / totalVolumes;
    this->InvokeEvent( itk::StartScanUIDEvent() );

    size_t size3D = totalSeries / (3*maxTimeStep);
    FileNamesContainer fileNames( size3D );
    for ( size_t comp = 0, firstSlice = 0; comp < 3; ++comp ) {
      TRACE( "Processing " << comp << "-component\n" );
      for ( size_t t = 0; t < maxTimeStep; t++ ) {
        TRACE( "Processing timestep " << t << "\n" );
        // copy the filenames for this volumen, don't know why
        // std::copy does no work for me as expected.
#ifdef TRACE_SLICES
        if ( timeStepToTrace == t ) {
          std::cout.flush( );
          std::cout << "----------------------------------------------------\n";
          std::cout << "Slices for componente " << comp << " at TS = " << t << "\n";
          std::cout << "----------------------------------------------------\n";
          std::cout.flush( );
        }
#endif
        //for ( size_t i = 0; i < size3D; i++ ) {
        for ( int i = size3D-1; i >= 0; --i ) {
          fileNames[i] = allFileNames[ firstSlice++ ];
#ifdef TRACE_SLICES
          if ( timeStepToTrace == t ) {
            std::cout << fileNames[i] << "\n";
          }
#endif
        }
#ifdef TRACE_SLICES
        std::cout.flush( );
#endif
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileNames( fileNames );
        reader->Register( );
        // Connect the reader with the tiler 
        this->m_Tilers[comp]->SetInput( t, reader->GetOutput() );
        m_Readers[ comp ].push_back( reader );
        m_progress += pstep;
        this->InvokeEvent( itk::ProgressUIDEvent() ); 
      }
    }
  } else {
    // Read in the directory given and get the number of series
    const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
    
    TRACE( "seriesUID created: " << seriesUID.size() << "\n" );
    ECHO_TIME( "nameGenerator->GetSeriesUIDs" );
    
    if ( !seriesUID.size() ) {
      // aqui hay que lanzar una exception
      // "no series were found"
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
    // contador de iteraciones por componentes
    // [0] --> contador de iteracion para X
    // [1] --> contador de iteracion para Y
    // [2] --> contador de iteracion para Z
    int iterCounter[ 3 ] = { 0, 0, 0 };
    ECHO_TIME( "Before progressing" );
    // este valor 0.124 debemos colocarlo en una constante
    m_progress = 0.124;
    // este valor 0.138 deberia ir en una constante
    float pstep = 0.138 / seriesUID.size();
    this->InvokeEvent( itk::StartScanUIDEvent() );
#ifdef TRACE_SLICES
    std::cout << "There are " << seriesUID.size() << " series UID\n";
#endif
    while ( seriesItr != seriesEnd ) {
      TRACE( "seriesItr: progress\n" );
      // Set file names of new iteration 
      std::string seriesIdentifier;
      seriesIdentifier = seriesItr->c_str( );
      FileNamesContainer fileNames = nameGenerator->GetFileNames( seriesIdentifier );
      
      // Read the tag of the first image. Necessary in order to get the
      // iteration number and component (xyz)
      FileNamesContainer::const_iterator fileNamesItr = fileNames.begin();
      tagreader->SetFileNames( fileNames );
      // read Image
      try {
        tagreader->Update();
      } catch (itk::ExceptionObject &ex) {
        // aqui hay que propagar la exception
        TRACE( "exception catch: Cannot read image and/or image\n" );
        return;
      }
      
      ////////////////////////////////////////
      // Not needed only for debug purposes
      ////////////////////////////////////////
      // Using the tagreader and dictionary, Get the time of image serie
      //std::string reftag = "0018|1060"; //"0008|0032";
      std::string reftag = DCM_TIMESTEP_TAG;
      DictionaryType::ConstIterator tagItr = dictionary.Find( reftag );
      if( tagItr != dictEnd ) {
        MetaDataStringType::ConstPointer entryvalue = dynamic_cast<const MetaDataStringType *>( tagItr->second.GetPointer() );
        if( entryvalue ) {
          std::string tagvalue = entryvalue->GetMetaDataObjectValue();
        }
      }
      
      // Using the tagreader and dictionary, Get the component of the image serie (x,y or z)
      //reftag = "0020|0012";
      reftag = DCM_COMPONENT_TAG;
      int comp = 0, icomp;
      tagItr = dictionary.Find( reftag );
      if( tagItr != dictEnd ) {
        MetaDataStringType::ConstPointer entryvalue = dynamic_cast<const MetaDataStringType *>( tagItr->second.GetPointer( ) );
        if( entryvalue ) {
          std::string tagvalue = entryvalue->GetMetaDataObjectValue( );
          comp = atoi( tagvalue.c_str( ) );
        }
      }
      icomp = comp - 1;
      if ( comp >= 1 && comp <= 3 ) {
        // Create a reader for the actual serie
        ReaderType::Pointer reader = ReaderType::New( );
        // Set file names of new iteration
        std::string seriesIdentifier = seriesItr->c_str( );
        const FileNamesContainer &fileNames =
          nameGenerator->GetFileNames( seriesIdentifier );
        reader->SetFileNames( fileNames );
        // Connect the reader with the tiler
        m_Tilers[ icomp ]->SetInput( iterCounter[ icomp ], reader->GetOutput( ) );
#ifdef TRACE_SLICES
        if ( timeStepToTrace == iterCounter[ icomp ] ) {
          std::cout.flush( );
          std::cout << "----------------------------------------------------\n";
          std::cout << "Slices for component " << icomp << " at TS = " << iterCounter[ icomp ] << "\n";
          std::cout << "----------------------------------------------------\n";
          for ( size_t i = 0; i < fileNames.size(); i++ ) {
            std::cout << fileNames[i] << "\n";
          }
          std::cout.flush( );
        }
#endif
        m_Readers[ icomp ].push_back( reader );
        (iterCounter[ icomp ])++;
        seriesItr++;
      } else {
        std::cout << "The component tag was not found or it has a non legal value\n";
        std::cout << "This will probably lead to an error when reading the images.\n";
        seriesItr++;
        }
      m_progress += pstep;
      this->InvokeEvent( itk::ProgressUIDEvent() ); 
    }
  }
  this->InvokeEvent( itk::EndScanUIDEvent() );
  ECHO_TIME( "Readers created" );
}

void DICOM4DVPixelReader::
Update( )
{
  std::list<ReaderType::Pointer>::iterator riter;
  
  // esto debe disparar el update de los tilers y estos a su vez el de los readers
  INIT_PROFILE;
  size_t nreaders =
    m_Readers[ 0 ].size() +
    m_Readers[ 1 ].size() +
    m_Readers[ 2 ].size();
  // este valor 0.738 deberia ir en una constante
  float rstep = 0.738 / nreaders;
  this->InvokeEvent( itk::StartReadEvent() );
  std::list<ReaderType::Pointer>::iterator it;
  for ( int i = 0; i < 3; i++ ) {
    for ( riter = m_Readers[i].begin(); riter != m_Readers[i].end(); riter++ ) {
      //ReaderType::Pointer reader = ;
      (*riter)->Update();
      m_progress += rstep;
      this->InvokeEvent( itk::ProgressReadEvent() );
    }
    m_Tilers[ i ]->Update( );
  }
  m_Vectorizer->Update( );
  this->InvokeEvent( itk::EndReadEvent() );
  ECHO_TIME( "Update" );
}

///////////////////////////////////////////////////////////////////////////
/*
  StartEvent: 0.000387907 sec ~ 1.95321247711e-05

  ScanSeriesEvent: 1.47159 sec + 0.981451 sec = 2.453041 sec ~ 0.123516984434

  ProgressSeriesUIDEvent: 2.73392 sec ~ 0.137659971473
  
  ProgressReaderEvent: 14.6726 sec ~ 0.738803511968

  EndEvent: 0
  
  Total: 19.859948907 sec
 */
