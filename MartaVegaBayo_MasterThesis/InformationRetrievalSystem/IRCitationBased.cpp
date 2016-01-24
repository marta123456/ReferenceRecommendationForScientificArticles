//Definition of the class IRCitationBased
#include "IRCitationBased.h"

/**
	Constructor of the class IRCitationBased. Calls the constructor of the base class with the parameters needed to initialize the database
	manager.
*/
IRCitationBased::IRCitationBased(const string &DBname, const string &DBserver, const string &DBuser, const string &DBpwd):
	IR(DBname, DBserver, DBuser, DBpwd)
{}
/**
	Asks the user for a query, gets a recommendation for it and shows the results
*/
void IRCitationBased::userInterface()
{
	cout<<endl<<endl<<"************************************************"<<endl;
	cout<<"****_________CITATION BASED IR SYSTEM_______****"<<endl;
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
	@param results: vector with the ids of the recommended articles and their similarity, ranked from highest similarity to lowest
	@param excludeCitation: if a value different to -1 (default value) is give, that citation is excluded from the list of citations.
	Values excludeCitation different to -1 are given in test application to exclude from the algorith the citation from which the query has
	been taken from.
*/
void IRCitationBased::getRecommendations(string &query, vector<ir_result> &results, int excludeCitation)
{
	results.empty();

	//Get terms from query
	vector<string> queryTerms=getTerms(query);

	//Get metadata from the database
	DB.connect();
	vector<int> citationIds;
	DB.getCitationsIds(citationIds);
	//In test, exclude the citation from which the query has been taken from
	if(excludeCitation!=-1){
		for(uint i=0; i<citationIds.size(); i++){
			if(citationIds[i]==excludeCitation){
				citationIds.erase(citationIds.begin()+i);
			}
		}
	}
	uint maxCit=citationIds[citationIds.size()-1];
	
	//Compute similarities
	uint pageSize=10000; 
	uint minId=1;
	uint maxId=pageSize;
	vector<float> similarities;
	while(maxCit>=minId)
	{
		cout<<"I"<<flush;
		vector<string> contexts;
		vector<int> citIds;
		vector<int> refArticlesIds;
		DB.getCitations(minId, maxId, citIds, refArticlesIds, contexts);
		for(uint i=0; i<citIds.size(); i++)
		{
			if(excludeCitation!=citIds[i])
			{
				vector<string> contextTerms;
				contextTerms=getTerms(contexts[i]);
		
				float simValue=computeSimilarity(queryTerms, contextTerms);
				results.push_back(ir_result(refArticlesIds[i], simValue));
			}
		}

		minId+=pageSize;
		maxId+=pageSize;
	}
	cout<<endl;

	//Sort by similarities
	rankBySimirarity(results);
	DB.close();

}
