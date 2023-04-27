#include <iostream>

#include "Message.h"
#include "Point.h"
#include "Address.h"
#include "RoutingTable.h"
#include "Constants.h"

using namespace std;

Point parseMessageContentsForSenderCoordinates(string const &pString)
{
	string serializedCoordinates = pString.substr(pString.find(BEGINSENDERCOORDINATESCHAR),
							(pString.find(ENDSENDERCOORDINATESCHAR) - pString.find(BEGINSENDERCOORDINATESCHAR)) + 1);
	int xCoordinate = atoi(serializedCoordinates.substr(serializedCoordinates.find("(") + 1, 
					  (serializedCoordinates.find(",") - serializedCoordinates.find("(") + 1)).c_str());
	int yCoordinate = atoi(serializedCoordinates.substr(serializedCoordinates.find(",") + 1, 
					  (serializedCoordinates.find(")") - serializedCoordinates.find(",") + 1)).c_str());
	Point coordinates(xCoordinate, yCoordinate);
	return(coordinates);
}

Point parseMessageForSenderCoordinates(Message &pMessage)
{
	return(parseMessageContentsForSenderCoordinates(pMessage.GetMessageContents()));
}

Address parseMessageContentsForSenderAddress(string const &pString)
{
	string serializedSenderAddress = pString.substr(pString.find(BEGINSENDERADDRESSCHAR), 
							  (pString.find(ENDSENDERADDRESSCHAR) - pString.find(BEGINSENDERADDRESSCHAR)) + 1);
	string address = serializedSenderAddress.substr(serializedSenderAddress.find(BEGINSENDERADDRESSCHAR) + 1, 
				 (serializedSenderAddress.find(IPPORTSEPARATOR) - (serializedSenderAddress.find(BEGINSENDERADDRESSCHAR) + 1)));
	int port = atoi( serializedSenderAddress.substr(serializedSenderAddress.find(IPPORTSEPARATOR) + 1, 
				 (serializedSenderAddress.find(ENDSENDERADDRESSCHAR) - (serializedSenderAddress.find(IPPORTSEPARATOR) + 1))).c_str());

	Address senderAddress(address, port);
	return(senderAddress);
}

Address parseMessageForSenderAddress(Message &pMessage)
{
	return(parseMessageContentsForSenderAddress(pMessage.GetMessageContents()));
}

Address parseMessageContentsForRecipientAddress(string const &pString)
{
	string serializedRecipientAddress = pString.substr(pString.find(BEGINRECIPIENTADDRESSCHAR), 
							  (pString.find(ENDRECIPIENTADDRESSCHAR) - pString.find(BEGINRECIPIENTADDRESSCHAR)) + 1);
	string address = serializedRecipientAddress.substr(serializedRecipientAddress.find(BEGINRECIPIENTADDRESSCHAR) + 1, 
				 (serializedRecipientAddress.find(IPPORTSEPARATOR) - (serializedRecipientAddress.find(BEGINRECIPIENTADDRESSCHAR) + 1)));
	int port = atoi( serializedRecipientAddress.substr(serializedRecipientAddress.find(IPPORTSEPARATOR) + 1, 
				 (serializedRecipientAddress.find(ENDRECIPIENTADDRESSCHAR) - (serializedRecipientAddress.find(IPPORTSEPARATOR) + 1))).c_str());

	Address senderAddress(address, port);
	return(senderAddress);
}

Address parseMessageForRecipientAddress(Message &pMessage)
{
	return(parseMessageContentsForRecipientAddress(pMessage.GetMessageContents()));
}

void addSingleEntryToRoutingTable(string serializedRoutingTable, int currBeginEntryIndex, int currEndEntryIndex, RoutingTable &messageRoutingTable)
{
	string entry = serializedRoutingTable.substr(currBeginEntryIndex, ((currEndEntryIndex - currBeginEntryIndex) + 1));

	string destinationAddressSection = entry.substr(entry.find(BEGINDESTINATIONCHARACTER) + 1, 
							   ((entry.find(ENDDESTINATIONCHARACTER) - entry.find(BEGINDESTINATIONCHARACTER)) + 1));

	string destinationAddressString = destinationAddressSection.substr(destinationAddressSection.find(BEGINDESTINATIONCHARACTER) + 1, 
					 		   (destinationAddressSection.find(IPPORTSEPARATOR) - (destinationAddressSection.find(BEGINDESTINATIONCHARACTER) + 1)));
	
	int destinationPort = atoi(destinationAddressSection.substr(destinationAddressSection.find(IPPORTSEPARATOR) + 1, 
				 		 (destinationAddressSection.find(ENDDESTINATIONCHARACTER) - (destinationAddressSection.find(IPPORTSEPARATOR) + 1))).c_str());

	Address destinationAddress(destinationAddressString, destinationPort);

	string firstHopAddressSection = entry.substr(entry.find(BEGINFIRSTHOPCHARACTER) + 1, 
						     ((entry.find(ENDFIRSTHOPCHARACTER) - entry.find(BEGINFIRSTHOPCHARACTER)) + 1));

	string firstHopAddressString = firstHopAddressSection.substr(firstHopAddressSection.find(BEGINFIRSTHOPCHARACTER) + 1, 
					 	     (firstHopAddressSection.find(IPPORTSEPARATOR) - (firstHopAddressSection.find(BEGINFIRSTHOPCHARACTER) + 1)));
	
	int firstHopPort = atoi(firstHopAddressSection.substr(firstHopAddressSection.find(IPPORTSEPARATOR) + 1, 
					   (firstHopAddressSection.find(ENDFIRSTHOPCHARACTER) - (firstHopAddressSection.find(IPPORTSEPARATOR) + 1))).c_str());

	Address firstHopAddress(firstHopAddressString, firstHopPort);

	int distance = atoi(entry.substr((entry.find(BEGINDISTANCECHARACTER) + 1), (entry.find(ENDDISTANCECHARACTER) - 1) - entry.find(BEGINDISTANCECHARACTER)).c_str());

	messageRoutingTable.AddEntry(destinationAddress, firstHopAddress, distance);
}

RoutingTable parseMessageContentsForRoutingTable(string const &pString)
{
	string RoutingTablePortion = pString.substr(pString.find(BEGINROUTINGTABLECHAR), 
						    (((pString.find(ENDROUTINGTABLECHAR)) + 1) - (pString.find(BEGINROUTINGTABLECHAR))));
	RoutingTable messageRoutingTable;

	bool allEntriesEntered;

	if(RoutingTablePortion == EMPTYROUTINGTABLE)
	{
		allEntriesEntered = true;
	}
	else
	{
		allEntriesEntered = false;

		string serializedRoutingTable = pString.substr(pString.find(BEGINROUTINGTABLECHAR) + 1, 
							    (((pString.find(ENDROUTINGTABLECHAR)) + 1) - (pString.find(BEGINROUTINGTABLECHAR))));

		int currBeginEntryIndex = serializedRoutingTable.find(BEGINENTRYCHARACTER);
		int currEndEntryIndex = serializedRoutingTable.find(ENDENTRYCHARACTER);

		while (!allEntriesEntered)
		{
			addSingleEntryToRoutingTable(serializedRoutingTable, currBeginEntryIndex, currEndEntryIndex, messageRoutingTable);
			if(serializedRoutingTable[currEndEntryIndex + 1] == BEGINENTRYCHARACTER[0])
			{
				currBeginEntryIndex = currEndEntryIndex + 1;
				currEndEntryIndex = serializedRoutingTable.find(ENDENTRYCHARACTER, currBeginEntryIndex);
			}
			else 
			{
				allEntriesEntered = true;
			}
		}
	}
	return(messageRoutingTable);
}

RoutingTable parseMessageForRoutingTable(Message &pMessage)
{
	return(parseMessageContentsForRoutingTable(pMessage.GetMessageContents()));
}

	// ========================================
	// constructors
	// ========================================

	Message::Message()
	{
		messageContents = "";
	}

	Message::Message(string pMessageContents)
	{
		messageContents = pMessageContents;
		senderCoordinates = parseMessageContentsForSenderCoordinates(pMessageContents);
		senderAddress = parseMessageContentsForSenderAddress(pMessageContents);
		recipientAddress = parseMessageContentsForRecipientAddress(pMessageContents);
		routingTable = parseMessageContentsForRoutingTable(pMessageContents);
	}

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

	string Message::GetMessageContents() const
	{
		return(messageContents);
	}

	Point Message::GetSenderCoordinates() const
	{
		return(senderCoordinates);
	}

	Address Message::GetSenderAddress() const
	{
		return(senderAddress);
	}

	Address Message::GetRecipientAddress() const
	{
		return(recipientAddress);
	}

	RoutingTable Message::GetRoutingTable() const
	{
		return(routingTable);
	}

	// ========================================
	// modifier functions
	// ========================================

	void Message::SetMessage(string pMessage)
	{
		messageContents = pMessage;
		senderCoordinates = parseMessageContentsForSenderCoordinates(pMessage);
		senderAddress = parseMessageContentsForSenderAddress(pMessage);
		recipientAddress = parseMessageContentsForRecipientAddress(pMessage);
		routingTable = parseMessageContentsForRoutingTable(pMessage);
	}

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

	ostream &operator<<(ostream &os, const Message &pMessage)
	{
		os << pMessage.messageContents;
		return(os);
	}

	istream &operator>>(istream &is, Message &pMessage)
	{
		is >> pMessage.messageContents;
		return(is);
	}
