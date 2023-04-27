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

	Address();

	Address(string pAddress);

	Address(string pAddress, int pPort);

	// ========================================
	// accessor functions
	// ========================================

	string GetAddress() const;

	int GetPort() const;

	string Serialize() const;

	// ========================================
	// modifier functions
	// ========================================

	void SetAddress(string pAddress);

	void SetPort(int pPort);

	void SetInformation(string pAddress, int pPort);

	// ========================================
	// predicate functions
	// ========================================

	bool HasAddress();

	bool HasPort();

	// ========================================
	// operators
	// ========================================

	Address &operator=(const Address &pAddress);

	bool operator==(const Address &pAddress);

	// ========================================
	// Input/Output Member Functions
	// ========================================	

	void PrintAddress();

};

#endif