#ifndef INCLUDED_Address
#define INCLUDED_Address

#include <iostream>

#include "Constants.h"

using namespace std;

class Address
{

private:
	string address;
	int port;

public:

	// ========================================
	// constructors
	// ========================================

	// Default Constructor

	// PRE: 
	// POST: this object contains a blank address with an undefined port.
	Address();

	// PRE: pAddress is a defined string object containing an ip address.
	// POST: this object contains pAddress with an undefined port.
	Address(string pAddress);

	// PRE: pAddress is a defined string object containing an ip address.
	//      pPort is a defined integer denoting a port.
	// POST: this object contains pAddress with a port of pPort.
	Address(string pAddress, int pPort);

	// ========================================
	// accessor functions
	// ========================================

	// PRE: 
	// POST: RV is the ip address portion of this object.
	string GetAddress() const;

	// PRE: 
	// POST: RV is the port portion of this object.
	int GetPort() const;

	// PRE: 
	// POST: RV is a string containing the information stored within this object.
	string Serialize() const;

	// ========================================
	// modifier functions
	// ========================================

	// PRE: pAddress is a defined string object containing an ip address.
	// POST: this object now holds pAddress as its ip address portion.
	void SetAddress(string pAddress);

	// PRE: pPort is a defined integer containing a port.
	// POST: this object now holds pPort as its port portion.
	void SetPort(int pPort);

	// PRE: pAddress is a defined string object containing an ipAddress.
	//      pPort is a defined integer containing a port.
	// POST: this object now holds pAddress as its ip address portion and
	//       pPort as its port portion.
	void SetInformation(string pAddress, int pPort);

	// ========================================
	// predicate functions
	// ========================================

	// PRE: 
	// POST: RV is true if address is not a blank string, and false otherwise.
	bool HasAddress();

	// PRE: 
	// POST: RV is true if port is not undefined, and false otherwise.
	bool HasPort();

	// ========================================
	// operators
	// ========================================

	// PRE: pAddress is a defined address object.
	// POST: this object holds the information pAddress holds.
	Address &operator=(const Address &pAddress);

	// PRE: pAddress is a defined, constant address object.
	// POST: RV is true if this object and pAddress are equivalent, and false otherwise.
	bool operator==(const Address &pAddress);

	// ========================================
	// Input/Output Member Functions
	// ========================================	

	// PRE: 
	// POST: standard output contains a serialized version of this object.
	void PrintAddress();

};

#endif