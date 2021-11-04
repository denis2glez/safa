#ifndef DICOMHEADERREADER_H_
#define DICOMHEADERREADER_H_


#include "Common/itkImageTypes.h"
#include "itkImageFileReader.h"
#include "itkGDCMImageIO.h"
#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"
using namespace itk;
class DICOMHeaderReader: public Object {
public:
	typedef DICOMHeaderReader  Self;
	typedef Object                    Superclass;
	typedef SmartPointer<Self>        Pointer;
	typedef SmartPointer<const Self>  ConstPointer;
	typedef itk::ImageFileReader< Scalar3DImageType >    ReaderType;
	typedef itk::GDCMImageIO                             DICOMIOType;
	typedef itk::MetaDataDictionary                      DictionaryType;
	typedef itk::MetaDataObject< std::string >           MetaDataStringType;
	typedef std::vector< std::string >                   SeriesIdContainer;
	typedef std::vector< std::string >                   FileNamesContainer;

	/** Method for creation through the object factory. */
	itkNewMacro( Self );

	/** Run-time type information (and related methods). */
	itkTypeMacro( Self, Object );

	DICOMHeaderReader();
	virtual ~DICOMHeaderReader();

	void SetFileName(const char *fileName);

	std::string GetTag(std::string entryId);

	void Update();

private:
	 DICOMIOType::Pointer dicomIO;
};

#endif /* DICOMHEADERREADER_H_ */
