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

private:
	string messageContents; // string form of contents of message object.
	Point senderCoordinates; // coordinates of the sender of the message.
	Address senderAddress; // address object of the sender of the message.
	Address recipientAddress; // address object of the intended recipient of the message.
	RoutingTable routingTable; // routing table of the sender of the message at the time the message was sent.

public: 
	
	// ========================================
	// constructors
	// ========================================

	// Default Constructor

	// PRE: 
	// POST: this object is an "empty" message.
	Message();

	// PRE: pMessageContents is a defined string that contains the components of a message in the correct order.
	// POST: this object is a defined Message object containing the information stored in pMessageContents.
	Message(string pMessageContents);

	// PRE: pSenderCoordinates is a defined point object representing the coordinates of the device sending the message.
	//      pSenderAddress is a defined address object representing the address of the device sending the message.
	//      pRecipientAddress is a defined address object representing the address of the device to have the message sent to it.
	//      pRoutingTable is a defined routing table object representing the routing table of the device sending the message at the
	//      time this object is being constructed.
	// POST: this object is a defined message object containing all aformentioned components of the message in order.
	Message(Point &pSenderCoordinates, Address &pSenderAddess, Address &pRecipientAddress, RoutingTable &pRoutingTable);

	// ========================================
	// accessor functions
	// ========================================

	// PRE: 
	// POST: RV is a defined string object containing the serialized components of this object.
	string GetMessageContents() const;

	// PRE: 
	// POST: RV is a defined Point object containing the sender coordinates of this object.
	Point GetSenderCoordinates() const;

	// PRE: 
	// POST: RV is a defined Address object containing the sender address of this object.
	Address GetSenderAddress() const;

	// PRE: 
	// POST: RV is a defined Address object containing the recipient address of this object.
	Address GetRecipientAddress() const;

	// PRE: 
	// POST: RV is a defined Routing Table object containing the routing table of this object.
	RoutingTable GetRoutingTable() const;

	// ========================================
	// modifier functions
	// ========================================

	// PRE: pMessage is a defined string object containing the contents of the message this object will contain.
	// POST: this object contains the information stored within pMessage.
	void SetMessage(string pMessage);

	// PRE: pSenderCoordinates is a defined point object representing what the coordinates are of the sender of this message.
	//      pSenderAddress is a defined address object representing what the address and port are of the sender of this message.
	//      pRecipientAddress is a defined address object representing what the address and port are of the intended recipient of this message.
	//      pRoutingTable is a defined routing table object representing what the routing table is of the sender of this message.
	// POST: this object contains the information stored in the objects listed above.
	void SetMessage(Point pSenderCoordinates, Address pSenderAddress, Address pRecipientAddress, RoutingTable pRoutingTable);

	// PRE: pSenderCoordinates is a defined point object representing the coordinates of the sender device of this message.
	// POST: this object contains pSenderCoordinates instead of senderCoordinates.
	void UpdateSenderCoordinates(Point pSenderCoordinates);

	// PRE: pSenderAddress is a defined address object representing the address and port of the sender device of this message.
	// POST: this object contains pSenderAddress instead of senderAddress.
	void UpdateSenderAddress(Address pSenderAddress);

	// PRE: pRecipientAddress is a defined address object representing the address and port of the intended recipient of this message.
	// POST: this object contains pRecipientAddress instead of recipientAddress.
	void UpdateRecipientAddress(Address pRecipientAddress);

	// PRE: pRoutingTable is a defined routing table object representing the routing table of the sender device of this message.
	// POST: this object contains pRoutingTable instead of routingTable.
	void UpdateRoutingTable(RoutingTable pRoutingTable);

	// ========================================
	// predicate functions
	// ========================================



	// ========================================
	// operators
	// ========================================

	// PRE: pMessage is a defined Message object.
	// POST: this object contains the information stored within pMessage.
	Message &operator=(const Message &pString);

	// ========================================
	// Input/Output Member Functions
	// ========================================

	// PRE: 
	// POST: os contains the contents of pMessage.
	friend ostream &operator<<(ostream &os, const Message &pMessage);

	// PRE: 
	// POST: pMessage's contents are now what was in is.
	friend istream &operator>>(istream &is, Message &pMessage);

};

#endif