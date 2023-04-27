#include <iostream>

#include "Device.h"
#include "Constants.h"
#include "Address.h"
#include "Point.h"
#include "RoutingTable.h"

using namespace std;

	// ========================================
	// constructors
	// ========================================

	Device::Device()
	{

	}

	Device::Device(Address &pAddress, Point &pPoint, RoutingTable &pRoutingTable)
	{
		address = pAddress;
		location = pPoint;
		routingTable = pRoutingTable;
	}

	// ========================================
	// accessor functions
	// ========================================

	Address Device::GetAddress() const
	{
		return(address);
	}

	Point Device::GetLocation() const
	{
		return(location);
	}

	RoutingTable Device::GetRoutingTable() const
	{
		return(routingTable);
	}

	// ========================================
	// modifier functions
	// ========================================

	void Device::SetAddress(Address &pAddress)
	{
		address = pAddress;
	}

	void Device::SetLocation(Point &pPoint)
	{
		location = pPoint;
	}

	void Device::SetRoutingTable(RoutingTable &pRoutingTable)
	{
		routingTable = pRoutingTable;
	}

	// ========================================
	// predicate functions
	// ========================================



	// ========================================
	// operators
	// ========================================



	// ========================================
	// Input/Output Member Functions
	// ========================================
