//Declare the class IR
#if !defined (_IR_H_)
#define _IR_H_
#include <string>
#include <cstring>
#include <vector>
#include "../DBmanager/DBManager.h"

/**
	Class defining objects to record the recommendations.
*/
class ir_result
{
public:
	int articleId;//id of the recommended article
	float similarity;//similarity of the article to a query
	//Constructor of the class. Initializes the attributes
	ir_result(int artId, float sim):articleId(artId), similarity(sim){};
	//Overload of the opertaro smaller than
	bool operator<(ir_result B){return similarity<B.similarity;}
};


/**
	Class IR
	This class implements the common functionality if the two IR systems developed, which are based on terms similarity.
	This class provides functionality such as terms extraction, similarity measure and ranking of results.
*/
class IR
{
protected:
	DBManager DB; //DBManager object used to read from the database

public:
	IR(const string &DBname, const string &DBserver, const string &DBuser, const string &DBpwd);
	string getQuery();
	vector<string> getTerms(string &text);
	float computeSimilarity(vector<string> &queryTerms, vector<string> &articleTerms);
	void rankBySimirarity(vector<ir_result> &results);
	void showResult(const string &query, vector<ir_result> &results, int n=10);

};

#endif
