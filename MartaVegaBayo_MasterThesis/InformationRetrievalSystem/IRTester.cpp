#include "IRTester.h"

IRTester::IRTester(const string &DBname, const string &DBserver, const string &DBuser, const string &DBpwd, int n):
	nSamples(n), DBname(DBname), DBserver(DBserver), DBuser(DBuser), DBpwd(DBpwd), DB(DBname, DBserver, DBuser, DBpwd)
{}
void IRTester::testIRSystems()
{
	//Initialize and declare variables
	citationsIds.empty();
	vector<float> reciprocalRanks_IRTitleBased;
	vector<float> reciprocalRanks_IRCitationBased;
	struct timeval start, stop;
	long sec, msec;
	vector<int> exeTimes_msec_IRTitleBased;
	vector<int> exeTimes_msec_IRCitationBased;
	//Get all citations ids form db
	DB.connect();
	DB.getCitationsIds(citationsIds);
	//Open files to record the results
	ofstream fdRR_IR1, fdRR_IR2, fdMSEC_IR1, fdMSEC_IR2;
	string RRname_IR1="./Results/IRtesting/IRTitleBased_RR.txt";
	string RRname_IR2="./Results/IRtesting/IRCitationBased_RR.txt";
	string MSECname_IR1="./Results/IRtesting/IRTitleBased_MSEC.txt";
	string MSECname_IR2="./Results/IRtesting/IRCitationBased_MSEC.txt";
	fdRR_IR1.open(RRname_IR1.c_str(), ios::out);
	fdRR_IR2.open(RRname_IR2.c_str(), ios::out);
	fdMSEC_IR1.open(MSECname_IR1.c_str(), ios::out);
	fdMSEC_IR2.open(MSECname_IR2.c_str(), ios::out);
	//Start simulation of the IR system
	IRTitleBased irTitleBased(DBname, DBserver, DBuser, DBpwd);
	IRCitationBased irCitationBased(DBname, DBserver, DBuser, DBpwd);
	for(uint i=0; i<nSamples; i++)
	{
		//CREATE QUERY TO TEST
		//Get random citation
		int citationId, referencedArticleId;
		string context="";
		bool res=getRandomCitation(citationId, referencedArticleId, context);
		cout<<"Doing experiment "<<i<<" with citation "<< citationId<<endl;
		//Get query (sub context of the citation)
		if(res==false)return;
		string query="";
		getQueryFromContext(referencedArticleId, context, query);

		//TEST TITLE BASED IR SYSTEM
		//Get recommendation for the query with the IR Title Based
		vector<ir_result> recommendations;
		gettimeofday(&start, NULL);
		irTitleBased.getRecommendations(query, recommendations);
		gettimeofday(&stop, NULL);
		sec=stop.tv_sec-start.tv_sec;
		msec=sec*1000+(stop.tv_usec-start.tv_usec)/1000;
		fdMSEC_IR1<<msec<<endl;
		//Get reciprocal rank for the IR Title Based
		float rr=getReciprocalRank(referencedArticleId, recommendations);
		fdRR_IR1<<rr<<endl;

		cout<<endl;

		//TEST CITATION BASED IR SYSTEM
		//Get recommendation for the query with the IR Citation Based
		recommendations.empty();
		gettimeofday(&start, NULL);
		irCitationBased.getRecommendations(query, recommendations);
		gettimeofday(&stop, NULL);
		sec=stop.tv_sec-start.tv_sec;
		msec=sec*1000+(stop.tv_usec-start.tv_usec)/1000;
		fdMSEC_IR2<<msec<<endl;
		//Get reciprocal rank for the IR Title Based
		rr=getReciprocalRank(referencedArticleId, recommendations);
		fdRR_IR2<<rr<<endl;
	}
	//Close DB connection
	DB.close();
	
	fdRR_IR1.close();
	fdRR_IR2.close();
	fdMSEC_IR1.close();
	fdMSEC_IR2.close();
}



/**
	Randomly selects one citation from the vector citationsIds, and gets the metadata of that citation
	@param citationId: id of the citation selected
	@param referencedArticleId: id of the cited article
	@param context: context of the citation
*/
bool IRTester::getRandomCitation(int &citationId, int &referencedArticleId, string &context)
{
	uint nCit=citationsIds.size();
	if(nCit==0)return false;
	//Get random citation
	srand(time(NULL));
	int rndNumber=rand()%nCit;
	citationId=citationsIds[rndNumber];

	int unusedInt;
	CSection unusedSect;
	DB.getCitation(citationId, unusedInt, referencedArticleId, context, unusedSect);
	//Remove citationId from the list and return true
	citationsIds.erase(citationsIds.begin()+rndNumber);
	return true;

	
}
/**
	Gets a query from the citation given. The query is a substring composed of the 80 caracters previous to the citation. The citation location of found by looking for tha names of the authors of the referenced article within the context of the citation.
	If none of the authors is found, the whole paragraph is used as the query.
	@param referencedArticleId: id of the cited article
	@param context: context of the citation from where the query will be extracted
	@param query: query extracted
*/
void IRTester::getQueryFromContext(int &referencedArticleId, const string &context, string &query)
{		
	vector<CName> authors;
	DB.selectAuthors(referencedArticleId, authors);
	for(uint i=0; i<authors.size(); i++)
	{
		//cout<<authors[i].getLastName()<<endl;
		std::size_t refPos = context.find(authors[i].getLastName());
		if(refPos!=std::string::npos)
		{
			uint iniPos, nChar=80;
			if(refPos<nChar)iniPos=0;
			else iniPos=refPos-nChar;			
			query=context.substr(iniPos, nChar);
			return;
		}
		
	}
	query=context;
}
/**
	Given the ranked list of the recommended articles and the ID of the cited article from where the query was extracted from, the method compuetes the reciprocal rank: 1/rank_of_the_real_article
	@param realArticleId: id of the cited article. Is supposed to be the good and only one solution
	@param recommendedArticlesId: ranked list of recommendations
*/
float IRTester::getReciprocalRank(int realArticleId, vector<ir_result> &recommendations)
{
	uint nArt=recommendations.size();
	for(uint i=0; i<nArt; i++)
	{	
		if(realArticleId==recommendations[i].articleId)
		{
			return float(1)/(i+1);
		}
	}
	return 0;
}
