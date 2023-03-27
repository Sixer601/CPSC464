#include "ConnectionList.h"
#include "Constants.h"

using namespace std;

// PRE: 
// POST: 
ConnectionList::ConnectionList()
{
	numConnections = 0;
	arraySize = MAXSOCKS;
	connectionArray = new Socket[arraySize];
	activeStatusArray = new bool[arraySize];

	for (int i = 0; i < arraySize; i++) 
	{
		connectionArray[i].set_non_blocking(true);
		activeStatusArray[i] = false;
	}
}

// PRE: 
// POST: 
ConnectionList::ConnectionList(const ConnectionList &pConnectionList)
{
	numConnections = pConnectionList.numConnections;
	arraySize = pConnectionList.arraySize;
	connectionArray = new Socket[arraySize];
	activeStatusArray = new bool[arraySize];
}

// PRE: 
// POST: 
ConnectionList::~ConnectionList()
{
	delete[] connectionArray;
	delete[] activeStatusArray;
}

// PRE: 
// POST: 
Socket ConnectionList::GetIthSocketInConnectionList(int i) const
{
	return (connectionArray[i]);
}

// PRE: 
// POST: 
bool ConnectionList::GetIthActiveStatusInConnectionList(int i) const
{
	return (activeStatusArray[i]);
}

// PRE: 
// POST: 
int ConnectionList::GetNumConnections() const
{
	return (numConnections);
}

// PRE: 
// POST: 
int ConnectionList::GetCapacity() const
{
	return (arraySize);
}



// PRE: 
// POST: 
void ConnectionList::AddConnection(Socket pSocket, bool pIsActive)
{
	bool success = pSocket.accept (connectionArray[numConnections]);
	if(success)
	{
		activeStatusArray[numConnections] = true;
		numConnections++;
	}
}

// PRE: 
// POST: 
void ConnectionList::SetIthConnectionToStatus(int i, bool pNewStatus)
{
	activeStatusArray[i] = pNewStatus;
}

// PRE: 
// POST: 
bool ConnectionList::AllActive()
{
	bool isAllActive = true;
	for(int i = 0; i < numConnections; i++)
	{
		if(!activeStatusArray[i])
		{
			isAllActive = false;
		}
	}
	return(isAllActive);
}

// PRE: 
// POST: 
bool ConnectionList::AllInactive()
{
	bool isAllInactive = true;
	for(int i = 0; i < numConnections; i++)
	{
		if(activeStatusArray[i])
		{
			isAllInactive = false;
		}
	}
	return(isAllInactive);
}

// PRE: 
// POST: 
Socket ConnectionList::operator [](const int & i) const
{
	return (connectionArray[i]);
}

// PRE: 
// POST: 
Socket & ConnectionList::operator [](const int & i)
{
	return (connectionArray[i]);
}

