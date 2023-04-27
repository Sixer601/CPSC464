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

	ConnectionsList::ConnectionsList()
	{
		addressList = new Address [MYMAXCONNECTIONS];
		connectionsList = new Socket * [MYMAXCONNECTIONS];
		numConnections = 0;
	}

	// ========================================
	// destructor
	// ========================================

	ConnectionsList::~ConnectionsList()
	{
		delete[] addressList;
		delete[] connectionsList;
	}
	
	// ========================================
	// accessor functions
	// ========================================

	Address * ConnectionsList::GetAddressList() const
	{
		return(addressList);
	}

	Address ConnectionsList::GetNthAddress(int n) const
	{
		return(addressList[n]);
	}

	Socket ** ConnectionsList::GetConnectionsList() const
	{
		return(connectionsList);
	}

	Socket * ConnectionsList::GetNthConnection(int n) 
	{
		return(connectionsList[n]);
	}

	Socket * ConnectionsList::GetConnectionWithPort(int pPort) const
	{
		bool isFound = false;
		int index = 0;
		while(!isFound && index <= numConnections)
		{
			if(addressList[index].GetPort() == pPort)
			{
				isFound = true;
			}
			else 
			{
				index++;
			}
		}
		return(connectionsList[index]);
	}

	int ConnectionsList::GetNumConnections() const
	{
		return(numConnections);
	}

	// ========================================
	// modifier functions
	// ========================================

	void ConnectionsList::AddConnection(Socket * connectionSocket, Address connectionAddress)
	{
		connectionsList[numConnections] = connectionSocket;
		addressList[numConnections] = connectionAddress;
		numConnections++;
	}

	// ========================================
	// predicate functions
	// ========================================

	bool ConnectionsList::HasConnections()
	{
		return(numConnections > 0);
	}

	bool ConnectionsList::HasConnectionWithAddress(Address pAddress)
	{
		bool isFound = false;
		int index = 0;
		while(!isFound && index <= numConnections)
		{
			if(addressList[index] == pAddress)
			{
				isFound = true;
			}
			else 
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

	void ConnectionsList::PrintConnectionsList()
	{
		cout << "Connections List: " << endl;
		for(int i = 0; i < numConnections; i++)
		{
			cout << i << " " << hex << &connectionsList[i] << dec << " " << addressList[i].Serialize() << endl;
		}
	}