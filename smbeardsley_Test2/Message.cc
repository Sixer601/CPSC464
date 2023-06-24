#include <iostream>

#include "Message.h"
#include "Point.h"
#include "Address.h"
#include "RoutingTable.h"
#include "Constants.h"

using namespace std;

// PRE: pString is a defined constant string object that represents a message.
// POST: RV is the sender coordinates stored in pString.
Point parseMessageContentsForSenderCoordinates(string const &pString)
{
	string serializedCoordinates = pString.substr(pString.find(BEGINSENDERCOORDINATESCHAR),
							(pString.find(ENDSENDERCOORDINATESCHAR) - pString.find(BEGINSENDERCOORDINATESCHAR)) + 1);
	// string object to contain the serialized coordinates stored within pString.
	// ASSERT: serializedCoordinates contains the section of the message storing the sender coordinates.
	int xCoordinate = atoi(serializedCoordinates.substr(serializedCoordinates.find("(") + 1, 
					  (serializedCoordinates.find(",") - serializedCoordinates.find("(") + 1)).c_str());
	// integer representing the x coordinate portion of serializedCoordinates.
	// ASSERT: xCoordinate is the integer value of the portion of serialized coordinates that represents the x coordinate.
	int yCoordinate = atoi(serializedCoordinates.substr(serializedCoordinates.find(",") + 1, 
					  (serializedCoordinates.find(")") - serializedCoordinates.find(",") + 1)).c_str());
	// integer representing the y coordinate portion of serializedCoordinates.
	// ASSERT: yCoordinate is the integer value of the portion of serialized coordinates that represents the y coordinate.
	Point coordinates(xCoordinate, yCoordinate); // object to be returned
	// ASSERT: coordinates is a defined point with xCoordinate and yCoordinate being passed as constructor parameters.
	return(coordinates);
}

// PRE: pString is a defined constant string object that represents a message.
// POST: RV is the sender address stored in pString.
Address parseMessageContentsForSenderAddress(string const &pString)
{
	string serializedSenderAddress = pString.substr(pString.find(BEGINSENDERADDRESSCHAR), 
							  (pString.find(ENDSENDERADDRESSCHAR) - pString.find(BEGINSENDERADDRESSCHAR)) + 1);
	// string object to contain the serialized sender address stored within pString.
	// ASSERT: serializedSenderAddress contains the section of the message storing the sender address.
	string address = serializedSenderAddress.substr(serializedSenderAddress.find(BEGINSENDERADDRESSCHAR) + 1, 
				 (serializedSenderAddress.find(IPPORTSEPARATOR) - (serializedSenderAddress.find(BEGINSENDERADDRESSCHAR) + 1)));
	// string object representing the ip address portion of serializedSenderAddress.
	// ASSERT: address is the portion of serializedSenderAddress that represents the ip address.
	int port = atoi( serializedSenderAddress.substr(serializedSenderAddress.find(IPPORTSEPARATOR) + 1, 
				 (serializedSenderAddress.find(ENDSENDERADDRESSCHAR) - (serializedSenderAddress.find(IPPORTSEPARATOR) + 1))).c_str());
	// integer representing the port portion of serializedSenderAddress.
	// ASSERT: port is the integer value of the portion of serializedSenderAddress that represents the port portion of the address.
	Address senderAddress(address, port); // object to be returned.
	// ASSERT: senderAddress is a defined address object with address and port being passed as constructor parameters.
	return(senderAddress);
}

// PRE: pString is a defined constant string object that represents the contents of a message.
// POST: RV is the recipient address stored in pString.
Address parseMessageContentsForRecipientAddress(string const &pString)
{
	string serializedRecipientAddress = pString.substr(pString.find(BEGINRECIPIENTADDRESSCHAR), 
							  (pString.find(ENDRECIPIENTADDRESSCHAR) - pString.find(BEGINRECIPIENTADDRESSCHAR)) + 1);
	// string object to contain the serialized recipient address stored within pString.
	// ASSERT: serializedRecipientAddress contains the section of the message storing the recipient address.
	string address = serializedRecipientAddress.substr(serializedRecipientAddress.find(BEGINRECIPIENTADDRESSCHAR) + 1, 
				 (serializedRecipientAddress.find(IPPORTSEPARATOR) - (serializedRecipientAddress.find(BEGINRECIPIENTADDRESSCHAR) + 1)));
	// string object representing the ip address portion of serializedRecipientAddress.
	// ASSERT: address is the portion of serializedRecipientAddress that represents the ip address.
	int port = atoi( serializedRecipientAddress.substr(serializedRecipientAddress.find(IPPORTSEPARATOR) + 1, 
				 (serializedRecipientAddress.find(ENDRECIPIENTADDRESSCHAR) - (serializedRecipientAddress.find(IPPORTSEPARATOR) + 1))).c_str());
	// integer representing the port portion of serializedRecipientAddress.
	// ASSERT: port is the integer value of the portion of serializedRecipientAddress that represents the port portion of the address.
	Address recipientAddress(address, port);
	// ASSERT: recipientAddress is a defined address object with address and port being passed as constructor parameters.
	return(recipientAddress);
}

// PRE: serializedRoutingTable is the string equivalent of a routing table object. currBeginEntryIndex contains the location within the routing table
//      that the entry to add begins. currEndEntryIndex contains the location within the routing table that the entry to add ends. messageRoutingTable
//      is the routing table to add the entry into once parsed.
// POST: messageRoutingTable contains the entry stored in serializedRoutingTable from currBeginEntryIndex to currEndEntryIndex.
void addSingleEntryToRoutingTable(string serializedRoutingTable, int currBeginEntryIndex, int currEndEntryIndex, RoutingTable &messageRoutingTable)
{
	string entry = serializedRoutingTable.substr(currBeginEntryIndex, ((currEndEntryIndex - currBeginEntryIndex) + 1));
	// string object containing the contents of the entry stored in serializedRoutingTable.
	// ASSERT: entry contains the substring in serializedRoutingTable starting at currBeginEntryIndex and ending at currEndEntryIndex.
	string destinationAddressSection = entry.substr(entry.find(BEGINDESTINATIONCHARACTER) + 1, 
							   ((entry.find(ENDDESTINATIONCHARACTER) - entry.find(BEGINDESTINATIONCHARACTER)) + 1));
	// string object containing the destination address component to the entry.
	// ASSERT: destinationAddressSection contains the information stored in entry starting at BEGINDESTINATIONCHARACTER and ending at
	//         ENDDESTINATIONCHARACTER.
	string destinationAddressString = destinationAddressSection.substr(destinationAddressSection.find(BEGINDESTINATIONCHARACTER) + 1, 
					 		   (destinationAddressSection.find(IPPORTSEPARATOR) - (destinationAddressSection.find(BEGINDESTINATIONCHARACTER) + 1)));
	// string object containing the ip address portion of the destination address.
	// ASSERT: destinationAddressString contains the information stored in destinationAddressSection from BEGINDESTINATIONCHARACTER to IPPORTSEPARATOR.
	int destinationPort = atoi(destinationAddressSection.substr(destinationAddressSection.find(IPPORTSEPARATOR) + 1, 
				 		 (destinationAddressSection.find(ENDDESTINATIONCHARACTER) - (destinationAddressSection.find(IPPORTSEPARATOR) + 1))).c_str());
	// integer containing the port portion of the destination address.
	// ASSERT: destinationPort contains the information stored in destinationAddressSection from IPPORTSEPARATOR to ENDDESTINATIONCHARACTER.
	Address destinationAddress(destinationAddressString, destinationPort); // address object to store destinationAddressString and destinationPort.
	// ASSERT: destinationAddress is a defined address object being passed destinationAddressString and destinationPort as its constructor parameters.
	string firstHopAddressSection = entry.substr(entry.find(BEGINFIRSTHOPCHARACTER) + 1, 
						     ((entry.find(ENDFIRSTHOPCHARACTER) - entry.find(BEGINFIRSTHOPCHARACTER)) + 1));
	// string object containing the first hop address component to the entry.
	// ASSERT: firstHopAddressSection contains the information stored in entry starting at BEGINFIRSTHOPCHARACTER and endint at ENDFIRSTHOPCHARACTER.
	string firstHopAddressString = firstHopAddressSection.substr(firstHopAddressSection.find(BEGINFIRSTHOPCHARACTER) + 1, 
					 	     (firstHopAddressSection.find(IPPORTSEPARATOR) - (firstHopAddressSection.find(BEGINFIRSTHOPCHARACTER) + 1)));
	// string object containing the ip address portion of the destination address.
	// ASSERT: firstHopAddressString contains the information stored in firstHopAddressSection from BEGINFIRSTHOPCHARACTER to IPPORTSEPARATOR.
	int firstHopPort = atoi(firstHopAddressSection.substr(firstHopAddressSection.find(IPPORTSEPARATOR) + 1, 
					   (firstHopAddressSection.find(ENDFIRSTHOPCHARACTER) - (firstHopAddressSection.find(IPPORTSEPARATOR) + 1))).c_str());
	// integer containing the port portion of the first hop address.
	// ASSERT: firstHopPort contains the information stored in firstHopAddressSection from IPPORTSEPARATOR to ENDFIRSTHOPCHARACTER.
	Address firstHopAddress(firstHopAddressString, firstHopPort); // address object to store firstHopAddressString and firstHopPort.
	// ASSERT: firstHopAddress is a defined address object being passed firstHopAddressString and firstHopPort as its constructor parameters.
	int distance = atoi(entry.substr((entry.find(BEGINDISTANCECHARACTER) + 1), (entry.find(ENDDISTANCECHARACTER) - 1) - entry.find(BEGINDISTANCECHARACTER)).c_str());
	// integer containing the number of hops needed to reach the destination address.
	// ASSERT: distance contains the information stored in entry from BEGINDISTANCECHARACTER to ENDDISTANCECHARACTER.
	messageRoutingTable.AddEntry(destinationAddress, firstHopAddress, distance);
}

// PRE: pString is a defined constant string object containing contents of a message.
// POST: RV is the routing table stored within pString.
RoutingTable parseMessageContentsForRoutingTable(string const &pString)
{
	string RoutingTablePortion = pString.substr(pString.find(BEGINROUTINGTABLECHAR), 
						    (((pString.find(ENDROUTINGTABLECHAR)) + 1) - (pString.find(BEGINROUTINGTABLECHAR))));
	// string to contain the portion of pString that holds the serialized routing table of the sender at the time the message was sent.
	// ASSERT: RoutingTablePortion is a substring of pString from BEGINROUTINGTABLECHAR to ENDROUTINGTABLECHAR.
	RoutingTable messageRoutingTable; // object to store routing table in pString.
	// ASSERT: messageRoutingTable is a defaultly constructed routing table.
	bool allEntriesEntered; // true if all entries in pString have been entered, and false or undefined otherwise.
	// ASSERT: allEntriesEntered begins as undefined, as its value depends on a condition below.
	if(RoutingTablePortion == EMPTYROUTINGTABLE)
	// ASSERT: the message contains an empty routing table.
	{
		allEntriesEntered = true;
		// ASSERT: there are no entries to enter, therefore all entries have been entered.
	}
	else
	{
		allEntriesEntered = false;
		// ASSERT: we have not entered any entries, and at least 1 entry exists, therefore all entries have not been entered.
		string serializedRoutingTable = pString.substr(pString.find(BEGINROUTINGTABLECHAR) + 1, 
							    (((pString.find(ENDROUTINGTABLECHAR)) + 1) - (pString.find(BEGINROUTINGTABLECHAR))));
		// string object to hold the serialized routing table.
		// ASSERT: serialziedRoutingTable contains the information in pString stored between BEGINROUTINGTABLECHAR and ENDROUTINGTABLECHAR.
		int currBeginEntryIndex = serializedRoutingTable.find(BEGINENTRYCHARACTER); // integer to denote what index the current entry begins at.
		// ASSERT: currBeginEntryIndex is equal to the first BEGINENTRYCHARACTER in serializedRoutingTable.
		int currEndEntryIndex = serializedRoutingTable.find(ENDENTRYCHARACTER); // integer to denote what index the current entry ends at.
		// ASSERT: currEndEntryIndex is equal to the first ENDENTRYCHARACTER in serializedRoutingTable.
		while (!allEntriesEntered)
		// ASSERT: all entries have not been entered.
		{
			addSingleEntryToRoutingTable(serializedRoutingTable, currBeginEntryIndex, currEndEntryIndex, messageRoutingTable);
			if(serializedRoutingTable[currEndEntryIndex + 1] == BEGINENTRYCHARACTER[0])
			// ASSERT: the value after currEndEntryIndex is the BEGINENTRYCHARACTER.
			{
				currBeginEntryIndex = currEndEntryIndex + 1;
				// ASSERT: currBeginEntryIndex is equal to the index following currEndEntryIndex.
				currEndEntryIndex = serializedRoutingTable.find(ENDENTRYCHARACTER, currBeginEntryIndex);
				// ASSERT: currEndEntryIndex is equal to the index the first ENDENTRYCHARACTER resides at, starting the search at currBeginEntryIndex.
			}
			else 
			// ASSERT: the value after currEndEntryIndex is not the begin entry character.
			{
				allEntriesEntered = true;
				// ASSERT: all entries have been entered.
			}
		}
	}
	return(messageRoutingTable);
}

	// ========================================
	// constructors
	// ========================================

	// PRE: 
	// POST: this object is an "empty" message.
	Message::Message()
	{
		messageContents = "";
	}

	// PRE: pMessageContents is a defined string that contains the components of a message in the correct order.
	// POST: this object is a defined Message object containing the information stored in pMessageContents.
	Message::Message(string pMessageContents)
	{
		messageContents = pMessageContents;
		senderCoordinates = parseMessageContentsForSenderCoordinates(pMessageContents);
		senderAddress = parseMessageContentsForSenderAddress(pMessageContents);
		recipientAddress = parseMessageContentsForRecipientAddress(pMessageContents);
		routingTable = parseMessageContentsForRoutingTable(pMessageContents);
	}

	// PRE: pSenderCoordinates is a defined point object representing the coordinates of the device sending the message.
	//      pSenderAddress is a defined address object representing the address of the device sending the message.
	//      pRecipientAddress is a defined address object representing the address of the device to have the message sent to it.
	//      pRoutingTable is a defined routing table object representing the routing table of the device sending the message at the
	//      time this object is being constructed.
	// POST: this object is a defined message object containing all aformentioned components of the message in order.
	Message::Message(Point &pSenderCoordinates, Address &pSenderAddress, Address &pRecipientAddress, RoutingTable &pRoutingTable)
	{
		messageContents = (string)BEGINMESSAGECHAR
					 	    + BEGINSENDERCOORDINATESCHAR
						    + pSenderCoordinates.Serialize()
						    + ENDSENDERCOORDINATESCHAR
						    + BEGINSENDERADDRESSCHAR
						    + pSenderAddress.Serialize()
						    + ENDSENDERADDRESSCHAR
						    + BEGINRECIPIENTADDRESSCHAR
						    + pRecipientAddress.Serialize()
						    + ENDRECIPIENTADDRESSCHAR
						    + pRoutingTable.Serialize()
						    + ENDMESSAGECHAR;

		senderCoordinates = pSenderCoordinates;
		senderAddress = pSenderAddress;
		recipientAddress = pRecipientAddress;
		routingTable = pRoutingTable;
	}

	// ========================================
	// accessor functions
	// ========================================

	// PRE: 
	// POST: RV is a defined string object containing the serialized components of this object.
	string Message::GetMessageContents() const
	{
		return(messageContents);
	}

	// PRE: 
	// POST: RV is a defined Point object containing the sender coordinates of this object.
	Point Message::GetSenderCoordinates() const
	{
		return(senderCoordinates);
	}

	// PRE: 
	// POST: RV is a defined Address object containing the sender address of this object.
	Address Message::GetSenderAddress() const
	{
		return(senderAddress);
	}

	// PRE: 
	// POST: RV is a defined Address object containing the recipient address of this object.
	Address Message::GetRecipientAddress() const
	{
		return(recipientAddress);
	}

	// PRE: 
	// POST: RV is a defined Routing Table object containing the routing table of this object.
	RoutingTable Message::GetRoutingTable() const
	{
		return(routingTable);
	}

	// ========================================
	// modifier functions
	// ========================================

	// PRE: pMessage is a defined string object containing the contents of the message this object will contain.
	// POST: this object contains the information stored within pMessage.
	void Message::SetMessage(string pMessage)
	{
		messageContents = pMessage;
		senderCoordinates = parseMessageContentsForSenderCoordinates(pMessage);
		senderAddress = parseMessageContentsForSenderAddress(pMessage);
		recipientAddress = parseMessageContentsForRecipientAddress(pMessage);
		routingTable = parseMessageContentsForRoutingTable(pMessage);
	}

	// PRE: pSenderCoordinates is a defined point object representing what the coordinates are of the sender of this message.
	//      pSenderAddress is a defined address object representing what the address and port are of the sender of this message.
	//      pRecipientAddress is a defined address object representing what the address and port are of the intended recipient of this message.
	//      pRoutingTable is a defined routing table object representing what the routing table is of the sender of this message.
	// POST: this object contains the information stored in the objects listed above.
	void Message::SetMessage(Point pSenderCoordinates, Address pSenderAddress, Address pRecipientAddress, RoutingTable pRoutingTable)
	{
		messageContents = (string)BEGINMESSAGECHAR
					 	    + BEGINSENDERCOORDINATESCHAR
						    + pSenderCoordinates.Serialize()
						    + ENDSENDERCOORDINATESCHAR
						    + BEGINSENDERADDRESSCHAR
						    + pSenderAddress.Serialize()
						    + ENDSENDERADDRESSCHAR
						    + BEGINRECIPIENTADDRESSCHAR
						    + pRecipientAddress.Serialize()
						    + ENDRECIPIENTADDRESSCHAR
						    + pRoutingTable.Serialize()
						    + ENDMESSAGECHAR;
		
		senderCoordinates = pSenderCoordinates;
		senderAddress = pSenderAddress;
		recipientAddress = pRecipientAddress;
		routingTable = pRoutingTable;
	}

	// PRE: pSenderCoordinates is a defined point object representing the coordinates of the sender device of this message.
	// POST: this object contains pSenderCoordinates instead of senderCoordinates.
	void Message::UpdateSenderCoordinates(Point pSenderCoordinates)
	{
		senderCoordinates = pSenderCoordinates;

		messageContents = (string)BEGINMESSAGECHAR
					 	    + BEGINSENDERCOORDINATESCHAR
						    + senderCoordinates.Serialize()
						    + ENDSENDERCOORDINATESCHAR
						    + BEGINSENDERADDRESSCHAR
						    + senderAddress.Serialize()
						    + ENDSENDERADDRESSCHAR
						    + BEGINRECIPIENTADDRESSCHAR
						    + recipientAddress.Serialize()
						    + ENDRECIPIENTADDRESSCHAR
						    + routingTable.Serialize()
						    + ENDMESSAGECHAR;


	}

	// PRE: pSenderAddress is a defined address object representing the address and port of the sender device of this message.
	// POST: this object contains pSenderAddress instead of senderAddress.
	void Message::UpdateSenderAddress(Address pSenderAddress)
	{
		senderAddress = pSenderAddress;

		messageContents = (string)BEGINMESSAGECHAR
					 	    + BEGINSENDERCOORDINATESCHAR
						    + senderCoordinates.Serialize()
						    + ENDSENDERCOORDINATESCHAR
						    + BEGINSENDERADDRESSCHAR
						    + senderAddress.Serialize()
						    + ENDSENDERADDRESSCHAR
						    + BEGINRECIPIENTADDRESSCHAR
						    + recipientAddress.Serialize()
						    + ENDRECIPIENTADDRESSCHAR
						    + routingTable.Serialize()
						    + ENDMESSAGECHAR;
	}

	// PRE: pRecipientAddress is a defined address object representing the address and port of the intended recipient of this message.
	// POST: this object contains pRecipientAddress instead of recipientAddress.
	void Message::UpdateRecipientAddress(Address pRecipientAddress)
	{
		recipientAddress = pRecipientAddress;

		messageContents = (string)BEGINMESSAGECHAR
					 	    + BEGINSENDERCOORDINATESCHAR
						    + senderCoordinates.Serialize()
						    + ENDSENDERCOORDINATESCHAR
						    + BEGINSENDERADDRESSCHAR
						    + senderAddress.Serialize()
						    + ENDSENDERADDRESSCHAR
						    + BEGINRECIPIENTADDRESSCHAR
						    + recipientAddress.Serialize()
						    + ENDRECIPIENTADDRESSCHAR
						    + routingTable.Serialize()
						    + ENDMESSAGECHAR;
	}

	// PRE: pRoutingTable is a defined routing table object representing the routing table of the sender device of this message.
	// POST: this object contains pRoutingTable instead of routingTable.
	void Message::UpdateRoutingTable(RoutingTable pRoutingTable)
	{
		routingTable = pRoutingTable;

		messageContents = (string)BEGINMESSAGECHAR
					 	    + BEGINSENDERCOORDINATESCHAR
						    + senderCoordinates.Serialize()
						    + ENDSENDERCOORDINATESCHAR
						    + BEGINSENDERADDRESSCHAR
						    + senderAddress.Serialize()
						    + ENDSENDERADDRESSCHAR
						    + BEGINRECIPIENTADDRESSCHAR
						    + recipientAddress.Serialize()
						    + ENDRECIPIENTADDRESSCHAR
						    + routingTable.Serialize()
						    + ENDMESSAGECHAR;
	}

	// ========================================
	// predicate functions
	// ========================================



	// ========================================
	// operators
	// ========================================

	// PRE: pMessage is a defined Message object.
	// POST: this object contains the information stored within pMessage.
	Message &Message::operator=(const Message &pMessage)
	{
		messageContents = pMessage.messageContents;
		senderCoordinates = pMessage.senderCoordinates;
		senderAddress = pMessage.senderAddress;
		recipientAddress = pMessage.recipientAddress;
		routingTable = pMessage.routingTable;

		return (*this);
	}

	// ========================================
	// Input/Output Member Functions
	// ========================================

	// PRE: 
	// POST: os contains the contents of pMessage.
	ostream &operator<<(ostream &os, const Message &pMessage)
	{
		os << pMessage.messageContents;
		return(os);
	}

	// PRE: 
	// POST: pMessage's contents are now what was in is.
	istream &operator>>(istream &is, Message &pMessage)
	{
		is >> pMessage.messageContents;
		return(is);
	}
