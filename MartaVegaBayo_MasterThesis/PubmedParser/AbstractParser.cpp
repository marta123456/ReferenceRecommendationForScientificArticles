//Definition of the class AbstractParser
#include "AbstractParser.h"

/**
	Saves in its attribute aArticleMetaNode the xml node given, which should be an 
	'article-meta' node containing the abstract.
	@param parentNode: root node of the xml document
*/
void AbstractParser::loadXMLnodes(pugi::xml_node &rootNode)
{
	//Check that the given node is not null
	if(rootNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}
	//Load article.front.article-meta node
	aArticleMetaNode=rootNode.child("front").child("article-meta");
	if(!aArticleMetaNode){
		throw "XML doc not valid according to JAT dtd.";
	}
}
/**
	Provided that the article has correctly been loaded, parses from the loaded node
	the abstract and, if it exists, sets the corresponding attribute of the aArticle 
	object.
	@throws string exception if the article has not been loaded jet.
*/
void AbstractParser::parse()
{
	//Check if tha document has correctly been loaded
	if(aArticleMetaNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}
	//Get and save abstract
	pugi::xml_node abstractNode=aArticleMetaNode.child("abstract");
	if(abstractNode){
		string abstractText="";
		getTextWithinItem(abstractNode, abstractText);
		aArticle.setAbstract(abstractText);
	}
}



