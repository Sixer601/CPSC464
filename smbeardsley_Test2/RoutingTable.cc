#include <algorithm>
#include <iostream>
#include <iomanip>

#include "RoutingTable.h"
#include "Address.h"
#include "Constants.h"


	// ========================================
	// constructors
	// ========================================

	// PRE: 
	// POST: This is a defined routing table object with no entries. 
	RoutingTable::RoutingTable()
	{
		destinationList = new Address[MAXENTRIES];
		// ASSERT: destinationList is an array of Addresses, of size MAXENTRIES.
		firstHopList = new Address[MAXENTRIES];
		// ASSERT: firstHopList is an array of Addresses, of size MAXENTRIES.
		distanceList = new int[MAXENTRIES];
		// ASSERT: distanceList is an array of integers, of size MAXENTRIES.
		numEntries = 0;
		// ASSERT: numEntries is 0, as no information is provided.
	}

	// PRE: pRoutingTable is a defined routing table object.
	// POST: This is a defined routing table object that is a copy of pRoutingTable.
	RoutingTable::RoutingTable(const RoutingTable &pRoutingTable)
	{
		destinationList = new Address[MAXENTRIES];
		// ASSERT: destinationList is an array of Addresses, of size MAXENTRIES.
		firstHopList = new Address[MAXENTRIES];
		// ASSERT: firstHopList is an array of Addresses, of size MAXENTRIES.
		distanceList = new int[MAXENTRIES];
		// ASSERT: distanceList is an array of integers, of size MAXENTRIES.
		numEntries = pRoutingTable.numEntries;
		// ASSERT: numEntries is equal to the number of entries in pRoutingTable.
		for(int i = 0; i < numEntries; i++)
		// ASSERT: i is less than numEntries.
		{
			destinationList[i] = pRoutingTable.destinationList[i];
			// ASSERT: the ith index of destinationList is now the same as 
			//         the ith index of pRoutingTable's destinationList.
			firstHopList[i] = pRoutingTable.firstHopList[i];
			// ASSERT: the ith index of firstHopList is now the same as
			//         the ith index of pRoutingTable's firstHopList.
			distanceList[i] = pRoutingTable.distanceList[i];
			// ASSERT: the ith index of distanceList is now the same as
			//         the ith index of pRoutingTable's distanceList.
		}
	}

	// ========================================
	// destructor
	// ========================================

	// PRE: 
	// POST: This object is destroyed. 
	RoutingTable::~RoutingTable()
	{
		delete []destinationList;
		delete []firstHopList;
		delete []distanceList;
	}

	// ========================================
	// accessor functions
	// ========================================

	// PRE: 
	// POST: RV is the contents of destinationList.
	Address * RoutingTable::GetDestinationList() const
	{
		return(destinationList);
	}

	// PRE: n is a defined integer denoting the index at which the desired
	//      destination resides.
	// POST: RV is a defined address object containing the destination for the nth 
	//       entry.
	Address RoutingTable::GetNthDestination(int n) const
	{
		return(destinationList[n]);
	}

	// PRE: pDestination is a defined address denoting an existing address within
	//      this routing table object.
	// POST: RV is the index of the entry pDestination belongs to in this
	//       routing table object.
	int RoutingTable::GetIndexOfDestination(Address &pDestination) const
	{
		bool destinationFound = false; // bool representing if pDestination has
								 // been found in this object. 
		// ASSERT: destinationFound begins as false, as we are unsure where in
		//         this object it resides.
		int index = 0; // integer to denote which entry the method is currently
		               // looking at.
		// ASSERT: index begins at 0.
		while(!destinationFound && index < numEntries)
		// ASSERT: both the destination has not been found and index is within
		//         the bounds of the routing table.
		{
			if(GetNthDestination(index) == pDestination)
			// ASSERT: the index'th destination in this object is equivalent to
			//         pDestination.
			{
				destinationFound = true;
				// ASSERT: this method has found where in this object
				//         pDestination resides.
			}
			else 
			{
				index++;
			}
		}
		return(index);
	}

	// PRE:
	// POST: RV is the contents of firstHopList.
	Address * RoutingTable::GetFirstHopList() const
	{
		return(firstHopList);
	}

	// PRE: n is a defined integer denoting the index at which the desired 
	//      first hop address resides.
	// POST: RV is a defined address object containing the first hop for the nth
	//       entry.
	Address RoutingTable::GetNthFirstHop(int n) const
	{
		return(firstHopList[n]);
	}

	// PRE: 
	// POST: RV is the contents of distanceList.
	int * RoutingTable::GetDistanceList() const
	{
		return(distanceList);
	}

	// PRE: n is a defined integer denoting the index at which the desired distance
	//      resides.
	// POST: RV is a defined integer denoting the number of hops required to reach
	//       the destination in the nth entry.
	int RoutingTable::GetNthDistance(int n) const
	{
		return(distanceList[n]);
	}

	// PRE: 
	// POST: RV is a defined integer denoting the number of entries that are in this
	//       routing table object.
	int RoutingTable::GetNumEntries() const
	{
		return(numEntries);
	}

	// PRE: 
	// POST: RV is a string version of the routing table. 
	string RoutingTable::Serialize() const
	{
		string serializedRoutingTable = (string)BEGINROUTINGTABLECHAR; 
		// string to hold the contents of this object as a string. 
		// ASSERT: serializedRoutingTable begins containing the begin routing table 
		//         character.
		for(int i = 0; i < numEntries; i++)
		// ASSERT: i is less than numEntries.
		{
			serializedRoutingTable += (string)BEGINENTRYCHARACTER 
									  + BEGINDESTINATIONCHARACTER
									  + destinationList[i].Serialize()
									  + ENDDESTINATIONCHARACTER
									  + BEGINFIRSTHOPCHARACTER
									  + firstHopList[i].Serialize()
									  + ENDFIRSTHOPCHARACTER
									  + BEGINDISTANCECHARACTER
									  + to_string(distanceList[i])
									  + ENDDISTANCECHARACTER
									  + ENDENTRYCHARACTER;
			// ASSERT: serializedRoutingTable contains the ith entry in string form.
		}

		serializedRoutingTable += (string)ENDROUTINGTABLECHAR;
		// ASSERT: serializedRoutingTable ends with the end routing table character.

		return(serializedRoutingTable);
	}

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
	void RoutingTable::AddEntry(Address &pDestination, Address &pFirstHop, int pDistance)
	{
		destinationList[numEntries] = pDestination;
		// ASSERT: the numEntries'th item in destinationList is now pDestination.
		firstHopList[numEntries] = pFirstHop;
		// ASSERT: the numEntries'th item in firstHopList is now pFirstHop.
		distanceList[numEntries] = pDistance;
		// ASSERT: the numEntries'th item in distanceList is now pDistance.
		
		numEntries++;
		// ASSERT: there is now one more entry in this object.
	}

	// PRE: pDestination is a defined address object denoting the destination address
	//      for the entry to remove.
	// POST: RV is this object being updated with 1 fewer entry, with the entry removed
	//       being the entry with pDestination as its destination address.
	void RoutingTable::RemoveEntryWithDestinationAddress(Address &pDestination)
	{
		if(this->HasDestination(pDestination))
		// ASSERT: this routing table has pDestination as part of an entry.
		{
			int indexToRemove = GetIndexOfDestination(pDestination); 
			// indexToRemove is the index of the entry that pDestination resides in.
			// ASSERT: indexToRemove is equal to the result of the method
			//         GetIndexOfDestination being passed pDestination as its parameter.
			for(int i = indexToRemove + 1; i < numEntries; i++)
			// ASSERT: i is less than the number of entries in this routing table.
			{
				destinationList[i - 1] = destinationList[i];
				// ASSERT: the i-1'th element of destinationList is now the element that
				//         followed it.
				firstHopList[i - 1] = firstHopList[i];
				// ASSERT: the i-1'th element of firstHopList is now the element that
				//         followed it.
				distanceList[i - 1] = distanceList[i];
				// ASSERT: the i-1'th element of distanceList is now the element that
				//         followed it.
			}
			numEntries--;
		}
	}

	// ========================================
	// predicate functions
	// ========================================

	// PRE: 
	// POST: RV is true if there are 0 numEntries and false otherwise.
	bool RoutingTable::IsEmpty()
	{
		return(numEntries == 0);
	}

	// PRE: pAddress is a defined address object denoting the destination address to 
	//      check if the routing table has or not.
	// POST: RV is true if pAddress belongs to an entry in this object as a destination
	//       address, and false otherwise.
	bool RoutingTable::HasDestination(Address pAddress)
	{
		bool isFound = false; // boolean to represent if we have found an entry that
						  // contains pAddress.
		// ASSERT: isFound begins as false.
		int index = 0; // int to represent which entry we are currently checking for
					// if pAddress belongs to it. 
		// ASSERT: index begins at 0.
		while((!isFound) && (index < numEntries))
		// ASSERT: Both the method has not found the destination in this object and 
		//         index is less than the number of entries in this object.
		{
			if(destinationList[index] == pAddress)
			// ASSERT: the index'th item in destinationList is equivalent to pAddress.
			{
				isFound = true;
				// ASSERT: the destination exists within this object.
			}
			index++;
		}
		return(isFound);
	}

	// PRE: pAddress is a defined address object denoting the first hop address to 
	//      check if the routing table has or not.
	// POST: RV is true if pAddress belongs to an entry in this object as a first hop
	//       address, and false otherwise.
	bool RoutingTable::HasFirstHop(Address pAddress)
	{
		bool isFound = false; // boolean to represent if we have found an entry that
						  // contains pAddress.
		// ASSERT: isFound begins as false.
		int index = 0; // int to represent which entry we are currently checking for
					// if pAddress belongs to it. 
		// ASSERT: index begins at 0.
		while((!isFound) && (index < numEntries))
		// ASSERT: Both the method has not found the first hop in this object and 
		//         index is less than the number of entries in this object.
		{
			if(firstHopList[index] == pAddress)
			// ASSERT: the index'th item in firstHopList is equivalent to pAddress.
			{
				isFound = true;
				// ASSERT: the first hop exists within this object.
			}
			index++;
		}
		return(isFound);
	}

	// PRE: pDistance is a defined integer denoting the distance to check if the routing
	//      table has or not.
	// POST: RV is true if pDistance belongs to an entry in this object as a distnace, and
	//       false otherwise.
	bool RoutingTable::HasDistance(int pDistance)
	{
		bool isFound = false; // boolean to represent if we have found an entry that
						  // contains pDistance.
		// ASSERT: isFound begins as false.
		int index = 0; // int to represent which entry we are currently checking for
					// if pDistance belongs to it. 
		// ASSERT: index begins at 0.
		while((!isFound) && (index < numEntries))
		// ASSERT: Both the method has not found the first hop in this object and 
		//         index is less than the number of entries in this object.
		{
			if(distanceList[index] == pDistance)
			// ASSERT: the index'th item in distanceList is equivalent to pDistance.
			{
				isFound = true;
				// ASSERT: the distance exists within this object.
			}
			index++;
		}
		return(isFound);
	}

	// ========================================
	// operators
	// ========================================

	// PRE: pRoutingTable is a defined routing table object.
	// POST: RV is this object contains the contents of pRoutingTable.
	RoutingTable &RoutingTable::operator=(const RoutingTable &pRoutingTable)
	{
		numEntries = pRoutingTable.numEntries;
		// ASSERT: numEntries is equal to the number of entries in pRoutingTable.
		for(int i = 0; i < numEntries; i++)
		// ASSERT: i is less than the number of entries this object will have.
		{
			destinationList[i] = pRoutingTable.destinationList[i];
			// ASSERT: the i'th item in destinationList is now equivalent to
			//         the i'th item in pRoutingTable's destinationList.
			firstHopList[i] = pRoutingTable.firstHopList[i];
			// ASSERT: the i'th item in firstHopList is now equivalent to
			//         the i'th item in pRoutingTable's firstHopList.
			distanceList[i] = pRoutingTable.distanceList[i];
			// ASSERT: the i'th item in distanceList is now equivalent to
			//         the i'th item in pRoutingTable's distanceList.
		}

		return (*this);
	}

	// ========================================
	// Input/Output Member Functions
	// ========================================

	// PRE: 
	// POST: This object is formatted in a table, and output to standard output.
	void RoutingTable::PrintRoutingTable()
	{
		cout << setw(10) << ROUTINGTABLECOLUMN1HEADER
			<< setw(20) << ROUTINGTABLECOLUMN2HEADER 
			<< setw(11) << ROUTINGTABLECOLUMN3HEADER << endl;
		// ASSERT: The headers are output to standard output.
		for(int i = 0; i < numEntries; i++)
		// ASSERT: i is less than the number of entries in this object.
		{
			cout << setw(10);
			destinationList[i].PrintAddress();
			cout << setw(17);
			firstHopList[i].PrintAddress();
			cout << setw(10);
			cout << distanceList[i] << endl;
			// ASSERT: the i'th row in this object is output to standard output.
		}
	}

