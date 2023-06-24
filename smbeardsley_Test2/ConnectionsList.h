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

private:
	Address * addressList; // list of addresses stored that are connected.
	Socket ** connectionsList; // list of socket pointers associated with
						  // those addresses.
	int numConnections; // number of entries in this object.

public:

	// ========================================
	// constructors
	// ========================================

	// Default Constructor

		// PRE: 
		// POST: this object is an "empty" connections list.
		ConnectionsList();

	// ========================================
	// destructor
	// ========================================

		// PRE: 
		// POST: this object is destroyed.
		~ConnectionsList();
	
	// ========================================
	// accessor functions
	// ========================================

		// PRE: 
		// POST: RV is the addressList member of this object.
		Address * GetAddressList() const;

		// PRE: 
		// POST: RV is the nth index of the addressList member of this object.
		Address GetNthAddress(int n) const;

		// PRE: 
		// POST: RV is the connectionsList member of this object.
		Socket ** GetConnectionsList() const;

		// PRE: 
		// POST: RV is the nth index of the connectionsList member of this object.
		Socket * GetNthConnection(int n);

		// PRE: pAddress is a defined Address object.
		// POST: RV is a socket pointer object represented by pAddress.
		Socket * GetConnectionWithPort(int pPort) const;

		// PRE: pAddress is a defined Address object.
		// POST: RV is a socket pointer object represented by pAddress.
		Socket * GetConnectionWithAddress(Address pAddress) const;

		// PRE: 
		// POST: RV is the number of connections stored within this object.
		int GetNumConnections() const;

	// ========================================
	// modifier functions
	// ========================================

		// PRE: connectionSocket is a defined socket pointer object that points to a socket that has been connected to.
		//      connectionAddress is a defined address object that contains the address information of the device that has connected.
		// POST: this object reflects the connection made.
		void AddConnection(Socket * connectionSocket, Address connectionAddress);

	// ========================================
	// predicate functions
	// ========================================

		// PRE: 
		// POST: RV is true if there are more than 0 connections stored in this object 
		bool HasConnections();

		// PRE: pAddress is a defined address object.
		// POST: RV is true if pAddress is an address stored in this object, and false otherwise.
		bool HasConnectionWithAddress(Address pAddress);

	// ========================================
	// operators
	// ========================================



	// ========================================
	// Input/Output Member Functions
	// ========================================

		// PRE: 
		// POST: standard output contains this object's contents.
		void PrintConnectionsList();

};

#endif