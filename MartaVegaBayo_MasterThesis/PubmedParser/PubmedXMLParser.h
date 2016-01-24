//Declaration of the PubmedXMLParser class
#if !defined (_PUBMEDXMLPARSER_H_)
#define _PUBMEDXMLPARSER_H_
#include <fstream>
#include "../Pugixml/pugixml.hpp"
#include "../DataModel/CArticleParsed.h"
#include "MetadataParser.h"
#include "ReferencesParser.h"
#include "CitationParser.h"
#include "BodyParser.h"
#include "ParserBase.h"

/**
	PubmedXMLParser class
	This class groups together and coordinates the functionality of the different parsers developed to parse the XML files of pùbmed articles.
	This class provides a constructor, a load method, a parse method and a method to retrieve the CArticleParsed where the metadata parsed is recorded.
*/
class PubmedXMLParser: public MetadataParser, ReferencesParser, CitationParser, BodyParser
{
private:
	string aFilePath;	//Path name and filename of the file to parse
	pugi::xml_document aXMLDoc;	//XML documment
	pugi::xml_node aArticleNode;	//'article' node of the DOM of the xml documment
	bool aParseBody;

public:
	PubmedXMLParser(bool parseBody=true);
	void loadXML(const string &filePath) ;
	void parseAll();
	const CArticleParsed & getArticle() const;


};

#endif
