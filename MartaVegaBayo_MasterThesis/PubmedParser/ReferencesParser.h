//Declaration of the class ReferenceParser
#if !defined (_REFERENCESPARSER_H_)
#define _REFERENCESPARSER_H_

#include "../Pugixml/pugixml.hpp"
#include "../DataModel/CArticleParsed.h"
#include "ParserBase.h"
#include <regex>

/**
	ReferencesParser class
	This class implements method to parse the DOM of the XML files of pubmed and get the reference list with metadata of the referenced works and the id of the reference entries.
*/
class ReferencesParser: public virtual ParserBase
{
private:
	pugi::xml_node aBackNode;	// 'back' node of the DOM of the xml document

	//Method to parse the DOM and iterate over the citation's nodes
	void parseRefList(const pugi::xml_node &refListNode);
	void parseRef(const pugi::xml_node &refNode);

	//Methods to parse the metadata of a reference from the XML tags
	bool parseRefFromXML(pugi::xml_node &citNode, string &title, int &year, int &month, int &day, vector<CName> &authors);
	bool parsePubDateFromXML(pugi::xml_node &citNode, int &year, int &month, int &day);
	bool parseTitleFromXML(pugi::xml_node &citNode, string &title);
	bool parseAuthorsFromXML(pugi::xml_node &citNode, vector<CName> &authors);
	void parseName(pugi::xml_node &node, CName &name);

	//Methods to parse the metadata of a reference from a given string with APA referencing style
	bool parseAPAref(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors);
	bool parseAPArefWithAuthors(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors);
	bool parseAPArefWithoutAuthors(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors);
	bool parseAPApubDate(string &sDate, int &year, int &month, int &day);


	//Methods to parse the metadata of a reference from a given string with ICMJE referencing style
	bool parseICMJEref(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors);
	bool parseICMJErefWithAuthors(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors);
	bool parseICMJErefWithoutAuthors(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors);

	bool parseAuthorsFromText(string &sAuthors, vector<CName> &authors);
	bool parseTitleFromText(string &sTitle, string &title);
	bool checkValidTitle(string &sTitle);
	bool checkValidAuthors(vector<CName> &authors);

public:
	ReferencesParser();
	void loadXMLnodes(const pugi::xml_node &rootNode);
	void parse();
};

#endif
