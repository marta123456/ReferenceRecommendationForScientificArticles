//Declaration of the class CSection
#if !defined (_CSECTION_H_)
#define _CSECTION_H_

#include <string>
using namespace std;

/**
	Class CSection. This class represents a section within an article. Therefore, the class contains attributes to 
	describe the title of the section, the type of section and a subsection.
	The class also provides methods to set and read the attributes of the objects.

	In order to represent nested sections within an article, CSection objects contain a pointer to another CSection object. If the
	current section does not have any subsection of interest, the pointer value is NULL. Nevertheless, if there is another subsection of interest, 
	a new CSection object is dynamically created to represent that section, and the pointer of the current object is set to point to the
	newly created section. Therefore, in the end, there can be a list of chained CSection objects, pending from a single CSection object.
	The class provides methods add and delete a new section at the end of the list.
	
	The subsection's objects are deleted by the desctructor of the class.

*/
class CSection
{
private:
	string aSectionType; //Type of section
	string aTitle; //Title of the section
	CSection *pSubSection; //Pointer to another CSection object, which is a subsection of the current section
public:
	//Constructors
	CSection( const string &sectionType="", const string &title="");
	CSection(const CSection &section);
	//Operator =
	const CSection& operator= (const CSection &section);
	//Destructor
	~CSection();
	//Modifiers
	void setSectionType(const string &sectionType);
	void setTitle(const string &title);
	void pushSubSection(const string &sectionType="", const string &title="");
	void popSubSection();
	//Selectors
	const string & getSectionType() const;
	const string & getTitle() const;
	const CSection * getPSubSection() const;

};

#endif
