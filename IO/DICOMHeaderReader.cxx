#include "IO/DICOMHeaderReader.h"

DICOMHeaderReader::DICOMHeaderReader() {
	// TODO Auto-generated constructor stub
	dicomIO = DICOMIOType::New();
}

DICOMHeaderReader::~DICOMHeaderReader() {
	// TODO Auto-generated destructor stub
	dicomIO->Delete();
}

void DICOMHeaderReader::SetFileName(const char* fileName) {
	dicomIO->SetFileName(fileName);
}

std::string DICOMHeaderReader::GetTag(std::string entryId) {
	const DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
	DictionaryType::ConstIterator tagItr = dictionary.Find(entryId);
	DictionaryType::ConstIterator end = dictionary.End();
	std::string tagvalue;
	if (tagItr != end) {
		MetaDataStringType::ConstPointer
				entryvalue = dynamic_cast<const MetaDataStringType *> (tagItr->second.GetPointer());
		if (entryvalue) {
			std::string tagkey = tagItr->first;
			std::string labelId;
			bool found = itk::GDCMImageIO::GetLabelFromTag(tagkey, labelId);
			tagvalue = entryvalue->GetMetaDataObjectValue();
			if (found)
				return tagvalue;
		}
	} else {
		return tagvalue;       // Return the string ""
	}
	std::cerr << "DICOMHeaderReader ERROR: the tag " << entryId << " not found.\n" ;
	return tagvalue;
}
void DICOMHeaderReader::Update(){
	dicomIO->ReadImageInformation();
}
