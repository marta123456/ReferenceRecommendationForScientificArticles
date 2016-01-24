#include "PubmedXMLParser.h"

/**
	Constructor of the class PubmedXMLParser. Sets the aArticleNode to NULL in order to
	be able to know whether the article has been loaded.
*/
PubmedXMLParser::PubmedXMLParser(bool parseBody):
	aArticleNode(NULL), aParseBody(parseBody)
{}
/**
	Loads the xml DOM of the documment with the file path and name given, gets the 
	'article' node of the DOM, calls the 'loadXMLnodes()' method of the superclasses.
	@param filePath: name of the path and filename of the file to parse
	@throws string exception if the xml file can not be loaded, if the root node of the
	DOM is not an 'article' node or if one of the 'loadXMLnodes()' methods called throws it.
*/
void PubmedXMLParser::loadXML(const string &filePath)
{
	aFilePath=filePath;
	//Load xml article
	pugi::xml_parse_result result = aXMLDoc.load_file(aFilePath.c_str());
	if(!result){
		throw result.description();
	}
	//Get article node
	aArticleNode=aXMLDoc.child("article");
	if(!aArticleNode){
		throw "XML doc not valid according to JAT dtd";
	}

	//Set usefull nodes for MetadataParser
	MetadataParser::loadXMLnodes(aArticleNode);
	ReferencesParser::loadXMLnodes(aArticleNode);
	CitationParser::loadXMLnodes(aArticleNode);
	BodyParser::loadXMLnodes(aArticleNode);
}

/**
	Parses all the information of the article previously loaded. To parse
	the information, the method calls the 'parse()' method of the superclasses
	MetadataParser, ReferencesParser and CiationParser.
	@throws string exception if the article has not been loaded jet.
*/
void PubmedXMLParser::parseAll()
{
	//Check if the article has already been loaded
	if(aArticleNode==NULL){
		string error="The article has to be loaded before being parsed.";
		throw error;
	}
	//Parse metadata of the article
	MetadataParser::parse();
	//Parse references of the article
	ReferencesParser::parse();
	//Parse citations within the article
	CitationParser::parse();
	//Parse body text of the article
	if(aParseBody)BodyParser::parse();
}
/**
	Returns the CArticleParsed object which contains the metadata of
	the parsed article.
*/
const CArticleParsed & PubmedXMLParser::getArticle() const
{
	return aArticle;
}
