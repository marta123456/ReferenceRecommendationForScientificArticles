#if !defined (_IR_TESTER_H_)
#define _IR_TESTER_H_
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sys/time.h>
#include "../DataModel/CSection.h"
#include "../DataModel/CName.h"
#include "../DBmanager/DBManager.h"
#include "IRTitleBased.h"
#include "IRCitationBased.h"

class IRTester
{	
private:
	uint nSamples;
	string DBname;
	string DBserver;
	string DBuser;
	string DBpwd;
	DBManager DB;
	vector<int> citationsIds;
	vector<float> reciprocalRanks;

public:
	IRTester(const string &DBname, const string &DBserver, const string &DBuser, const string &DBpwd, int n=50);
	void testIRTitleBased();
	void testIRCitationBased();
	void testIRSystems();
	bool getRandomCitation(int &citationId, int &referencedArticleId, string &context);
	void getQueryFromContext(int &referencedArticleId, const string &context, string &query);
	float getReciprocalRank(int realArticleId,vector<ir_result> &recommendations);
	
};


#endif
