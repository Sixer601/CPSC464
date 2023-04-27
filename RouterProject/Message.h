#ifndef INCLUDED_Message
#define INCLUDED_Message

#include <iostream>

#include "Address.h"
#include "Point.h"
#include "RoutingTable.h"
#include "Constants.h"

using namespace std;

class Message
{
	/* Class Invariant (CI):
	
	*/

private:
	string messageContents;
	Point senderCoordinates;
	Address senderAddress;
	Address recipientAddress;
	RoutingTable routingTable;

public: 
	
	// ========================================
	// constructors
	// ========================================

	Message();

	Message(string pMessageContents);

	Message(Point &pSenderCoordinates, Address &pSenderAddess, Address &pRecipientAddress, RoutingTable &pRoutingTable);

	// ========================================
	// accessor functions
	// ========================================

	string GetMessageContents() const;

	Point GetSenderCoordinates() const;

	Address GetSenderAddress() const;

	Address GetRecipientAddress() const;

	RoutingTable GetRoutingTable() const;

	// ========================================
	// modifier functions
	// ========================================

	void SetMessage(string pMessage);

	void SetMessage(Point pSenderCoordinates, Address pSenderAddress, Address pRecipientAddress, RoutingTable pRoutingTable);

	void UpdateSenderCoordinates(Point pSenderCoordinates);

	void UpdateSenderAddress(Address pSenderAddress);

	void UpdateRecipientAddress(Address pRecipientAddress);

	void UpdateRoutingTable(RoutingTable pRoutingTable);

	// ========================================
	// predicate functions
	// ========================================



	// ========================================
	// operators
	// ========================================

	Message &operator=(const Message &pString);

	// ========================================
	// Input/Output Member Functions
	// ========================================

	friend ostream &operator<<(ostream &os, const Message &pMessage);

	friend istream &operator>>(istream &is, Message &pMessage);

};

Point parseMessageForSenderCoordinates(string &pString);

Point parseMessageForSenderCoordinates(Message &pMessage);

Address parseMessageForSenderAddress(string &pString);

Address parseMessageForSenderAddress(Message &pMessage);

Address parseMessageForRecipientAddress(string &pString);

Address parseMessageForRecipientAddress(Message &pMessage);

void addSingleEntryToRoutingTable(string serializedRoutingTable, int currBeginEntryIndex, int currEndEntryIndex, RoutingTable &messageRoutingTable);

RoutingTable parseMessageForRoutingTable(string &pString);

RoutingTable parseMessageForRoutingTable(Message &pMessage);

#endif