//Definition of the class ParserBase
#if !defined (_PARSERBASE_H_)
#define _PARSERBASE_H_
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "../Pugixml/pugixml.hpp"
#include "../DataModel/CArticleParsed.h"

/**
	ParserBase class. This class is the base class from where the rest of the parsers inherit.
	The parser developed if made of different classes and subclasses. These classes inherit creating a diamond structure, 
	being this class, ParserBase, the base of the hierarchical structure. Therefore, all the subclasses inherit the CArticleParses
	which is an attribute of this class, and record there the parsed metadata.
*/
class ParserBase
{
protected:
	CArticleParsed aArticle;//Article object where the metadata parsed by the different Parsers (which are ubclasses of this class) will be set


	void getTextWithinItem(pugi::xml_node node, string &text);

	vector<string> split(const string &s, const string &delim);

	vector<string> splitIntoSentences(const string &s);

};

#endif
