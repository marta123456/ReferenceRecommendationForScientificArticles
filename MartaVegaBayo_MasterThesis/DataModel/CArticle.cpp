//Definition of the class CArticle
#include "CArticle.h"

//SELECTORS
/**
	Returns the aTitle attribute of object
	@return title of the article
*/
const string & CArticle::getTitle() const
{
	return aTitle;
}
/**
	Returns the aAuthors attribute of the object
	@return vector of CName objects, one per author of the article
*/
const vector<CName> & CArticle::getAuthors() const
{
	return aAuthors;
}
/**
	Returns the aPubDate attribute of the object
	@return CDate with the date of publication
*/
const CDate & CArticle::getPubDate() const
{
	return aPubDate;
}

//MODIFIERS
/**
	Sets the aTitle attribute of the object
	@param title: title of the article
*/
void CArticle:: setTitle(const string &title)
{
	aTitle=title;
}

/**
	Sets the aPubDate attribute of the object with the same date than the given 
	CDate object
	@param date: CDate object with the publication date
*/
void CArticle::setPubDate(const CDate &date) throw(std::invalid_argument)
{
	aPubDate=date;
}
/**
	Sets the aPubDate attribute of the object by setting the year, month and day 
	of the CDate object.
	@param year: year of publication
	@param month: month of publication
	@param day: day of publication
*/
void CArticle::setPubDate(int year, int month, int day)throw(std::invalid_argument)
{
	aPubDate.setDate(year, month, day);
}
/**
	Sets the aAuthors attribute with the vector given.
	@param authors: vector with the CNames objects of the different authors
*/
void CArticle::setAuthors(const vector<CName> & authors)
{
	aAuthors=authors;
}
/**
	Deletes the different authors (CName objects) of the aAuthors vectors
*/
void CArticle::deleteAllAuthors()
{
	aAuthors.clear();
}
/**
	Adds to the aAuthors vector one author (CName object) with the same
	information as the CName object given
	@param name: CName object with the name of the author
*/
void CArticle::addAuthor(const CName &name)
{
	aAuthors.push_back(name);
}
/**
	Adds to the aAuthors vector one author (CName object) with first and lastname
	given
	@param firstName: firstName of the author
	@param lastName: lastName of the author
*/
void CArticle::addAuthor(const string &firstName, const string &lastName)
{
	aAuthors.push_back(CName(firstName, lastName));
}

//OVERLOADED OPERATORS
/**
	Overload of the operator << to show in an output the information of the 
	object.
	@param os: output stream
	@param article: article to show
	@return output stream
*/
ostream& operator<<(ostream& os, const CArticle& article)
{
	os << "___ARTICLE METADATA_____";
	os <<"\nTitle: " << article.aTitle;
	os <<"\nAuthors: ";
	for(uint i=0; i< article.aAuthors.size(); i++)
	{
		os << article.aAuthors[i] << " ";
	}
	os <<"\nDate: " << article.aPubDate;
	return os;
}

//OTHERS
/**
	Shows in the standard output the values of the attributes of the article
*/
void CArticle::showArticleMetadata() const
{
	cout << "\n___ARTICLE METADATA_____";
	cout <<"\nTitle: " << aTitle;
	cout <<"\nAuthors: ";
	for(uint i=0; i< aAuthors.size(); i++)
	{
		cout << aAuthors[i] << " ";
	}
	cout <<"\nDate: " << aPubDate;

}
