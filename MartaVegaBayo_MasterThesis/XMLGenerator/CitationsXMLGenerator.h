#if !defined (_CITATIONSXMLGENERATOR_H_)
#define _CITATIONSXMLGENERATOR_H_
#include "../DataModel/CArticleParsed.h"
#include "../DBmanager/DBManager.h"
#include "../Pugixml/pugixml.hpp"
#include "BasicGenerator.h"
#include <vector>
#include <stdio.h>
/**
	Class CitationsXMLGenerator
	This class implements methods to generate the XML file 'Citation.xml' with metadata of all the citations of the dataset.
*/
class CitationsXMLGenerator: public virtual BasicGenerator
{
private:

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
	void addCitation(int sourceId, int referencedId, const string &context, const CSection &sections);

	void addSourceArticleId(int sourceArticleId);
	void addReferencedArticleId(int referencedArticleId);
	void addContext(const string &context);

	void addSection(const CSection &section);
	void addSectionType(const string &sectionType);
	void addSectionTitle(const string &sectionTitle);

public:
	//Constructor
	CitationsXMLGenerator(const string &DB,const string &DBserver, const string &DBuser, const string &DBpwd);
	//Method to create the XML	
	void createXML(const string &fileName=defaultXMLName, const string &filePath=defaultXMLPath, int nMax=-1);
};
#endif
