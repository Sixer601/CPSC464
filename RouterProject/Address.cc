#include <iostream>

#include "Address.h"
#include "Constants.h"

using namespace std;

	// ========================================
	// constructors
	// ========================================

	// Default Constructor

	// PRE: 
	// POST: this object contains a blank address with an undefined port.
	Address::Address()
	{
		address = "";
		// ASSERT: address is blank.
		port = -1;
		// ASSERT: port is an "undefined" port.
	}

	// PRE: pAddress is a defined string object containing an ip address.
	// POST: this object contains pAddress with an undefined port.
	Address::Address(string pAddress)
	{
		address = pAddress;
		// ASSERT: address is equal to pAddress.
		port = -1;
		// ASSERT: port is an "undefined" port.
	}

	// PRE: pAddress is a defined string object containing an ip address.
	//      pPort is a defined integer denoting a port.
	// POST: this object contains pAddress with a port of pPort.
	Address::Address(string pAddress, int pPort)
	{
		address = pAddress;
		// ASSERT: address is now equal to pAddress.
		port = pPort;
		// ASSERT: port is now equal to pPort.
	}

	// ========================================
	// accessor functions
	// ========================================

	// PRE: 
	// POST: RV is the ip address portion of this object.
	string Address::GetAddress() const
	{
		return(address);
	}

	// PRE: 
	// POST: RV is the port portion of this object.
	int Address::GetPort() const
	{
		return(port);
	}

	// PRE: 
	// POST: RV is a string containing the information stored within this object.
	string Address::Serialize() const
	{
		char tempStorage[BASESTRINGSIZE]; // char array to store contents of serialized address.
		// ASSERT: tempStorage is a defaultly constructed character array of size BASESTRINGSIZE.
		snprintf(tempStorage, BASESTRINGSIZE, "%s:%d", address.c_str(), port);
		string serializedAddress = tempStorage; // string to store contents of tempStorage.
		// ASSERT: serializedAddress now holds the contents of tempStorage.
		return(serializedAddress);
	}

	// ========================================
	// modifier functions
	// ========================================

	// PRE: pAddress is a defined string object containing an ip address.
	// POST: this object now holds pAddress as its ip address portion.
	void Address::SetAddress(string pAddress)
	{
		address = pAddress;
		// ASSERT: address holds the information stored in pAddress.
	}

	// PRE: pPort is a defined integer containing a port.
	// POST: this object now holds pPort as its port portion.
	void Address::SetPort(int pPort)
	{
		port = pPort;
		// ASSERT: port holds the information stored in pPort.
	}

	// PRE: pAddress is a defined string object containing an ipAddress.
	//      pPort is a defined integer containing a port.
	// POST: this object now holds pAddress as its ip address portion and
	//       pPort as its port portion.
	void Address::SetInformation(string pAddress, int pPort)
	{
		address = pAddress;
		// ASSERT: address holds the information stored in pAddress.
		port = pPort;
		// ASSERT: port holds the information stored in pPort.
	}

	// ========================================
	// predicate functions
	// ========================================

	// PRE: 
	// POST: RV is true if address is not a blank string, and false otherwise.
	bool Address::HasAddress()
	{
		return(address != "");
	}

	// PRE: 
	// POST: RV is true if port is not undefined, and false otherwise.
	bool Address::HasPort()
	{
		return(port != -1);
	}

	// ========================================
	// operators
	// ========================================

	// PRE: pAddress is a defined address object.
	// POST: this object holds the information pAddress holds.
	Address &Address::operator=(const Address &pAddress)
	{
		address = pAddress.address;
		// ASSERT: address now holds the information that pAddress's address holds.
		port = pAddress.port;
		// ASSERT: port now holds the information that pAddress's port holds.
		return (*this);
	}

	// PRE: pAddress is a defined, constant address object.
	// POST: RV is true if this object and pAddress are equivalent, and false otherwise.
	bool Address::operator==(const Address &pAddress)
	{
		return((address == pAddress.address) && (port == pAddress.port));
	}

	// ========================================
	// Input/Output Member Functions
	// ========================================	

	// PRE: 
	// POST: standard output contains a serialized version of this object.
	void Address::PrintAddress()
	{
		cout << this->Serialize();
	}
