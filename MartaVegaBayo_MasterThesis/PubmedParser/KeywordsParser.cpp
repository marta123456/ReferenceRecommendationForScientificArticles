#include "KeywordsParser.h"

/**
	Saves in its attribute aArticleMetaNode the xml node given
	@param rootNode: root node of the document
*/
void KeywordsParser::loadXMLnodes(pugi::xml_node &rootNode)
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
	the keywords andsets the corresponding attribute of the aArticle object.
	@throws string exception if the article has not been loaded jet.
*/
void KeywordsParser::parse()
{
	//Check if tha document has correctly been loaded
	if(aArticleMetaNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}
	//GET AND SAVE THE KEYWORDS
	//Get group of keywords
	pugi::xml_node kwdGroupNode=aArticleMetaNode.child("kwd-group");
	vector<string> keywords;
	if(kwdGroupNode){
		//Parses each child of the kwd-group node
		for (pugi::xml_node child: kwdGroupNode.children())
		{
			vector<string> subKwd=getKeywords(child);
			for(string kwd :subKwd){keywords.push_back(kwd);}
		}	
	}
	aArticle.setKeywords(keywords);
}
/**
	Parses the given node, which should be a child of a 'kwd-group' and tries to
	get the keywords inside it.
	@param node: xml node to parse. Should be the child of a 'kwd-group'
	@return vector of string with the keyword, or keywords found in the node
*/
vector<string> KeywordsParser::getKeywords(pugi::xml_node &node)
{
	vector<string> keywords;
	//Get node name and parse accordingly
	string nodeName=node.name();
	if (nodeName == "kwd"){
		keywords.push_back(getSimpleKeyword(node));
	}
	else if(nodeName=="compound-kwd"){
		keywords=getCompoundKeyword(node);
	}else if(nodeName=="nested-kwd"){
		keywords=getNestedKeyword(node);
	}
	return keywords;
}
/**
	Parses the content of the given node, which should be a 'kwd' xml node,
	and gets the keyword inside it.
	@param node: xml node to parse
	@return string with the keyword found
*/
string KeywordsParser::getSimpleKeyword(pugi::xml_node &node)
{
	return node.child_value();
}
/**
	Parses the content of the given node, which should be a 'compound-kwd' 
	xml node, and gets all the keywords inside it.
	@param node: xml node to parse
	@return vector of string with the keywords found
*/
vector<string> KeywordsParser::getCompoundKeyword(pugi::xml_node &node)
{
	vector<string> keywords;
	for (pugi::xml_node child: node.children())
	{
		string childName=child.name();
			if (childName == "compound-kwd-part"){
				string kwd=child.child_value();
				keywords.push_back(kwd);
			}
	}
	return keywords;
}
/**
	Parses the content of the given node, which should be a 'nested-kwd' 
	xml node, and gets all the keywords inside it.
	@param node: xml node to parse
	@return vector of string with the keywords found
*/
vector<string> KeywordsParser::getNestedKeyword(pugi::xml_node &node)
{
	vector<string> keywords;
	for (pugi::xml_node child: node.children())
	{
		vector<string> subKwd=getKeywords(child);
		for(string kwd :subKwd){keywords.push_back(kwd);}
	}
	return keywords;
}



