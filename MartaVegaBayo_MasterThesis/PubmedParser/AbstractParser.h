//Declaration of the class AbstractParser
#if !defined (_ABSTRACTPARSER_H_)
#define _ABSTRACTPARSER_H_
#include "../DataModel/CArticleParsed.h"
#include "../Pugixml/pugixml.hpp"
#include "ParserBase.h"

/**
	AbstractParser class.
	This class has methods and attributes to parse from the DOM of an XML article with the JATS schema the abstract of the article and set with it the corresponding attribute of the CArticleParsed object inherited form ParserBase.
*/
class AbstractParser: public virtual ParserBase
{
private:
	pugi::xml_node aArticleMetaNode; //DOM node from where the abstract will be looked for

public:
	void loadXMLnodes(pugi::xml_node &rootNode);
	void parse();
};
#endif
