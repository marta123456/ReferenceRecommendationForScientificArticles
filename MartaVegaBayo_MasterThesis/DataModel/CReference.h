//Declaration of the class CReference
#if !defined (_CREFERENCE_H_)
#define _CREFERENCE_H_

#include "CArticle.h"
#include <string>
using namespace std;

/**
	Class CReference. This class represents one of the bibliographic works referenced in the reference list of an article. Therefore, 
	this objects have as attributes a CArticle object and the ID used to cite the referenced work within the article.
	This class also provides methods to read and write the attributes of the object.
*/
class CReference
{
private:
	string aIdRef; //identifier of the reference in the article
	CArticle aReferencedArticle; //Referenced article  with its corresponding information
public:
	//Constructors
	CReference();
	CReference(const string &idRef, const CArticle &referencedArticle);
	CReference(const CReference &reference);
	//Modifiers
	void setIdRef(const string &idRef);
	void setDbId(int id);
	void setReferencedArticle(const CArticle &referencedArticle);
	//Selectors
	const string & getIdRef() const;
	int getDbId()const;
	const CArticle & getReferencedArticle()const;
};

#endif
