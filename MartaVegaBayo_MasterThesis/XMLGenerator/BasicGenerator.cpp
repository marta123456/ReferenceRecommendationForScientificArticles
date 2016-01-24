#include "BasicGenerator.h"

/**
	Constructor of the class BasicGenerator. Initializes the DBManager object and connects with the database with the metadata to be dumped into the xml files to generate.
*/
BasicGenerator::BasicGenerator(const string &DBname,const string &DBserver, const string &DBuser, const string &DBpwd):
	aDBname(DBname), aDBserver(DBserver), aDBuser(DBuser), aDBpwd(DBpwd), DB(aDBname, DBserver, DBuser, DBpwd)
{
	DB.connect();
}
/**
	Destructor of the class BasicGenerator. Closes the connection with the database.
*/
BasicGenerator::~BasicGenerator()
{
	DB.close();
}
/**
	Method to open in write mode the file with given name and path
	@param file: file stream to open
	@param filePath: path to the file to open
	@param fileName: name of the path to open
*/
void BasicGenerator::openFile(ofstream &file, const string &filePath, const string &fileName)
{
	string fileFullName=filePath+fileName;
	file.open(fileFullName.c_str(), ios::out);
}
/**
	Method to close the file associated with the given stream
	@param file: file stream to close
*/
void BasicGenerator::closeFile(ofstream &file)
{
	file.close();
}
/**
	Method to write in the given file the header of the xml document
	@param file: file stream
*/
void BasicGenerator::addXMLHeader(ofstream &file)
{

}
/**
	Method to write in the given file the openning xml tag with the
	name given.
	@param file: file stream
	@param tagName: name of the xml tag to write
*/
void BasicGenerator::addOpenningTag( ofstream &file, const string &tagName)
{
	 file << "<" << tagName <<">" << endl;
}

/**
	Method to write in the given file the closing xml tag with the
	name given.
	@param file: file stream
	@param tagName: name of the xml tag to write
*/
void BasicGenerator::addClosingTag(ofstream &file, const string &tagName)
{
	file << "</" << tagName <<">" << endl;
}
/**
	Method to write in the given file the given data
	@param file: file stream
	@param tagName: name of the xml tag to write
*/
void BasicGenerator::addData( ofstream &file, const string &data)
{
	string escapedData=data;
	escapeXMLCharacters(escapedData);
	file <<  escapedData << endl;
}
/**
	Escapes in the given string some characters that in XML are given an special meaning.
	The characters replaced are the following ones:
		' is replaced with &apos
		" is replaced with &quot
		& is replaced with &amp
		< is replaced with &lt
		> is replaced with &gt
	@param data: string to be escaped
*/
void BasicGenerator::escapeXMLCharacters(string &data)
{
	replaceString(data, "'", "&apos;");
	replaceString(data, "\"", "&quot;" );
	replaceString(data, "&", "&amp;");
	replaceString(data, "<", "&lt;" );
	replaceString(data, ">" , "&gt;");
	replaceString(data, "\r\n" , "\n");
	replaceString(data, "\r" , "\n");
	replaceString(data, "\b" , "\n");
}

/**
	Replaces in the given string, al the ocurrences of a subtring by another substring.
	@param subject: string to analyze and modifie
	@param search: substring to search and replace
	@param replace: new substring to insert
*/
void BasicGenerator::replaceString(string& subject, const string& search, const string& replace) 
{
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
	pos += replace.length();
    }
}
