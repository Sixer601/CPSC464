#include <iostream>

#include "ConnectionsList.h"

#include "Address.h"
#include "Socket.h"
#include "SocketException.h"
#include "Constants.h"

using namespace std;

	// ========================================
	// constructors
	// ========================================

	// Default Constructor

	// PRE: 
	// POST: this object is an "empty" connections list.
	ConnectionsList::ConnectionsList()
	{
		addressList = new Address [MYMAXCONNECTIONS];
		connectionsList = new Socket * [MYMAXCONNECTIONS];
		numConnections = 0;
	}

	// ========================================
	// destructor
	// ========================================

	// PRE: 
	// POST: this object is destroyed.
	ConnectionsList::~ConnectionsList()
	{
		delete[] addressList;
		delete[] connectionsList;
	}
	
	// ========================================
	// accessor functions
	// ========================================

	// PRE: 
	// POST: RV is the addressList member of this object.
	Address * ConnectionsList::GetAddressList() const
	{
		return(addressList);
	}

	// PRE: 
	// POST: RV is the nth index of the addressList member of this object.
	Address ConnectionsList::GetNthAddress(int n) const
	{
		return(addressList[n]);
	}

	// PRE: 
	// POST: RV is the connectionsList member of this object.
	Socket ** ConnectionsList::GetConnectionsList() const
	{
		return(connectionsList);
	}

	// PRE: 
	// POST: RV is the nth index of the connectionsList member of this object.
	Socket * ConnectionsList::GetNthConnection(int n) 
	{
		return(connectionsList[n]);
	}

	// PRE: pPort is a defined integer representing the port of a particular connection.
	// POST: RV is the socket pointer object to the connection with pPort.
	Socket * ConnectionsList::GetConnectionWithPort(int pPort) const
	{
		bool isFound = false; // true if pPort has been found in this object.
		// ASSERT: to begin with, we assume isFound is false.
		int index = 0; // entry currently being checked for pPort.
		// ASSERT: index begins at 0, to ensure we begin by checking the first entry.
		while(!isFound && index < numConnections)
		// ASSERT: pPort has not been found and index is less than the number of connections.
		{
			if(addressList[index].GetPort() == pPort)
			// ASSERT: the index'th entry in this object's address's port is equivalent to pPort.			{
				isFound = true;
				// ASSERT: pPort has been found.
			}
			else 
			// ASSERT: the index'th entry in this object's address's port is not equivalent to pPort.
			{
				index++;
			}
		}
		return(connectionsList[index]);
	}

	// PRE: pAddress is a defined Address object.
	// POST: RV is a socket pointer object represented by pAddress.
	Socket * ConnectionsList::GetConnectionWithAddress(Address pAddress) const
	{
		bool isFound = false; // true if pAddress has been found in this object.
		// ASSERT: to begin with, we assume isFound is false.
		int index = 0; // entry currently being checked for pAddress.
		// ASSERT: index begins at 0, to ensure we begin by checking the first entry.
		while(!isFound && index < numConnections)
		// ASSERT: pAddress has not been found and index is less than the number of connections.
		{
			if(addressList[index] == pAddress)
			// ASSERT: the index'th entry in addressList is equivalent to pAddress.
			{
				isFound = true;
				// ASSERT: pAddress has been found in the index'th entry.
			}
			else 
			// ASSERT: the index'th entry in addressList is not equivalent to pAddress.
			{
				index++;
			}
		}
		return(connectionsList[index]);
	}

	// PRE: 
	// POST: RV is the number of connections stored within this object.
	int ConnectionsList::GetNumConnections() const
	{
		return(numConnections);
	}

	// ========================================
	// modifier functions
	// ========================================

	// PRE: connectionSocket is a defined socket pointer object that points to a socket that has been connected to.
	//      connectionAddress is a defined address object that contains the address information of the device that has connected.
	// POST: this object reflects the connection made.
	void ConnectionsList::AddConnection(Socket * connectionSocket, Address connectionAddress)
	{
		connectionsList[numConnections] = connectionSocket;
		// ASSERT: the numConnections'th index of connectionList now holds connectionSocket.
		addressList[numConnections] = connectionAddress;
		// ASSERT: the numConnections'th index of addressList now holds connectionAddress.
		numConnections++;
		// ASSERT: 1 more connection has been made.
	}

	// ========================================
	// predicate functions
	// ========================================

	// PRE: 
	// POST: RV is true if there are more than 0 connections stored in this object 
	bool ConnectionsList::HasConnections()
	{
		return(numConnections > 0);
	}

	// PRE: pAddress is a defined address object.
	// POST: RV is true if pAddress is an address stored in this object, and false otherwise.
	bool ConnectionsList::HasConnectionWithAddress(Address pAddress)
	{
		bool isFound = false; // true if pAddress has been found in this object.
		// ASSERT: to begin with, we assume isFound is false.
		int index = 0; // entry currently being checked for pAddress.
		// ASSERT: index begins at 0, to ensure we begin by checking the first entry.
		while(!isFound && index < numConnections)
		// ASSERT: pAddress has not been found and index is less than the number of connections.
		{
			if(addressList[index] == pAddress)
			// ASSERT: the index'th entry in addressList is equivalent to pAddress.
			{
				isFound = true;
				// ASSERT: pAddress has been found in the index'th entry.
			}
			else 
			// ASSERT: the index'th entry in addressList is not equivalent to pAddress.
			{
				index++;
			}
		}
		return(isFound);
	}

	// ========================================
	// operators
	// ========================================



	// ========================================
	// Input/Output Member Functions
	// ========================================

	// PRE: 
	// POST: standard output contains this object's contents.
	void ConnectionsList::PrintConnectionsList()
	{
		cout << "Connections List: " << endl;
		for(int i = 0; i < numConnections; i++)
		// ASSERT: i is less than the number of connections this object holds.
		{
			cout << i << " " << hex << &connectionsList[i] << dec << " " << addressList[i].Serialize() << endl;
		}
	}
