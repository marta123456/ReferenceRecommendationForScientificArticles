//Definition of the class CCitation
#if !defined (_CCITATION_H_)
#define _CCITATION_H_

#include <string>
#include <vector>
#include "CSection.h"
#include "CReference.h"

using namespace std;

/**
	Class CCitation. This class represents a citation within an article. Therefore, it has attributes to represent the context (the parragraph)
	whrere the citation is, the section within the article, the id used for the citation and a pointer to the referenced work.
*/
class CCitation
{
private:
	string aIdRef; //Identifier of the reference cited
	string aContext; //Paragraph surrounding the citation
	CSection aSection; //Article section (and subsections) where the citation is
	CReference *apRef; //Reference of the citation

public:
	//Constructors
	CCitation();
	CCitation(const string &idRef, const string &context, const CSection &section, CReference *pRef);
	//Modifiers
	void setIdRef(const string &idRef);
	void setContext(const string &context);
	void setSection(const CSection &section);
	void setReference(CReference *pRef);
	//Selectors
	const string & getIdRef() const;
	const string & getContext() const;
	const CSection&  getSection()const ;
	const string&  getRefTitle()const ;
};

#endif
