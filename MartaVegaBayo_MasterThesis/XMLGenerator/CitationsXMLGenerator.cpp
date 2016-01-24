#include "CitationsXMLGenerator.h"
//Initialization of the static variables, which contain default values for the configuration of the system
string CitationsXMLGenerator::defaultXMLName = "Citations.xml";
string CitationsXMLGenerator::defaultXMLPath = "./XMLGenerated/";

/**
	Constructor of the class CitationsXMLGenerator. Sets the attributes aDBserver, aDBuser and aDBpwd with the 
	given values or with a default value of server if no server is given.
	@param DBuser: username of the user of the database
	@param DBpwd: password of the username of the database
	@param DBserver: server name or IP address of the database. If no value is given, this parameter takes as
	default value the value of the static variable 'defaultDBserver'.
*/
CitationsXMLGenerator::CitationsXMLGenerator(const string &DB,const string &DBserver, const string &DBuser, const string &DBpwd):
	BasicGenerator(DB, DBserver, DBuser, DBpwd)
{
}


void CitationsXMLGenerator::createXML(const string &fileName, const string &filePath, int nMax)
{	
	aXMLName=fileName;
	aXMLPath=filePath;
	//Start xml doc
	startXMLGeneration();
	
	//Get citations ids from the database
	vector<int> citationIdList;
	//cout<<"antes del DB.getCitationsIds()" <<endl;
	//getchar();
	DB.getCitationsIds(citationIdList, nMax);
	uint nCitations=citationIdList.size();
	//Loop getting articles from the database and adding them to the xml doc
	for(uint i=0; i<nCitations; i++){
		int citId=citationIdList[i];
		int sourceId, referencedId;
		string context;
		CSection section;
		DB.getCitation(citId, sourceId, referencedId, context, section);
		addCitation(sourceId, referencedId, context, section);	
	}

	//End xml doc
	endXMLGeneration();
}
/**
	Starts the generation of the Citations xml documment. In order to do that, the file
	 is opened, the xml header is written and the openning root tag is written.
*/
void CitationsXMLGenerator::startXMLGeneration()
{
	openFile(aXMLFile, aXMLPath, aXMLName);
	addXMLHeader(aXMLFile);
	addOpenningTag(aXMLFile, "citations");
}
/**
	Finishes the generation of the Citation Index xml documment. In order to do that,
	the closing root tag is written and the file is closed.
*/
void CitationsXMLGenerator::endXMLGeneration()
{
	addClosingTag(aXMLFile, "citations");
	closeFile(aXMLFile);
}
/**
	Writes a 'citation' node containing the metadata of one citation.
	@param sourceId: id of the article wich contains the citation
	@param referencedId: id of the referenced article/book
	@param context: context of the citation
	@param section: section where the citation is within the article
*/
void CitationsXMLGenerator::addCitation(int sourceId, int referencedId, const string &context, const CSection &section)
{
	addOpenningTag(aXMLFile, "citation");
	addSourceArticleId(sourceId);
	addReferencedArticleId(referencedId);
	addContext(context);
	addSection(section);
	addClosingTag(aXMLFile, "citation");
}
/**
	Writes an 'sourceArticleId' node containing the id of the article containing the citation.
	@param sourceArticleId: id of the article with the citation.
*/
void CitationsXMLGenerator::addSourceArticleId(int sourceArticleId)
{
	addOpenningTag(aXMLFile, "sourceArticleId");
	addData(aXMLFile, to_string(sourceArticleId));
	addClosingTag(aXMLFile, "sourceArticleId");
}
/**
	Writes a 'referencedArticleId' containing the id of the referenced article of the
	citation.
	@param referencedArticleId: id of the referenced article.
*/
void CitationsXMLGenerator::addReferencedArticleId(int referencedArticleId)
{
	addOpenningTag(aXMLFile, "referencedArticleId");
	addData(aXMLFile, to_string(referencedArticleId));
	addClosingTag(aXMLFile, "referencedArticleId");
}
/**
	Writes a 'context' node containing the text of the paragraph where the citation is
	withinthe article.
	@param context: context where the citation is.
*/
void CitationsXMLGenerator::addContext(const string &context)
{
	addOpenningTag(aXMLFile, "context");
	addData(aXMLFile, context);
	addClosingTag(aXMLFile, "context");
}

/**
	Writes a 'section' node containing the metadata of one single section.
	@param section: section object with the metadata of a section
*/
void CitationsXMLGenerator::addSection(const CSection &section)
{
	addOpenningTag(aXMLFile, "section");
	addSectionType(section.getSectionType());
	addSectionTitle(section.getTitle());
	if(section.getPSubSection()!=NULL){
		addSection(*section.getPSubSection());
	}
	addClosingTag(aXMLFile, "section");
}
/**
	Writes a 'sectionType' node containing the name of the type of section.
	@param sectionType: type of section
*/
void CitationsXMLGenerator::addSectionType(const string &sectionType)
{
	if(sectionType!="")
	{
		addOpenningTag(aXMLFile, "sectionType");
		addData(aXMLFile, sectionType);
		addClosingTag(aXMLFile, "sectionType");
	}

}
/**
	Writes a 'sectionTitle' node containing the title of a section.
	@param sectionTitle: title of the section.
*/
void CitationsXMLGenerator::addSectionTitle(const string &sectionTitle)
{
	addOpenningTag(aXMLFile, "sectionTitle");
	addData(aXMLFile, sectionTitle);
	addClosingTag(aXMLFile, "sectionTitle");
}





