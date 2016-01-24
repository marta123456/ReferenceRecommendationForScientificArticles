#include "MetadataParser.h"

/**
	Constructor of the class MetadataParser. Sets the aFrontNode and aArticleMeta to 
	NULL in order to be able to know whether the article has been loaded.
*/
MetadataParser::MetadataParser():
	aFrontNode(NULL), aArticleMetaNode(NULL)
{}
/**
	Loads from the DOM node given, the child nodes 'front' and 'article-meta'. It also
	calls the 'loadXMLnodes()' method of the superclasses, so they load their xml nodes
	needed
	@param rootNode: root node of the xml document
	@throws string exception if the given node is a NULL node, or if the given node
	does not have a 'front' child node with an 'article-meta' child node.
*/
void MetadataParser::loadXMLnodes(pugi::xml_node &rootNode)
{
	//Check that the given node is not null
	if(rootNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}
	//Load front node
	aFrontNode=rootNode.child("front");
	if(!aFrontNode){
		throw "XML doc not valid according to JAT dtd.";
	}
	//Load article-meta node
	aArticleMetaNode=aFrontNode.child("article-meta");
	if(!aArticleMetaNode){
		throw "XML doc not valid according to JAT dtd.";
	}

	//Load nodes for titleParser
	TitleParser::loadXMLnodes(rootNode);
	AbstractParser::loadXMLnodes(rootNode);
	PubDateParser::loadXMLnodes(rootNode);
	KeywordsParser::loadXMLnodes(rootNode);
	AuthorParser::loadXMLnodes(rootNode);
}
/**
	Parses some metadata of the article and sets with it the corresponding attribute
	of the aArticle object (attribute of the class ParserBase)
*/
void MetadataParser::parse(){

	TitleParser::parse();
	AbstractParser::parse();
	PubDateParser::parse();
	KeywordsParser::parse();
	AuthorParser::parse();
}
