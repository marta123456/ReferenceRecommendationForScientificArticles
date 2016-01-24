//Declaration of the BodyParser class
#if !defined (_BODYPARSER_H_)
#define _BODYPARSER_H_

#include "../Pugixml/pugixml.hpp"
#include "../DataModel/CArticleParsed.h"
#include "ParserBase.h"

/**
	BodyParser class.
	This class has methods and attributes to parse from the DOM of an XML article with the JATS schema the body text of the article and set with it the corresponding attribute of the CArticleParsed object inherited form ParserBase.
*/
class BodyParser: public virtual ParserBase
{
private:
	pugi::xml_node aBodyNode;

	void parseSection(pugi::xml_node &secNode, string &articleText);
	void parseParagraph(pugi::xml_node &pNode, string &articleText);

public:
	BodyParser();
	void loadXMLnodes(const pugi::xml_node &rootNode);
	void parse();

};

#endif
