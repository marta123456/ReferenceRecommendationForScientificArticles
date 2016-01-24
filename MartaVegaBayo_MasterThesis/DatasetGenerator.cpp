#include "DatasetGenerator.h"



/**
	Constructor of the class. Calls the constructor of the DBManager object in order to initilize to connect to the Database of the
application
*/
DatasetGenerator::DatasetGenerator(const string & database, const string & server, const string & user, const string & password):
database(database), server(server), user(user), pwd(password), DB(database, server, user, password), nArticles(0)
{

}
/**
	Creates the MySql database with its tables. If the database already exists, its tables are dropped and created again.
*/
void DatasetGenerator::createDatabase()
{
	//Create database
	DB.connect();
	DB.createDB();
	DB.close();

}
/**
	Process iteratively all the directories within the given directory. Inside each directory, all the files are processed.
	The name of the file processed is saved in a file.
	@param path: path to the directory to process
	@param name: name of the directory to process

*/
void DatasetGenerator::processDirectory(string &path, const string &directory)
{
	//Save dir name if file
	dirNames<<path+directory<<endl;

	//Open directory and add dir to the path
	string dirToOpen=path+directory;
	auto dir=opendir(dirToOpen.c_str());
	path=dirToOpen+'/';

	if(dir==NULL){
		cout << "Could not open directory "<<dirToOpen<<endl;
		return;
	}
	//Read directory and process items
	struct dirent* entity=readdir(dir);
	while(entity!=NULL)
	{
		processEntity(path, entity);
		entity= readdir(dir);
	}
	//Remove dir from the path
	path.resize(path.length() -1 -directory.length());
	closedir(dir);
}
/**
	Processes the given entity in order to know whether it is a directory or a file, an then call the corresponding method to process the entity.
	@param path: path to the entity to process
	@param name: name of the entity to process
*/
void DatasetGenerator::processEntity(string &path, struct dirent* entity)
{
	//PROCESS DIRECTORIES
	if(entity->d_type==DT_DIR)
	{
		//Do not process directory . and ..
		if(entity->d_name[0]=='.'){
			return;
		}
		processDirectory(path, string(entity->d_name));	
		return;
	}
	//PROCESS REGULAR FILES
	if(entity->d_type==DT_REG)
	{
		processFile(path, string(entity->d_name));
		return;
	}
	//PROCESS REST OF ENTITIES
	cout <<"Not a file or directory: "<<entity->d_name<<endl;
}
/**
	Process the given file. In order to do that, first the file is parsed and and a CArticleParsed object is got with the metadata of the article. Afterwards, the metadata of the article is inserted into the MySql database.
	The parsing time and processing time is saved in files.
	@param path: path to the file to process
	@param name: name of the file to process
*/
void DatasetGenerator::processFile(const string &filePath, const string &fileName)
{
	cout<<"Processing file "<<fileName<< endl;
	struct timeval start, stop1, stop2;
	try{
		//Parse
		PubmedXMLParser parser(aParseBody);
		gettimeofday(&start, NULL);
		parser.loadXML(filePath+fileName);
		parser.parseAll();
		//Add to database
		gettimeofday(&stop1, NULL);
		CArticleParsed article=parser.getArticle();
		
		DB.addParsedArticle(article);
		gettimeofday(&stop2, NULL);
		//Save times
		parsingTimesFile<<((stop1.tv_sec-start.tv_sec)*1000+(stop1.tv_usec-start.tv_usec)/1000)<<endl;
		processingTimesFile<<((stop2.tv_sec-start.tv_sec)*1000+(stop2.tv_usec-start.tv_usec)/1000)<<endl;
		
	}catch(string &e){
		cout <<e<<endl;
		
	}catch(exception &e){
		cout <<e.what()<<endl;
		throw;
	}

	
}

/**
	Parses the file with the given name and adds it to the database.
	@param path: path to the file to process
	@param name: name of the file to process
	@param parseBody: boolean value stating whether the body must be parsed or not
*/
void DatasetGenerator::parseFile(const string &filePath, const string &fileName, bool parseBody)
{
	aParseBody=parseBody;
	DB.connect();
	processFile(filePath, fileName);
	DB.close();
}
/**
	Parses all the files within the given directory (and within its subdirectories) and adds them to the database.
	@param path: path to the directory to process
	@param name: name of the directory to process
	@param parseBody: boolean value stating whether the body must be parsed or not

*/
void DatasetGenerator::parseDirectory(string &path, const string &directory, bool parseBody)
{
	aParseBody=parseBody;
	//Open files where the times will be saved
	string fileFullNameParsing="./Results/Parsing/parsingTimes_WB.txt";
	string fileFullNameProcessing="./Results/Parsing/processingTimes_WB.txt";
	string fileFullNameDirs="./Results/Parsing/dirNames_WB.txt";
	parsingTimesFile.open(fileFullNameParsing.c_str(), ios::app);
	processingTimesFile.open(fileFullNameProcessing.c_str(), ios::app);
	dirNames.open(fileFullNameDirs.c_str(), ios::app);
	//Parse directory
	DB.connect();
	processDirectory(path,directory);
	DB.close();
	//close files
	parsingTimesFile.close();
	processingTimesFile.close();
	dirNames.close();	

}

void DatasetGenerator::testXMLGenerator(const string &dir, int n)
{
	ofstream fdArt, fdExtArt, fdCit;
	string timesArt_name="./Results/XMLGenerator/ArticlesTimes.txt";
	string timesExtArt_name="./Results/XMLGenerator/ExtendedArticlesTimes.txt";
	string timesCit_name="./Results/XMLGenerator/CitationsArticlesTimes.txt";
	fdArt.open(timesArt_name.c_str(), ios::out);
	fdExtArt.open(timesExtArt_name.c_str(), ios::out);
	fdCit.open(timesCit_name.c_str(), ios::out);
	

	XMLGenerator XMLG(database, server, user, pwd);

	int nArt=100;
	for(uint i=0; i<n; i++)
	{
		int tArticles, tExtArticles, tCitations;
		generateXML(XMLG, dir, tArticles, tExtArticles, tCitations, nArt);
		fdArt<<tArticles<<endl;
		fdExtArt<<tExtArticles<<endl;
		fdCit<<tCitations<<endl;
		//Increase nArt
		nArt+=100;
	}

	fdArt.close();
	fdExtArt.close();
	fdCit.close();
}
/**
	Generates the XML files with the metadata of the parsed files, which is obtained from the pubmed MySql database
	@param XMLG: XMLGenerator object to use
	@param dir: directory wheret the files will be saved
	@param tArticles: variable where the time needed to generate the Articles.xml file will be saved
	@param tExtArticles:variable where the time needed to generate the ExtendedArticles.xml file will be saved
	@param tCitations:variable where the time needed to generate the Citations.xml file will be saved
	@param nArt: maximum number of articles that the XML files, or -1 if the files will contain all the articles of the DB
	@param dir: directory where the generated files will be saved
	@output ArticleIndex.xml
	@output ExtendedArticleIndex.xml
	@output CitationIndex.xml

*/
void DatasetGenerator::generateXML(XMLGenerator &XMLG, const string &dir, int &tArticles, int &tExtArticles, int &tCitations, int nArt)
{
	struct timeval start, stop;
	long sec, msec;
	
	cout <<"----------N="<<nArt<<"------------"<<endl;

	//GENERATE Citations.xml
	//Init
	gettimeofday(&start, NULL);
	string name="Citations.xml";
	if(nArt!=-1)name="Citations_"+to_string(nArt)+".xml";
	//Create XML file
	XMLG.createCitationsXML(name, dir, nArt);
	//Get execution time
	gettimeofday(&stop, NULL);
	sec=stop.tv_sec-start.tv_sec;
	msec=sec*1000+(stop.tv_usec-start.tv_usec)/1000;
	tCitations=msec;
	cout << "Time lapsed for the generation of the Citation XML: " << msec << endl;


	//GENERATE ExtendedArticles.xml
	//Init
	gettimeofday(&start, NULL);
	name="ExtendedArticles.xml";
	if(nArt!=-1)name="ExtendedArticles_"+to_string(nArt)+".xml";
	//Create XML file
	XMLG.createExtendedArticlesXML(name, dir, nArt);
	//Get execution time
	gettimeofday(&stop, NULL);
	sec=stop.tv_sec-start.tv_sec;
	msec=sec*1000+(stop.tv_usec-start.tv_usec)/1000;
	tExtArticles=msec;
	cout << "Time lapsed for the generation of the Extended Article XML: " << msec << endl;

	
	//GENERATE Articles.xml
	//Init
	gettimeofday(&start, NULL);
	name="Articles.xml";
	if(nArt!=-1)name="Articles_"+to_string(nArt)+".xml";
	//Create XML file
	XMLG.createArticlesXML(name, dir, nArt);
	//Get execution time
	gettimeofday(&stop, NULL);
	sec=stop.tv_sec-start.tv_sec;
	msec=sec*1000+(stop.tv_usec-start.tv_usec)/1000;
	tArticles=msec;
	cout << "Time lapsed for the generation of the Article XML: " << msec << endl;

		
}

