//Declaration of the class PubDateParser
#if !defined (_PUBDATEPARSER_H_)
#define _PUBDATEPARSER_H_
#include "../DataModel/CArticleParsed.h"
#include "ParserBase.h"
#include "../Pugixml/pugixml.hpp"

/**
	PubDateParser class.
	This class has methods and attributes to parse from the DOM of an XML article with the JATS schema the publication date of the article and set with it the corresponding attribute of the CArticleParsed object inherited form ParserBase.
*/
class PubDateParser: public virtual ParserBase
{
private:
	pugi::xml_node aArticleMetaNode; //node of the DOM of the xml document where the publication date will be looked for

public:
	void loadXMLnodes(pugi::xml_node &rootNode);
	void parse();
};
#endif
