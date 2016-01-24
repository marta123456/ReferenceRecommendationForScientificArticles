//Definition of the class IR
#include "IR.h"

/**
	Constructor of the class IR.
	Initializes the DBManager with the parameters given
*/
IR::IR(const string &DBname, const string &DBserver, const string &DBuser, const string &DBpwd):
	DB(DBname, DBserver, DBuser, DBpwd)
{

}

/**
	Asks the user for a query in the standard output, gets que string from the standard input and returns the query.
	@return query given by the user
*/
string IR::getQuery()
{
	cout<<"Search: ";
	string userInput;
	getline(cin, userInput);
	return userInput;
}
/**
	Transforms the given string into a set of terms. In order to do that, the string is splitted into words, they are
	transformed into lowercase words and duplicated words are removed. Words are obtained by splitting by whitespaces and 
	removing special characters (such as ().,) at the beginning or end of the string.
	@param text: text to process
	@return vector with the terms extracted
*/
vector<string> IR::getTerms(string &text)
{
	vector<string> terms;

	//Split by whitespace
	istringstream is(text);
	string word;
	if (text.length()==0) return terms; 
	
	while(is>>word)
		terms.push_back(word);


	//Remove special characters at the beginning
	string spB="([{¿¡";
	for(uint i=0; i<terms.size(); i++){
		for(uint j=0; j<5; j++){
			if(terms[i][0]==spB[j]){
				terms[i].erase(0, 1);
				if(terms[i].size()==0)terms.erase(terms.begin()+i);
				break;
			}
		}
	}
	
	//Remove special characters at the end
	string spE=")]}?!.,:;";
	for(uint i=0; i<terms.size(); i++){
		for(uint j=0; j<8; j++){
			if(terms[i].back()==spE[j]){
				terms[i].erase(terms[i].size()-1, 1);
				if(terms[i].size()==0)terms.erase(terms.begin()+i);
				break;
			}
		}
	}
	

	//Set lower case
	for(uint i=0; i<terms.size(); i++)
	{
		std::transform(terms[i].begin(), terms[i].end(), terms[i].begin(), ::tolower);
	}

	
	//Remove duplicates

	if(terms.size()>1)
	{
		for(uint i=0; i<(terms.size()-1); i++)
		{
			for(uint j=i+1; j<terms.size(); j++)
			{
				if(terms[i]==terms[j])
				{
					terms.erase(terms.begin() + j);
					j--;
				}
			}
		}
	}
	
	//Return results
	return terms;
	
}

/**
	Computes a similarity measure between the two sets of terms give, which are the terms of the query and terms of metadata of the 	article. 
	The similarity measure computed is the Jaccard coefficient, which the relation between the size of the intersection of the two sets 		and the size of the union of the two sets.
	@param queryTerms: vector with the terms of the query
	@param articleTerms: vector with the terms of metadata of the article
	@return value of the Jaccard coefficient
*/
float IR::computeSimilarity(vector<string> &queryTerms, vector<string> &articleTerms)
{
	uint sizeQuery, sizeArticle, sizeIntersection, sizeUnion;
	//Get size of the sets of terms
	sizeQuery=queryTerms.size();
	sizeArticle=articleTerms.size();

	//Get intersection size
	sizeIntersection=0;
	for(uint i=0; i<sizeQuery; i++)
	{
		for(uint j=0; j<sizeArticle; j++)
		{
			if(queryTerms[i]==articleTerms[j])
			{
				sizeIntersection++;
			}
		}
	}
	//Get union size
	sizeUnion=sizeQuery+sizeArticle-sizeIntersection;

	//Return similarity
	return float(sizeIntersection)/float(sizeUnion);

}

/**
	Gets a vector a vector with the ids of recommended articles and their similarity measuse, and sorts the vector according to their
	similarity (from highest similarity to lowest similarity).
	@param results: vector with the ids of the recommended articles and their similarity
*/
void IR::rankBySimirarity(vector<ir_result> &results)
{
	sort(results.begin(), results.end());
	reverse(results.begin(), results.end());
}

/**
	Shows in the standard output the recommendations for the query.
	@param query: query for which the recommendation is going to be show
	@param results: vector with the ids of the recommended articles and their similarity
	@param n: number of articles to show
*/
void IR::showResult(const string &query, vector<ir_result> &results, int n)
{
	DB.connect();
	uint nArt=results.size();
	uint nToShow;
	if(nArt<uint(n))nToShow=nArt;
	else nToShow=n;

	cout<<endl;

	for(uint i=0; i<nToShow; i++)
	{
		string title;
		DB.getArticleTitle(results[i].articleId, title);
		cout<<results[i].articleId<<" "<<title<<endl;
	}

	DB.close();

}





