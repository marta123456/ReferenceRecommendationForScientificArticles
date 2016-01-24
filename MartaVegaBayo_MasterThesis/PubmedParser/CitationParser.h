#if !defined (_CITATIONPARSER_H_)
#define _CITATIONPARSER_H_

#include "../Pugixml/pugixml.hpp"
#include "../DataModel/CArticleParsed.h"
#include "../DataModel/CSection.h"
#include "ParserBase.h"


class CitationParser: public virtual ParserBase
{
private:
	pugi::xml_node aBodyNode; // 'body' node of the DOM of the xml document

	void parseSection(pugi::xml_node &secNode, CSection &nestedSections, bool isFirstSection);
	void parseParagraph(pugi::xml_node &pNode, CSection &nestedSections);
	void parseXref(pugi::xml_node &xrefNode,  string context, CSection &nestedSections);


public:
	CitationParser();
	void loadXMLnodes(const pugi::xml_node &rootNode);
	void parse();
	void parseSection(pugi::xml_node &secNode, CSection &nestedSections);
	
};


#endif
