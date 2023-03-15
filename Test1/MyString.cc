#include "MyString.h"
#include <iostream>

using namespace std;

// PRE: An array of characters being copied from, the length of the array being copied from, and an array to be copied to.
// POST: resultingarray's contents are the same as arraytocopy's contents.
void copy(char *arraytocopy, int arraytocopylength, char *resultingarray){
	for (int i = 0; i < arraytocopylength; i++){
	// ASSERT: i is less than arraytocopylength.
		resultingarray[i] = arraytocopy[i];
		// ASSERT: index i of resultingarray is equal to the value at index i of arraytocopy.
	}
}

// PRE:
// POST: This object = strLength: 1, arraySize: 1, theString: char[1], with theString[0] = '\0'.
MyString::MyString(){
	strLength = 1;
	arraySize = 1;
	theString = new char[arraySize];
	theString[0] = '\0';
	// ASSERT: This object has an strLength of 1, an arraySize of 1, and theString = [\0]. Thus, CI is satisfied.
}

// PRE: pString satisfies the CI.
// POST: This object has a strLength equal to pString's strLength, an arraySize equal to pString's arraySize, and theString = pString's theString and satisfies the CI.
MyString::MyString(const MyString &pString){
	strLength = pString.strLength;
	arraySize = pString.arraySize;
	theString = new char[arraySize];
	copy(pString.theString, pString.strLength, theString);
}

// PRE: pString is defined.
// POST: This object has a strLength equal to number of elements in array, arraySize equal
MyString::MyString(const char pString[]){
	int LengthOfPString = 0; // Integer value to hold the amount of characters within pString.
	// ASSERT: LengthOfPString is equal to 0.
	while(pString[LengthOfPString] != '\0'){
	// ASSERT: index LengthOfPString of pString does not equal the end of string character.
		LengthOfPString++;
		// ASSERT: LengthOfPString is equal to the sum of itself and 1.
	}
	LengthOfPString++;
	// ASSERT: LengthOfPString is equal to the sum of itself and 1.
	strLength = LengthOfPString;
	// ASSERT: strLength is equal to LengthOfPString.
	arraySize = LengthOfPString;
	// ASSERT: arraySize is equal to LengthofPString.
	theString = new char[arraySize];
	// ASSERT: theString is set to point to a new character array with size equal to arraySize belonging to this object.
	for(int i = 0; i < strLength; i++) {
	// ASSERT: i is less than strLength.
		theString[i] = pString[i];
		// ASSERT: index i of theString is set to equal the value at index i of pString.
	}
}

// PRE: This object is defined.
// POST: This object is destroyed.
MyString::~MyString(){
	delete[] theString;
	// ASSERT: theString is destroyed, thus satisfying our POST condition.
}

// PRE: This object that satisfies CI. i is the index at which we are trying to find what character resides.
// POST: RV = Character at index i in This object's theString array.
char MyString::GetIthCharInString(int i) const{
	return (theString[i]);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p].
// POST: RV = n - 1.
int MyString::GetStrLength() const{
	return (strLength - 1);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pSubString = strLength: x, arraySize: y, theString: char[y].
// POST: RV = location within This object's theString where pSubString is, or -1 if it is not within this object.
int MyString::FindSubString(const MyString &pSubString) const{
	bool SubString = false; // true or false value denoting whether pSubString is a substring of this object.
	// ASSERT: SubString is assumed to be false until proven true.
	for(int i = 0; i < strLength; i++) {
	// ASSERT: i is less than strLength belonging to this object.
		if(this->theString[i] == pSubString.theString[0]) {
		// ASSERT: index i of this object's theString is the same character as the character to be checked against.
			for(int n = 0; n < pSubString.strLength - 1; n++){
			// ASSERT: n is less than the length of pSubString.
				if(pSubString.theString[n] == this->theString[i + n]) {
				// ASSERT: index n of theString belonging to pSubString is equal to the index equal to the sum of i and n in this object's theString.
					SubString = true;
					// ASSERT: SubString is set to true.
				}
				else{
					return(-1);
				}
			}
			if(SubString = true) {
			// ASSERT: pSubString is a substring of this object.
				return(i);
			}
		}
	}
	return(-1);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p].
// POST: RV = char[p].
char * MyString::GetTheString() const{
	return(theString);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pStringToReplace = strLength: a, arraySize: b, theString: char[b]. pReplacementString = strLength: c, arraySize: d, theString: char[d].
// POST: This object = strLength: n + c, arraySize: p + b, theString: char[p + b].
void MyString::FindAndReplace(const MyString &pStringToReplace, const MyString &pReplacementString){
	while(pStringToReplace.IsSubString(*this)) {
	// ASSERT: pStringToReplace is a substring of this object.
		int LocationOfSubString = this->FindSubString(pStringToReplace); // Index of this object where firstcharacter of pStringToReplace is located.
		// ASSERT: LocationWithinString is equal to pStringToReplace's index within this object.
		for(int i = 0; i < (pStringToReplace.strLength - 1); i++) {
		// ASSERT: i is less than the difference between the length of pStringToReplace and 1.
			this->RemoveIthChar(LocationOfSubString);
			// ASSERT: character at index i of this object is removed.
		}
		for(int n = LocationOfSubString; n < (pReplacementString.strLength - 1) + LocationOfSubString; n++) {
		// ASSERT: n is less than the diference between the length of pReplacementString and 1.
			this->InsertChar(n, pReplacementString.theString[n - LocationOfSubString]);
			// ASSERT: character at the index equal to the difference between n and LocationOfSubString is inserted into this object at index n.
		}
	}
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p].
// POST: This object = strLength: 1, arraySize: p, theString: char[p], with theString[0] = '\0'.
void MyString::clear(){
	strLength = 1;
	// ASSERT: Reset the length of the string to 1.
	theString[0] = '\0';
	// ASSERT: Set the first character in theString to be the end of string character.
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pChar is the passed character needing to be added to the end of theString array.
// POST: This object = strLength: n+1, arraySize: p * 2 or p, theString: char[p * 2] or char[p].
void MyString::addChar(char pChar){
	if (strLength == arraySize){
	// ASSERT: strLength is equal to arraySize
		char *temparray = new char[arraySize * 2]; // new theString array that is double the size of previous theString array.
		// ASSERT: Each index of temparray is undefined.
		for (int i = 0; i < strLength - 1; i++){
		// ASSERT: i is less than the difference between 1 and strLength.
			temparray[i] = theString[i];
			// ASSERT: index i of temparray is equal to index i of theString.
		}
		temparray[strLength - 1] = pChar;
		// ASSERT: the index equal to the difference between strLength and 1 is equal to pChar.
		temparray[strLength] = '\0';
		// ASSERT: the index equal to strLength is equal to the end of string character.
		delete[] theString;
		// ASSERT: theString is destroyed.
		theString = temparray;
		// ASSERT: theString is equal to temparray.
		arraySize = arraySize * 2;
		// ASSERT: arraySize is doubled.
	}
	else{
		theString[strLength - 1] = pChar;
		// ASSERT: the index equal to the difference between strLength and 1 is equal to pChar.
		theString[strLength] = '\0';
		// ASSERT: the index equal to strLength is equal to the end of string character.
	}
	strLength++;
	// ASSERT: strLength = strLength + 1.
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. IndexToInsert is the location within theString array that pChar needs to be inserted into. pChar is the passed character needing to be inserted into theString array.
// POST: This object = strLength: n + 1, arraySize: p * 2 or p, theString: char[p * 2] or char[p].
void MyString::InsertChar(int IndexToInsert, char pChar){
	if(strLength == arraySize) {
	// ASSERT: strLength is equal to arraySize.
		char * tempArray = new char[arraySize * 2]; // array that is double the size of theString to hold all current values and pChar.
		// ASSERT: tempArray is a new character array with space allocated to it equal to double arraySize.
		for(int n = 0; n < IndexToInsert; n++) {
		// ASSERT: n is less than IndexToInsert.
			tempArray[n] = theString[n];
			// ASSERT: index n of tempArray is equal to the value at index n of theString.
		}
		tempArray[IndexToInsert] = pChar;
		// ASSERT: index IndexToInsert of tempArray is equal to pChar.
		for(int i = strLength + 1; i > IndexToInsert; i--) {
		// ASSERT: i is greater than IndexToInsert.
			tempArray[i] = theString[i - 1];
			// ASSERT: index i of tempArray is equal to the value at index equal to the difference between i and 1 of theString.
		}
		delete [] theString;
		// ASSERT: theString is destroyed.
		theString = tempArray;
		// ASSERT: theString is set to equal tempArray.
		arraySize = arraySize * 2;
		// ASSERT:
	}
	else {
		for(int i = strLength + 1; i > IndexToInsert; i--) {
		// ASSERT: i is greater than IndexToInsert.
			theString[i] = theString[i - 1];
			// ASSERT: index i of theString is equal to the value at index equal to the difference between i and 1 of theString.
		}
		theString[IndexToInsert] = pChar;
		// ASSERT: index IndexToInsert of theString is equal to pChar.
	}
	strLength++;
	// ASSERT: strLength is equal to the sum of itself and 1.
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p].
//	   i is the index at which you want to replace the character within this object.
// 	   CharacterToAdd is the character you are replacing the existing character with.
// POST: This object = strLength: n, arraySize: p, theString: char[p].
void MyString::ReplaceChar(int i, char CharacterToReplace) {
	theString[i] = CharacterToReplace;
	// ASSERT: index i of theString belonging to this object is set to be CharacterToReplace.
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p].
// POST: This object = strLength: n-1, arraySize: p, theString: char[p].
void MyString::RemoveLastChar(){
	theString[strLength - 1] = '\0';
	// ASSERT: the index equal to the difference of strLength and 1 of theString is equal to the end of string character.
	strLength--;
	// ASSERT: strLength is equal to 1 subtracted from itself.
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p].
// POST: This object = strLength: n-1, arraySize: p, theString: char[p].
void MyString::RemoveFirstChar(){
	for (int i = 0; i < strLength; i++){
	// ASSERT: i is less than strLength.
		theString[i] = theString[i + 1];
		// ASSERT: the index i of theString is equal to the index equal to the sum of i and 1 of theString.
	}
	strLength--;
	// ASSERT: strLength is equal to 1 subtracted from itself.
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. i is the index of the character you want to remove from theString of the object.
// POST: This object = strLength: n-1, arraySize: p, theString: char[p].
void MyString::RemoveIthChar(int i){
	for (int p = i; p < strLength; p++){
	// ASSERT: p is less than strLength.
		theString[p] = theString[p + 1];
		// ASSERT: index p of theString is equal to the index equal to the sum of p and 1 of theString.
	}
	strLength--;
	// ASSERT: strLength is equal to 1 subtracted from itself.
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p].
// POST: This object = strLength: n, arraySize: p, theString: char[p], however at theString[0], the first character is capitalized.
MyString MyString::CapitalizeFirstCharacter() {
	theString[0] = toupper(theString[0]);
	// ASSERT: index 0 of theString is set to equal the uppercase equivalent of index 0 of theString.
	return(*this);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p].
// POST: This object = strLength: n, arraySize: p, theString: char[p], however each value in theString is capitalized.
MyString MyString::CapitalizeAllCharacters() {
	for(int i = 0; i < strLength; i++) {
	// ASSERT: i is less than strLength.
		theString[i] = toupper(theString[i]);
		// ASSERT: index i of theString is set to equal the uppercase equivalent of index i of theString.
	}
	return(*this);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p].
// POST: RV = a true or false value depending on if this object has no characters other than the end of string character.
bool MyString::IsEmpty(){
	bool isEmpty; // Boolean value that holds the true or false value depending on if the object is empty or not.
	// ASSERT: isEmpty is undefined.
	if (theString[0] == '\0'){
	// ASSERT: index 0 of theString is equal to the end of string character.
		isEmpty = true;
		// ASSERT: isEmpty is true.
	}
	else{
		isEmpty = false;
		// ASSERT: isEmpty is false.
	}
	return (isEmpty);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. CurrObj = strLength: a, arraySize: b, theString: char[b].
// POST: RV = a true or false value depending on if this object is a substring of CurrObj.
bool MyString::IsSubString(const MyString &CurrObj) const{
	bool Issubstring = false; // boolean value denoting whether this object is a substring of CurrObj or not.
	// ASSERT: Issubstring is assumed to be false until proven true.
	if(CurrObj.FindSubString(*this) != -1) {
	// ASSERT: Location of CurrObj within this object 
		Issubstring = true;
		// ASSERT: Issubstring is equal to true.
	}
	else {
		Issubstring = false;
		// ASSERT: Issubstring is equal to false.
	}
	return (Issubstring);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
// POST: This object = strLength: a, arraySize: b, theString: char[b].
MyString &MyString::operator=(const MyString &pString){
	delete[] theString;
	// ASSERT: theString is destroyed.
	theString = new char[pString.arraySize];
	// ASSERT: each index of theString is undefined.
	for (int i = 0; i < pString.strLength; i++){
	// ASSERT: i is less than strLength of pString.
		theString[i] = pString.theString[i];
		// ASSERT: the index i of theString is equal to the index i of theString belonging to pString.
	}
	strLength = pString.strLength;
	// ASSERT: strLength is equal to strLength belonging to pString.
	arraySize = pString.arraySize;
	// ASSERT: arraySize is equal to arraySize belonging to pString.
	return (*this);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
// POST: New object = strLength: ((n - 1) + a), arraySize: p + b, theString: char[p + b].
MyString MyString::operator+(const MyString &pString) const{
	MyString Sum; // New object used to store the sum of both parameters.
	// ASSERT: Sum is built using the default constructor.
	delete[] Sum.theString;
	// ASSERT: theString beloning to Sum is destroyed.
	Sum.strLength = (strLength - 1) + pString.strLength;
	// ASSERT: strLength belonging to Sum is equal to the sum of strLength belonging to pString and the difference between 1 and strLength belonging to this object.
	Sum.arraySize = Sum.strLength;
	// ASSERT: arraySize belonging to Sum is equal to strLength belonging to Sum.
	Sum.theString = new char[Sum.arraySize];
	// ASSERT: each index of theString belonging to Sum is undefined.
	for (int i = 0; i < (strLength - 1); i++){
	// ASSERT: i is less than the difference between 2 and strLength. 2 is used due to the index of array's starting at 0 and the lack of wanting to copy the end of string character from the implicit parameter.
		Sum.theString[i] = theString[i];
		// ASSERT: index i of theString belonging to Sum is equal to the index i of theString belonging to this object.
	}
	for (int p = 0; p < (pString.strLength); p++){
	// ASSERT: p is less than strLength belonging to pString.
		Sum.theString[p + (strLength - 1)] = pString.theString[p];
		// ASSERT: the index equal to the sum of p and the difference between 1 and strLength is equal to the index p of theString belonging to pString.
	}
	return (Sum);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
// POST: RV = a true or false value depending on if this object is less than pString.
bool MyString::operator<(const MyString &pString){
	bool isLessThan = false; // Boolean value that holds the true or false value depending on if this object is less than pString.
	// ASSERT: isLessThan is false until proven true.
	int count = 0; // An increment counter to determine which iteration of the while loop below the program is currently on.
	// ASSERT: count is equal to 0.
	while ((count < strLength) && (count < pString.strLength) && (!isLessThan)){
	// ASSERT: the index count of theString is not equal to the end of string character and the index count of theString belonging to pString is not equal to the end of string character and isLessThan is false.
		if (theString[count] != pString.theString[count]){
		// ASSERT: the index count of theString is not equal to the index count of theString belonging to pString.
			if (theString[count] < pString.theString[count]){
			// ASSERT: the index count of theString is less than the index count of theString belonging to pString.
				isLessThan = true;
				// ASSERT: isLessThan is true.
			}
			else{
				break;
				// Exit the while loop.
			}
		}
		count++;
		// ASSERT: count is equal to the sum of 1 and itself.
	}
	return (isLessThan);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
// POST: RV = a true or false value depending on if this object is greater than pString.
bool MyString::operator>(const MyString &pString){
	bool isGreaterThan = false; // Boolean value that holds the true or false value depending on if this object is greater than pString.
	// ASSERT: isGreaterThan is false until proven true.
	int count = 0; // An increment counter to determine which iteration of the while loop below the program is currently on.
	// ASSERT: count is equal to 0.
	while ((theString[count] != '\0') && (pString.theString[count] != '\0') && (!isGreaterThan)){
	// ASSERT: the index count of theString is not equal to the end of string character and the index count of theString belonging to pString is not equal to the end of string character and isGreaterThan is false.
		if (theString[count] != pString.theString[count]){
		// ASSERT: the index count of theString is not equal to the index count of theString belonging to pString.
			if (theString[count] > pString.theString[count]){
			// ASSERT: the index count of theString is greater than the index count of theString belonging to pString.
				isGreaterThan = true;
				// ASSERT: isGreaterThan is true.
			}
			else{
				break;
				// Exit the while loop.
			}
		}
		count++;
		// ASSERT: count is equal to the sum of 1 and itself.
	}
	return (isGreaterThan);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
// POST: RV = a true or false value depending on if this object is less than or equal to pString.
bool MyString::operator<=(const MyString &pString){
	bool isLessThanOrEqualTo; // Boolean value that holds the true or false value depending on if this object is less than or equal to pString.
	// ASSERT: isLessThanOrEqualTo is undefined.
	isLessThanOrEqualTo = !(*this > pString);
	// ASSERT: isLessThanOrEqualTo is equal to the negation of if this object is greater than pString.
	return (isLessThanOrEqualTo);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
// POST: RV = a true or false value depending on if this object is greater than or equal to pString.
bool MyString::operator>=(const MyString &pString){
	bool isGreaterThanOrEqualTo; // Boolean value that holds the true or false value depending on if this object is greater than or equal to pString.
	// ASSERT: isGreaterThanOrEqualTo is undefined.
	isGreaterThanOrEqualTo = !(*this < pString);
	// ASSERT: isGreaterThanOrEqualTo is equal to the negation of if this object is less than pString.
	return (isGreaterThanOrEqualTo);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
// POST: RV = a true or false value depending on if this object is equal to pString.
bool MyString::operator==(const MyString &pString){
	bool isEqualTo = true; // Boolean value that holds the true or false value depending on if this object is equal to pString.
	// ASSERT: isEqualTo is true until proven false.
	if (strLength == pString.strLength){
	// ASSERT: strLength is equal to strLength of pString.
		for (int i = 0; i < strLength; i++){
		// ASSERT: i is less than strLength.
			if (theString[i] != pString.theString[i]){
			// ASSERT: index i of theString is not equal to index i of theString belonging to pString.
				isEqualTo = false;
				// ASSERT: isEqualTo is false.
			}
		}
	}
	else{
		isEqualTo = false;
		// ASSERT: isEqualTo is false.
	}
	return (isEqualTo);
}

// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
// POST: RV = a true or false value depending on if each of this object's characters in theString are equal to each character in pString.
bool MyString::operator==(const char pString[]){
	bool isEqualTo = true; // Boolean value that holds the true or false value depending on if this object is equal to pString.
	// ASSERT: isEqualTo is true until proven false.
	for (int i = 0; i < strLength; i++){
	// ASSERT: i is less than strLength.
		if (theString[i] != pString[i]){
		// ASSERT: index i of theString is not equal to index i of theString belonging to pString.
			isEqualTo = false;
			// ASSERT: isEqualTo is false.
		}
	}
	return (isEqualTo);
}

// PRE: pString = strLength: a, arraySize: b, theString: char[b], stream is a defined readable stream.
// POST: stream contains strLength: a, arraySize: b, and theString: char[b].
istream &operator>>(istream &is, MyString &pString){
	pString.clear();
	char chartoadd = '0'; // Single character being added to theString.
	// ASSERT: chartoadd is set to a dummy value to ensure there is not garbage in the variable.
	while (chartoadd != '\n' && !is.eof()){
	// ASSERT: chartoadd is not equal to the newline character.
		is.get(chartoadd);
		if (chartoadd != '\n' && !is.eof() && chartoadd != ' '){
		// ASSERT: chartoadd is not equal to the newline character.
			pString.addChar(chartoadd);
			// ASSERT: chartoadd is put into theString belonging to pString.
		}
	}
	return (is);
}

// PRE: pString = strLength: a, arraySize: b, theString: char[b], stream is a defined readable stream.
// POST: stream contains strLength: a, arraySize: b, and theString: char[b].
istream &get(istream &is, MyString &pString){
	pString.clear();
	char chartoadd = '0'; // Single character being added to theString.
	// ASSERT: chartoadd is set to a dummy value to ensure there is not garbage in the variable.
	while (chartoadd != '\n' && !is.eof()){
	// ASSERT: chartoadd is not equal to the newline character.
		is.get(chartoadd);
		if (!is.eof()){
		// ASSERT: chartoadd is not equal to the newline character.
			pString.addChar(chartoadd);
			// ASSERT: chartoadd is put into theString belonging to pString.
		}
	}
	return (is);
}

// PRE: pString = strLength: a, arraySize: b, theString: char[b], stream is a defined writeable stream.
// POST: stream contains a list of characters, one after the other, followed by the end of string character.
ostream &operator<<(ostream &os, const MyString &pString){
	for (int i = 0; i < pString.GetStrLength(); i++){
	// ASSERT: i is less than strLength of pString.
		os << pString.GetIthCharInString(i);
	}
	return (os);
}

// PRE: pString = strLength: a, arraySize: b, theString: char[b], stream is a defined writeable stream.
// POST: stream contains a list of characters, one after the other, followed by the end of string character.
ostream &operator<<(ostream &os, const MyString * pStringPtr){
	for (int i = 0; i < pStringPtr->GetStrLength(); i++){
	// ASSERT: i is less than strLength of pString.
		os << pStringPtr->GetIthCharInString(i);
	}
	return (os);
}