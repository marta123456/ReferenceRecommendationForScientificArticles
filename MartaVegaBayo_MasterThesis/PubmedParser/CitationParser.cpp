#include "CitationParser.h"

/**
	Constructor of the class CitationParser. Sets the aBodyNode to NULL in order to
	be able to know whether the article has been loaded.
*/
CitationParser::CitationParser():
	aBodyNode(NULL)
{}
/**
	Loads from the DOM node given, the child node 'body'.
	@param rootNode: root node of the xml document
	@throws string exception if the given node is a NULL node, or if the given node
	does not have a 'body' child node.
*/
void CitationParser::loadXMLnodes(const pugi::xml_node &rootNode)
{
	//Check that the given node is not null
	if(rootNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}
	//Load body node
	aBodyNode=rootNode.child("body");
	if(!aBodyNode){
		string error="XML doc not valid for the purpose of this dataset.";
		throw error;
	}

}
/**
	Provided that the article has correctly been loaded, parses from the document
	the all the citations and sets with it the aCitations attribute of the 
	aArticle object (attribute of ParserBase).
	In orther to do that, all the sections of the body are parsed
	@throws string exception if the article has not been loaded jet.
*/
void CitationParser::parse()
{
	//Check that the article has been loaded
	if(aBodyNode==NULL){
		string error="The article has to be loaded before being parsed.";
		throw error;
	}

	//select all sections child of 'body' and iterate over each section
	pugi::xpath_node_set sectionSet = aBodyNode.select_nodes("sec");
	for (uint i = 0; i < sectionSet.size(); i++)
	{
		//Parse the section
		CSection nestedSections;
		pugi:: xml_node secNode=sectionSet[i].node();
		parseSection(secNode, nestedSections, true);
	}
}

/**
	Parses the given section looking for the citations within it. The citations found are 
	added to the aArticle object. Nested sections are also parsed.

	@param secNode: 'sec' xml node to be analysed
	@param nestedSections: CSection object with the sections of hier levels to the current section
	@param isFirstSection: flag to know weather the current section section is a first level section (non-nested section)

*/
void CitationParser::parseSection(pugi::xml_node &secNode, CSection &nestedSections, bool isFirstSection)
{
	//GET SECTION IDENTIFIER, SECTION TYPE AND TITLE
	//Get section type
	string secType="";
	pugi::xml_attribute secTypeAttr=secNode.attribute("sec-type");
	if(secTypeAttr){secType=secTypeAttr.value();}
	//Get title
	string secTitle="";
	pugi::xml_node secTitleNode=secNode.child("title");
	if(secTitleNode){secTitle=secTitleNode.child_value();}

	//PUSH SECTION TO THE SECTIONS LIST
	if(isFirstSection){
		nestedSections.setSectionType(secType);
		nestedSections.setTitle(secTitle);
	}else{
		nestedSections.pushSubSection(secType, secTitle);
	}

	//ITERATE OVER THE ITEMS OF THE SECTION AND PARSE PARAGRAPHS AND SECTIONS
	for (pugi::xml_node secChildNode: secNode.children())
	{
		string secChildName=secChildNode.name();
		if(secChildName=="sec"){
			parseSection(secChildNode, nestedSections, false);
		}
		else if(secChildName=="p"){
			parseParagraph(secChildNode, nestedSections);
		}
	}	

	//POP SECTION FROM THE SECTIONS LIST
	nestedSections.popSubSection();
}

/**
	Parses one paragraph within a section in order to look for and parse the citations
	within it.
	@param pNode: 'p' xml node to be parsed.
	@param nestedSections: section (with nested sections) where the paragraph given is
*/
void CitationParser::parseParagraph(pugi::xml_node &pNode, CSection &nestedSections )
{
	//If there are no references in the paragraph, do not parse
	if(!pNode.child("xref"))return;
	//Select the content of the paragraph
	string context="";
	getTextWithinItem(pNode, context);
	//Iterate over the references
	for (pugi::xml_node childNode: pNode.children())
	{
		string childName=childNode.name();
		if(childName=="xref"){
			parseXref(childNode, context, nestedSections);
		}
	}
}
/**
	Parses the given 'xref' xml node. With the metadata parsed and the metadata given as parameters,
	creates and adds a citation to the aArticle object
	@param xrefNode: 'xref' xml node to parse
	@param context: paragraph where the xref node is
	@param nestedSections: section (with nested sections) where the paragraph is
	
*/
void CitationParser::parseXref(pugi::xml_node &xrefNode,  string context, CSection &nestedSections)
{
	//If not bibliographic reference, return
	string refType=xrefNode.attribute("ref-type").value();
	if(refType!="bibr"){return;}
	//If there is not an reference id, return
	pugi::xml_attribute ridAttr=xrefNode.attribute("rid");
	if(!ridAttr){return;}
	//Get reference id
	string refId=ridAttr.value();
	//Add citation
	aArticle.addCitation(refId, context, nestedSections);


}


