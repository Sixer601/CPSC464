#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>

#include "Point.h"
#include "Constants.h"

using namespace std;

	// ========================================
	// constructors
	// ========================================

	// PRE: 
	// POST: this object is a point representing the origin.
	Point::Point()
	{
		xCoord = 0;
		yCoord = 0;
	}

	// PRE: 
	// POST: this object is a defined point object represented as (pXCoord, pYCoord).
	Point::Point(int pXCoord, int pYCoord)
	{
		xCoord = pXCoord;
		// ASSERT: xCoord is equal to pXCoord.
		yCoord = pYCoord;
		// ASSERT: yCoord is equal to pYCoord.
	}

	// ========================================
	// accessor functions
	// ========================================

	// PRE: 
	// POST: RV is the xCoord of this object.
	int Point::GetXCoord() const
	{
		return(xCoord);
	}

	// PRE: 
	// POST: RV is the yCoord of this object.
	int Point::GetYCoord() const
	{
		return(yCoord);
	}

	// PRE: 
	// POST: RV is a string version of this object.
	string Point::Serialize() const
	{
		char tempStorage[BASESTRINGSIZE]; // character array to contain serialized point.
		// ASSERT: tempStorage is a defaultly constructed character array of size BASESTRINGSIZE.
		snprintf(tempStorage, BASESTRINGSIZE, "(%d,%d)", xCoord, yCoord);
		string serializedPoint = tempStorage; // string object to contain serialized point.
		// ASSERT: serializedPoint contains the contents of tempStorage.
		return(serializedPoint);
	}

	// ========================================
	// modifier functions
	// ========================================

	// PRE: pXCoord is a defined integer object representing the x coordinate to update this object with.
	// POST: RV this object's xCoord is equal to pXCoord.
	void Point::SetXCoord(int pXCoord)
	{
		xCoord = pXCoord;
		// ASSERT: xCoord is equal to pXCoord.
	}

	// PRE: pYCoord is a defined integer object representing the y coordinate to update this object with.
	// POST: RV this object's yCoord is equal to pYCoord.
	void Point::SetYCoord(int pYCoord)
	{
		yCoord = pYCoord;
		// ASSERT: yCoord is equal to pYCoord.
	}

	// ========================================
	// predicate functions
	// ========================================

	// PRE: 
	// POST: RV is true if this point represents the origin, and false otherwise.
	bool Point::isOrigin()
	{
		return(xCoord == 0 && yCoord == 0);
	}

	// PRE: 
	// POST: RV is true if this point's xCoord and yCoord are equivalent.
	bool Point::xyEqual()
	{
		return(xCoord == yCoord);
	}

	// ========================================
	// operators
	// ========================================

	// PRE: pPoint is a defined point object.
	// POST: RV is this object now represents the same place on a coordinate plane as pPoint.
	Point &Point::operator=(const Point &pPoint)
	{
		xCoord = pPoint.xCoord;
		// ASSERT: xCoord is equivalent to pPoint's xCoord.
		yCoord = pPoint.yCoord;
		// ASSERT: yCoord is equivalent to pPoint's yCoord.
		return (*this);
	}

	// ========================================
	// Input/Output Member Functions
	// ========================================

	// PRE: 
	// POST: standard output contains a serialized version of this point.
	void Point::PrintPoint()
	{
		cout << this->Serialize();
	}