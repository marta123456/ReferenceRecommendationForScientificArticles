//Definition of the class CArticleParsed
#include "CArticleParsed.h"


//SELECTORS
/**
	Returns the aAbstract attribute of the object
	@return abstract of the article
*/
const string & CArticleParsed::getAbstract() const
{
	return aAbstract;
}
/**
	Returns the aKeywords vector, attribute of the object
	@return vector with the keywords of the article
*/
const vector<string> & CArticleParsed::getKeywords() const
{
	return aKeywords;
}
/**
	Returns the aReferences vector, attribute of the object
	@return vector with the bibliographic references of the article
*/
vector<CReference> & CArticleParsed::getReferences()
{
	return aReferences;
}
/**
	Returns the aCitations vector, attribute of the object
	@return the vector with the citations of the article
*/
vector<CCitation> & CArticleParsed::getCitations()
{
	return aCitations;
}
/**
	Returns the aArticleBody attribute of the object
	@return text content of the article
*/
const string & CArticleParsed::getArticleBody() const
{
	return aArticleBody;
}
//MODIFIERS
/**
	Sets the aAbstract attribute of the article object
	@param abstract: abstract of the article
*/
void CArticleParsed::setAbstract(const string & abstract)
{
	aAbstract=abstract;
}
/**
	Sets the aKeywords attribute of the article object.
	@params keywords: vector with the keywords of the article
*/
void CArticleParsed::setKeywords(const vector<string> &keywords)
{
	aKeywords=keywords;
}
/**
	Deletes all keywords of the attribute aKeywords (which is a vector)
*/
void CArticleParsed::deleteAllKeywords()
{
	aKeywords.clear();
}
/**
	Adds the given keyword to the vector of keywords, aKeywords
	@param keyword: keyword to add
*/
void CArticleParsed::addKeyword(const string &keyword)
{
	aKeywords.push_back(keyword);
}
/**
	Adds the a reference to the list of references of the parsed article.
	@param idRef: id of the reference used in the list of references of the article
	@param referencedArticle: CArticle object containing metadata of the work being referenced.
*/
 void CArticleParsed::addReference(string &idRef, const CArticle &referencedArticle)
 {
	aReferences.push_back(CReference(idRef, referencedArticle));
 }
 /**
	Adds a citation to the list of citations of the article. In order to set the pointer of the CCitation object added, this method uses the given idRef
	to look in the list of references of the object for the cited work.
	As one id can be used in the list of references for referencing multiple works, for each citation found within the text, there are added as many
	citations as works with the corresping id.
	@param idRef: reference identifier used to do the citation within the text
	@param context: text of the paragraph where the citation was found.
	@param section: section where the paragraph was found.
*/
 void CArticleParsed::addCitation(string &idRef,  string &context,  CSection &section)
 {
	//Iterate over the references
	uint nRefs=aReferences.size();
	for(uint i=0; i<nRefs; i++)
	{
		//Find the references being cited
		if(aReferences[i].getIdRef()==idRef)
		{
			aCitations.push_back(CCitation(idRef, context, section, &aReferences[i]));
		}
	}	
 }
/**
	Sets the aArticleBody attribute of the article object
	@param articleBody: text content of the article
*/
void CArticleParsed::setArticleBody(const string & articleBody)
{
	aArticleBody=articleBody;
}

//OTHERS
 void CArticleParsed::showArticleMetadata() const
{
	
	CArticle::showArticleMetadata();
	cout<<"\nAbstract: " << aAbstract;
	cout << "\nKeywords: ";
	for(uint i=0; i<aKeywords.size(); i++)
	{
		cout << aKeywords[i] << ", ";
	}
	cout << "\n\nReferences: ";
	for(uint i=0; i<aReferences.size(); i++)
	{
		//cout << "\n\t" << aReferences[i].aReferencedArticle.getTitle();
		cout << "\n\tidRef=" << aReferences[i].getIdRef() << " Autores: ";
		vector<CName> authors= aReferences[i].getReferencedArticle().getAuthors();
		for(uint j=0; j<authors.size(); j++){
			cout << authors[j].getFirstName()  << " " << authors[j].getLastName()  << ", ";
		}

	}
	cout << "\n\nCitations: "<<aCitations.size() ;
	for(uint i=0; i<aCitations.size(); i++)
	{
		cout << "\n\tidRef=" << aCitations[i].getIdRef() << " Sections: ";
		CSection sec=aCitations[i].getSection();
		bool flag=true;
		while(flag){
			cout << sec.getTitle()<<" ";
			if(sec.getPSubSection()!=NULL){
				sec=*sec.getPSubSection();
			}else{	
				flag=false;
			}
		}
		//cout  << "Context: "<<aCitations[i].getContext()  << endl<< endl<< endl<< endl<< endl;
	}
}
