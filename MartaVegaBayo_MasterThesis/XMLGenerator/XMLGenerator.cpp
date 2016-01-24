#include "XMLGenerator.h"


/**
	Constructor of the class ArticlesXMLGenerator. Sets the attributes aDBserver, aDBuser and aDBpwd with the given values or with a default value of server if no server is given. It also calls the constructors of the superclasses.
	@param DBuser: username of the user of the database
	@param DBpwd: password of the username of the database
	@param DBserver: server name or IP address of the database. If no value is given, this parameter takes as
	default value the value of the static variable 'defaultDBserver'.
*/
XMLGenerator::XMLGenerator(const string &DB,const string &DBserver, const string &DBuser, const string &DBpwd):
	BasicGenerator(DB, DBserver, DBuser, DBpwd),
	ArticlesXMLGenerator(DB, DBserver, DBuser, DBpwd),
	ExtendedArticlesXMLGenerator(DB, DBserver, DBuser, DBpwd),
	CitationsXMLGenerator(DB, DBserver, DBuser, DBpwd)
{

}
/**
	Creates the Articles xml documment, which contains the basic metadata of all the articles in the database (both pubmed articles and articles referenced by them).
	@param filename: name to give to the file to generate
	@param filepath: path to the location where the file will be saved
*/
void XMLGenerator::createArticlesXML(const string &fileName, const string &filePath, int nMax)
{
	ArticlesXMLGenerator::createXML(fileName, filePath, nMax);
}
/**
	Creates the Extended Articles xml documment, which contains the advanced metadata of the pubmed articles parsed.
	@param filename: name to give to the file to generate
	@param filepath: path to the location where the file will be saved
*/
void XMLGenerator::createExtendedArticlesXML(const string &fileName, const string &filePath, int nMax)
{
	ExtendedArticlesXMLGenerator::createXML(fileName, filePath, nMax);
}
/**
	Creates a Citations xml documment which contains the citations of the articles of pubmed parsed.
	@param filename: name to give to the file to generate
	@param filepath: path to the location where the file will be saved
*/
void XMLGenerator::createCitationsXML(const string &fileName, const string &filePath, int nMax)
{
	CitationsXMLGenerator::createXML(fileName, filePath, nMax);
}


