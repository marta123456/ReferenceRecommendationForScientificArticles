#include "TitleParser.h"

/**
	Saves in its attribute aArticleMetaNode the xml node given, which should be an 
	'article-meta' node containing the title.
	@param rootNode:root node of the document
*/
void TitleParser::loadXMLnodes(pugi::xml_node &rootNode)
{
	//Check that the given node is not null
	if(rootNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}
	//Load front.article-meta node
	aArticleMetaNode=rootNode.child("front").child("article-meta");
	if(!aArticleMetaNode){
		throw "XML doc not valid according to JAT dtd.";
	}
}
/**
	Provided that the article has correctly been loaded, parses from the loaded node
	the title and sets the corresponding attribute of the aArticle object.
	@throws string exception if the article has not been loaded jet.
*/
void TitleParser::parse()
{
	//Check if tha document has correctly been loaded
	if(aArticleMetaNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}
	//Get and save the title
	pugi::xml_node titleGroup=aArticleMetaNode.child("title-group");
	if(titleGroup){
		string title="";
		getTextWithinItem(titleGroup.child("article-title"), title);
		aArticle.setTitle(title);
		
	}
}

