//Declaration of the AuthorParser class
#if !defined (_AUTHORPARSER_H_)
#define _AUTHORPARSER_H_
#include "../DataModel/CArticle.h"
#include "../Pugixml/pugixml.hpp"
#include "ParserBase.h"
using namespace std;

/**
	AuthorParser class.
	This class has methods and attributes to parse from the DOM of an XML article with the JATS schema the authors of the article and
	set with it the corresponding attribute of the CArticleParsed object inherited form ParserBase.
*/
class AuthorParser: public virtual ParserBase
{
private:
	pugi::xml_node aArticleMetaNode;//DOM node from where the authors will be looked for

	CName parseName(pugi::xml_node &node);

public:
	void loadXMLnodes(pugi::xml_node &rootNode);
	void parse();
};
#endif
