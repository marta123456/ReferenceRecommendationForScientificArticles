//Definition of functions used by the parsers
#include "ParserUtils.h"

/**
	Gets the text within the given XML node. If nested nodes are found, this function is called iteratively to get the
	text within them.
	If a 'sub' or 'sup' node is found, the simbols '_' or '^' are added accordingly.
	@param node: XML node whose text is going to be got
	@param text: string where the parsed text will be set
*/
void getTextWithinItem(pugi::xml_node node, string &text)
{
	//Iterate over all the nodes of the given node
	for (pugi::xml_node child: node.children())
	{
		//Get raw text
		if (child.type() == pugi::node_pcdata){
			text+=child.value();
		//Parse subnodes
		}else{
			string nodeName=child.name();
			if(nodeName=="sub"){text=text+"_";}
			else if(nodeName=="sup"){text=text+"^";}
			getTextWithinItem(child, text);

		}	
	}
}

/**
	Splits the given string into different substrings if the given delimiter is found.
	@param s: string to split
	@param delim: sequence of characters used as delimiter to split the string
	@return vector with the substrings
*/
vector<string> split(const string &s, const string &delim)
{
	vector<string> splits;
 	string::size_type start=0;
	string::size_type end=s.find(delim);
    while(end != string::npos) 
	{
		splits.push_back(s.substr(start, end-start));
		start=end+delim.length();
		end=s.find(delim, start);
	}
	splits.push_back(s.substr(start, end-start));

  	return splits;
}

/**
	Splits the given string into sentences. A sentence is considered to be found if the sequence '. ' is found and the first character of the
	next sentence is a number of a capital lettre.
	@param s: string to split
	@return vector with sentences
*/
vector<string> splitIntoSentences(const string &s)
{
	//GET SENTENCES SPLITED BY DOTS
	//Split by dots
	vector<string> splitsByDot;
	string delim=". ";
	splitsByDot=split(s, delim);
	//Get sentences by identifying abbreviation dots and appending splits accordingly
	vector<string> sentences;
	uint nSplits=splitsByDot.size();
	uint nSent=0;
	for(uint i=0; i<nSplits; i++){
		//First split is allways the begining of a sentence
		if(i==0){
			sentences.push_back(splitsByDot[i]);
			nSent+=1;
		}else{
			regex capital_init_regex("^[A-Z0-9\\s\\(\\{\\[]{1}");
			bool firstLetreIsCapital=regex_search(splitsByDot[i].c_str(), capital_init_regex);
			//If the first letre is not capital, the previous dot was not a sentence dot, but and abbreviation
			if(!firstLetreIsCapital){
				sentences[nSent-1]+=". "+splitsByDot[i];
			//If the first letter is capital, it is a new sentence
			}else{
				sentences.push_back(splitsByDot[i]);
				nSent+=1;
			}	
		}	
	}

  	return sentences;
}
