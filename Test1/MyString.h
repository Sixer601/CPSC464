#ifndef INCLUDED_MyString
#define INCLUDED_MyString

#include <iostream>

using namespace std;

// PRE: An array of characters being copied from, the length of the array being copied from, and an array to be copied to.
// POST: resultingarray's contents are the same as arraytocopy's contents.
void copy(char *arraytocopy, int arraytocopylength, char *resultingarray);

class MyString
{
	/* Class Invariant (CI): arraySize = S > 0
                           	theString has S locations allocated to it.
                           	strLength = L, where 0 <= L < arraySize.
                              theString[0]..theString[L-1] are the
                              characters of this MyString object.
                              theString[L] = '\0'
     */

private:
	char *theString; // holds the characters for this MyString object.
	int strLength;	  // holds the number of characters (not including
				  // the end of string character) in this
				  // MyString object
	int arraySize;	  // holds the current size (number of locations)
				  // allocated for theString.

public:

	// ========================================
	// constructors
	// ========================================

		// Default constructor

			// PRE:
			// POST: This object = strLength: 1, arraySize: 1, theString: char[1], with theString[0] = '\0'.
			MyString();

		// Copy constructor

			// PRE: pString satisfies the CI.
			// POST: This object has a strLength equal to pString's strLength, an arraySize equal to pString's arraySize, and theString = pString's theString and satisfies the CI.
			MyString(const MyString &pString);

		// PRE: char array that is defined.
		// POST: This object = strLength: n, arraySize: p, theString: char[p].
			MyString(const char pString[]);

	// ========================================
	// destructor
	// ========================================

		// PRE: This object is defined.
		// POST: This object is destroyed.
		~MyString();

	// ========================================
	//       accessor functions
	// ========================================

		// PRE: This object that satisfies CI. i is the index at which we are trying to find what character resides.
		// POST: RV = Character at index i in This object's theString array.
		char GetIthCharInString(int i) const;

		// PRE: This object = strLength: n, arraySize: p, theString: char[p].
		// POST: RV = n - 1.
		int GetStrLength() const;

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pSubString = strLength: x, arraySize: y, theString: char[y].
		// POST: RV = location within This object's theString where pSubString is, or -1 if it is not within this object.
		int FindSubString(const MyString &pSubString) const;

		// PRE: This object = strLength: n, arraySize: p, theString: char[p].
		// POST: RV = char[p].
		char * GetTheString() const;

	// ========================================
	// modifier functions
	// ========================================

		// PRE: This object = strLength: n, arraySize: p, theString: char[p].
		// POST: This object = strLength: 1, arraySize: p, theString: char[p], with theString[0] = '\0'.
		void clear();

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pChar is the passed character needing to be added to the end of theString array.
		// POST: This object = strLength: n+1, arraySize: p * 2 or p, theString: char[p * 2] or char[p].
		void addChar(char pChar);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. IndexToInsert is the location within theString array that pChar needs to be inserted into. pChar is the passed character needing to be inserted into theString array.
		// POST: This object = strLength: n + 1, arraySize: p * 2 or p, theString: char[p * 2] or char[p].
		void InsertChar(int i, char pChar);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p].
		//	   i is the index at which you want to replace the character within this object.
		// 	   CharacterToAdd is the character you are replacing the existing character with.
		// POST: This object = strLength: n, arraySize: p, theString: char[p].
		void ReplaceChar(int i, char pChar);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p].
		// POST: This object = strLength: n-1, arraySize: p, theString: char[p].
		void RemoveLastChar();

		// PRE: This object = strLength: n, arraySize: p, theString: char[p].
		// POST: This object = strLength: n-1, arraySize: p, theString: char[p].
		void RemoveFirstChar();

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. i is the index of the character you want to remove from theString of the object.
		// POST: This object = strLength: n-1, arraySize: p, theString: char[p].
		void RemoveIthChar(int i);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pStringToReplace = strLength: a, arraySize: b, theString: char[b]. pReplacementString = strLength: c, arraySize: d, theString: char[d].
		// POST: This object = strLength: n + c, arraySize: p + b, theString: char[p + b].
		void FindAndReplace(const MyString &pStringToReplace, const MyString &pReplacementString);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p].
		// POST: This object = strLength: n, arraySize: p, theString: char[p], however at theString[0], the first character is capitalized.
		MyString CapitalizeFirstCharacter();

		// PRE: This object = strLength: n, arraySize: p, theString: char[p].
		// POST: This object = strLength: n, arraySize: p, theString: char[p], however each value in theString is capitalized.
		MyString CapitalizeAllCharacters();

	// ========================================
	// predicate functions
	// ========================================

		// PRE: This object = strLength: n, arraySize: p, theString: char[p].
		// POST: RV = a true or false value depending on if this object has no characters other than the end of string character.
		bool IsEmpty();

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. CurrObj = strLength: a, arraySize: b, theString: char[b].
		// POST: RV = a true or false value depending on if this object is a substring of CurrObj.
		bool IsSubString(const MyString &CurrObj) const;

	// ========================================
	// operators
	// ========================================

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
		// POST: This object = strLength: a, arraySize: b, theString: char[b].
		MyString &operator=(const MyString &pString);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
		// POST: RV = a true or false value depending on if this object is less than pString.
		bool operator<(const MyString &pString);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
		// POST: RV = a true or false value depending on if this object is greater than pString.
		bool operator>(const MyString &pString);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
		// POST: RV = a true or false value depending on if this object is less than or equal to pString.
		bool operator<=(const MyString &pString);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
		// POST: RV = a true or false value depending on if this object is greater than or equal to pString.
		bool operator>=(const MyString &pString);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
		// POST: RV = a true or false value depending on if this object is equal to pString.
		bool operator==(const MyString &pString);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
		// POST: RV = a true or false value depending on if each of this object's characters in theString are equal to each character in pString.
		bool operator==(const char *pString);

		// PRE: This object = strLength: n, arraySize: p, theString: char[p]. pString = strLength: a, arraySize: b, theString: char[b].
		// POST: New object = strLength: ((n - 1) + a), arraySize: p + b, theString: char[p + b].
		MyString operator+(const MyString &pString) const;

	// ========================================
	// Input/Output Member Functions
	// ========================================

		// PRE: pString = strLength: a, arraySize: b, theString: char[b], stream is a defined readable stream.
		// POST: stream contains a list of characters, one after the other, followed by the end of string character.
		friend ostream &operator<<(ostream &os, const MyString &pString);

		// PRE: pString = strLength: a, arraySize: b, theString: char[b], stream is a defined writeable stream.
		// POST: stream contains a list of characters, one after the other, followed by the end of string character.
		friend ostream &operator<<(ostream &os, const MyString * pStringPtr);

		// PRE: pString = strLength: a, arraySize: b, theString: char[b], stream is a defined readable stream.
		// POST: stream contains strLength: a, arraySize: b, and theString: char[b].
		friend istream &get(istream &is, MyString &pString);

		// PRE: pString = strLength: a, arraySize: b, theString: char[b], stream is a defined writeable stream.
		// POST: stream contains strLength: a, arraySize: b, and theString: char[b].
		friend istream &operator>>(istream &is, MyString &pString);

};

#endif
