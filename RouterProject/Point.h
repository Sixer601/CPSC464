#ifndef INCLUDED_Point
#define INCLUDED_Point

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

class Point
{

private:
	int xCoord; // The x coordinate of the point.
	int yCoord; // The y coordinate of the point.

public:

	// ========================================
	// constructors
	// ========================================

	// Default Constructor

	// PRE: 
	// POST: this object is a point representing the origin.
	Point();

	// PRE: 
	// POST: this object is a defined point object represented as (pXCoord, pYCoord).
	Point(int pXCoord, int pYCoord);

	// ========================================
	// accessor functions
	// ========================================

	// PRE: 
	// POST: RV is the xCoord of this object.
	int GetXCoord() const;

	// PRE: 
	// POST: RV is the yCoord of this object.
	int GetYCoord() const;

	// PRE: 
	// POST: RV is a string version of this object.
	string Serialize() const;

	// ========================================
	// modifier functions
	// ========================================

	// PRE: pXCoord is a defined integer object representing the x coordinate to update this object with.
	// POST: RV this object's xCoord is equal to pXCoord.
	void SetXCoord(int pXCoord);

	// PRE: pYCoord is a defined integer object representing the y coordinate to update this object with.
	// POST: RV this object's yCoord is equal to pYCoord.
	void SetYCoord(int pYCoord);

	// ========================================
	// predicate functions
	// ========================================

	// PRE: 
	// POST: RV is true if this point represents the origin, and false otherwise.
	bool isOrigin();

	// PRE: 
	// POST: RV is true if this point's xCoord and yCoord are equivalent.
	bool xyEqual();

	// ========================================
	// operators
	// ========================================

	// PRE: pPoint is a defined point object.
	// POST: RV is this object now represents the same place on a coordinate plane as pPoint.
	Point &operator=(const Point &pPoint);

	// ========================================
	// Input/Output Member Functions
	// ========================================

	// PRE: 
	// POST: standard output contains a serialized version of this point.
	void PrintPoint();

};

#endif