#if !defined (_DATASETGENERATOR_H_)
#define _DATASETGENERATOR_H_

#include <string>
#include <iostream>
#include <dirent.h>
#include <sys/time.h>
#include <fstream>
#include <stdio.h>


#include "DataModel/CDate.h"
#include "DataModel/CName.h"
#include "DataModel/CArticle.h"
#include "DataModel/CArticleParsed.h"
#include "PubmedParser/PubmedXMLParser.h"
#include "Pugixml/pugixml.hpp"
#include "DBmanager/DBManager.h"
#include "XMLGenerator/XMLGenerator.h"

using namespace std;

/**
	Class DatasetGenerator
	This class provides methods to methods to use the classes developed to parse articles, generate the dataset and generate the xml files. In addition, this class provides methods to test the performance of that classes.
*/
class DatasetGenerator
{
private:
	//Attributes to set up the system
	string database;
	string server; //DB Server name
	string user;	//Name of the user of the database
	string pwd;	//Password for the user
	DBManager DB;		//Database manager object
	bool aParseBody;	//Flag defining whether the body of the article should be parsed or not
	int nArticles;

	//Atributes to save the testing measures
	ofstream parsingTimesFile;	//File stream were the parsing time will be saved
	ofstream processingTimesFile;	//File stream were the processing time will be saved
	ofstream dirNames;	//File stream were the name of the parsed file will be saved
	
	//Private methods
	void processEntity(string &path, struct dirent* entity);
	void processFile(const string &filePath, const string &fileName);
	void processDirectory(string &path, const string &directory);

public:
	//Public Methods
	DatasetGenerator(const string & database, const string & server, const string & user, const string & password);
	void createDatabase();
	void parseFile(const string &filePath, const string &fileName, bool parseBody=true);
	void parseDirectory(string &path, const string &directory, bool parseBody=true);
	void generateXML(XMLGenerator &XMLG, const string &dir, int &tArticles, int &tExtArticles, int &tCitations, int nArt=-1);
	void testXMLGenerator(const string &dir, int n=1);
};






#endif

