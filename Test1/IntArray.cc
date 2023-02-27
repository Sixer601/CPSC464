#include "IntArray.h"
#include <iostream>

using namespace std;

void copy(int *pArray, int pArrayLength, int *newArray)
{
	for (int i = 0; i < pArrayLength; i++)
	{
		newArray[i] = pArray[i];
	}
}

// ==============================
// Constructors
// ==============================

IntArray::IntArray()
{
	contentLength = 0;
	arraySize = 1;
	theArray = new int[arraySize];
}

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

IntArray::~IntArray()
{
	delete[] theArray;
}

// ==============================
// Accessor Functions
// ==============================

int IntArray::getNthIntInArray(int n) const {
	return (theArray[n]);
}

int IntArray::getContentLength() const {
	return(contentLength);
}

int * IntArray::getTheArray() const {
	return(theArray);
}

// ==============================
// Modifier Functions
// ==============================

void IntArray::clear() {
	contentLength = 1;
}

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

void IntArray::replaceInt(int i, int pInt) {
	theArray[i] = pInt;
}

void IntArray::removeLastInt() {
	contentLength--;
}

void IntArray::removeFirstInt() {
	for (int i = 0; i < contentLength; i++) {
		theArray[i] = theArray[i + 1];
	}
}

void IntArray::removeIthInt(int i) {
	for (int j = i; j < contentLength; j++) {
		theArray[j] = theArray[j + 1];
	}
	contentLength--;
}

// ==============================
// Predicate Functions
// ==============================

bool IntArray::isEmpty() {
	return(contentLength == 0);
}































