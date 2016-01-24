//Declaration of the class IRTitleBased
#if !defined (_IR_TITLE_BASED_H_)
#define _IR_TITLE_BASED_H_
#include <string>
#include <vector>
#include "IR.h"
#include "../DBmanager/DBManager.h"
#include <stdio.h>

/**
	Class IRTitleBased.
	This class implements the Title Based IR. It is an IR system based on term similarity in which the query of the user is
	compared against the title of the articles of the database.
*/
class IRTitleBased: public IR
{
public:
	IRTitleBased(const string &DBname, const string &DBserver, const string &DBuser, const string &DBpwd);
	void getRecommendations(string &query, vector<ir_result> &results);
	void userInterface();

};
#endif
