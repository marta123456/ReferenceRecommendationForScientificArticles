//Definition of the class TitleParser
#if !defined (_TITLEPARSER_H_)
#define _TITLEPARSER_H_
#include "../Pugixml/pugixml.hpp"
#include "ParserBase.h"

/**
	TitleParser class.
	This class has methods and attributes to parse from the DOM of an XML article with the JATS schema the title of the article and
	set with it the corresponding attribute of the CArticleParsed object inherited form ParserBase.
*/
class TitleParser: public virtual ParserBase
{
private:
	pugi::xml_node aArticleMetaNode;//node of the DOM of the xml document where the title will be looked for

public:
	void loadXMLnodes(pugi::xml_node &rootNode);
	void parse();
};
#endif
