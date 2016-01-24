#include "ExtendedArticlesXMLGenerator.h"

//Initialization of the static variables, which contain default values for the configuration of the system
string ExtendedArticlesXMLGenerator::defaultXMLName = "ExtendedArticlesXML.xml";
string ExtendedArticlesXMLGenerator::defaultXMLPath = "./XMLGenerated/";

/**
	Constructor of the class ExtendedArticlesXMLGenerator. Sets the attributes aDBserver, aDBuser and aDBpwd with the 
	given values or with a default value of server if no server is given.
	@param DBuser: username of the user of the database
	@param DBpwd: password of the username of the database
	@param DBserver: server name or IP address of the database. If no value is given, this parameter takes as
	default value the value of the static variable 'defaultDBserver'.
*/
ExtendedArticlesXMLGenerator::ExtendedArticlesXMLGenerator(const string &DB,const string &DBserver, const string &DBuser, const string &DBpwd):
	BasicGenerator(DB, DBserver, DBuser, DBpwd)
{
}
/**
	Creates the pubmed XML xml document with the advanced metadata (abstract, keywords and references) of the articles parsed and saved in the database.
	@param filename: name to give to the file to generate
	@param filepath: path to the location where the file will be saved
*/
void ExtendedArticlesXMLGenerator::createXML(const string &fileName, const string &filePath, int nMax)
{	
	//Initialize attributes
	aXMLName=fileName;
	aXMLPath=filePath;
	//Start xml doc
	startXMLGeneration();
	
	//Get articles ids from the database
	vector<int> articleIdList;
	DB.getCrawledArticlesIds(articleIdList, nMax);
	uint nArticles=articleIdList.size();

	//Loop getting articles from the database and adding them to the xml doc
	for(uint i=0; i<nArticles; i++){
		string abstract, articleBody;
		vector<string> keywords;
		vector<int> references;
		int articleId=articleIdList[i];
		DB.getArticleExtendedMetadata(articleId,  abstract, articleBody, keywords, references);
		addArticle(articleId,  abstract, articleBody, keywords, references);	
	}

	//End xml doc
	endXMLGeneration();
}

/**
	Starts the generation of the Pubmed XML documment. In order to do that, the file
	 is opened, the xml header is written and the openning root tag is written.
*/
void ExtendedArticlesXMLGenerator::startXMLGeneration()
{
	openFile(aXMLFile, aXMLPath, aXMLName);
	addXMLHeader(aXMLFile);
	addOpenningTag(aXMLFile, "extendedArticles");
}
/**
	Finishes the generation of the Article XML documment. In order to do that,
	the closing root tag is written and the file is closed.
*/
void ExtendedArticlesXMLGenerator::endXMLGeneration()
{
	addClosingTag(aXMLFile, "extendedArticles");
	closeFile(aXMLFile);
}
/**
	Writes in the xml file an 'pubmedArticle' node containing the advanced metadata given of an article
	@param articleId: id of the article to add to the xml documment
	@param abstract: abstract of the article to add
	@param keywords: vector with the keywords of the article to add
	@param references: vector with the ids of the articles referenced by the article to add
*/
void ExtendedArticlesXMLGenerator::addArticle(int articleId, const string &abstract, const string &articleBody, const vector<string> &keywords, const vector<int> &references)
{
	//Add openning tag
	addOpenningTag(aXMLFile, "extendedArticle");
	//Add nodes
	addArticleId(articleId);
	addKeywords(keywords);
	addAbstract(abstract);
	addArticleBody(articleBody);
	addReferences(references);
	//Add closing tag
	addClosingTag(aXMLFile, "extendedArticle");
	
}
/**
	Writes in the xml file an 'articleId' node with the id of the article.
	@param articleId: id of the article to add
*/
void ExtendedArticlesXMLGenerator::addArticleId(int articleId)
{
	addOpenningTag(aXMLFile, "articleId");
	addData(aXMLFile, to_string(articleId));
	addClosingTag(aXMLFile, "articleId");
}

/**
	Writes a 'keywords' node with the keywords of the article.
	@param keywords: vector with the keywords of the article to add
*/
void ExtendedArticlesXMLGenerator::addKeywords(const vector<string> &keywords)
{
	//Add an openning tag
	addOpenningTag(aXMLFile, "keywords");
	//Get the list of keywords and add one 'kwd' node per each one
	uint nKwd=keywords.size();
	for(uint i=0; i<nKwd; i++){
		addKwd(keywords[i]);
	}
	//Add the closing tag
	addClosingTag(aXMLFile, "keywords");
}
/**
	Writes a 'kwd' node containing one single keyword
	@param kwd: keyword to write
*/
void ExtendedArticlesXMLGenerator::addKwd(const string &kwd){
	addOpenningTag(aXMLFile, "kwd");
	addData(aXMLFile, kwd);
	addClosingTag(aXMLFile, "kwd");
}
/**
	Writes an 'abstract' node containing the abstract of the article.
	@param abstract: abstract of the article to add
*/
void ExtendedArticlesXMLGenerator::addAbstract(const string &abstract){
	addOpenningTag(aXMLFile, "abstract");
	addData(aXMLFile, abstract);
	addClosingTag(aXMLFile, "abstract");
}

/**
	Writes an 'articleBody' node containing the whole text of the article, provided that the body is not empty.
	@param articleBody: body of the article to add
*/
void ExtendedArticlesXMLGenerator::addArticleBody(const string &articleBody){
	if(articleBody!=""){
		addOpenningTag(aXMLFile, "articleBody");
		addData(aXMLFile, articleBody);
		addClosingTag(aXMLFile, "articleBody");
	}
}
/**
	Writes a 'references' node containing the different bibliographic references of
	the given article.
	@param references: vector with the ids of the articles referenced by the article to add
*/
void ExtendedArticlesXMLGenerator::addReferences(const vector<int> &references)
{
	addOpenningTag(aXMLFile, "references");
	uint nRefs=references.size();
	for(uint i=0; i<nRefs; i++){
		addReference(references[i]);
	}
	addClosingTag(aXMLFile, "references");
}
/**
	Writes a 'referenceArticleId' containning the id of the article referenced
	@param referencedArticleId: id of the referenced article
*/
void ExtendedArticlesXMLGenerator::addReference(int referencedArticleId)
{
	addOpenningTag(aXMLFile, "referencedArticleId");
	addData(aXMLFile, to_string(referencedArticleId));
	addClosingTag(aXMLFile, "referencedArticleId");
}







