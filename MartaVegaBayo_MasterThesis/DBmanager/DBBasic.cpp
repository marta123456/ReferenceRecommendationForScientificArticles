#include "DBBasic.h"

/**
	Constructor of the class DBBasic. Initializes the attriutes of the class.
	@param database: name of the database to access
	@param server: server name or ip address
	@param user: username of the user of the database
	@param password: password of the user
*/
DBBasic::DBBasic(const string &database, const string &server, const string &user, const string &password):
	aServer(server), aUser(user), aPassword(password), aDatabase(database)
{


}

/**
	Connects to the database with the attributes set by the constructor.
	@thows string with information of the error if the conection can not be stablished.
*/
void DBBasic::connect()
{
	aConn = mysql_init(NULL);
	if(!aConn){
		string error="MySql connection was not correctly initiated.";
		error+=mysql_error(aConn);
		throw error;
	}
	if (!mysql_real_connect(aConn, aServer.c_str(), aUser.c_str(), aPassword.c_str(), aDatabase.c_str(), 0, NULL, 0)) {
		mysql_close(aConn);
		string error="Could not conect MySql server: ";
		error+=mysql_error(aConn);
		throw error;
	}
}
/**
	Closes the connection with the database server.
*/
void DBBasic::close()
{
	mysql_close(aConn);
}

/**
	Executes a query, which sholud be a CREATE query.
	@param query to execute
	@thows string with information of the error if an error happens while executing the query.
*/
void DBBasic::exeCreateQuery(const string &query)
{
	if(mysql_query(aConn, query.c_str())){
		string error="An error ocurred while executing the sql query:"+query;
		error+=mysql_error(aConn);
		throw error;
	}	
}
/**
	Executes a query, which sholud be a DROP query.
	@param query to execute
	@thows string with information of the error if an error happens while executing the query.
*/
void DBBasic::exeDropQuery(const string &query)
{
	if(mysql_query(aConn, query.c_str())){
		string error="An error ocurred while executing the sql query:"+query;
		error+=mysql_error(aConn);
		throw error;
	}	
}
/**
	Executes a query, which sholud be a INSET query.
	@param query to execute
	@thows string with information of the error if an error happens while executing the query.
*/
void DBBasic::exeInsertQuery(const string &query)
{
	if(mysql_query(aConn, query.c_str())){
		string error="An error ocurred while executing the sql query:"+query;
		error+=mysql_error(aConn);
		throw error;
	}	
}
/**
	Executes a query, which sholud be a SELECT query.
	@param query to execute
	@return vector of vectors with the result of the select query. There is one vector per row selected.
	Each row if defined by one vector with one string per column selected.
	@throw string with information of the error if an error happens while executing the query.
*/
vector<vector<string>> DBBasic::exeSelectQuery(const string &query)
{
	//Execute query
	if(mysql_query(aConn, query.c_str())){
		string error="An error ocurred while executing the sql query:"+query;
		error+=mysql_error(aConn);
		throw error;
	}
	//Store result
	MYSQL_RES *result=mysql_store_result(aConn);
	if(result==NULL)
	{
		string error="An error ocurred while saving the result of the query:"+query;
		error+=mysql_error(aConn);
		throw error;
	}
	//Get number of fields	
	int nFields=mysql_num_fields(result);
	//Create a vector of vector with the results
	MYSQL_ROW row;
	vector<vector<string>> resultVector;
	while((row=mysql_fetch_row(result)))
	{
		vector<string> rowVector(nFields);
		for(int i=0; i < nFields; i++)
		{
			rowVector[i]=row[i]? row[i]: "NULL";
		}
		resultVector.push_back(rowVector);
	}
	//Free result
	mysql_free_result(result);
	return resultVector;
}

/**
	Replaces the single quotes ' of the given string by \' in order to avoid sql injection.
	@param sqlString: string to process
*/
void DBBasic::escapeQuotes(string &sqlString)
{
	replaceString(sqlString, "'", "\\'");
}
/**
	Replaces the escaping sequence \' by one sinlge quote, '.
	@param sqlString: string to process
*/
void DBBasic::replaceEscapingSlashes(string &sqlString)
{
	replaceString(sqlString, "\\'", "'");
}

/**
	Replaces in the given string, al the ocurrences of a subtring by another substring.
	@param subject: string to analyze and modifie
	@param search: substring to search and replace
	@param replace: new substring to insert
*/
void DBBasic::replaceString(string& subject, const string& search, const string& replace) 
{
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}






