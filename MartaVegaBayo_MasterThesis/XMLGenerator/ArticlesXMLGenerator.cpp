#include "ArticlesXMLGenerator.h"

//Initialization of the static variables, which contain default values for the configuration of the system
string ArticlesXMLGenerator::defaultXMLName = "Articles.xml";
string ArticlesXMLGenerator::defaultXMLPath = "./XMLGenerated/";


/**
	Constructor of the class ArticlesXMLGenerator. Sets the attributes aDBserver, aDBuser and aDBpwd with the given values or with a default value of server if no server is given.
	@param DBuser: username of the user of the database
	@param DBpwd: password of the username of the database
	@param DBserver: server name or IP address of the database. If no value is given, this parameter takes as
	default value the value of the static variable 'defaultDBserver'.
*/
ArticlesXMLGenerator::ArticlesXMLGenerator(const string &DB,const string &DBserver, const string &DBuser, const string &DBpwd):
	BasicGenerator(DB, DBserver, DBuser, DBpwd)
{

}
/**
	Creates the article index xml document with the basic metadata (id, title, publication date and authors)of all the articles (both articles from pubmed and articles referenced by them) saved in the database.
	@param filename: name to give to the file to generate
	@param filepath: path to the location where the file will be saved
*/
void ArticlesXMLGenerator::createXML(const string &fileName, const string &filePath, int nMax)
{	
	//Initialize attributes
	aXMLName=fileName;
	aXMLPath=filePath;
	//Start xml doc
	startXMLGeneration();
	//Get articles ids from the database
	vector<int> articleIdList;
	DB.getArticlesIds(articleIdList, nMax);
	uint nArticles=articleIdList.size();
	//Loop getting articles from the database and adding them to the xml doc
	for(uint i=0; i<nArticles; i++){
		string title;
		CDate pubDate;
		vector<CName> authors;
		int articleId=articleIdList[i];
		DB.getArticleMetadata(articleId, title, pubDate, authors);
		addArticle(articleId, title, pubDate, authors);	
	}
	//End xml doc
	endXMLGeneration();
}

/**
	Starts the generation of the Articles xml documment. In order to do that, the file
	 is opened, the xml header is written and the openning root tag is written.
*/
void ArticlesXMLGenerator::startXMLGeneration()
{
	openFile(aXMLFile, aXMLPath, aXMLName);
	addXMLHeader(aXMLFile);
	addOpenningTag(aXMLFile, "articles");
}
/**
	Finishes the generation of the Articles xml documment. In order to do that,
	the closing root tag is written and the file is closed.
*/
void ArticlesXMLGenerator::endXMLGeneration()
{
	addClosingTag(aXMLFile, "articles");
	closeFile(aXMLFile);
}
/**
	Writes in the xml file an 'article' node containing the basic metadata given of an article.
	@param articleId: id of the article to add to the xml documment
	@param title: title of the article to add
	@param pubDate: CDate object with the date of the article to add
	@param authors: vector with the names of the authors of the article to add

*/
void ArticlesXMLGenerator::addArticle(int articleId, const string &title, const CDate &pubDate, const vector<CName> &authors)
{
	//Add openning tag
	addOpenningTag(aXMLFile, "article");
	//Add nodes
	addArticleId(articleId);
	addTitle(title);
	addPublicationDate(pubDate);
	addAuthors(authors);
	//Add closing tag
	addClosingTag(aXMLFile, "article");
	
}
/**
	Writes in the xml file an 'articleId' node with the id of the article.
	@param articleId: id of the article
*/
void ArticlesXMLGenerator::addArticleId(int articleId)
{
	addOpenningTag(aXMLFile, "articleId");
	addData(aXMLFile, to_string(articleId));
	addClosingTag(aXMLFile, "articleId");
}
/**
	Writes in the xml file an 'title' node with the title of the article.
	@param title: title of the object
*/
void ArticlesXMLGenerator::addTitle(const string &title)
{
	addOpenningTag(aXMLFile, "title");
	addData(aXMLFile, title);
	addClosingTag(aXMLFile, "title");
}
/**
	Writes in the xml file an 'authors' node with some 'author' nodes with the
	metadata of the authors of the given article.
	@param authors: vector with the names of the authors to add
*/
void ArticlesXMLGenerator::addAuthors(const vector<CName> &authors)
{
	//Add openning tag
	addOpenningTag(aXMLFile, "authors");
	//Get authors and add an 'author' node per each one
	uint nAuthors=authors.size();
	for(uint i=0; i<nAuthors; i++){
		addAuthor(authors[i]);
	}
	//Add closing tag
	addClosingTag(aXMLFile, "authors");
}
/**
	Writes an 'author' node with the metadata of the given author.
	@param author: CName object with the metadata of the author
*/
void ArticlesXMLGenerator::addAuthor(const CName &author)
{
	addOpenningTag(aXMLFile, "author");
	addFirstname(author.getFirstName());
	addLastname(author.getLastName());
	addClosingTag(aXMLFile, "author");
}
/**
	Writes a 'firstname' node with the firstname of the author given
	@param firstname: name to add
*/
void ArticlesXMLGenerator::addFirstname(const string &firstname)
{
	addOpenningTag(aXMLFile, "firstname");
	addData(aXMLFile, firstname);
	addClosingTag(aXMLFile, "firstname");
}
/**
	Writes a 'lastname' node with the firstname of the author given
	@param lastname: name to add
*/
void ArticlesXMLGenerator::addLastname(const string &lastname)
{
	addOpenningTag(aXMLFile, "lastname");
	addData(aXMLFile, lastname);
	addClosingTag(aXMLFile, "lastname");
}
/**
	Writes 'publicationDate' node with the publication date of the article.
	@param pubDate: date object with the publication date of the article to add
*/
void ArticlesXMLGenerator::addPublicationDate(const CDate &pubDate)
{
	addOpenningTag(aXMLFile, "publicationDate");
	int year, month, day;
	pubDate.getDate(year, month, day);
	addYear(year);
	addMonth(month);
	addDay(day);
	addClosingTag(aXMLFile, "publicationDate");
}

/**
	Writes a 'year' node with the year of publication.
	@param year: year of publication
*/
void ArticlesXMLGenerator::addYear(int year){
	addOpenningTag(aXMLFile, "year");
	addData(aXMLFile, to_string(year));
	addClosingTag(aXMLFile, "year");
}
/**
	Writes a 'month' node with the month of publication.
	@param month: month of publication
*/
void ArticlesXMLGenerator::addMonth(int month){
	addOpenningTag(aXMLFile, "month");
	addData(aXMLFile, to_string(month));
	addClosingTag(aXMLFile, "month");
}
/**
	Writes a 'day' node with the day of publication.
	@param day: day of publication
*/
void ArticlesXMLGenerator::addDay(int day){
	addOpenningTag(aXMLFile, "day");
	addData(aXMLFile, to_string(day));
	addClosingTag(aXMLFile, "day");
}

