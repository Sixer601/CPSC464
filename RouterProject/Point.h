#ifndef INCLUDED_Point
#define INCLUDED_Point

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

class Point
{

private:
	int xCoord;
	int yCoord;

public:

	// ========================================
	// constructors
	// ========================================

	Point();

	Point(int pXCoord);

	Point(int pXCoord, int pYCoord);

	// ========================================
	// accessor functions
	// ========================================

	int GetXCoord() const;

	int GetYCoord() const;

	string Serialize() const;

	// ========================================
	// modifier functions
	// ========================================

	void SetXCoord(int pXCoord);

	void SetYCoord(int pYCoord);

	// ========================================
	// predicate functions
	// ========================================

	bool isOrigin();

	bool xyEqual();

	// ========================================
	// operators
	// ========================================

	Point &operator=(const Point &pPoint);

	// ========================================
	// Input/Output Member Functions
	// ========================================

	void PrintPoint();

};

#endif