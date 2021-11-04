cmake_minimum_required(VERSION 2.8)
PROJECT(SAFA)
FIND_PACKAGE(ITK)
IF(NOT ITK_DIR)
  MESSAGE(FATAL_ERROR "Please set ITK_DIR.")
ENDIF(NOT ITK_DIR)
FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF(NOT VTK_DIR)

INCLUDE(${ITK_USE_FILE})
INCLUDE(${VTK_USE_FILE})

# use what QVTK built with
SET(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
SET(DESIRED_QT_VERSION ${VTK_DESIRED_QT_VERSION} CACHE FILEPATH "")

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

# Use the include path and library for Qt that is used by VTK.
INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)
########################################################################
SET (MainSrc 
	System/AortaFlowMain.cxx
	)	
	
SET (AortaFlowAppHeaders 
	Common/AortaUtility.h
	Common/vtkKWImage.h
	Common/vtkKWImageIO.h
	Common/itkDICOMTags.h
	Common/itkImageTypes.h
	Common/itkReaderEvents.h
	Common/HeuristicBloodBehaviorDetection.h
	Common/ImageCursor.h
	
	Segmentation/AF4DCenterLine.h
	Segmentation/itkBinaryThinningImageFilter3D.h
	Segmentation/WriteImageRoutines.h
	Segmentation/AortaSegmentationPipeline.h
	Segmentation/itkVelocityEncodingImageFilter.h
	Segmentation/itkVectorMagnitudeMinimumMaximumImageCalculator.cxx
	
	IO/DICOMScalar4DImageLoader.h
	IO/DICOMVectorial3DImageLoader.h
	IO/DICOMVectorial4DImageLoader.h
	IO/AortaSeriesImporter.h
	IO/DICOMHeaderReader.h
	IO/ObserverReader.h
	IO/itkDICOM4DVPixelReader.h
	
	GUI/ViewerSynchronize.h	
	GUI/AortaFlowApp.h
	GUI/Form3D.h
	GUI/Viewer.h
    )
	
SET (AortaFlowAppSrcs 
	Common/AortaUtility.cxx
	Common/vtkKWImage.cxx
	Common/vtkKWImageIO.cxx
	Common/HeuristicBloodBehaviorDetection.cpp
	Common/ImageCursor.cxx
	
	Segmentation/AF4DCenterLine.cxx
	Segmentation/itkBinaryThinningImageFilter3D.txx
	Segmentation/WriteImageRoutines.cxx
	Segmentation/AortaSegmentationPipeline.cxx
	Segmentation/itkVelocityEncodingImageFilter.cxx
	Segmentation/itkVectorMagnitudeMinimumMaximumImageCalculator.cxx
	
	IO/DICOMScalar4DImageLoader.cxx
	IO/DICOMVectorial3DImageLoader.cxx
	IO/DICOMVectorial4DImageLoader.cxx
	IO/AortaSeriesImporter.cxx
	IO/DICOMHeaderReader.cxx
	IO/itkDICOM4DVPixelReader.cxx
	
	GUI/ViewerSynchronize.cxx	
	GUI/AortaFlowApp.cxx
	GUI/Form3D.cxx
	GUI/Viewer.cxx
	)

SET (UIS 
	GUI/AortaFlowApp.ui
	GUI/Form3D.ui
	GUI/Viewer.ui
	)

SET (AortaFlowAppResources 
	GUI/ResourcesFile.qrc
	)
	
SET (TestAll 	
	Test/TestAll.cxx 
	Test/TestLoaders.cxx
	
	Test/TestDICOMVectorial3DImageLoader.h
	Test/TestDICOMScalar4DImageLoader.h
	Test/TestDICOMVectorial4DImageLoader.h	
	Test/TestAF4DCenterLine.h

	)

########################################################################

QT4_WRAP_UI(UIHeaders ${UIS})
QT4_WRAP_CPP(MOCSrcs ${AortaFlowAppHeaders} )
QT4_ADD_RESOURCES(ResourceSrcs ${AortaFlowAppResources})

ADD_DEFINITIONS(-DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT)
SET_SOURCE_FILES_PROPERTIES(${AortaFlowAppSrcs} PROPERTIES  OBJECT_DEPENDS "${UIHeaders}")


# It's nice to have the ui in the windows project file...just double click on it
# and designer comes up on that ui file :)
IF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
  SET (AortaFlowAppSrcs ${AortaFlowAppSrcs} ${UIS})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
IF (${CMAKE_BUILD_TOOL} MATCHES "devenv")
  SET (AortaFlowAppSrcs ${AortaFlowAppSrcs} ${UIS})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "devenv")

ADD_EXECUTABLE( SAFA MACOSX_BUNDLE ${AortaFlowAppSrcs} ${UISrcs} ${MOCSrcs} ${ResourceSrcs} ${MainSrc})

TARGET_LINK_LIBRARIES( SAFA
	QVTK
	${QT_LIBRARIES}	
	vtkRendering
	vtkVolumeRendering
	vtkGraphics
	vtkIO
	vtkCommon
	ITKBasicFilters
	ITKCommon 
	ITKIO	
)

# ADD_EXECUTABLE( TestAllAortaFlowApp MACOSX_BUNDLE ${AortaFlowAppSrcs} ${UISrcs} ${MOCSrcs} ${ResourceSrcs} ${TestAll} )

# TARGET_LINK_LIBRARIES( TestAllAortaFlowApp
	# QVTK
	# ${QT_LIBRARIES}	
	# vtkRendering
	# vtkGraphics
	# vtkIO
	# vtkCommon
	# ITKBasicFilters
	# ITKCommon 
	# ITKIO	
# )

