//Declaration of the class MetadataParser
#if !defined (_METADATAPARSER_H_)
#define _METADATAPARSER_H_

#include "../Pugixml/pugixml.hpp"
#include "../DataModel/CArticleParsed.h"
#include "TitleParser.h"
#include "AbstractParser.h"
#include "PubDateParser.h"
#include "KeywordsParser.h"
#include "AuthorParser.h"

/**
	MetadataParser class.
	This class inherits simultaneously from different parsers in order to aggregate their functionality in a single class.
	This class, therefore can parse from DOM of the XMl article metadata of the article such as title, abstract, publication date, keywords and authors
*/
class MetadataParser: public TitleParser, AbstractParser, PubDateParser, KeywordsParser, AuthorParser
{
private:
	pugi::xml_node aFrontNode;// 'front' node of the DOM of the xml document
	pugi::xml_node aArticleMetaNode;// 'article-meta' node of the DOM of the xml document

public:
	MetadataParser();
	void loadXMLnodes(pugi::xml_node &rootNode);
	void parseTitle(){TitleParser::parse();}
	void parseAbstract(){AbstractParser::parse();}
	void parsePubDate(){PubDateParser::parse();}
	void parseKeywords(){KeywordsParser::parse();}
	void parseAuthor(){AuthorParser::parse();}
	void parse();
};

#endif
