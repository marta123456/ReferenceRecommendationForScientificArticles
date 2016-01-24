//Declaration of the class CDate
#if !defined (_CDATE_H_)
#define _CDATE_H_

#include <iostream>
#include <stdexcept>
using namespace std;

/**
	Class CDate. This class represents one date, with a year, month and day in number format. If the day or month of the date
	is not provided, they take 0 as their default value.
	The class provides methods to set, read and compare CDate objects.
*/
class CDate
{
private:
	int aYear; //Year of the date
	int aMonth; //Month of the date
	int aDay; //Day of the day

private:
	bool isLeapYear(void);

public:
	//Constructor
	CDate(int year=0, int month=0, int day=0) throw(std::invalid_argument);
	CDate(const CDate &date) throw(std::invalid_argument);
	//Selectors
	int getYear();
	int getMonth();
	int getDay();
	void getDate(int &year, int &month, int &day) const;
	void showDate();
	//Modifiers
	void setDate(int year=0, int month=0, int day=0) throw(std::invalid_argument);
	//Overloaded operators
	const CDate & operator=(const CDate &date);
	bool operator<(const CDate &date);
	bool operator<=(const CDate &date);
	bool operator>(const CDate &date);
	bool operator>=(const CDate &date);
	friend ostream& operator<<(ostream& os, const CDate& date);
	//Others
	bool isValidDate();

};

#endif

