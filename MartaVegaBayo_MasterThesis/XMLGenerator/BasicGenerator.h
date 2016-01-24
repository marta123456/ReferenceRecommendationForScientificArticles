#if !defined (_BASICGENERATOR_H_)
#define _BASICGENERATOR_H_
#include "../DataModel/CArticleParsed.h"
#include "../DBmanager/DBManager.h"
#include "../Pugixml/pugixml.hpp"
#include <fstream>
#include <iostream>
#include <string>
/**
	BasicGenerator class
	This class provides basic methods used by other classes to generate the XML files, such as openning and closing files,
	adding openning XML tags, adding clossing XML tags or adding data.
	This class also contains as an attributew a DBManager object, which is inherited and used by other classes to read from 
	the database the metadata to add to the XML files to generate. The connection of the DBManager is openned by the constructor 
	and closed by the destructor of the BasicGenerator.
*/
class BasicGenerator
{
private:
	void escapeXMLCharacters(string &data);
	void replaceString(string& subject, const string& search, const string& replace);
protected:
	//Attributes of the object
	string aDBname;
	string aDBserver;		//Name of the server with the database
	string aDBuser;			//Username of the user of the database
	string aDBpwd;			//Password of the user of the database
	DBManager DB; 			//Database manager


	//Method to open and close a given file
	void openFile(ofstream &file, const string &filePath, const string &fileName);
	void closeFile(ofstream &file);

	//Methods to add xml tags and elements to the file
	void addXMLHeader( ofstream &file);
	void addOpenningTag(ofstream &file, const string &tagName);
	void addClosingTag(ofstream &file, const string &tagName);
	void addData( ofstream &file, const string &data);
public:
	BasicGenerator(const string &DB,const string &DBserver, const string &DBuser, const string &DBpwdr);
	~BasicGenerator();
};
#endif
