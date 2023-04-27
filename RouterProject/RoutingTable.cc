#include <algorithm>
#include <iostream>
#include <iomanip>

#include "RoutingTable.h"
#include "Address.h"
#include "Constants.h"


	// ========================================
	// constructors
	// ========================================

	RoutingTable::RoutingTable()
	{
		destinationList = new Address[MAXENTRIES];
		firstHopList = new Address[MAXENTRIES];
		distanceList = new int[MAXENTRIES];
		numEntries = 0;
	}

	RoutingTable::RoutingTable(const RoutingTable &pRoutingTable)
	{
		destinationList = new Address[MAXENTRIES];
		firstHopList = new Address[MAXENTRIES];
		distanceList = new int[MAXENTRIES];
		numEntries = pRoutingTable.numEntries;
		for(int i = 0; i < numEntries; i++)
		{
			destinationList[i] = pRoutingTable.destinationList[i];
			firstHopList[i] = pRoutingTable.firstHopList[i];
			distanceList[i] = pRoutingTable.distanceList[i];
		}
	}

	// ========================================
	// destructor
	// ========================================

	RoutingTable::~RoutingTable()
	{
		delete []destinationList;
		delete []firstHopList;
		delete []distanceList;
	}

	// ========================================
	// accessor functions
	// ========================================

	Address * RoutingTable::GetDestinationList() const
	{
		return(destinationList);
	}

	Address RoutingTable::GetNthDestination(int n) const
	{
		return(destinationList[n]);
	}

	int RoutingTable::GetIndexOfDestination(Address &pDestination) const
	{
		bool destinationFound = false;
		int index = 0;
		while(!destinationFound && index < numEntries)
		{
			if(GetNthDestination(index) == pDestination)
			{
				destinationFound = true;
			}
			else 
			{
				index++;
			}
		}
		return(index);
	}

	Address * RoutingTable::GetFirstHopList() const
	{
		return(firstHopList);
	}

	Address RoutingTable::GetNthFirstHop(int n) const
	{
		return(firstHopList[n]);
	}

	int * RoutingTable::GetDistanceList() const
	{
		return(distanceList);
	}

	int RoutingTable::GetNthDistance(int n) const
	{
		return(distanceList[n]);
	}

	int RoutingTable::GetNumEntries() const
	{
		return(numEntries);
	}

	string RoutingTable::Serialize() const
	{
		string serializedRoutingTable = (string)BEGINROUTINGTABLECHAR;

		for(int i = 0; i < numEntries; i++)
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
		}

		serializedRoutingTable += (string)ENDROUTINGTABLECHAR;

		return(serializedRoutingTable);
	}

	// ========================================
	// modifier functions
	// ========================================

	void RoutingTable::AddEntry(Address &pDestination, Address &pFirstHop, int pDistance)
	{
		destinationList[numEntries] = pDestination;
		firstHopList[numEntries] = pFirstHop;
		distanceList[numEntries] = pDistance;
		
		numEntries++;
	}

	void RoutingTable::RemoveEntryWithDestinationAddress(Address &pDestination)
	{
		if(this->HasDestination(pDestination))
		{
			int indexToRemove = GetIndexOfDestination(pDestination);
			for(int i = indexToRemove + 1; i < numEntries; i++)
			{
				destinationList[i - 1] = destinationList[i];
				firstHopList[i - 1] = firstHopList[i];
				distanceList[i - 1] = distanceList[i];
			}
			numEntries--;
		}
	}

	// ========================================
	// predicate functions
	// ========================================

	bool RoutingTable::IsEmpty()
	{
		return(numEntries == 0);
	}

	bool RoutingTable::HasDestination(Address pAddress)
	{
		bool isFound = false;
		int index = 0;

		while((!isFound) && (index < numEntries))
		{
			if(destinationList[index] == pAddress)
			{
				isFound = true;
			}
			index++;
		}
		return(isFound);
	}

	bool RoutingTable::HasFirstHop(Address pAddress)
	{
		bool isFound = false;
		int index = 0;

		while((!isFound) && (index < numEntries))
		{
			if(firstHopList[index] == pAddress)
			{
				isFound = true;
			}
			index++;
		}
		return(isFound);
	}

	bool RoutingTable::HasDistance(int pDistance)
	{
		bool isFound = false;
		int index = 0;

		while((!isFound) && (index < numEntries))
		{
			if(distanceList[index] == pDistance)
			{
				isFound = true;
			}
			index++;
		}
		return(isFound);
	}

	// ========================================
	// operators
	// ========================================

	RoutingTable &RoutingTable::operator=(const RoutingTable &pRoutingTable)
	{
		numEntries = pRoutingTable.numEntries;
		
		for(int i = 0; i < numEntries; i++)
		{
			destinationList[i] = pRoutingTable.destinationList[i];
			firstHopList[i] = pRoutingTable.firstHopList[i];
			distanceList[i] = pRoutingTable.distanceList[i];
		}

		return (*this);
	}

	// ========================================
	// Input/Output Member Functions
	// ========================================

	void RoutingTable::PrintRoutingTable()
	{
		cout << setw(10) << ROUTINGTABLECOLUMN1HEADER
			<< setw(20) << ROUTINGTABLECOLUMN2HEADER 
			<< setw(11) << ROUTINGTABLECOLUMN3HEADER << endl;
		for(int i = 0; i < numEntries; i++)
		{
			cout << setw(10);
			destinationList[i].PrintAddress();
			cout << setw(17);
			firstHopList[i].PrintAddress();
			cout << setw(10);
			cout << distanceList[i] << endl;
		}
	}

