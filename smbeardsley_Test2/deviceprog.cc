#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#include "Socket.h"
#include "SocketException.h"
#include "Constants.h"
#include "HelperFunctions.h"
#include "Point.h"
#include "Address.h"
#include "RoutingTable.h"
#include "Message.h"

using namespace std;

// PRE: point1 and point2 are defined point objects to determine the distance between them.
// POST: RV is the distance between point1 and point2.
float distance(Point point1, Point point2)
{
	float distance = sqrt((pow((point1.GetXCoord() - point2.GetXCoord()), 2.0)) + (pow((point1.GetYCoord() - point2.GetYCoord()), 2.0)));
	// value to be returned.
	// ASSERT: distance is equal to the result of the distance formula performed on point1 and point2.
	return(distance);
}

// PRE: pMessage is a defined Message object containing coordinates of the sender to compare to, to determine if the devices are in range of each other.
//      myCoordinates is a defined Point object containing the coordinates of this device.
// POST: RV is true if the devices are in range of each other to recieve messages directly from one another, and false if that is not the case.
bool isMessageInRange(Message &pMessage, Point &myCoordinates)
{
	float messageDistance = distance(pMessage.GetSenderCoordinates(), myCoordinates); // float denoting the distance between devices.
	// ASSERT: messageDistance is equal to the RV of the distance function being passed the message's sender's coordinates, and myCoordinates.
	return(messageDistance <= SIGNALSTRENGTH);
}

// PRE: currBeginMessageIndex is a defined integer representing the beginning of the message being parsed.
//      currEndMessageIndex is a defined integer representing the end of the message being parsed.
//      pString is a defined string object containing the message being parsed. allMessagesParsed is a defined boolean object denoting if 
//      all messages have been parsed or not.
// POST: currBeginMessageIndex, currEndMessageIndex and allMessagesParsed are updated based upon if there is another message, and where in
//       pString that message resides if it exists.
void updateIndicesAndAllParsed(int &currBeginMessageIndex, int &currEndMessageIndex, const string &pString, bool &allMessagesParsed)
{
	if(pString[currEndMessageIndex + 1] == BEGINMESSAGECHAR[0])
	// ASSERT: the index of pString equal to the sum of 1 and currEndMessageIndex is equal to the begin message character.
	{
		currBeginMessageIndex = currEndMessageIndex + 1;
		// ASSERT: currBeginMessageIndex is equal to the index following currEndMessageIndex.
		currEndMessageIndex = pString.find(ENDMESSAGECHAR, currBeginMessageIndex);
		// ASSERT: currEndMessageIndex is equal to the first instance of ENDMESSAGECHAR in pString starting at currBeginMessageIndex.
		if(currEndMessageIndex == -1)
		// ASSERT: currEndMessageIndex is out of range.
		{
			allMessagesParsed = true;
			// ASSERT: there are no messages to parse left.
		}
	}
	else
	// ASSERT: 
	{
		allMessagesParsed = true;
		// ASSERT: all messages have been parsed.
	}
}

// PRE: pString is a defined string object containing a series of messages to parse through and update myRoutingTable with.
//      myCoordinates is a defined point object containing this device's coordinates.
//      myAddress is a defined address object containing this device's address.
//      myRoutingTable is a defined routing table object containing this device's routing table.
// POST: pString's content has been handled and myRoutingTable has been updated accoringly.
void parseReply(string &pString, Point &myCoordinates, Address &myAddress, RoutingTable &myRoutingTable)
{
	bool allMessagesParsed = false; // bool to represent if all messages in pString have been handled.
	// ASSERT: allMessagesParsed begins as false.
	int currBeginMessageIndex = pString.find(BEGINMESSAGECHAR); // integer to keep track of beginning of current message being handled.
	// ASSERT: currBeginMessageIndex is the index where BEGINMESSAGECHAR first appears in pString.
	int currEndMessageIndex = pString.find(ENDMESSAGECHAR); // integer to keep track of end of current message being handled.
	// ASSERT: currEndMessageIndex is the index where ENDMESSAGECHAR first appears in pString.
	while(!allMessagesParsed)
	// ASSERT: all messages have not been parsed.
	{
		Message pMessage(pString.substr(currBeginMessageIndex, (currEndMessageIndex - currBeginMessageIndex) + 1));
		// Message object to hold a single message to handle.
		// ASSERT: pMessage contains the contents of pString from currBeginMessageIndex to currEndMessageIndex.
		if(isMessageInRange(pMessage, myCoordinates) && (distance(pMessage.GetSenderCoordinates(), myCoordinates) != 0))
		// ASSERT: the message is both in range and not originating from this device's coordinates.
		{
			Address senderAddress = pMessage.GetSenderAddress(); // address object to hold the sender's address.
			// ASSERT: senderAddress is the RV of GetSenderAddress method from pMessage.
			if(!myRoutingTable.HasDestination(pMessage.GetSenderAddress()))
			// ASSERT: this device's routing table does not contain senderAddress in its destinations.
			{
				myRoutingTable.AddEntry(senderAddress, senderAddress, 1);
			}
			RoutingTable senderRoutingTable(pMessage.GetRoutingTable()); // object to store the device that sent the message's routing table.
			// ASSERT: senderRoutingTable is constructed using the RV of GetRoutingTable from pMessage.
			for(int i = 0; i < senderRoutingTable.GetNumEntries(); i++)
			// ASSERT: i is less than the number of entries in senderRoutingTable.
			{
				if(!myRoutingTable.HasDestination(senderRoutingTable.GetNthDestination(i)) && !(senderRoutingTable.GetNthDestination(i) == myAddress))
				// ASSERT: this device's routing table does not already have the destination stored in the ith entry of senderRoutingTable and
				//         the ith entry of senderRoutingTable is not this device's address.
				{
					Address ithSenderRoutingTableDestination = senderRoutingTable.GetNthDestination(i);
					// address object containing the ith destination in senderRoutingTable.
					// ASSERT: ithSenderRoutingTableDestination contains the destination address stored in the ith entry of senderRoutingTable.
					myRoutingTable.AddEntry(ithSenderRoutingTableDestination, senderAddress, senderRoutingTable.GetNthDistance(i) + 1);
				}
			}
		}
		updateIndicesAndAllParsed(currBeginMessageIndex, currEndMessageIndex, pString, allMessagesParsed);
	}
}

// PRE: 
// POST: 
int main(int argc, char **argv)
{
	srand(time(NULL));
	// ASSERT: Picks random seed based on the current time when this line of code is invoked.
	Address myAddress(getIpAddress(), stoi(argv[1])); // address object storing this device's address.
	// ASSERT: myAddress is a defined address object constructed using the RV of getIpAddress and 
	//         the integer form of argv[1] as constructor parameters.
	Address broadcastAddress(BROADCASTIPADDRESS, BROADCASTPORT); // address to use to send discovery 
													 // messages across the network.
	// ASSERT: broadcastAddress is a defined address object constructed using BROADCASTIPADDRESS and
	//         BROADCASTPORT as constructor parameters.
	Point myLocation((rand() % 100) + 1, (rand() % 100) + 1); // point object storing this device's location.
	// ASSERT: myLocation is a defined point object constructed using 2 random numbers from 1 to 100 as 
	//         constructor parameters.
	cout << myLocation.Serialize() << endl;
	RoutingTable myRoutingTable; // routing table object storing this device's routing table.
	// ASSERT: myRoutingTable is a defaultly constructed routing table.
	Socket routerCommunicator(myAddress.GetAddress(), ROUTERPORTNUMBER); // socket to communicate with router on same machine.
	// ASSERT: routerCommunicator is a defined socket object passed the RV of GetAddress for myAddress and ROUTERPORTNUMBER as
	//         constructor parameters.
	routerCommunicator.set_non_blocking(true);
	// ASSERT: routerCommunicator is set to non-blocking.
	string initialHandshake = argv[1]; // string to hold what will be passed to the router to denote it is listening on a port.
	// ASSERT: initialHandshake contains the port of the device.
	string reply; // string to hold replies from router.
	// ASSERT: reply is a defaultly constructed string object.
	routerCommunicator << (DEVICEINITIALMESSAGEHEADERCHARACTER + initialHandshake).c_str();
	sleep(3);
	try 
	{
		while (true) 
		// ASSERT: this will loop indefinitely.
		{
			try
			{
				Message message(myLocation, myAddress, broadcastAddress, myRoutingTable); // discovery message
				// ASSERT: message is a defined message object passed myLocation, myAddress, broadcastAddress, and myRoutingTable
				//         as constructor parameters.
				routerCommunicator << message.GetMessageContents();
				routerCommunicator >> reply;
				parseReply(reply, myLocation, myAddress, myRoutingTable);
				cout << "My Routing Table: " << endl;
				myRoutingTable.PrintRoutingTable();
			}
			catch(SocketException &e)
			{}
			sleep(5);
		}
	} 
	catch (SocketException &e) 
	{}
	return(0);
}
