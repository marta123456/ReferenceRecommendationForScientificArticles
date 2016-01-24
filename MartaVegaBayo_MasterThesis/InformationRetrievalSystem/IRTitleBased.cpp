//Definition of the class IRTitleBased
#include "IRTitleBased.h"

/**
	Constructor of the class IRTitleBased. Calls the constructor of the base class with the parameters needed to initialize the database
	manager.
*/
IRTitleBased::IRTitleBased(const string &DBname, const string &DBserver, const string &DBuser, const string &DBpwd):
	IR(DBname, DBserver, DBuser, DBpwd)
{}

/**
	Asks the user for a query, gets a recommendation for it and shows the results
*/
void IRTitleBased::userInterface()
{
	cout<<endl<<endl<<"************************************************"<<endl;
	cout<<"****________TITLE BASED IR SYSTEM___________****"<<endl;
	cout<<"************************************************"<<endl;
	//Get a query
	string query=getQuery();
	//Get recommendation
	vector<ir_result> results;
	getRecommendations(query, results);
	//Show result
	showResult(query, results);

}

/**
	Gets a ranked list of recommended articles for the given query.
	@param query: query for which recommended articles are going to be fooked for
	@param results: vector with the ids of the recommended articles and their similarity ranked from highest similarity to lowest
*/
void IRTitleBased::getRecommendations(string &query, vector<ir_result> &results)
{
	//Empty vectors
	results.empty();

	//Get terms from query
	vector<string> queryTerms=getTerms(query);

	//Get metadata from the database 
	DB.connect();
	vector<int> articleIdsList;
	int nId=0;
	DB.getArticlesIds(articleIdsList);
	uint maxArt=articleIdsList[articleIdsList.size()-1];
	uint pageSize=10000; 
	uint minId=1;
	uint maxId=pageSize;
	//Compute similarities
	while(maxArt>=minId)
	{
		cout<<"I"<<flush;
		vector<string> titles;
		DB.getTitles(minId, maxId, titles);
		for(uint i=0; i<titles.size(); i++)
		{
			vector<string> titleTerms=getTerms(titles[i]);
			float simValue=computeSimilarity(queryTerms, titleTerms);
			results.push_back(ir_result(articleIdsList[nId++], simValue));
		}

		minId+=pageSize;
		maxId+=pageSize;
	}
	//Sort by similarities
	rankBySimirarity(results);
	DB.close();

}
