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

	Point::Point()
	{
		xCoord = 0;
		yCoord = 0;
	}

	Point::Point(int pXCoord)
	{
		xCoord = pXCoord;
		yCoord = 0;
	}

	Point::Point(int pXCoord, int pYCoord)
	{
		xCoord = pXCoord;
		yCoord = pYCoord;
	}

	// ========================================
	// accessor functions
	// ========================================

	int Point::GetXCoord() const
	{
		return(xCoord);
	}

	int Point::GetYCoord() const
	{
		return(yCoord);
	}

	string Point::Serialize() const
	{
		char tempStorage[BASESTRINGSIZE];
		snprintf(tempStorage, BASESTRINGSIZE, "(%d,%d)", xCoord, yCoord);
		string serializedPoint = tempStorage;

		return(serializedPoint);
	}

	// ========================================
	// modifier functions
	// ========================================

	void Point::SetXCoord(int pXCoord)
	{
		xCoord = pXCoord;
	}

	void Point::SetYCoord(int pYCoord)
	{
		yCoord = pYCoord;
	}

	// ========================================
	// predicate functions
	// ========================================

	bool Point::isOrigin()
	{
		return(xCoord == 0 && yCoord == 0);
	}

	bool Point::xyEqual()
	{
		return(xCoord == yCoord);
	}

	// ========================================
	// operators
	// ========================================

	Point &Point::operator=(const Point &pPoint)
	{
		xCoord = pPoint.xCoord;
		yCoord = pPoint.yCoord;

		return (*this);
	}

	// ========================================
	// Input/Output Member Functions
	// ========================================

	void Point::PrintPoint()
	{
		cout << this->Serialize();
	}