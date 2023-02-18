#ifndef INCLUDED_IntArray
#define INCLUDED_IntArray

#include <iostream>

using namespace std;

// PRE:
// POST:
void copy(int *pArray, int pArrayLength, int *newArray);

class IntArray
{
	/*
	     Class Invariant (CI):
		The object is a defined integer array such that theArray contains
		integers added to it and potentially more space, content length 
		is the number of numbers added to it, and array size is the amount
		of spots that are allocated to theArray in memory.
	*/

private:
	int *theArray;
	int contentLength;
	int arraySize;

public:
	// ==============================
	// Constructors
	// ==============================

	// Default Constructor

	IntArray();

	// Copy Constructor

	IntArray(const IntArray &pIntArray);

	// ==============================
	// Destructor
	// ==============================

	~IntArray();

	// ==============================
	// Accessor Functions
	// ==============================

	int getNthIntInArray(int n) const;

	int getContentLength() const;

	int *getTheArray() const;

	// ==============================
	// Modifier Functions
	// ==============================

	void clear();

	void addInt(int pInt);

	void insertInt(int i, int pInt);

	void replaceInt(int i, int pInt);

	void removeLastInt();

	void removeFirstInt();

	void removeIthInt(int i);

	// ==============================
	// Predicate Functions
	// ==============================

	bool isEmpty();

};

#endif
