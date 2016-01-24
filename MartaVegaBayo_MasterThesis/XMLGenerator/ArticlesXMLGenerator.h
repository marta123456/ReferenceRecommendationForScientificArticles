#if !defined (_ARTICLESXMLGENERATOR_H_)
#define _ARTICLESXMLGENERATOR_H_
#include "../DataModel/CArticleParsed.h"
#include "../DBmanager/DBManager.h"
#include "../DataModel/CName.h"
#include "../Pugixml/pugixml.hpp"
#include "BasicGenerator.h"
#include <fstream>
#include <string>

/**
	Class ArticlesXMLGenerator
	This class implements methods to generate the XML file 'Articles.xml' with metadata of all the articles of the dataset.
*/
class ArticlesXMLGenerator:  public virtual BasicGenerator
{

protected:
	//Attributes of the object
	string aXMLName;	//Name of the generated file
	string aXMLPath;	//Path to the generated file
	ofstream aXMLFile;	//File stream

	//Static attributes of the class
	static string defaultXMLName;	//Default name for the generated file
	static string defaultXMLPath;	//Default path to the generated file


	//Methods to start and finish the creation of the index
	void startXMLGeneration();
	void endXMLGeneration();

	//Method to add specific nodes to the xml document
	void addArticle(int articleId, const string &title, const CDate &pubDate, const vector<CName> &authors);

	void addArticleId(int articleId);
	void addTitle(const string &title);

	void addAuthors( const vector<CName> &authors);
	void addAuthor(const CName &author);
	void addFirstname(const string &firstname);
	void addLastname(const string &lastname);

	void addPublicationDate(const CDate &pubDate);
	void addYear(int year);
	void addMonth(int month);
	void addDay(int day);



public:
	//Constructor
	ArticlesXMLGenerator(const string &DB,const string &DBserver, const string &DBuser, const string &DBpwd);
	//Method to create the index	
	void createXML(const string &fileName=defaultXMLName, const string &filePath=defaultXMLPath, int nMax=-1);
	
	
};
#endif
