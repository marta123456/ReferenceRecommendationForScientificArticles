//Definition of the AuthorParser class
#include "AuthorParser.h"

/**
	Saves in its attribute aArticleMetaNode the xml node given, which should be an 
	'article-meta' node containing the authors of the article.
	@param rootNode: root node of the document
*/
void AuthorParser::loadXMLnodes(pugi::xml_node &rootNode)
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
	the authors and sets the corresponding attribute of the aArticle 
	object.
	@throws string exception if the article has not been loaded jet.
*/
void AuthorParser::parse()
{
	//Check if tha document has correctly been loaded
	if(aArticleMetaNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}

	//GET AND SAVE THE AUTHORS
	vector<CName> authors;
	//Get all contribution groups and iterate over each group
	pugi::xpath_node_set contribGroupSet = aArticleMetaNode.select_nodes("contrib-group");
	for (size_t i = 0; i < contribGroupSet.size(); ++i)
	{
		//Select the different items of the group and iterate over them
		pugi:: xml_node contribGroupNode=contribGroupSet[i].node();
	
		for (pugi::xml_node child: contribGroupNode.children())
		{
			//Select those items with name 'contrib'
			string childName=child.name();
			if (childName == "contrib"){
				//Skip contrib if the attribute 'contrib-type' exits and has a value different to 'author'
				string contribType= child.attribute("contrib-type").value();
				if(contribType=="author"){
					CName authorName=parseName(child);
					authors.push_back(authorName);
				}
			}
		}	
	}
	aArticle.setAuthors(authors);
}
/**
	Parses the given xml node to get the last name and firstname or abreviation
	of a name.
	@param node: xml node to parse, should be a contrib node with a 'name' or 'string-name' as a child
	@return CName object containing the parsed data
*/
CName AuthorParser::parseName(pugi::xml_node &node)
{
	//Select the 'name' item
	pugi::xml_node nameNode=node.child("name");
	if(!nameNode){
		nameNode=node.child("string-name");
		if(!nameNode){
				return CName("", "");
		}
	}
	//Select firstname and surname
	string authorLastname=nameNode.child("surname").child_value();
	string authorFirstname=nameNode.child("given-names").child_value();
	return CName(authorFirstname, authorLastname);
}

