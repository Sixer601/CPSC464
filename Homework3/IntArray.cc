#include "IntArray.h"
#include <iostream>

using namespace std;

// PRE:
// POST:
void copy(int *pArray, int pArrayLength, int *newArray)
{
	for (int i = 0; i < pArrayLength; i++)
	{
		// ASSERT:
		newArray[i] = pArray[i];
		// ASSERT:
	}
}

// ==============================
// Constructors
// ==============================

// PRE:
// POST:
IntArray::IntArray()
{
	contentLength = 0;
	arraySize = 1;
	theArray = new int[arraySize];
}

// PRE:
// POST:
IntArray::IntArray(const IntArray &pIntArray)
{
	contentLength = pIntArray.contentLength;
	arraySize = pIntArray.arraySize;
	theArray = new int[arraySize];
	copy(pIntArray.theArray, pIntArray.contentLength, theArray);
}

// ==============================
// Destructor
// ==============================

// PRE:
// POST:
IntArray::~IntArray()
{
	delete[] theArray;
}

// ==============================
// Accessor Functions
// ==============================

// PRE: 
// POST: 
int IntArray::getNthIntInArray(int n) const {
	return (theArray[n]);
}

// PRE: 
// POST: 
int IntArray::getContentLength() const {
	return(contentLength);
}

// PRE: 
// POST: 
int * IntArray::getTheArray() const {
	return(theArray);
}

// ==============================
// Modifier Functions
// ==============================

// PRE: 
// POST: 
void IntArray::clear() {
	contentLength = 1;
}

// PRE: 
// POST: 
void IntArray::addInt(int pInt) {
	if (contentLength == arraySize) {
		int *tempArray = new int[arraySize * 2];

		for (int i = 0; i < contentLength; i++) {
			tempArray[i] = theArray[i];
		}
		tempArray[contentLength] = pInt;
		delete[] theArray;
		theArray = tempArray;
		arraySize *= 2;
	}
	else {
		theArray[contentLength] = pInt;
	}
	contentLength++;
}

// PRE: 
// POST: 
void IntArray::insertInt(int i, int pInt) {
	if (contentLength == arraySize) {
		int *tempArray = new int[arraySize * 2];

		for (int j = 0; j < i; j++) {
			tempArray[i] = theArray[i];
		}
		tempArray[i] = pInt;
		for (int k = contentLength + 1; k > i; i--) {
			tempArray[k] = theArray[k - 1];
		}
		delete[] theArray;
		theArray = tempArray;
		arraySize *= 2;
	}
	else {
		for (int l = contentLength + 1; l > i; l--) {
			theArray[l] = theArray[l - 1];
		}
		theArray[i] = pInt;
	}
	contentLength++;
}

// PRE: 
// POST: 
void IntArray::replaceInt(int i, int pInt) {
	theArray[i] = pInt;
}

// PRE: 
// POST: 
void IntArray::removeLastInt() {
	contentLength--;
}

// PRE: 
// POST: 
void IntArray::removeFirstInt() {
	for (int i = 0; i < contentLength; i++) {
		theArray[i] = theArray[i + 1];
	}
}

// PRE: 
// POST: 
void IntArray::removeIthInt(int i) {
	for (int j = i; j < contentLength; j++) {
		theArray[j] = theArray[j + 1];
	}
	contentLength--;
}

// ==============================
// Predicate Functions
// ==============================

// PRE: 
// POST: 
bool IntArray::isEmpty() {
	return(contentLength == 0);
}































