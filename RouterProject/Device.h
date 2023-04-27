#ifndef INCLUDED_Device
#define INCLUDED_Device

#include <iostream>

#include "Constants.h"
#include "Address.h"
#include "Point.h"
#include "RoutingTable.h"

using namespace std;

class Device
{

private:
	Address address;
	Point location;
	RoutingTable routingTable;

public:

	// ========================================
	// constructors
	// ========================================

	Device();

	Device(Address &pAddress, Point &pPoint, RoutingTable &pRoutingTable);

	// ========================================
	// accessor functions
	// ========================================

	Address GetAddress() const;

	Point GetLocation() const;

	RoutingTable GetRoutingTable() const;

	// ========================================
	// modifier functions
	// ========================================

	void SetAddress(Address &pAddress);

	void SetLocation(Point &pPoint);

	void SetRoutingTable(RoutingTable &pRoutingTable);

	// ========================================
	// predicate functions
	// ========================================



	// ========================================
	// operators
	// ========================================



	// ========================================
	// Input/Output Member Functions
	// ========================================



};

#endif