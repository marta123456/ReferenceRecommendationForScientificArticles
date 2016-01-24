//Definition of the class CSection
#include "CSection.h"

//CONSTRUCTORS
/**
	Constructor with parameters of the class CSection to set the diferent attributes
	of the object.
	@param sectionType: type of section. This parameter can be omited, in with case the default value is an empty string
	@param title: title of the section. This parameter can be omited, in with case the default value is an empty string
*/
CSection::CSection(const string &sectionType, const string &title)
	: aSectionType(sectionType), aTitle(title), pSubSection(NULL){}
/**
	Copy constructor. Creates a CSection objects with its attributes sets with the values of the given object.
	@param section: object to copy
*/
CSection::CSection(const CSection &section)
	: aSectionType(section.aSectionType), aTitle(section.aTitle)
{
	if(section.pSubSection==NULL){
		pSubSection=NULL;
	}else{
		pSubSection= new CSection(*section.pSubSection);
	}

}

//DESTRUCTOR
/**
	If the pointer attribute is not NULL, deletes the dymanic memory reserved for the subsection
*/
CSection::~CSection()
{
	if(pSubSection!=NULL){
		delete pSubSection;
	}

}
//OPERATOR=
/**
	Overloads the equal operator for the class CSection in order to reserve new dimanic memory
	@param section: section to copy	
	@return constant reference to the section with the new values
*/
const CSection& CSection::operator=(const CSection &section)
{
	aTitle=section.aTitle;
	aSectionType=section.aSectionType;

	if(section.pSubSection==NULL)
	{
		pSubSection=NULL;
	}else{
		pSubSection=new CSection(*section.pSubSection);
	}
	return *this;
}

//MODIFIERS

/**
	Sets the section type of the object.
	@param sectionType: type of section
*/
void CSection::setSectionType(const string &sectionType)
{
	aSectionType=sectionType;
}
/**
	Sets the title of the section.
	@param title: title of the section.
*/
void CSection::setTitle(const string &title)
{
	aTitle=title;
}
/**
	Adds a new section at the end of the list of sections. In order to do that, if the pSubSection pointer points to NULL, a new section is dynamically created. Otherwise, the pushSubSection() method is called recursively until the end of the list is reached.
	@param sectionType: type of section
	@param title: title of the section
*/
void CSection::pushSubSection(const string &sectionType, const string &title)
{
	if(pSubSection==NULL){
		pSubSection=new CSection(sectionType, title);
	}else{
		pSubSection->pushSubSection(sectionType, title);
	}
}
/**
	Deletes (freeing its memory) the last section of the list of sections
*/
void CSection::popSubSection()
{
	//Check current section is not the last section
	if(pSubSection!=NULL)
	{
		//Check the next section
		if(pSubSection->pSubSection!=NULL){
			pSubSection->popSubSection();
		}
		else
		{
			delete pSubSection;
			pSubSection=NULL;
		}
	}
}
//SELECTORS
/**
	Returns the section type.
	@return aSectionType of the object
*/
const string & CSection::getSectionType() const
{
	return aSectionType;
}
/**
	Returns the title of the section
	@return aTitle attribute of the object
*/
const string & CSection::getTitle() const
{
	return aTitle;
}

/**
	Returns a pointer to the following section
	@return pSubSection pointer
*/
const CSection * CSection::getPSubSection() const 
{
	return pSubSection;
}



