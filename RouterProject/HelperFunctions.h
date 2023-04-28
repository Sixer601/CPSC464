#ifndef INCLUDED_HelperFunctions
#define INCLUDED_HelperFunctions

#include <iostream>

using namespace std;

// PRE: 
// POST: RV is the ip address of the computer running the program this function is invoked in.
string getIpAddress();

// PRE: pString is a defined string object containing delimiters. 
//      currDelimIndex is where the current delimiter is in pString.
// POST: RV is the index where the next delimiter is stored.
int findNextDelimiterIndex(string pString, int currDelimIndex);


#endif