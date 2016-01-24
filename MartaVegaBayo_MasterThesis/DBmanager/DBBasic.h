//Declaration of the class DBmanagement
#if !defined (_DBBASIC_H_)
#define _DBBASIC_H_
#include <mysql/mysql.h>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
	Class DBBasic.
	This class implements method that simplify the use of any MySql database. It provides methods to connect with the database,
	insert data, select data and execute queries, making sure that no sql injection is done.
	This class also has attributes to record the database connection and its description.
*/
class DBBasic
{
private:
	void replaceString(string& subject, const string& search, const string& replace);
protected:
	MYSQL *aConn;	//MySql connector
	string aServer;	//Server name or ip address
	string aUser;	//User name of the user of the database
	string aPassword;	//Password of the user
	string aDatabase;	//Name of the database to connect and manage

public:
	//Constructor
	DBBasic(const string &database, const string &server, const string &user, const string &password);
	//Connect and close methods
	void connect();
	void close();
	//Methods to simplify the execution of queries
	void exeCreateQuery(const string &query);
	void exeDropQuery(const string &query);
	void exeInsertQuery(const string &query);
	vector<vector <string> > exeSelectQuery(const string &query);
	//Method to avoid sql injection
	void escapeQuotes(string &sqlString);
	void replaceEscapingSlashes(string &sqlString);
	
};

#endif
