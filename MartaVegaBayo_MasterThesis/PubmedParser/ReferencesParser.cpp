#include "ReferencesParser.h"


/*######################################################################################

	Methods to parse the DOM and iterate over the citation's nodes

#######################################################################################*/

/**
	Constructor of the class ReferencesParser. Sets the aBackNode to NULL in order to
	be able to know whether the article has been loaded.
*/
ReferencesParser::ReferencesParser():
	aBackNode(NULL)
{}
/**
	Loads from the DOM node given, the child node 'back'.
	@param rootNode: root node of the XML document, which is an 'article' node
	@throws string exception if the given node is a NULL node, or if the given node
	does not have a 'back' child node.
*/
void ReferencesParser::loadXMLnodes(const pugi::xml_node &rootNode)
{	
	//Check that the given node is not null
	if(rootNode==NULL){
		string error="The DOM node given is not valid.";
		throw error;
	}
	//Load back node
	aBackNode=rootNode.child("back");
	if(!aBackNode){
		string error="XML doc not valid for the purpose of this dataset.";
		throw error;
	}

}
/**
	Provided that the article has correctly been loaded, parses from the document
	the bibliographic references and sets with it the aReferences attribute of the 
	aArticle object (attribute of ParserBase).
	@throws string exception if the article has not been loaded jet.
*/
void ReferencesParser::parse()
{
	//Check that the article has been loaded
	if(aBackNode==NULL){
		string error="The article has to be loaded before being parsed.";
		throw error;
	}
	//Get and parse the lists of references
	for (pugi::xml_node node: aBackNode.children())
	{
		string nodeName=node.name();
		if (nodeName == "ref-list"){
			parseRefList(node);
		}
	}
}
/**
	Analyses the childs of the given node, which should be a 'ref-list' xml node,
	gets the references within it and sets with it the aReferences attribute of the 
	aArticle object (attribute of ParserBase).
	@param refListNode: 'ref-list' xml node to be parsed.
*/
void ReferencesParser::parseRefList(const pugi::xml_node &refListNode)
{
	for (pugi::xml_node node: refListNode.children())
	{
		//if node is a 'ref-list' node, iterate
		string nodeName=node.name();
		if (nodeName == "ref-list"){
			parseRefList(node);
		}
		//id node is a 'ref' node, parse its content
		else if(nodeName=="ref")
		{
			parseRef(node);
		}
	}
}
/**
	Parses the content of given xml node, which should be a 'ref' node, gets from it the metadata of the bibliographic reference (or references) and, if the metadata is correctly got, adds reference objects to the aReferences attribute of the aArticle object (attribute of ParserBase).
	In order to get the metadata, the method parses the DOM tags. If the citation node is a 'mixed-citation' node and no title is found in its tags, the methods tries then to get the metadata from the raw string given withing the citation node. The raw string is parsed following the APA standard and, if the citation does not follow that format, the raw string is then parsed following the ICJME. 
	@param refNode: 'ref' xml node to be parsed

*/
void ReferencesParser::parseRef(const pugi::xml_node &refNode)
{
	string idRef=refNode.attribute("id").value();
	
	for (pugi::xml_node citationNode: refNode.children())
	{
		CArticle referencedArticle;
		string title="";
		int year=0, month=0, day=0;
		vector<CName> authors;
		authors.empty();

		//If node type is not citation, continue
		string nn=citationNode.name();
		if((nn!="citation-element")&&(nn!="mixed-citation")&&(nn!="nlm-citation")&&(nn!="citation")&&(nn!="element-citation")){continue;}

		//Try to get the metadata from the XML DOM
		bool xmlParserResult=parseRefFromXML(citationNode, title, year, month, day, authors);
		if(xmlParserResult)
		{
			referencedArticle.setTitle(title);
			referencedArticle.setPubDate(year, month, day);
			referencedArticle.setAuthors(authors);
			aArticle.addReference(idRef, referencedArticle);
		}
		
		//Try to get the metadata from row text given (only form mixed-citation)
		else if(nn=="mixed-citation")
		{
			//Get metadata of the citation
			string mixedCitationText="";
			getTextWithinItem(citationNode, mixedCitationText);
			//Parse text to get information 
			bool parseTextResult=parseAPAref(mixedCitationText, title, year, month, day, authors);
			if(!parseTextResult){
				parseTextResult=parseICMJEref(mixedCitationText, title, year, month, day, authors);
			}
			if(parseTextResult){
				referencedArticle.setTitle(title);
				referencedArticle.setPubDate(year, month, day);
				referencedArticle.setAuthors(authors);
				aArticle.addReference(idRef, referencedArticle);
			}
		}
		
		
	}
}
/*######################################################################################

	Methods to get the metadata of a reference from the DOM tags content

#######################################################################################*/
/**
	Gets from the DOM nodes which are children of the given node some metadata of the reference and sets with that metadata the parameters given as references.
	@param citNode: citation XML node which is going to be parsed
	@param title: reference to a variable where the title will be set
	@param year: reference to the year variable to set
	@param month: reference to the month variable to set
	@param day: reference to the day variable to set
	@param authors: reference to a vector of CNames where the authors parsed will be added

	@return false if the title can not be correctly parsed, true otherwise.
*/
bool ReferencesParser::parseRefFromXML(pugi::xml_node &citNode, string &title, int &year, int &month, int &day, vector<CName> &authors)
{
	//Select title
	bool parseTitleResult=parseTitleFromXML(citNode, title);

	if(parseTitleResult)
	{
		//Select publication date
		parsePubDateFromXML(citNode, year, month, day);
		//Select authors
		parseAuthorsFromXML(citNode, authors);
	}

	return parseTitleResult;

}

/**
	Gets from within the xml node given the title of the reference and sets with it the title variable given as a parameter
	@param citNode: xml node corresponding to the citation whose date of publication is going to be looked for
	@param title: reference to an string variable where the title got will be set
	@return false if no title is found, true otherwise
*/
bool ReferencesParser::parseTitleFromXML(pugi::xml_node &citNode, string &title)
{
	pugi::xml_node ttlNode=citNode.child("article-title");
	if(!ttlNode){
		ttlNode=citNode.child("source");
	}
	if(!ttlNode){
		ttlNode=citNode.child("comment");
	}


	if(!ttlNode){
		return false;
	}
	else{
		getTextWithinItem(ttlNode, title);
		return true;
	}

}
/**

	Gets from within the xml node given the date, which is set in the variables given as references
	@param citNode: xml node corresponding to the citation whose date of publication is going to be looked for
	@param year: reference to the year variable to set
	@param month: reference to the month variable to set
	@param day: reference to the day variable to set

	@return false if the date is not found, true otherwise.

*/
bool ReferencesParser::parsePubDateFromXML(pugi::xml_node &citNode, int &year, int &month, int &day)
{
	string sYear="", sMonth="", sDay="";
	//Get date from xml DOM
	sYear=citNode.child("year").child_value();
	sMonth=citNode.child("month").child_value();
	sDay=citNode.child("day").child_value();

	year=0;
	month=0;
	day=0;
	if(sYear!=""){
		year=atoi(sYear.c_str());
		if(sMonth!=""){
			month=atoi(sMonth.c_str());
			if(sDay!=""){
				day= atoi(sDay.c_str());
			}
		}
		
		return true;
	}
	return false;
	
}
/**
	Gets from within the xml node given the author's names. The author's names got are added to the article given
	@param citNode: xml node corresponding to the citation whose authors are going to be look for
	@param authors: reference to a CName vector where the authors got will be added

	@return false if no author is found, true otherwise.

*/
bool ReferencesParser::parseAuthorsFromXML(pugi::xml_node &citNode,  vector<CName> &authors)
{

	//Select authors given within a 'person-group' node 
	pugi::xml_node  personGroupNode=citNode.child("person-group");
	if(personGroupNode){
		for (pugi::xml_node person: personGroupNode.children())
		{
			string pnn=person.name();
			if((pnn=="name")||(pnn=="string-name")){
				CName name;
				parseName(person, name);
				authors.push_back(name);
			}else if(pnn=="etal"){
				authors.push_back(CName("etal", "etal"));
			}else if(pnn=="collab"){
				authors.push_back(CName(person.child_value(), ""));
			}
		}
	//Select authors given directly as childs of the citation node
	}else{
		for (pugi::xml_node person: citNode.children())
		{
			string pnn=person.name();
			if((pnn=="name")||(pnn=="string-name")){
				CName name;
				parseName(person, name);
				authors.push_back(name);
			}else if(pnn=="etal"){
				authors.push_back(CName("etal", "etal"));
			}
		}
	}

	//If any author has been found, add the to the article object
	if(authors.size()>0){
		return true;
	}
	return false;

}


/**

	Parses a 'name' or 'string-name' xml node and gets from it the lastname and firstname
	(or abreviation) of the given name. The names got are set in the CName object give
	@param node: xml node to be parsed.
	@param name: reference to a name object where the parsed name will be set


*/
void ReferencesParser::parseName(pugi::xml_node &node, CName &name)
{	
	//Select firstname and surname
	string authorLastname=node.child("surname").child_value();
	string authorFirstname=node.child("given-names").child_value();

	name.setFirstName(authorFirstname);
	name.setLastName(authorLastname);
}



/*################################################################################################################

	Methods to the metadata of a reference from string following the APA standard

#################################################################################################################*/

/**
	Parses the string given as if it where following the APA referencing style (with or without authors), gets some metadata from the string and sets with the metadata got the variables given as parameters.
	@param refText: raw string to parse
	@param title: reference to a variable where the title will be set
	@param year: reference to the year variable to set
	@param month: reference to the month variable to set
	@param day: reference to the day variable to set
	@param authors: reference to a vector of CNames where the authors parsed will be added

	@return false if the given string does not have the expected format, true otherwise
*/
bool ReferencesParser::parseAPAref(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors)
{

	bool parseWithAuthorsResult=false, parseWithoutAuthorsResult=false;
	parseWithAuthorsResult=parseAPArefWithAuthors(refText, title, year, month, day, authors);
	if(!parseWithAuthorsResult){
		parseWithoutAuthorsResult=parseAPArefWithoutAuthors(refText, title, year, month, day, authors);
	}
	
	return parseWithAuthorsResult || parseWithoutAuthorsResult;
}
/**
	Parses the given string applying the APA standard style for referencing bibligraphic works with authors and extracts some metadata from it.
	The string given is expected to have the following format: 
	"List_of_authors_splitted_by_comas[. ](date)[. ]Title. [Other_information]"

	@param refText: raw string to parse
	@param title: reference to a variable where the title will be set
	@param year: reference to the year variable to set
	@param month: reference to the month variable to set
	@param day: reference to the day variable to set
	@param authors: reference to a vector of CNames where the authors parsed will be added

	@return false if the given string does not have the expected format, true otherwise
	
*/
bool ReferencesParser::parseAPArefWithAuthors(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors)
{
	//Split string given into authors substring, date substring and the rest of the string
	string sDate, sAuthors;
	regex ref_regex("[\\.]?[\\s]?\\(([0-9]{4}[^\\)]*)\\)[\\.]?[\\s]?");
	smatch match;
	bool parseResult=regex_search(refText, match, ref_regex);
	if(parseResult==false) return false;
	
	//Get authors and date substring
	sAuthors=match.prefix();
	sDate=match[1];
	//Get title (The first sentence after the date)
	parseTitleFromText(splitIntoSentences(match.suffix())[0], title);
	bool validTitle=checkValidTitle(title);
	//Parse authors form the subtring
	parseAuthorsFromText(sAuthors, authors);
	bool validAuthors=checkValidAuthors(authors);
	//Parse Publication date from substring 
	parseAPApubDate(sDate, year, month, day);
	
	//Check valid parsing 
	if((!validAuthors)||(!validTitle) ){
		return false;
	}
	

	return true;
	
}
/**
	Parses the given string applying the APA standard style for referencing bibligraphic works without authors and extracts some metadata from it.
	The string given is expected to have the following format: 
	"Title[. ](date)[. ][Other_information]"

	@param refText: raw string to parse
	@param title: reference to a variable where the title will be set
	@param year: reference to the year variable to set
	@param month: reference to the month variable to set
	@param day: reference to the day variable to set
	@param authors: reference to a vector of CNames where the authors parsed will be added

	@return false if the given string does not have the expected format, true otherwise
	
*/
bool ReferencesParser::parseAPArefWithoutAuthors(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors)
{
	//Split string given into date substring and title substring
	regex ref_regex("[\\.]?[\\s]?\\(([0-9]{4}[^\\)]*)\\)");
	smatch match;
	bool parseResult=regex_search(refText, match, ref_regex);
	if(parseResult==false) return false;
	
	//Get authors and date substring
	string sDate=match[1];
	//Get title
	parseTitleFromText(splitIntoSentences(match.prefix())[0], title);
	bool validTitle=checkValidTitle(title);
	//Parse Publication date from substring 
	parseAPApubDate(sDate, year, month, day);
	
	return validTitle;
}
/**
	Parses the string given, which is the date of a reference in APA referencing style, gets from the string the year, day and month and sets with then the variables given as parameters.
	The string given is expected to have one of the following formats: "yyyy" or "yyyy, Month_name_or_abreviation dd"
	@param sDate: string with the date to be parse
	@param year: reference to the year variable to set
	@param month: reference to the month variable to set
	@param day: reference to the day variable to set

	@return false if the date given does not have any of the expected formats
*/
bool ReferencesParser::parseAPApubDate(string &sDate, int &year, int &month, int &day)
{
	string sYear="0", sMonth="0", sDay="0";

	//Try to get complete date with format (yyyy, Month dd)
	regex date_regex("^([0-9]{4})[a-z]?,\\s([\\S]+)\\s([0-9]{1,2})$");
	smatch resDate;
	bool parseResult=regex_search(sDate, resDate, date_regex);
	if(parseResult){
		if(resDate[1].matched && resDate[2].matched && resDate[3].matched){
			sYear=resDate[1];
			string sMonthAux=resDate[2];
			string monthsFormat1[12]={"Jannuary", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
			string monthsFormat2[12]={"Jan.", "Feb.", "Mar.", "Apr.", "May", "June", "July", "Aug.", "Sept.", "Oct.", "Nov.", "Dec."};	
			for(int i=0; i<12; i++){
				if((monthsFormat1[i]==sMonthAux)||(monthsFormat2[i])==sMonthAux){
					sMonth=to_string(i+1);
					sDay=resDate[3];
				}
			}
		}
	//Try to get year from date with format (yyyy)
	}else{
		regex year_regex("^([0-9]{4})");
		smatch resYear;
		parseResult=regex_search(sDate, resYear, year_regex);
		if(parseResult){
			sYear=resYear[1];
		}	
	}
	year=stoi(sYear);
	month=stoi(sMonth);
	day=stoi(sDay);

	//Return
	if(!parseResult)return false;
	else return true;
}

/*################################################################################################################

	Methods to the metadata of a reference from string following the ICMJE standard

#################################################################################################################*/
/**
	Parses the string given as if it where following the ICMJE referencing style (with or without authors), gets some metadata from the string and sets with the metadata got the variables given as parameters.
	@param refText: raw string to parse
	@param title: reference to a variable where the title will be set
	@param year: reference to the year variable to set
	@param month: reference to the month variable to set
	@param day: reference to the day variable to set
	@param authors: reference to a vector of CNames where the authors parsed will be added

	@return false if the given string does not have the expected format, true otherwise
*/
bool ReferencesParser::parseICMJEref(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors)
{

	bool parseWithAuthorsResult=false, parseWithoutAuthorsResult=false;
	parseWithAuthorsResult=parseICMJErefWithAuthors(refText, title, year, month, day, authors);
	if(!parseWithAuthorsResult){
		parseWithoutAuthorsResult=parseICMJErefWithoutAuthors(refText, title, year, month, day, authors);
	}
	
	return parseWithAuthorsResult || parseWithoutAuthorsResult;
}
/**
	Parses the given string applying the ICMJE standard style for referencing bibligraphic works with authors and extracts some metadata from it.
	The string given is expected to have the following format: 
	"List_of_authors. Title. [other_information] date [other information]"

	@param refText: raw string to parse
	@param title: reference to a variable where the title will be set
	@param year: reference to the year variable to set
	@param month: reference to the month variable to set
	@param day: reference to the day variable to set
	@param authors: reference to a vector of CNames where the authors parsed will be added

	@return false if the given string does not have the expected format, true otherwise
	
*/
bool ReferencesParser::parseICMJErefWithAuthors(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors)
{
	//Split string into sentences
	vector<string> refSplit=splitIntoSentences(refText);

	if(refSplit.size()<3) return false;
	
	//Get authors and parse Authors substring to get the different author's names
	string sAuthors=refSplit[0];
	parseAuthorsFromText(sAuthors, authors);
	bool validAuthors=checkValidAuthors(authors);
	
	//Get title
	parseTitleFromText(refSplit[1], title);

	//Check valid format
	if(!validAuthors){
		return false;
	}
	if(!checkValidTitle(title)){
		return false;
	}
			
	//Get date
	uint nSplits=refSplit.size();
	for(uint i=2; i<nSplits; i++){
		regex date_regex("([0-9]{4})");
		cmatch resDate;
		bool parseResult=regex_search(refSplit[i].c_str(), resDate, date_regex);
		if(parseResult){
			string sYear=resDate[1];
			string sMonth="0";
			string sDay="0";
			year=stoi(sYear);
			month=stoi(sMonth);
			day=stoi(sDay);
			break;
		}
	}

	return true;
}
/**
	Parses the given string applying the ICMJE standard style for referencing bibligraphic works without authors and extracts some metadata from it.
	The string given is expected to have the following format: 
	"Title. [other_information] date [other information]"

	@param refText: raw string to parse
	@param title: reference to a variable where the title will be set
	@param year: reference to the year variable to set
	@param month: reference to the month variable to set
	@param day: reference to the day variable to set
	@param authors: reference to a vector of CNames where the authors parsed will be added

	@return false if the given string does not have the expected format, true otherwise
	
*/
bool ReferencesParser::parseICMJErefWithoutAuthors(string & refText, string &title, int &year, int &month, int &day, vector<CName> &authors)
{
	//Split string into sentences
	vector<string> refSplit=splitIntoSentences(refText);

	if(refSplit.size()<2) return false;
	
	//Get title
	parseTitleFromText(refSplit[0], title);
			
	//Get date
	uint nSplits=refSplit.size();
	for(uint i=1; i<nSplits; i++){
		regex date_regex("([0-9]{4})");
		cmatch resDate;
		bool parseResult=regex_search(refSplit[i].c_str(), resDate, date_regex);
		if(parseResult){
			string sYear=resDate[1];
			string sMonth="0";
			string sDay="0";
			year=stoi(sYear);
			month=stoi(sMonth);
			day=stoi(sDay);
			break;
		}
	}
	
	return true;

}


/**
	Removes part of the title string given because it is not part of the title, but a different sentence. In particular, looks for the sequences ". http" or ". doi" and removes the rest of the string.
	@param sTitle: title string to process
	@param title: final title
*/
bool ReferencesParser::parseTitleFromText(string &sTitle, string &title)
{
	//Remove ". http" ending
	regex ex("(. http)");
	smatch match;
	bool validationResult=regex_search(sTitle,match, ex);	
	if(validationResult){
		title=match.prefix();
		return false;
	}
	//Remove ". doi" ending
	ex="(. doi)";
	validationResult=regex_search(sTitle,match, ex);	
	if(validationResult){
		title=match.prefix();
		return false;
	}
	title=sTitle;
	return true;
}

/**
	Checks if the given title is a valid title. A title is considered to be valid if it does not start with "In ", or "Available at: ".
	@param title: title to check
	@return: bool value, true if the title is valid and false if not 
*/
bool ReferencesParser::checkValidTitle(string &sTitle)
{
	//Check that the title does not start with "In "
	regex ex("^(In:\\s)");
	bool validationResult=regex_search(sTitle.c_str(), ex);	
	if(validationResult){
		return false;
	}
	ex="(^Available at)";
	validationResult=regex_search(sTitle.c_str(), ex);	
	if(validationResult){
		return false;
	}
	return true;
}
/**
	Checks if the given list of authors is valid. If the list of authors is empty, the validation returns false.
	In addition, the method checks whether each one of the authors is valid or not. An author name is considered to be valid if the proportion of capitalized words in it is greater than 0.5. Otherwise, the author is not considered to be valid.
	@param authors: vector with the authors to check
	@return: bool value with the result of the validation
*/
bool ReferencesParser::checkValidAuthors(vector<CName> &authors)
{
	//Check there is some author
	if(authors.size()==0){
		return false;
	}
	//Check that the authors'names are really names and not pieces of titles
	uint nAuthors=authors.size();
	for(uint i=0; i<nAuthors; i++)
	{
		vector<string> words=split(authors[i].getLastName()+" "+authors[i].getFirstName(), " ");
		int nW=words.size();//Nº of words
		int nNC=0; //Nº of non-capitalized words
		//Count number of non-capitalized words
		for(int j=0; j<nW; j++)
		{
			regex nonCap_regex("^[a-z]{1}");
			bool isNonCap=regex_search(words[j].c_str(), nonCap_regex);	
			if(isNonCap){
				nNC++;
			}
		}
		//Get percentage of non-capitalized words and decide weather the author name is valid
		if((float(nNC)/nW)>0.5){
			return false;
		}
	}
	
	return true;
}



/**

*/
bool ReferencesParser::parseAuthorsFromText(string &sAuthors, vector<CName> &authors)
{
	//SPLIT THE GIVEN STRING INTO AUTHORS 
	//(each author will be a string with the title, firstnames and lastnames without any spliter)
	vector<string> authorsNames;
	//Split by ;
	string authDelim="; ";
	vector<string> comaDotSplits=split(sAuthors, authDelim);
	if(comaDotSplits.size()>1){
		for(uint i=0; i<comaDotSplits.size(); i++){
			string authDelim=", ";
			vector<string> comaSplits=split(sAuthors, authDelim);
			string name="";
			for(uint j=0; j<comaSplits.size(); j++){
				name+=comaSplits[j];
			}
			authorsNames.push_back(name);
		}
	}
	
	//If there were no ;, split by ,
	if(authorsNames.size()==0){
		string authDelim=", ";
		vector<string> comaSplits=split(sAuthors, authDelim);
		//if there is only one name, add it
		if(comaSplits.size()==1){
			authorsNames.push_back(comaSplits[0]);
				
		}else{
			//Look for & at the end, and remove it
			for(uint i=0; i<comaSplits.size(); i++)
			{
				string name=comaSplits[i];
				vector<string> ampersanSplits=split(name, " & ");
				if(ampersanSplits.size()>1){
					comaSplits.erase(comaSplits.begin()+i);
					comaSplits.insert(comaSplits.begin()+i, ampersanSplits[0]);
					comaSplits.insert(comaSplits.begin()+i+1, ampersanSplits[1]);	
					
				}
			}
			//Add names (attaching abbreviations)
			for(uint i=0; i<comaSplits.size(); i++)
			{
				//Add first
				if(i==0){
					authorsNames.push_back(comaSplits[i]);
					continue;
				}
				//Look for abreviation
				regex abbr_regex("^[A-Z\\.\\s-_]+$");
				bool isAbbr=regex_search(comaSplits[i].c_str(), abbr_regex);
				if(isAbbr){
					authorsNames[authorsNames.size()-1]+=" "+comaSplits[i];
				}else{
					authorsNames.push_back(comaSplits[i]);
				}
				
			}
			

		}
		
	}
	//REMOVE ET AT
	if(authorsNames[authorsNames.size()-1]=="et al"){
		authorsNames.pop_back();
	}

	//PARSE EACH AUTHOR
	for(uint i=0; i<authorsNames.size(); i++){
        	
		//Get words
		vector<string> words=split(authorsNames[i], " ");
		string firstName="", lastName="";
		bool abbrFound=false;
		for(uint j=0; j<words.size(); j++){
			if(abbrFound){
				firstName+=" "+words[j];
			}else{
				//Look for abbreviations
				regex abbr_regex("^[A-Z\\.\\s-_]+$");
				bool isAbbr=regex_search(words[j].c_str(), abbr_regex);
				if(isAbbr){
					firstName+=words[j];
					abbrFound=true;
				}else{
					if(lastName.length()==0){
						lastName=words[j];
					}else{
						lastName+=" "+words[j];
					}
				}
			}
			
		}
		
		authors.push_back(CName(firstName, lastName));
	}
	
	//Return value
	if(authors.size()==0){
		return false;
	}else{
		return true;
	}
}






