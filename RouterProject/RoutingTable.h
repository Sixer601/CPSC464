#ifndef INCLUDED_RoutingTable
#define INCLUDED_RoutingTable

#include <iostream>

#include "Address.h"
#include "Constants.h"

using namespace std;

class RoutingTable
{

private:
	Address * destinationList;
	Address * firstHopList;
	int * distanceList;

	int numEntries;

public:

	// ========================================
	// constructors
	// ========================================

	RoutingTable();

	RoutingTable(const RoutingTable &pRoutingTable);

	// ========================================
	// destructor
	// ========================================

	~RoutingTable();

	// ========================================
	// accessor functions
	// ========================================

	Address * GetDestinationList() const;

	Address GetNthDestination(int n) const;

	int GetIndexOfDestination(Address &pDestination) const;

	Address * GetFirstHopList() const;

	Address GetNthFirstHop(int n) const;

	int * GetDistanceList() const;

	int GetNthDistance(int n) const;

	int GetNumEntries() const;

	string Serialize() const;

	// ========================================
	// modifier functions
	// ========================================

	void AddEntry(Address &pDestination, Address &pFirstHop, int pDistance);

	void RemoveEntryWithDestinationAddress(Address &pDestination);

	// ========================================
	// predicate functions
	// ========================================

	bool IsEmpty();

	bool HasDestination(Address pAddress);

	bool HasFirstHop(Address pAddress);

	bool HasDistance(int pDistance);

	// ========================================
	// operators
	// ========================================

	RoutingTable &operator=(const RoutingTable &pRoutingTable);

	// ========================================
	// Input/Output Member Functions
	// ========================================

	void PrintRoutingTable();

};


#endif