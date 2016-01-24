#pragma warning( disable : 4290 )
#include "DatasetGenerator.h"
#include "./InformationRetrievalSystem/IRTester.h"
#include "./DBmanager/DBBasic.h"
#include <string>
#include <iostream>
#include <stdio.h>


#include <regex>

#define ARTICLES_PATH "/media/SAMSUNG/MASTER_THESIS/Pubmed_articles"
//#define ARTICLES_PATH ""
#define XML_DIR "/media/SAMSUNG/MASTER_THESIS/XMLGenerated/"

using namespace std;

int main()
{
	try{	
		/*//DATASET GENERATOR
		DatasetGenerator generator("ArticleDataset", "localhost", "root", "1234");
		//Process directory
		string articlesPath=ARTICLES_PATH;
		string dir="";
		generator.parseDirectory(articlesPath, dir, true);
		//Test XML generator
		string xmlDir=XML_DIR;
		generator.testXMLGenerator(xmlDir, 126);*/
			
	
		//INFORMATION RETRIEVAL SYSTEMS
		IRTitleBased ir1("ArticleDataset", "localhost", "root", "1234");
		ir1.userInterface();
		IRCitationBased ir2("ArticleDataset", "localhost", "root", "1234");
		ir2.userInterface();
		
		//IRTester tester("ArticleDataset", "localhost", "root", "1234", 30);
		//tester.testIRSystems();
	
	}catch(exception e){
		cout << "\nCatch:"<< e.what();
	}catch(string &e){
		cout << "\nCatch:"<< e;
	}catch(char const *e){
		cout << "\nCatch:"<< e;
	}
}






