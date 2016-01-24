#if !defined (_XMLGENERATOR_H_)
#define _XMLGENERATOR_H_
#include "../DataModel/CArticleParsed.h"
#include "BasicGenerator.h"
#include "ArticlesXMLGenerator.h"
#include "ExtendedArticlesXMLGenerator.h"
#include "CitationsXMLGenerator.h"
#include "../Pugixml/pugixml.hpp"
#include <vector>
/**
	Class XMLGenerator
	This class implements methods to generate all the XML with the metadata of the dataset.
*/
class XMLGenerator:public ArticlesXMLGenerator, public ExtendedArticlesXMLGenerator, public CitationsXMLGenerator
{
public:
	//Constructor
	XMLGenerator(const string &DB,const string &DBserver, const string &DBuser, const string &DBpwd);

	//Method to create the XMLes	
	void createArticlesXML(const string &fileName=ArticlesXMLGenerator::defaultXMLName, const string &filePath=ArticlesXMLGenerator::defaultXMLPath, int nMax=-1);

	void createExtendedArticlesXML(const string &fileName=ExtendedArticlesXMLGenerator::defaultXMLName, const string &filePath=ExtendedArticlesXMLGenerator::defaultXMLPath, int nMAx=-1);

	void createCitationsXML(const string &fileName=CitationsXMLGenerator::defaultXMLName, const string &filePath=CitationsXMLGenerator::defaultXMLPath, int nMax=-1);
	
};
#endif
