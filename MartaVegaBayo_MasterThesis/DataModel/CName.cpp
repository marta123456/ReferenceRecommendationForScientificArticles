//Definition of the class CName
#include "CName.h"


//CONSTRUCTORS
/**
	Constructor of the CName objects. The constructor has empty strings as default parameters.
	
	@param firstName: firstname of the name
	@param lastName: lastname of the name
*/
CName::CName (const string &firstName, const string &lastName):
	aFirstName(firstName), aLastName(lastName)
{
}
/**
	Copy contructor. Creates a name object with the values of a given name object.

	@param name: object to copy
*/
CName::CName (const CName &name):
	aFirstName(name.aFirstName), aLastName(name.aLastName)
{
}


//SELECTORS
/**
	Shows the name (firstname and last name) in the standard output.
*/
void CName::showName() const
{
	cout << aFirstName << " "<< aLastName;
}
/**
	Gets the firstname of the name.
	@return firstname
*/
const string& CName::getFirstName() const
{
	return aFirstName;
}
/**
	Gets the lastname of the name.
	@return lastname
*/
const string& CName::getLastName() const
{
	return aLastName;
}
/**
	Gets the all name by setting the given references given as parameters with the
	values of the firstname and lastname.

	@param firstname: reference to the variable where the firstname will be assigned
	@param lastname: reference to the variable where the lastname will be asigned
*/
void CName::getName(string &firstname, string &lastname) const
{
	firstname=aFirstName;
	lastname=aLastName;
}


//MODIFIERS
/**
	Sets the firstname and lastname of the objects with the names given as parameters
	@param firstName:  firstname to set
	@param lastName: lastname to set
*/
void CName::setName(const string &firstName, const string &lastName)
{
	aFirstName=firstName;
	aLastName=lastName;
}

/**
	Sets the firstname with the value given as a parameter
	@param firstName:  firstname to set

*/
void CName::setFirstName(const string &firstName)throw(std::invalid_argument)
{
	aFirstName=firstName;
}
/**
	Sets the lastname with the value given as a parameter
	@param lastName:  lastname to set
*/
void CName::setLastName(const string &lastName)throw(std::invalid_argument)
{
	aLastName=lastName;
}


//OVERLOADED OPERATORS
/**
	Operator equal. Sets the attributes of the name with the values of tha name given.
	@param name: name to copy.
	@return constant reference to the object, so the operator = can be enchained
*/
const CName & CName::operator=(const CName &name)
{
	aFirstName=name.aFirstName;
	aLastName=name.aLastName;
	return *this;
}
/**
	Overload the operator << to print the first and last name int the given stream output
	@param ostream: stream where the name is going to be printed
	@param name: reference to the name object to print

	@return output stream
*/
ostream& operator<<(ostream& os, const CName& name)
{
	os << name.aFirstName << " " << name.aLastName;
	return os;
}


