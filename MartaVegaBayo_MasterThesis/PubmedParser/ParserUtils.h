//Declaration of functions used by the parsers
#if !defined (_PARSERUTILS_H_)
#define _PARSERUTILS_H_
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "../Pugixml/pugixml.hpp"

using namespace std;

void getTextWithinItem(pugi::xml_node node, string &text);

vector<string> split(const string &s, const string &delim);

vector<string> splitIntoSentences(const string &s);


#endif
