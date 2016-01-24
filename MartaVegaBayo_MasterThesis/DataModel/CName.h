//Declaration of the class CName
#if !defined (_CNAME_H_)
#define _CNAME_H_

#include <iostream>
#include <string>
#include <regex>
using namespace std;

/**
	Class CName. This class represents the name of a person, with a first and lastname.
	The class provides all the methods needed to write and read the first and lastnames.
*/
class CName
{
private:
	string aFirstName;
	string aLastName;


public:
	//Constructor
	CName(const string &firstName="", const string &lastName="");
	CName(const CName &name);
	//Selectors
	const string& getFirstName() const;
	const string& getLastName() const;
	void getName(string &firstName, string &lastName) const;
	void showName() const;
	//Modifiers
	void setName(const string &firstName="", const string &lastName="");
	void setFirstName(const string &firstName="") throw(std::invalid_argument);
	void setLastName(const string &lastName="") throw(std::invalid_argument);
	//Overloaded operators
	const CName & operator=(const CName &name);
	friend ostream& operator<<(ostream& os, const CName& name);
	

};

#endif
