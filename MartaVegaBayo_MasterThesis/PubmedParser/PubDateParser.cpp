#include "PubDateParser.h"

/**
	Saves in its attribute aArticleMetaNode the xml node given, which should be an 
	'article-meta' node containing the publication date.
	@param rootNode: DOM node ('article-meta' node which might contain the publication date).
*/
void PubDateParser::loadXMLnodes(pugi::xml_node &rootNode)
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
	the publication date and sets the corresponding attribute of the aArticle object.
	@throws string exception if the article has not been loaded jet.
*/
void PubDateParser::parse()
{
	pugi::xml_node pubDateNode=aArticleMetaNode.child("pub-date");
	//Check if tha document has correctly been loaded
	if(aArticleMetaNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}
	//Get and save publication date
	if(pubDateNode){
		string sDay="0", sMonth="0", sYear="0";
		for (pugi::xml_node child: pubDateNode.children())
		{
			string childName=child.name();
			if (childName == "day"){
				sDay=child.child_value();
			}else if(childName== "month"){
				sMonth=child.child_value();
			}else if(childName== "year"){
				sYear=child.child_value();
			}
		}
		CDate pubDate;
		pubDate.setDate(stoi(sYear), stoi(sMonth), stoi(sDay));
		aArticle.setPubDate(pubDate);
	}
	
}



