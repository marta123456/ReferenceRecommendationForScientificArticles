#include "BodyParser.h"

/**
	Constructor of the class. Initializes the attribute, which is an XML node, to NULL
*/
BodyParser::BodyParser():
	aBodyNode(NULL)
{}


/**
	Loads from the DOM node given, the child node 'body'.
	@param rootNode: root node of the document
	@throws string exception if the given node is a NULL node, or if the given node
	does not have a 'body' child node.
*/
void BodyParser::loadXMLnodes(const pugi::xml_node &rootNode)
{	
	//Check that the given node is not null
	if(rootNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}
	//Load back node
	aBodyNode=rootNode.child("body");
	if(!aBodyNode){
		string error="XML doc not valid for the purpose of this dataset.";
		throw error;
	}

}

/**
	Provided that the article has correctly been loaded, parses from the document
	the text content of the article and sets with it the aArticleBody attribute of the 
	aArticle object (attribute of ParserBase).
	@throws string exception if the article has not been loaded jet.
*/
void BodyParser::parse()
{
	//Check that the article has been loaded
	if(aBodyNode==NULL){
		string error="The article has to be loaded before being parsed.";
		throw error;
	}
	
	//Get and parse the content of the article
	string articleText="";
	for (pugi::xml_node node: aBodyNode.children())
	{
		string nodeName=node.name();
		if (nodeName == "sec"){
			parseSection(node, articleText);
		}
	}
	//Set the aArticle corresponding attribute
	aArticle.setArticleBody(articleText);
}
/**
	Gets the text (title an parragraphs) of the given section node, and appends it to the given string. Afterwards, 
	using the parseParagraph() method, parses the content of the paragraphs and appends it to the given string.
	If there are nested sections in the DOM, the method calls itself recursively.
	@param secNode: sec XML node to be parsed
	@param articleText: string where the title and parragraph content will be append
*/
void BodyParser::parseSection(pugi::xml_node &secNode, string &articleText)
{
	//Get title of the section
	pugi::xml_node secTitleNode=secNode.child("title");
	if(secTitleNode)
	{
		articleText+="\n";
		articleText+=secTitleNode.child_value();
		articleText+="\n";
	}
	//Iterate over the items of the section and parse sections and parragraphs
	for (pugi::xml_node secChildNode: secNode.children())
	{
		string secChildName=secChildNode.name();
		if(secChildName=="sec"){
			parseSection(secChildNode, articleText);
		}
		else if(secChildName=="p"){
			parseParagraph(secChildNode, articleText);
		}
	}	
}

/**
	Gets the text within the given node, which is expected to be a 'p' node, and appends it to the 
	given string.
	@param pNocde: 'p' XML node whose content is to be parsed
	@param articleText: string where the parsed text of the paragraph will be append.
*/
void BodyParser::parseParagraph(pugi::xml_node &pNode, string &articleText)
{
	//Get text within node
	string text="";
	getTextWithinItem(pNode, text);
	//Append to the article text string
	articleText+=text;
	articleText+="\n";

}










