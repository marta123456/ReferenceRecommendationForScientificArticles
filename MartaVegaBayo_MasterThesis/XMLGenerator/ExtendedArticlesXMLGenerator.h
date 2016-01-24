#if !defined (_EXTENDED_ARTICLES_XML_GENERATOR_H_)
#define _EXTENDED_ARTICLES_XML_GENERATOR_H_
#include "../DataModel/CArticleParsed.h"
#include "../DBmanager/DBManager.h"
#include "../DataModel/CName.h"
#include "../Pugixml/pugixml.hpp"
#include "BasicGenerator.h"
#include <fstream>
#include <string>

/**
	Class ExtendedArticlesXMLGenerator
	This class implements methods to generate the XML file 'ExtendedArticles.xml' with metadata of the articles of the dataset which have been parsed.
*/
class ExtendedArticlesXMLGenerator: public virtual BasicGenerator
{

protected:
	//Attributes of the object
	string aXMLName;	//Name of the generated file
	string aXMLPath;	//Path to the generated file
	ofstream aXMLFile;	//File stream

	//Static attributes of the class
	static string defaultXMLName;	//Default name for the generated file
	static string defaultXMLPath;	//Default path to the generated file


	//Methods to start and finish the creation of the XML
	void startXMLGeneration();
	void endXMLGeneration();

	//Method to add specific nodes to the xml document
	void addArticle(int articleId,  const string &abstract, const string &articleBody, const vector<string> &keywords, const vector<int> &references);

	void addArticleId(int articleId);
	
	void addKeywords(const vector<string> &keywords);
	void addKwd(const string &kwd);

	void addAbstract(const string &abstract);

	void addArticleBody(const string &articleBody);

	void addReferences(const vector<int> &references);
	void addReference(int referencedArticleId);

public:
	//Constructor
	ExtendedArticlesXMLGenerator(const string &DB,const string &DBserver, const string &DBuser, const string &DBpwd);
	//Method to create the XML	
	void createXML(const string &fileName=defaultXMLName, const string &filePath=defaultXMLPath, int nMax=-1);
	
	
};
#endif
