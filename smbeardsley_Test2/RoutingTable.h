#ifndef INCLUDED_RoutingTable
#define INCLUDED_RoutingTable

#include <iostream>

#include "Address.h"
#include "Constants.h"

using namespace std;

class RoutingTable
{

private:
	Address * destinationList; // List of destination addresses for where 
						  // messages can be sent.
	Address * firstHopList; // List of where you need to send a message first
					    // in order to it to reach the destination address.
	int * distanceList; // List of the number of hops required to reach the 
					// destination address.
	int numEntries; // How many entries are in the routing table.

public:

	// ========================================
	// constructors
	// ========================================

	// Default Constructor

	// PRE: 
	// POST: This is a defined routing table object with no entries. 
	RoutingTable();

	// Copy Constructor

	// PRE: pRoutingTable is a defined routing table object.
	// POST: This is a defined routing table object that is a copy of pRoutingTable.
	RoutingTable(const RoutingTable &pRoutingTable);

	// ========================================
	// destructor
	// ========================================

	// PRE: 
	// POST: This object is destroyed. 
	~RoutingTable();

	// ========================================
	// accessor functions
	// ========================================

	// PRE: 
	// POST: RV is the contents of destinationList.
	Address * GetDestinationList() const;

	// PRE: n is a defined integer denoting the index at which the desired
	//      destination resides.
	// POST: RV is a defined address object containing the destination for the nth 
	//       entry.
	Address GetNthDestination(int n) const;

	// PRE: pDestination is a defined address denoting an existing address within
	//      this routing table object.
	// POST: RV is the index of the entry pDestination belongs to in this
	//       routing table object.
	int GetIndexOfDestination(Address &pDestination) const;

	// PRE:
	// POST: RV is the contents of firstHopList.
	Address * GetFirstHopList() const;

	// PRE: n is a defined integer denoting the index at which the desired 
	//      first hop address resides.
	// POST: RV is a defined address object containing the first hop for the nth
	//       entry.
	Address GetNthFirstHop(int n) const;

	// PRE: 
	// POST: RV is the contents of distanceList.
	int * GetDistanceList() const;

	// PRE: n is a defined integer denoting the index at which the desired distance
	//      resides.
	// POST: RV is a defined integer denoting the number of hops required to reach
	//       the destination in the nth entry.
	int GetNthDistance(int n) const;

	// PRE: 
	// POST: RV is a defined integer denoting the number of entries that are in this
	//       routing table object.
	int GetNumEntries() const;

	// PRE: 
	// POST: RV is a string version of the routing table. 
	string Serialize() const;

	// ========================================
	// modifier functions
	// ========================================

	// PRE: pDestination is a defined address object denoting the destination address
	//      for the entry to add. pFirstHop is a defined address object denoting the 
	//      address for the first hop needed to go to, to reach the destination.
	//      pDistance is a defined integer denoting the number of hops required to
	//      reach the destination.
	// POST: RV is this object being updated with a new entry containing pDestination
	//       pFirstHop and pDistance.
	void AddEntry(Address &pDestination, Address &pFirstHop, int pDistance);

	// PRE: pDestination is a defined address object denoting the destination address
	//      for the entry to remove.
	// POST: RV is this object being updated with 1 fewer entry, with the entry removed
	//       being the entry with pDestination as its destination address.
	void RemoveEntryWithDestinationAddress(Address &pDestination);

	// ========================================
	// predicate functions
	// ========================================

	// PRE: 
	// POST: RV is true if there are 0 numEntries and false otherwise.
	bool IsEmpty();

	// PRE: pAddress is a defined address object denoting the destination address to 
	//      check if the routing table has or not.
	// POST: RV is true if pAddress belongs to an entry in this object as a destination
	//       address, and false otherwise.
	bool HasDestination(Address pAddress);

	// PRE: pAddress is a defined address object denoting the first hop address to 
	//      check if the routing table has or not.
	// POST: RV is true if pAddress belongs to an entry in this object as a first hop
	//       address, and false otherwise.
	bool HasFirstHop(Address pAddress);

	// PRE: pDistance is a defined integer denoting the distance to check if the routing
	//      table has or not.
	// POST: RV is true if pDistance belongs to an entry in this object as a distnace, and
	//       false otherwise.
	bool HasDistance(int pDistance);

	// ========================================
	// operators
	// ========================================

	// PRE: pRoutingTable is a defined routing table object.
	// POST: RV is this object contains the contents of pRoutingTable.
	RoutingTable &operator=(const RoutingTable &pRoutingTable);

	// ========================================
	// Input/Output Member Functions
	// ========================================

	// PRE: 
	// POST: This object is formatted in a table, and output to standard output.
	void PrintRoutingTable();

};


#endif