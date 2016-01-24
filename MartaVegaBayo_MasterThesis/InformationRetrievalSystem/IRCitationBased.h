//Declaration of the class IRCitationBased
#if !defined (_IR_CITATION_BASED_H_)
#define _IR_CITATION_BASED_H_
#include <string>
#include <vector>
#include "IR.h"
#include "../DBmanager/DBManager.h"
#include <stdio.h>

/**
	Class IRCitationBased.
	This class implements the Citation Based IR. It is an IR system based on term similarity in which the query of the user is
	compared against the title of the articles of the database.
*/
class IRCitationBased: public IR
{
public:
	IRCitationBased(const string &DBname, const string &DBserver, const string &DBuser, const string &DBpwd);
	void getRecommendations(string &query, vector<ir_result> &results, int excludeCitation=-1);
	void userInterface();
};
#endif
