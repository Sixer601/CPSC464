#include <iostream>

#include "Address.h"
#include "Constants.h"

using namespace std;

	// ========================================
	// constructors
	// ========================================

	Address::Address()
	{
		address = "";
		port = -1;
	}

	Address::Address(string pAddress)
	{
		address = pAddress;
		port = -1;
	}

	Address::Address(string pAddress, int pPort)
	{
		address = pAddress;
		port = pPort;
	}

	// ========================================
	// accessor functions
	// ========================================

	string Address::GetAddress() const
	{
		return(address);
	}

	int Address::GetPort() const
	{
		return(port);
	}

	string Address::Serialize() const
	{
		char tempStorage[BASESTRINGSIZE];
		snprintf(tempStorage, BASESTRINGSIZE, "%s:%d", address.c_str(), port);
		string serializedAddress = tempStorage;

		return(serializedAddress);
	}

	// ========================================
	// modifier functions
	// ========================================

	void Address::SetAddress(string pAddress)
	{
		address = pAddress;
	}

	void Address::SetPort(int pPort)
	{
		port = pPort;
	}

	void Address::SetInformation(string pAddress, int pPort)
	{
		address = pAddress;
		port = pPort;
	}

	// ========================================
	// predicate functions
	// ========================================

	bool Address::HasAddress()
	{
		return(address != "");
	}

	bool Address::HasPort()
	{
		return(port != -1);
	}

	// ========================================
	// operators
	// ========================================

	Address &Address::operator=(const Address &pAddress)
	{
		address = pAddress.address;
		port = pAddress.port;

		return (*this);
	}

	bool Address::operator==(const Address &pAddress)
	{
		return((address == pAddress.address) && (port == pAddress.port));
	}

	// ========================================
	// Input/Output Member Functions
	// ========================================	

	void Address::PrintAddress()
	{
		cout << address << IPPORTSEPARATOR << port;
	}
