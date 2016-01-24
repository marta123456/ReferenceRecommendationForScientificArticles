//Definition of the class CDate
#include "CDate.h"

//CONTRUCTOR
/**
	Constructor of the class CDate. Initializes the attributes of the object with the given values,
	or with the default value (0) if no parameter is given. 
	The constructors checks wether the date is valid or not. If the date is not valid, the default date is set and
	an exception is thrown.

	@param year
	@param month
	@param day

	@throws std::invalid_argument exception if the date given is not valid

*/
CDate:: CDate(int year, int month, int day) throw(std::invalid_argument):
	aYear(year), aMonth(month), aDay(day)
{
	if(!isValidDate())
	{
		aYear=0;
		aMonth=0;
		aDay=0;
		throw std::invalid_argument("The given date is not valid.");
	}
}
/**
	Copy coonstructor. Creates a CDate objets with its attributes set with the
	values of a given date.

	@param date: reference to the CDate object to be copied
*/
CDate::CDate(const CDate &date)  throw(std::invalid_argument):
	aYear(date.aYear), aMonth(date.aMonth), aDay(date.aDay)
{

}
//SELECTORS
/**
	Gets the year of the date
	@return year of the date
*/
int CDate::getYear()
{
	return aYear;
}
/**
	Gets the month of the date
	@return month of the date
*/
int CDate::getMonth()
{
	return aMonth;
}
/**
	Gets the day of the date
	@return day of the date
*/
int CDate::getDay()
{
	return aDay;
}
/**
	Gets the date of the object. The method receives the reference of 3 variables, 
	and sets those variables with the values of the year, month and day.

	@param year: reference to the year variable to be set
	@param month: reference to the month variable to be set
	@param day: reference to the day variable to be set
*/
void CDate::getDate(int &year, int &month, int &day) const
{
	year=aYear;
	month=aMonth;
	day=aDay;
}
/**
	Shows the date of the object in the standard output with the format d/m/Y
*/
void CDate::showDate()
{
	cout << aDay << "/" << aMonth << "/" << aYear;
}

//MODIFIERS
/**
	Sets the date with the values given as parameters, or with the default values 
	if no parameter is given.
	If the given date is not valid, the date is not changed.

	@param year: year to set
	@param month: month to set
	@param day: day to set

	@throws std::invalid_argument exception if the given date is not valid
*/
void CDate:: setDate(int year, int month, int day)  throw(std::invalid_argument)
{
	//Save date in local variables
	int previous_year=aYear, previous_month=aMonth, previous_day=aDay;
	//Assign new date
	aYear=year;
	aMonth=month;
	aDay=day;
	//Check valid date
	if(!isValidDate()){
		aYear=previous_year;
		aMonth=previous_month;
		aDay=previous_day;
		throw std::invalid_argument("The given date is not valid.");
	}
	
}

//OVERLOADED OPERATORS
/**
	Operator = 
	@param date: right term of the equal operation
	@return constant reference to the object, in order to be able to concatenate the equal operation
*/
const CDate& CDate::operator=(const CDate &date)
{
	aDay=date.aDay;
	aMonth=date.aMonth;
	aYear=date.aYear;
	return *this;
}
/**
	Operator smaller than. 
	@param date: right term of the comparaison
	@return bool value
*/
bool CDate::operator<(const CDate &date)
{
	if(aYear<date.aYear){
		return true;
	}
	else if(aYear==date.aYear){

		if(aMonth<date.aMonth){
			return true;
		}
		else if(aMonth==date.aMonth){
			if(aDay<date.aDay){
				return true;
			}
		}
	}
	return false;
}
/**
	Operator smaller or equeal than. 
	@param date: right term of the comparaison
	@return bool value
*/
bool CDate::operator<=(const CDate &date)
{
	if(aYear<date.aYear){
		return true;
	}
	else if(aYear==date.aYear){

		if(aMonth<date.aMonth){
			return true;
		}
		else if(aMonth==date.aMonth){
			if(aDay<=date.aDay){
				return true;
			}
		}
	}
	return false;
}
/**
	Operator grater than. 
	@param date: right term of the comparaison
	@return bool value
*/
bool CDate::operator>(const CDate &date)
{
	if(aYear>date.aYear){
		return true;
	}
	else if(aYear==date.aYear){

		if(aMonth>date.aMonth){
			return true;
		}
		else if(aMonth==date.aMonth){
			if(aDay>=date.aDay){
				return true;
			}
		}
	}
	return false;
}
/**
	Operator grater or equal than. 
	@param date: right term of the comparaison
	@return bool value
*/
bool CDate::operator>=(const CDate &date)
{
	if(aYear>date.aYear){
		return true;
	}
	else if(aYear==date.aYear){

		if(aMonth>date.aMonth){
			return true;
		}
		else if(aMonth==date.aMonth){
			if(aDay>date.aDay){
				return true;
			}
		}
	}
	return false;
}
/**
	Overload the operator << to print the date in the given stream output
	@param ostream: stream where the date is going to be printed
	@param name: reference to the date object to print

	@return output stream
*/
ostream& operator<<(ostream& os, const CDate& date)
{
	os << date.aDay << "/" << date.aMonth << "/" << date.aYear;
	return os;
}

//OTHERS
/**
	Checks if the year of the date is a leap-year
	@return bool value (true if the year is leap-year)
 */
bool CDate::isLeapYear()
{
	 return (aYear%4==0) && ( (aYear%100!=0) || (aYear%400==0) );
}
/**
	Checks if the date is valid. If day and/or month are 0, the date might still be considered as a valid date,
	as 0 is the default value when day and month are not known.
	@return bool value
*/
bool CDate::isValidDate()
{
	short days_per_month[13]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	//If month has not been given:
	if(aMonth==0)
	{
		if(aDay!=0){return false;} //Check that the day has not been set either
	}
	//If month has been set:
	else
	{
		//Check valid month
		if(aMonth<1 || aMonth>12)
		{
			return false;
		}
		//Check valid day
		if ( aDay<0 )  
		{
			return false;
		}
		
		if(aDay!=0)
		{
			if ( (aMonth!=2) || (!isLeapYear()) )
			{
				return (aDay<=days_per_month[aMonth-1]);
			}else{
				return (aDay<=days_per_month[1]+1);
			}
		}
	}
	return true;
}
