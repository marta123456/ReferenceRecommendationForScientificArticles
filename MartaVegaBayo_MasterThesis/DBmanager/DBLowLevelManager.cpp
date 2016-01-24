#include "DBLowLevelManager.h"

/**
	Constructor of the class DBLowLevelManager class. 
	@param database: name of the database
	@param server: server name or ip address
	@param user: username of the user of the database
	@param password: password of the user
*/
DBLowLevelManager::DBLowLevelManager(const string &database, const string &server, const string &user, const string &password):
	DBBasic(database, server, user, password)
{
}
/**
	Creates the different tables of the pubmed database.
	Firstly, it deletes the existing tables and then it creates the new and empty ones.
	If an error happens while creating the tables, the tables are droped and an excepion
	is thrown. 
*/
void DBLowLevelManager::createDB()
{
	//Delete existing database
	deleteDB();
	//Create tables
	try{
		createTableArticle();
		createTableAuthor();
		createTableCrawledArticle();
		createTableKeyword();
		createTableReference();
		createTableCitation();
		createTableSection();
	
	//Delete database if an error has happended
	}catch(...){
		deleteDB();
		throw;
	}
		
}
/**
	Deletes the tables of the database pubmed and its content.
*/
void DBLowLevelManager::deleteDB()
{
	vector<string> tablesList={ "reference",  "section","citation", "keyword", "crawled_article","author", "article"};
	for(uint i=0; i<tablesList.size(); i++)
	{
		string sqlQuery = "DROP TABLE IF EXISTS "+tablesList[i];
		exeDropQuery(sqlQuery);
	}
		
}

//#######################################################################################################################
/**
	Creates the table 'article' of the database.
*/
void DBLowLevelManager::createTableArticle()
{
	string sqlQuery="CREATE TABLE article (";
	sqlQuery+="id INT NOT NULL PRIMARY KEY AUTO_INCREMENT";
	sqlQuery+=", title VARCHAR(1024)";
	sqlQuery+=", pub_date DATE";
	sqlQuery+=")";
	exeCreateQuery(sqlQuery);
}
/**
	Creates the table 'author' of the database.
*/
void DBLowLevelManager::createTableAuthor()
{
	string sqlQuery="CREATE TABLE author (";
	sqlQuery+="id INT NOT NULL PRIMARY KEY AUTO_INCREMENT";
	sqlQuery+=", article_id INT";
	sqlQuery+=", firstname VARCHAR(255)";
	sqlQuery+=", lastname VARCHAR(255)";
	sqlQuery+=", FOREIGN KEY (article_id) REFERENCES article(id)";
	sqlQuery+=")";
	exeCreateQuery(sqlQuery);
}

/**
	Creates the table 'crawled_article' of the database.
*/
void DBLowLevelManager::createTableCrawledArticle()
{
	string sqlQuery="CREATE TABLE crawled_article (";
	sqlQuery+="article_id INT NOT NULL PRIMARY KEY";
	sqlQuery+=", abstract VARCHAR(4095)";
	sqlQuery+=", article_body VARCHAR(100000000)";
	sqlQuery+=", FOREIGN KEY (article_id) REFERENCES article(id)";
	sqlQuery+=")";
	exeCreateQuery(sqlQuery);
}
/**
	Creates the table 'keyword' of the database.
*/
void DBLowLevelManager::createTableKeyword()
{
	string sqlQuery="CREATE TABLE keyword (";
	sqlQuery+="crawled_article_id INT NOT NULL";
	sqlQuery+=", keyword VARCHAR(50)";
	sqlQuery+=", FOREIGN KEY (crawled_article_id) REFERENCES crawled_article(article_id)";
	sqlQuery+=")";
	exeCreateQuery(sqlQuery);
}
/**
	Creates the table 'reference' of the database.
*/
void DBLowLevelManager::createTableReference()
{
	string sqlQuery="CREATE TABLE reference (";
	sqlQuery+=" source_article_id INT NOT NULL";
	sqlQuery+=", referenced_article_id INT NOT NULL";
	sqlQuery+=", FOREIGN KEY (source_article_id) REFERENCES crawled_article(article_id)";
	sqlQuery+=", FOREIGN KEY (referenced_article_id) REFERENCES article(id)";
	sqlQuery+=", PRIMARY KEY (source_article_id, referenced_article_id)";
	sqlQuery+=")";
	exeCreateQuery(sqlQuery);
}
/**
	Creates the table 'citation' of the database.
*/
void DBLowLevelManager::createTableCitation()
{
	string sqlQuery="CREATE TABLE citation (";
	sqlQuery+="id INT NOT NULL PRIMARY KEY AUTO_INCREMENT";
	sqlQuery+=", source_article_id INT NOT NULL";
	sqlQuery+=", referenced_article_id INT NOT NULL";
	sqlQuery+=", context VARCHAR(4095)";
	sqlQuery+=", FOREIGN KEY (source_article_id) REFERENCES crawled_article(article_id)";
	sqlQuery+=", FOREIGN KEY (referenced_article_id) REFERENCES article(id)";
	sqlQuery+=")";
	exeCreateQuery(sqlQuery);
}
/**
	Creates the table 'section' of the database.
*/
void DBLowLevelManager::createTableSection()
{
	string sqlQuery="CREATE TABLE section (";
	sqlQuery+=" id INT NOT NULL PRIMARY KEY AUTO_INCREMENT";
	sqlQuery+=", citation_id INT NOT NULL";
	sqlQuery+=", type VARCHAR(255)";
	sqlQuery+=", title VARCHAR(511)";
	sqlQuery+=", parent_section_id INT";
	sqlQuery+=", FOREIGN KEY (citation_id) REFERENCES citation(id)";
	sqlQuery+=", FOREIGN KEY (parent_section_id) REFERENCES section(id)";
	sqlQuery+=")";
	exeCreateQuery(sqlQuery);
}

//#######################################################################################################################

/**
	Inserts a new row in the 'article' table.
	@param title: title of the article
	@param year: year of publication
	@param month: month of publication
	@param day: day of publication
*/
void DBLowLevelManager::insertArticle(const string &title, int year, int month, int day)
{
	//Process date
	string sYear=to_string(year);
	string sMonth=to_string(month);
	string sDay=to_string(day);
	string sDate="'"+sYear+"-"+sMonth+"-"+sDay+"'";
	//Process title
	string eTitle=title;
	escapeQuotes(eTitle);
	//Generate query
	string sqlQuery= "INSERT INTO article (title, pub_date)";
	sqlQuery+=" VALUES('"+eTitle+"', "+sDate+")";
	//Execute query
	exeInsertQuery(sqlQuery);
}
/**
	Inserts a new row in the 'author' table.
	@param firstname: firstname (or abreviation) of the author
	@param lastname: lastname of the author
*/
void DBLowLevelManager::insertAuthor(int articleId, const string &firstname, const string &lastname)
{
	//Process data
	string eFirstname=firstname;
	escapeQuotes(eFirstname);
	string eLastname=lastname;
	escapeQuotes(eLastname);
	//Generate query
	string sqlQuery= "INSERT INTO author (article_id, firstname, lastname)";
	sqlQuery+=" VALUES("+to_string(articleId)+", '"+eFirstname+"', '"+eLastname+"')";
	//Execute query
	exeInsertQuery(sqlQuery);
}
/**
	Inserts new rows in the 'author' table
	@param articleIds: ids of the articles
	@param firstnames: firstnames (or abreviation) of the authors
	@param lastnames: lastnames of the authors
*/
void DBLowLevelManager::insertAuthors(const vector<int> &articleIds, const vector<string> &firstnames, const vector<string> &lastnames)
{
	if(articleIds.size()==0)return;
	if(articleIds.size()!=firstnames.size() ||articleIds.size()!=lastnames.size() )return;

	//Generate query
	string sqlQuery= "INSERT INTO author (article_id, firstname, lastname) VALUES";
	for(uint i=0; i<articleIds.size(); i++)
	{
		if(i!=0)sqlQuery+=", ";
		//Process data
		string eFirstname=firstnames[i];
		escapeQuotes(eFirstname);
		string eLastname=lastnames[i];
		escapeQuotes(eLastname);
		sqlQuery+=" ("+to_string(articleIds[i])+", '"+eFirstname+"', '"+eLastname+"')";
	}
	//Execute query
	exeInsertQuery(sqlQuery);
}

/**
	Inserts a new row in the 'crawled_article' table.
	@param articleId: id of the article
	@param sAbstract: abstract of the article
	@param articleBody: body of the article
*/
void DBLowLevelManager::insertCrawledArticle(int articleId, const string &sAbstract, const string &articleBody)
{
	//Process data
	string eAbstract=sAbstract, eArticleBody=articleBody;
	escapeQuotes(eAbstract);
	escapeQuotes(eArticleBody);
	//Generate query
	string sqlQuery= "INSERT INTO crawled_article (article_id, abstract, article_body)";
	sqlQuery+=" VALUES("+to_string(articleId)+", '"+eAbstract+"', '"+eArticleBody+"')";
	//Execute query
	exeInsertQuery(sqlQuery);
}
/**
	Inserts a new row in the 'keyword' table.
	@param crawledArticleId: id of the article which contains the keyword
	@param keyword: keyword
*/
void DBLowLevelManager::insertKeyword(int crawledArticleId, const string &keyword)
{
	//Process data
	string eKeyword=keyword;
	escapeQuotes(eKeyword);
	//Generate query
	string sqlQuery= "INSERT INTO keyword (crawled_article_id, keyword)";
	sqlQuery+=" VALUES("+to_string(crawledArticleId)+", '"+eKeyword+"')";
	//Execute query
	exeInsertQuery(sqlQuery);
}
/**
	Inserts in the 'keyword' table many rows with the data given
	@param crawledArticleId: ids of the articles which contains the keyword
	@param keyword: keywords to add
*/
void DBLowLevelManager::insertKeywords(const vector<int> &crawledArticleIDs, const vector<string> &keywords)
{
	if(keywords.size()==0)return;
	if(keywords.size()!=crawledArticleIDs.size())return;

	//Generate query
	string sqlQuery= "INSERT INTO keyword (crawled_article_id, keyword) VALUES ";
	for(uint i=0; i<keywords.size(); i++)
	{
		if(i!=0)sqlQuery+=", ";
		//process data
		string eKeyword=keywords[i];
		escapeQuotes(eKeyword);
		sqlQuery+=" ("+to_string(crawledArticleIDs[i])+", '"+eKeyword+"')";	
	}
	//Execute query
	exeInsertQuery(sqlQuery);
}
/**
	Inserts a new row in the table 'reference'.
	@param sourceArticleId: id of the article which contains the reference
	@param referencedArticleId: id of the referenced article
*/
void DBLowLevelManager::insertReference(int sourceArticleId, int referencedArticleID)
{
	//Generate query
	string sqlQuery= "INSERT INTO reference (source_article_id, referenced_article_id)";
	sqlQuery+=" VALUES("+to_string(sourceArticleId)+", "+to_string(referencedArticleID)+")";
	//Execute query
	exeInsertQuery(sqlQuery);
}

/**
	Inserts new rows in the table 'reference'.
	@param sourceArticleIds: ids of the articles which contains the reference
	@param referencedArticleIds: ids of the referenced articles
*/
void DBLowLevelManager::insertReferences(const vector<int> &sourceArticleIDs, const vector<int> &referencedArticleIDs)
{
	if(sourceArticleIDs.size()==0)return;
	if(sourceArticleIDs.size()!=referencedArticleIDs.size())return;

	//Generate query
	string sqlQuery= "INSERT INTO reference (source_article_id, referenced_article_id) VALUES";
	for(uint i=0; i<sourceArticleIDs.size(); i++)
	{
		if(i!=0)sqlQuery+=", ";
		sqlQuery+=" ("+to_string(sourceArticleIDs[i])+", "+to_string(referencedArticleIDs[i])+")";
	}
	//Execute query
	exeInsertQuery(sqlQuery);
}
/**
	Inserts a new row in the 'citation' table.
	@param sourceArticleId: id of the article which contains the citation
	@param referencedArticleId: id of the cited article
	@param context: text of the paragraph where the citation is within the article
*/
void DBLowLevelManager::insertCitation(int sourceArticleId, int referencedArticleID, const string &context)
{
	//Process data
	string eContext=context;
	escapeQuotes(eContext);
	//Generate query
	string sqlQuery= "INSERT INTO citation (source_article_id, referenced_article_id, context)";
	sqlQuery+=" VALUES("+to_string(sourceArticleId)+", "+to_string(referencedArticleID)+", '"+eContext+"')";
	//Execute query
	exeInsertQuery(sqlQuery);

}
/**
	Insters a new row in the 'section' table.
	@param citationId: id of the citation (a citation can be in nested section, and therefore, have 
	many sections asociated)
	@param type: type of section, such as 'introduction', 'method', 'conclusions'
	@param title: title of the section
	@param parentSectionId: id of the parent section or empty (which will get the default value -1) if the section is the first section
*/
void DBLowLevelManager::insertSection(int citationId, const string &type, const string &title, int parentSectionId)
{
	//Process data
	string eType=type;
	escapeQuotes(eType);
	string eTitle=title;
	escapeQuotes(eTitle);
	string sParentSectionId;
	if(parentSectionId==0){
		sParentSectionId="NULL";
	}else{
		sParentSectionId=to_string(parentSectionId);
	}
	//Generate query
	string sqlQuery= "INSERT INTO section (citation_id, type, title, parent_section_id)";
	sqlQuery+=" VALUES("+to_string(citationId)+", '"+eType+"', '"+eTitle+"', "+sParentSectionId+")";
	//Execute query
	exeInsertQuery(sqlQuery);

}


//############################################################################################################## 
/**
	Checks if the article with the given id has been crawled and therefore the database already
	contains its information in the tables 'crawled_article', 'keyword', 'reference'....
	@param articleId: id of the article
	@return true if the article has been crawled or false otherwise
*/
bool DBLowLevelManager::isCrawled(int articleId)
{
	string sqlQuery="SELECT EXISTS(SELECT * FROM crawled_article WHERE ";
	sqlQuery+="article_id = "+to_string(articleId)+")";
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);
	if(sqlResult[0][0]=="1"){return true;}
	return false;
}
/**
	Returns the Id of the article with the given title.
	@param title: title of the article
	@return id of the article in the database or 0 is the article is not in the database
*/
int DBLowLevelManager::getIdOfArticle(const string &title)
{
	//Process data
	string eTitle=title;
	escapeQuotes(eTitle);
	//Generate and execute query
	string sqlQuery="SELECT id FROM article WHERE ";
	sqlQuery+="title = '"+eTitle+"'";
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);

	if(sqlResult.size()==0)return 0;
	string sId=sqlResult[0][0];
	return stoi(sId);
}



//#######################################################################################################################

/**
	Fetchs from the 'article' table of the 'pubmed' database the metadata of the article with given id, and sets with
	the obtained metadata the variables given as references.
	@param articleId: id of the article whose metadata is go be fetched
	@param title: reference to the variable where the title fetched will be set
	@param year: reference to the variable where the year of the publication date fetched will be set
	@param month: reference to the variable where the month of the publication date fetched will be set
	@param day: reference to the variable where the day of the publication date fetched will be set
*/
void DBLowLevelManager::selectArticle(int articleId, string &title, int &year, int &month, int &day)
{
	//Select data from the database
	string sqlQuery="SELECT title, pub_date FROM article";
	sqlQuery+=" WHERE id = "+to_string(articleId)+"";
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);
	//Get title
	title=sqlResult[0][0];
	replaceEscapingSlashes(title);
	//Get date
	string sDate=sqlResult[0][1];
	string sYear=sDate.substr(0, 4);
	year=stoi(sYear);
	string sMonth=sDate.substr(5, 7);
	month=stoi(sMonth);
	string sDay=sDate.substr(8, 10);
	day=stoi(sDay);

}

/**
	Fetchs from the 'author' table of the 'pubmed' database the names (CName objects with the first and lastname)
	of the authors of the article with the given id.
	@param articleId: article whose authors are going to be fetched
	@param authors: reference to a vector of CNames where the names of the authors fetched will be set
*/
void DBLowLevelManager::selectAuthors(int articleId, vector<CName> &authors)
{
	//Select data from the database
	string sqlQuery="SELECT firstname, lastname FROM author ";
	sqlQuery+=" WHERE article_id="+to_string(articleId)+"";
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);

	//Process the fetched data to obtain the CName objects of the authors and save them
	authors.empty();

	for(uint i=0; i<sqlResult.size();i++)
	{
		string firstname=sqlResult[i][0];
		replaceEscapingSlashes(firstname);
		string lastname=sqlResult[i][1];
		replaceEscapingSlashes(lastname);
		authors.push_back(CName(firstname, lastname));
	}
}

/**

	Fetchs form the 'crawled_article' table of 'pubmed' database the abstract of the article
	with the given id.
	@param crawledArticleId: id of the article whose id will be fetched
	@param sAbstract: reference to the variable where the fetched abstract will be set. If NULL is give, this columm is not fetched.
	@param articleBody: reference to the variable where the fethced article body will be set. If NULL is give, this columm is not fetched.
*/
void DBLowLevelManager::selectCrawledArticle(int crawledArticleId, string &sAbstract, string &articleBody)
{
	
	string sqlQuery;
	//Select data from the database
	sqlQuery="SELECT abstract, article_body FROM crawled_article";
	sqlQuery+=" WHERE article_id = "+to_string(crawledArticleId)+"";
	
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);

	//Get abstract from the fetched data
	sAbstract=sqlResult[0][0];
	articleBody=sqlResult[0][1];
	replaceEscapingSlashes(sAbstract);
	replaceEscapingSlashes(articleBody);

	
}

/**
	Fetchs form the 'keyword' table of 'pubmed' database the keywords of the article with
	the given id.
	@param crawledArticleId: id of the article whose keywords will be fetched
	@param keywords: reference to a vector variables where the keywords fetched will be set
*/
void DBLowLevelManager::selectKeywords(int crawledArticleId, vector<string>  &keywords)
{
	//Select data from the database
	string sqlQuery="SELECT keyword FROM keyword";
	sqlQuery+=" WHERE crawled_article_id = "+to_string(crawledArticleId)+"";
	
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);

	//Process the fetched data to create the vector of keywords.
	keywords.empty();
	for(uint i=0; i<sqlResult.size();i++)
	{
		string kwd=sqlResult[i][0];
		replaceEscapingSlashes(kwd);
		keywords.push_back(kwd);
	}
}

/**
	Fetchs from the 'reference' table of 'pubmed' the ids of the articles referenced by the article
	with the given id.
	@param crawledArticleId: id of the article whose references are going to be fetched
	@param referencedArticlesIds: reference to the vector variables where the ids of the referenced
	articles will be set.
*/
void DBLowLevelManager::selectReferences(int crawledArticleId, vector<int>  &referencedArticlesIds)
{
	//Select data from the database
	string sqlQuery="SELECT referenced_article_id FROM reference";
	sqlQuery+=" WHERE source_article_id = "+to_string(crawledArticleId)+"";
	
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);

	//Process the fetched data to create a vector of ids
	referencedArticlesIds.empty();
	for(uint i=0; i<sqlResult.size();i++)
	{
		referencedArticlesIds.push_back(stoi(sqlResult[i][0]));
	}
}

/**
	Fetchs from the 'citation' table of 'pubmed' database the some metadata of the citations within the
	article with the given id.
	@param crawledArticleId: id of the article whose citations are going to be fetched
	@param citationIdList: reference to the vector variable where the ids of the citations will be set
	@param citedArticleIdList: reference to the vector variable where the ids of the cited articles will be set
	@param contextList: reference to the vector variable where the contexts of the citations will be set
*/
void DBLowLevelManager::selectCitations(int crawledArticleId,vector<int> &citationIdList, vector<int>  &citedArticleIdList, vector<string> &contextList)
{
	//Select data from the database
	string sqlQuery="SELECT id, referenced_article_id, context FROM citation";
	sqlQuery+=" WHERE source_article_id = "+to_string(crawledArticleId)+"";
	
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);

	//Process the fetched data to create the vectors 
	citationIdList.empty();
	citedArticleIdList.empty();
	contextList.empty();
	for(uint i=0; i<sqlResult.size();i++)
	{
		citationIdList.push_back(stoi(sqlResult[i][0]));
		citedArticleIdList.push_back(stoi(sqlResult[i][1]));
		contextList.push_back(sqlResult[i][2]);
	}
}
/**
	Fetchs form the 'citation' table of the database the contexts and referenced_article_id of all the citations with the ids between the given ones
	@param minId: minimun id of the citations to fetch
	@param maxId: maximum id of the citations to fetch
	@param contexts: variable where the contexts of the articles will be set
	@param refArticlesIds: variable where the ids of the cited articles will be set
*/
void DBLowLevelManager::selectCitations(int minId, int maxId, vector<int> &citIds, vector<int> &refArticlesIds, vector<string> &contexts)
{
	//Select data from the database
	string sqlQuery="SELECT id, referenced_article_id, context FROM citation";
	sqlQuery+=" WHERE id>=  "+to_string(minId)+" AND id <="+to_string(maxId)+"";
	
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);

	//Process the fetched data to create the vectors 
	citIds.empty();
	refArticlesIds.empty();
	contexts.empty();
	for(uint i=0; i<sqlResult.size();i++)
	{
		citIds.push_back(stoi(sqlResult[i][0]));
		refArticlesIds.push_back(stoi(sqlResult[i][1]));
		contexts.push_back(sqlResult[i][2]);
	}
}
/**
	Fetchs form the 'article' table of the database the titles of all the articles with the ids between the given ones
	@param minId: minimun id of the articles to fetch
	@param maxId: maximum id of the articles to fetch
	@param titles: variable where the titles of the articles will be set
*/
void DBLowLevelManager::selectTitles(int minId, int maxId, vector<string> &titles)
{
	//Select data from the database
	string sqlQuery="SELECT title FROM article";
	sqlQuery+=" WHERE id>=  "+to_string(minId)+" AND id <="+to_string(maxId)+"";
	
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);

	//Process the fetched data to create the vectors 
	titles.empty();
	for(uint i=0; i<sqlResult.size();i++)
	{
		titles.push_back(sqlResult[i][0]);
	}
}


/**
	Fetchs from the 'section' table of 'pubmed' database the section (single section or nested sections) 
	where the citation with the id given is within the article.
	@param citationId: id of the citation whose sections are going to be fetched
	@param section: reference to a CSection variable where the sections of the citation will be set.
*/
void DBLowLevelManager::selectSections(int citationId, CSection &section)
{
	//Select data from the database
	string sqlQuery="SELECT type, title FROM section";
	sqlQuery+=" WHERE citation_id = "+to_string(citationId)+"";
	
	vector<vector<string>> sqlResult=exeSelectQuery(sqlQuery);

	//Process the fetched data to create the vector of CSections
	for(uint i=0; i<sqlResult.size();i++)
	{
		string sectionType=sqlResult[i][0];
		replaceEscapingSlashes(sectionType);
		string sectionTitle=sqlResult[i][1];
		replaceEscapingSlashes(sectionTitle);
		if(i==0){
			section.setTitle(sectionTitle);
			section.setSectionType(sectionType);
		}else{
			section.pushSubSection(sectionType, sectionTitle);
		}
	}
}














