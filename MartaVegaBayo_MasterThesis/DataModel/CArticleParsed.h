//Declaration of the class CArticleParsed
#if !defined (_CARTICLEPARSED_H_)
#define _CARTICLEPARSED_H_
#include <vector>
#include "CArticle.h"
#include "CReference.h"
#include "CCitation.h"

/**
	Class CArticleParsed. This class represents an article which has been parsed and, therefore, more metadata is available.
	It is an extension of the CArticle class and has, apart from the attributes of the CArticle class, attributes to record metadata
	of the article such as abstract, keywords, body of the article, list of referenced works and list of citations.
*/
class CArticleParsed : public CArticle
{
private:
	string aAbstract; // Abstract of the article
	vector<string> aKeywords; // Keywords of the article
	string aArticleBody; //Text content of the article

public:
	vector<CReference> aReferences; //Vector with the bibliographic references of the article
	vector<CCitation> aCitations; //Vector with the citations of the article

	//Selectors
	const string& getAbstract() const;
	const vector<string>& getKeywords() const;
	vector<CReference> & getReferences();
	vector<CCitation> & getCitations();
	const string& getArticleBody() const;

	//Modifiers
	void setAbstract(const string & abstract);
	void setKeywords(const vector<string> &keywords);
	void deleteAllKeywords();
	void addKeyword(const string & keyword);
	void addReference(string &idRef, const CArticle &referencedArticle);
	void addCitation(string &idRef, string &context, CSection &section);
	void setArticleBody(const string & articleBody);

	//Others
	void showArticleMetadata() const;


};


#endif
