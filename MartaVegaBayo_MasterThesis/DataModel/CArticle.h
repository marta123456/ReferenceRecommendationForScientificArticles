//Declaration of the class CArticle
#if !defined (_CARTICLE_H_)
#define _CARTICLE_H_
#include <string>
#include <vector>
#include "CName.h"
#include "CDate.h"

/**
	CArticle class. This class represents an article or scientific work and, therefore, has attributes to save metadata of the article, 
	such as title, authors and publication date.
*/
class CArticle
{
private:
	string aTitle; //Title of the article
	vector<CName> aAuthors; //Vector of authors of the article
	CDate aPubDate; //Date of publication

public:
	//Constructors
	
	//Selectors
	const string  & getTitle() const;
	const vector<CName> & getAuthors() const;
	const CDate & getPubDate() const;

	//Modifiers
	void setTitle(const string &title);
	void setPubDate(const CDate &date) throw(std::invalid_argument);
	void setPubDate(int year=0, int month=0, int day=0) throw(std::invalid_argument);
	void setAuthors(const vector<CName> & authors);
	void deleteAllAuthors();
	void addAuthor(const CName &name);
	void addAuthor(const string &firstName, const string &lastName);

	//Overloaded methods
	friend ostream& operator<<(ostream& os, const CArticle& article);

	//Others
	virtual void showArticleMetadata() const;

};

#endif
