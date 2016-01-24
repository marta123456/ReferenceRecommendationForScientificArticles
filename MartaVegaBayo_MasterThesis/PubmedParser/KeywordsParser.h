//Declaration of the class KeywordsParser
#if !defined (_KEYWORDSPARSER_H_)
#define _KEYWORDSPARSER_H_
#include "../DataModel/CArticleParsed.h"
#include "../Pugixml/pugixml.hpp"
#include "ParserBase.h"
#include <vector>
/**
	KeywordsParser class.
	This class has methods and attributes to parse from the DOM of an XML article with the JATS schema the keywords of the article and
	set with it the corresponding attribute of the CArticleParsed object inherited form ParserBase.
*/
class KeywordsParser: public virtual ParserBase
{
private:
	pugi::xml_node aArticleMetaNode;//node of the DOM of the xml document where the keywords will be looked for

	vector<string> getKeywords(pugi::xml_node &parentNode);
	string getSimpleKeyword(pugi::xml_node &parentNode);
	vector<string> getCompoundKeyword(pugi::xml_node &parentNode);
	vector<string> getNestedKeyword(pugi::xml_node &parentNode);
public:
	void loadXMLnodes(pugi::xml_node &rootNode);
	void parse();
};
#endif
