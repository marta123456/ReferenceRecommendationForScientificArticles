//Declaration of the class DBPubmedManagement
#if !defined (_DB_MANAGER_H_)
#define _DB_MANAGER_H_
#include <mysql/mysql.h>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "../DataModel/CArticleParsed.h"
#include "DBLowLevelManager.h"
using namespace std;

/**
	DBManager class.
	This class implements methods to work with the ArticleDataset database with a high level of abstraction. It provides methods
	to insert in the DB the metadata of a CArticleParsed and to select specific metadata from the database (in order to create the XML
	index or in order to make a recommendation in the IR systems)
*/
class DBManager : public DBLowLevelManager
{

protected:
	//Methods to push information from an article object to the database
	int addArticleMetadata(CArticle &article);
	void addCrawledArticleMetadata(int articleId, CArticleParsed &article);
	void addReferences(int articleId, vector<CReference> &references);
	void addCitation(int articleId, CCitation &citation);
	
	
public:
	//Constructor
	DBManager(const string &database, const string &server, const string &user, const string &password);
	//Method to add an article to the database
	bool addParsedArticle(CArticleParsed &article);
	
	//Methos to retrieve information from the db
	void getCrawledArticlesIds(vector<int> &articleIdList, int maxArt=-1);
	void getArticlesIds(vector<int> &articleIdList, int maxArt=-1);

	void getArticleMetadata(int articleId, string & title, CDate &pubDate, vector<CName> &authors);
	void getArticleExtendedMetadata(int crawledArticleId, string &abstract, string &articleBody, vector<string> &keywords, vector<int> &references);
	void getCitationsIds(vector<int> &citationIdList, int maxArt=-1);
	void getCitation(int citationId, int &sourceArticleId, int &referencedArticleId, string &context, CSection &sections);
	void getCitation(int citationId, int &sourceArticleId, int &referencedArticleId, string &context);

	void getArticleTitle(int &articleId, string &articleTitle);
	//Used by the IR systems
	void getTitles(int minId, int maxId, vector<string> &titles);
	void getCitations(int minId, int maxId, vector<int> &citIds, vector<int> &refArticlesIds, vector<string> &contexts);

	
};
#endif



