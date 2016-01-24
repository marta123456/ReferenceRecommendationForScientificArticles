//Definition of the CReference class
#include "CReference.h"

//CONSTRUCTORS
/**
	Constructor without parameters of the class CReference
*/
CReference::CReference(){}
/**
	Constructor with parameters to set the attributes of the object during its creation. 
	@param idRef: identifier of the reference
	@param referencedArticle: article referenced with its information (title, authors, date..)
*/
CReference::CReference(const string &idRef, const CArticle &referencedArticle):
	aIdRef(idRef),  aReferencedArticle(referencedArticle)
{
}
/**
	Copy constructor. Creates an object with its attributes set with the values of the
	given object.
	@param reference: reference object to copy
*/
CReference::CReference(const CReference &reference):
	aIdRef(reference.aIdRef), aReferencedArticle(reference.aReferencedArticle)
{}


//MODIFIERS
/**
	Sets the attribute aIdRef of the object
	@param idRef: identifier of the reference
*/
void CReference::setIdRef(const string &idRef)
{
	aIdRef=idRef;
}


/**
	Sets the attribute aReferencedArticle of the object.
	@param referencedArticle: article referenced
*/
void CReference::setReferencedArticle(const CArticle & referencedArticle)
{
	aReferencedArticle=referencedArticle;
}

//SELECTORS
/**
	Returns the aIdRef of the object
	@return identifier of the reference
*/
const string & CReference::getIdRef() const
{
	return aIdRef;
}


/**
	Returns the aReferencedArticle of the object
	@return referencedArticle
*/
const CArticle & CReference::getReferencedArticle() const
{
	return aReferencedArticle;
}
