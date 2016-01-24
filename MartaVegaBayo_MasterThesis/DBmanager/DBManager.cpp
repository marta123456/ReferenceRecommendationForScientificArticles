#include "DBManager.h"

/**
	Constructor of the class BDPubmedManagement.
	@param database: name of the database
	@param server: server name or ip address
	@param user: username of the user of the database
	@param password: password of the user
*/
DBManager::DBManager(const string &database, const string &server, const string &user, const string &password):
	DBLowLevelManager(database, server, user, password)
{
}


//#######################################################################################################################
//METHOS TO ADD THE METADATA OF A CARTICLEPARSED TO THE DATABASE

/**
	Adds to the database the information set in the object CArticleParsed given as an argument.
	@param article: parsed article with the information to save in the database
	@return true if the information is saved in te db without errors, false if some
	error ocurred and therefore, the information was not saved in the database.
*/
bool DBManager::addParsedArticle(CArticleParsed &article)
{
	int articleId;

	//Transaction to add the item
	try
	{
		//Init transaction
		exeInsertQuery("START TRANSACTION");
		//Check if the article already exists in the db
		string title= article.getTitle();
		articleId=getIdOfArticle(title);
		if(articleId==0){
			articleId = addArticleMetadata(article);
			addCrawledArticleMetadata(articleId, article);
		}else{
			if(!isCrawled(articleId)){
				addCrawledArticleMetadata(articleId, article);
			}
			
		}
		//Commit transaction
		exeInsertQuery("COMMIT");
		return true;

	}catch(string &e){
		throw e;
		return false;

	}
	return false;
}

/**
	Adds to the database the metadata of the given article (which can be a parsed
	article, CArticleParsed, or a referenced article, CArticle).
	The method saves in the database the title of the article, its publication date and
	its authors. In order to do that, the tables 'article' and 'author'
	of the database are affected.

	@param article: object with the metadata of the article to add to the db
	@return id of the added article in the database
*/
int DBManager::addArticleMetadata(CArticle &article)
{
	//Insert article row
	string title= article.getTitle();
	CDate date(article.getPubDate());
	insertArticle(title,date.getYear(), date.getMonth(), date.getDay() );
	int articleId=mysql_insert_id(aConn);

	//Insert authors
	vector<CName> authors=article.getAuthors();
	vector<string> firstnames;
	vector<string> lastnames;
	vector<int> ids;
	for(uint i=0; i<authors.size(); i++)
	{
		ids.push_back(articleId);
		firstnames.push_back(authors[i].getFirstName());
		lastnames.push_back(authors[i].getLastName());
	}
	insertAuthors(ids, firstnames, lastnames);

	return articleId;
}


/**
	Adds to the database the metadata of a parsed article: abstract, 
	keywords, references and citations.
	@param articleId: id in the database of the article whose extended metadata is beeing added
	@param article: article whose metadata is to be added to the database
	
*/
void DBManager::addCrawledArticleMetadata(int articleId, CArticleParsed &article)
{
	//Add row to crawled_article
	string abstract=article.getAbstract();
	string articleBody=article.getArticleBody();
	insertCrawledArticle(articleId, abstract, articleBody);
	//Add keywords
	vector<string> keywords=article.getKeywords();
	vector<int> IDs(keywords.size(), articleId);
	insertKeywords(IDs, keywords);

	//Add references
	vector<CReference> &references=article.getReferences();
	addReferences(articleId, references);


	//Add citations
	vector<CCitation> &citations=article.getCitations();
	for(uint i=0; i<citations.size(); i++)
	{
		addCitation(articleId, citations[i]);
	}
}

/**
	Adds to the database the metadata of all the references of the article. In order to do that, the referenced
	articles are added to the database if they do not already exist and, afterwards, the 'references' table
	is modified.
	@param articleId: id of the current article
	@param references: list of reference object with the metadata needed
*/
void DBManager::addReferences(int articleId, vector<CReference> &references)
{
	vector<int> refArticleIdList;

	//ADD REFERENCES TO THE DATABASE IF THEY DO NOT EXIST
	for(uint i=0; i<references.size(); i++)
	{
		//Get article info
		CArticle refArticle=references[i].getReferencedArticle();
		string title=refArticle.getTitle();
		//Get articleId (in db) and add article to db if it does not exist
		int refArticleId=getIdOfArticle(title);
		if(refArticleId==0){
			refArticleId=addArticleMetadata(refArticle);
		}else{
		}
		//Add id to the list if it is not already in it
		if(find(refArticleIdList.begin(),refArticleIdList.end(), refArticleId ) == (refArticleIdList.end()))
		{
			refArticleIdList.push_back(refArticleId);
		}
	}
	
	//Add references to the database
	vector<int> sourceArticleIdList(refArticleIdList.size(), articleId);
	insertReferences(sourceArticleIdList, refArticleIdList);
	
}

/**
	Adds to the database the metadata of one citation within the article. In order to do that, the tables
	'citation' and 'section' are affected.
	@param articleId: id of the current article
	@param citation: citation of the article
*/
void DBManager::addCitation(int articleId, CCitation &citation)
{
	//Get id of the cited article in the DB
	string refArticleTitle=citation.getRefTitle();
	int refArticleId=getIdOfArticle(refArticleTitle);

	//Insert citation to the db
	string context= citation.getContext();
	insertCitation(articleId, refArticleId, context);
	int citationId=mysql_insert_id(aConn);

	//Insert sections
	CSection section=citation.getSection();
	int parentSectionId=0;
	bool lastSection=false;
	while(lastSection==false)
	{
		insertSection(citationId, section.getSectionType(), section.getTitle(), parentSectionId);
		if(section.getPSubSection()==NULL){
			lastSection=true;
		}else{
			section=*section.getPSubSection();
			parentSectionId=mysql_insert_id(aConn);
		}
	}
}

//#######################################################################################################################
//METHODS TO RETRIEVE METADATA FROM THE DATABASE

/**
	Gets from the table 'article' the ids of the articles and sets with those ids
	the vector articleIdList given as a parameter.
	@param articleIdList: vector given as a reference where the ids of the articles fetched will be set.
*/
void DBManager::getArticlesIds(vector<int> &articleIdList, int maxArt)
{
	vector<vector <string> > queryResult;
	if(maxArt==-1){
		string selectQuery="SELECT id FROM article";
		queryResult=exeSelectQuery(selectQuery);
	}else{
		string selectQuery="SELECT article_id FROM crawled_article LIMIT "+to_string(maxArt+1);		
		vector<vector <string> > auxResult;
		auxResult=exeSelectQuery(selectQuery);
		int nextId=stoi(auxResult[auxResult.size()-1][0]);
		selectQuery="SELECT id FROM article WHERE id <"+to_string(nextId);
		queryResult=exeSelectQuery(selectQuery);
	}

	uint nResults=queryResult.size();
	for(uint i=0; i<nResults; i++){
		articleIdList.push_back(stoi(queryResult[i][0]));
	}
}

/**
	Gets from the table 'crawled_article' the ids of the articles and sets with those ids the vector articleIdList given as a parameter.
	@param articleIdList: vector given as a reference where the ids of the articles fetched will be set.
*/
void DBManager::getCrawledArticlesIds(vector<int> &articleIdList, int maxArt)
{
	string selectQuery="SELECT article_id FROM crawled_article";
	if(maxArt!=-1)selectQuery+=" LIMIT "+to_string(maxArt);
	vector<vector <string> > queryResult;
	queryResult=exeSelectQuery(selectQuery);
	uint nResults=queryResult.size();
	for(uint i=0; i<nResults; i++){
		articleIdList.push_back(stoi(queryResult[i][0]));
	}
}

/**
	Gets from the table 'citation' the ids of the citations and sets with those ids the vector citationIdList given as a parameter.
	@param citationIdList: vector given as a reference where the ids of the citations will be set.
*/
void DBManager::getCitationsIds(vector<int> &citationIdList, int maxArt)
{
	
	string selectQuery="SELECT id FROM citation";
	vector<vector <string> > queryResult;
	if(maxArt==-1){
		string selectQuery="SELECT id FROM citation";
		queryResult=exeSelectQuery(selectQuery);
	}else{
		string selectQuery="SELECT article_id FROM crawled_article LIMIT "+to_string(maxArt+1);		
		vector<vector <string> > auxResult;
		auxResult=exeSelectQuery(selectQuery);
		int nextId=stoi(auxResult[auxResult.size()-1][0]);
		selectQuery="SELECT id FROM citation WHERE source_article_id <"+to_string(nextId);
		queryResult=exeSelectQuery(selectQuery);
	}
	uint nResults=queryResult.size();
	for(uint i=0; i<nResults; i++){
		citationIdList.push_back(stoi(queryResult[i][0]));
	}
}

/**
	Fetches from the 'article' and 'author' tables of the database the metadata of the article
	with the given id, which can be an article from pubmed or a non-pubmed source, and sets with the
	fetched metadata the variables given as parameters.
	@param articleId: id of the article to fetch
	@param title: variable where the title of the article will be set
	@param pubDate: variable where the publication date of article will be set
	@param authors: vector variable where the authors of the article will be set
*/
void DBManager::getArticleMetadata(int articleId, string &title, CDate &pubDate, vector<CName> &authors)
{
	//Select metadata from the 'article' table
	int year, month, day;
	selectArticle(articleId, title, year, month, day);
	pubDate.setDate(year,month, day);
	
	//Select metadata from the 'author' table
	selectAuthors(articleId, authors);
}

/**
	Fetches from the 'crawled_article', 'keywords' and 'reference' tables of the database the metadata of the
	article with the given id, which should be an article from pubmed, and sets with the fetched metadata the 		
	variables given as parameters.
	@param crawledArticleId: id of the article to fetch
	@param abstract: variable where the abstract of the article will be set
	@param articleBody: variable where the body of the article will be set
	@param keywords: vector variable where the keywords of the article will be set
	@param references: vector variable where the id of the referenced articles of the article will be set
*/
void DBManager::getArticleExtendedMetadata(int crawledArticleId, string &abstract, string &articleBody, vector<string> &keywords, vector<int> &references){
	//Select metadata from the 'crawled_article' table
	selectCrawledArticle(crawledArticleId, abstract, articleBody);
	
	//Select metadata from the 'keywords' table
	selectKeywords(crawledArticleId, keywords);

	//Select metadata from the 'reference' table
	selectReferences(crawledArticleId, references);
}


/**
	Fetches from the 'citation' and 'section' tables of the database all the metadata of the citation
	with the given id, and sets with the fetched metadata the variables given as parameters.
	@param citationId: id of the citation to fetch
	@param sourceArticleId: variable where the id of the source article will be set
	@param referencedArticleId: variable where the id of the referenced article will be set
	@param context: variable where the context of the citation will be set
	@param section: variable where the linked list of sections of the citation will be set
*/
void DBManager::getCitation(int citationId, int &sourceArticleId, int &referencedArticleId, string &context, CSection &section)
{
	getCitation(citationId, sourceArticleId, referencedArticleId, context);	
	//Select metadata from the 'section' table
	selectSections(citationId, section);
}
/**
	Fetches from the 'citation' table of the database some metadata of the citation
	with the given id, and sets with the fetched metadata the variables given as parameters.
	@param citationId: id of the citation to fetch
	@param sourceArticleId: variable where the id of the source article will be set
	@param referencedArticleId: variable where the id of the referenced article will be set
	@param context: variable where the context of the citation will be set
*/
void DBManager::getCitation(int citationId, int &sourceArticleId, int &referencedArticleId, string &context)
{
	//Select metadata from the 'citation' table
	string selectQuery="SELECT * FROM citation where id="+to_string(citationId);
	vector<vector <string> > queryResult;
	queryResult=exeSelectQuery(selectQuery);
	sourceArticleId=stoi(queryResult[0][1]);
	referencedArticleId=stoi(queryResult[0][2]);
	context=queryResult[0][3];
}

/**
	Gets from the database the title of the article with the given id, and sets with it the variable given as a parameter
	@param articleId: id of the article being searched
	@param articleTitle: variable where the title of the article will be set
*/
void DBManager:: getArticleTitle(int &articleId, string &articleTitle)
{
	//Select metadata from the 'article' table
	int year, month, day;
	selectArticle(articleId, articleTitle, year, month, day);
}
/**
	Gets from the database the titles of all the articles with the ids between the given ones
	@param minId: minimun id of the articles to fetch
	@param maxId: maximum id of the articles to fetch
	@param titles: variable where the titles of the articles will be set
*/
void  DBManager::getTitles(int minId, int maxId, vector<string> &titles)
{
	selectTitles(minId, maxId, titles);
}
/**
	Gets from the database the contexts and referenced_article_id of all the citations with the ids between the given ones
	@param minId: minimun id of the citations to fetch
	@param maxId: maximum id of the citations to fetch
	@param contexts: variable where the contexts of the articles will be set
	@param refArticlesIds: variable where the ids of the cited articles will be set
	@param citIds: variable where the ids of the citatios will be set
*/
void  DBManager::getCitations(int minId, int maxId, vector<int> &citIds, vector<int> &refArticlesIds, vector<string> &contexts)
{
	selectCitations(minId, maxId, citIds, refArticlesIds, contexts);
}











