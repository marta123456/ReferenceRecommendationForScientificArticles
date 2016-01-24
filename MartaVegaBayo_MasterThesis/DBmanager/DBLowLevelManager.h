//Declaration of the class DBmanagement
#if !defined (_DB_LOW_LEVEL_MANAGER_H_)
#define _DB_LOW_LEVEL_MANAGER_H_
#include <mysql/mysql.h>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "DBBasic.h"
#include "../DataModel/CSection.h"
#include "../DataModel/CName.h"
using namespace std;

/**
	Class DBLowLeverManager
	This class implements methods to work with the database of the system, such as creating the specific tables of the system,
	inserting data in those tables and selecting data.
*/
class DBLowLevelManager : public DBBasic
{

protected:
	//Methods to create tables
	void createTableArticle();
	void createTableAuthor();
	void createTableKeyword();
	void createTableCrawledArticle();
	void createTableReference();
	void createTableCitation();
	void createTableSection();
	//Mehtods to check the existence and make validations
	bool isCrawled(int articleId);
	//Methods to get id of specific row
	int getIdOfArticle(const string &title);

	//Methods to insert rows
	void insertArticle(const string &title, int year, int month, int day);
	void insertAuthor(int articleId, const string &firstname, const string &lastname);
	void insertAuthors(const vector<int> &articleIds, const vector<string> &firstnames, const vector<string> &lastnames);
	void insertCrawledArticle(int articleID, const string &abstract, const string &articleBody);
	void insertKeyword(int crawledArticleID, const string &keyword);
	void insertKeywords(const vector<int> &crawledArticleIDs, const vector<string> &keywords);
	void insertReference(int sourceArticleId, int referencedArticleID);
	void insertReferences(const vector<int> &sourceArticleIds, const vector<int> &referencedArticleIDs);
	void insertCitation(int sourceArticleId, int referencedArticleID, const string &context);
	void insertSection(int citationId, const string &type, const string &title, int parentSectionId=0);
	//Methods to select rows
	void selectArticle(int articleId, string &title, int &year, int &month, int &day);
	void selectCrawledArticle(int crawledArticleId, string &abstract, string &articleBody);
	void selectKeywords(int crawledArticleId, vector<string>  &keywords);
	void selectReferences(int crawledArticleId, vector<int>  &referencedArticlesIds);
	void selectCitations(int crawledArticleId,vector<int> &citationIdList, vector<int>  &citedArticleIdList, vector<string> &contextList);
	void selectSections(int citationId, CSection &section);
	//Methods to select many rows
	void selectCitations(int minId, int maxId, vector<int> &citIds, vector<int> &refArticlesIds, vector<string> &contexts);
	void selectTitles(int minId, int maxId, vector<string> &titles);
	
public:
	void selectAuthors(int articleId, vector<CName> &authors);
	DBLowLevelManager(const string &database, const string &server, const string &user, const string &password);
	//Mehtods to create and delete the database
	void createDB();
	void deleteDB();
	
};
#endif



