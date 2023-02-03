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

	// PRE:
	// POST:
	IntArray();

	// Copy Constructor

	// PRE:
	// POST:
	IntArray(const IntArray &pIntArray);

	// ==============================
	// Destructor
	// ==============================

	// PRE:
	// POST:
	~IntArray();

	// ==============================
	// Accessor Functions
	// ==============================

	// PRE:
	// POST:
	int getNthIntInArray(int n) const;

	// PRE:
	// POST:
	int getContentLength() const;

	// PRE:
	// POST:
	int *getTheArray() const;

	// ==============================
	// Modifier Functions
	// ==============================

	// PRE:
	// POST:
	void clear();

	// PRE:
	// POST:
	void addInt(int pInt);

	// PRE:
	// POST:
	void insertInt(int i, int pInt);

	// PRE:
	// POST:
	void replaceInt(int i, int pInt);

	// PRE:
	// POST:
	void removeLastInt();

	// PRE:
	// POST:
	void removeFirstInt();

	// PRE:
	// POST:
	void removeIthInt(int i);

	// ==============================
	// Predicate Functions
	// ==============================

	// PRE:
	// POST:
	bool isEmpty();

};

#endif
