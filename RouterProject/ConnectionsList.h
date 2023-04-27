#ifndef INCLUDED_ConnectionsList
#define INCLUDED_ConnectionsList

#include <iostream>

#include "Address.h"
#include "Socket.h"
#include "SocketException.h"
#include "Constants.h"

using namespace std;

class ConnectionsList
{
	/* Class Invariant (CI): 


     */

private:
	Address * addressList;
	Socket ** connectionsList;
	int numConnections;

public:

	// ========================================
	// constructors
	// ========================================

		ConnectionsList();

	// ========================================
	// destructor
	// ========================================

		~ConnectionsList();
	
	// ========================================
	// accessor functions
	// ========================================

		Address * GetAddressList() const;

		Address GetNthAddress(int n) const;

		Socket ** GetConnectionsList() const;

		Socket * GetNthConnection(int n);

		Socket * GetConnectionWithPort(int pPort) const;

		int GetNumConnections() const;

	// ========================================
	// modifier functions
	// ========================================

		void AddConnection(Socket * connectionSocket, Address connectionAddress);

	// ========================================
	// predicate functions
	// ========================================

		bool HasConnections();

		bool HasConnectionWithAddress(Address pAddress);

	// ========================================
	// operators
	// ========================================



	// ========================================
	// Input/Output Member Functions
	// ========================================

		void PrintConnectionsList();

};

#endif