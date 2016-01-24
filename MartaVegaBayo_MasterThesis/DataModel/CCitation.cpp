#include "CCitation.h"

//CONSTRUCTORS
/**
	Constructor without parameters of the class CCitation
*/
CCitation::CCitation(){}

/**
	Constructor with parameters of the class CCitation to set its attributes when the object
	is created.
	@param idRef: identifier of the reference cited
	@param context: content of the paragraph where the citation is
	@param section: CSection object with the information of section and subsections where the citation is
	@param pRef: pointer to the reference being cited
*/
CCitation::CCitation(const string &idRef, const string &context, const CSection &section, CReference *pRef):
	aIdRef(idRef), aContext(context), aSection(section), apRef(pRef)
{
}


//MODIFIERS
/**
	Sets the aIdRef attribute of the object
	@param idRef: identifier of the reference
*/
void CCitation::setIdRef(const string &idRef)
{
	aIdRef=idRef;
}
/**
	Sets the aContext attribute of the object
	@param context: context where the citation is
*/
void CCitation::setContext(const string &context)
{
	aContext=context;
}
/**
	Sets the aSections attribute of the object
	@param section: CSection object
*/
void CCitation::setSection(const CSection &section)
{
	aSection=section;
}
/**
	Sets the aRefArticle attribute of the object. This method only sets the pointer value (does not dynamically create a new object).
	Therefore, the user of the object must make sure of not deleting the CReference object before the CCitation object (or do not try
	to access the pointer content).
	@param refArticle: reference to the article being cited
*/
void CCitation::setReference(CReference *pRef)
{
	apRef=pRef;
}
//SELECTORS
/**
	Returns the aIdRef attribute of the object
	@return identifier of the reference
*/
const string & CCitation::getIdRef() const
{
	return aIdRef;
}
/**
	Returns the aContext attribute of the object
	@return context where the citation is
*/
const string & CCitation::getContext() const
{
	return aContext;
}
/**
	Returns the aSections attribute of the object
	@return section where the citation is
*/
const CSection & CCitation::getSection() const
{
	return aSection;
}

/**
	Returns the title of the article being cited
*/
const string&  CCitation::getRefTitle()const
{
	return apRef->getReferencedArticle().getTitle();
}






