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
#include "Device.h"
#include "Message.h"
#include "MessageList.h"

using namespace std;

float distance(Point point1, Point point2)
{
	float distance = sqrt((pow((point1.GetXCoord() - point2.GetXCoord()), 2.0)) + (pow((point1.GetYCoord() - point2.GetYCoord()), 2.0)));

	return(distance);
}

bool isMessageInRange(Message &pMessage, Point &myCoordinates)
{
	float messageDistance = distance(pMessage.GetSenderCoordinates(), myCoordinates);
	return(messageDistance <= SIGNALSTRENGTH);
}

void parseReply(string &pString, Point &myCoordinates, Address &myAddress, RoutingTable &myRoutingTable)
{
	bool allMessagesParsed = false;

	int currBeginMessageIndex = pString.find(BEGINMESSAGECHAR);
	int currEndMessageIndex = pString.find(ENDMESSAGECHAR);

	while(!allMessagesParsed)
	{
		Message pMessage(pString.substr(currBeginMessageIndex, (currEndMessageIndex - currBeginMessageIndex) + 1));
		if(isMessageInRange(pMessage, myCoordinates) && (distance(pMessage.GetSenderCoordinates(), myCoordinates) != 0))
		{
			Address senderAddress = pMessage.GetSenderAddress();
			if(!myRoutingTable.HasDestination(pMessage.GetSenderAddress()))
			{
				myRoutingTable.AddEntry(senderAddress, senderAddress, 1);
			}
			RoutingTable senderRoutingTable(pMessage.GetRoutingTable());
			cout << "Message we are pulling the routing table from: " << pMessage << endl;
			cout << "Sender Routing Table with " << senderRoutingTable.GetNumEntries() << " entries: " << endl;
			senderRoutingTable.PrintRoutingTable();
			for(int i = 0; i < senderRoutingTable.GetNumEntries(); i++)
			{
				if(!myRoutingTable.HasDestination(senderRoutingTable.GetNthDestination(i)) && !(senderRoutingTable.GetNthDestination(i) == myAddress))
				{
					Address ithSenderRoutingTableDestination = senderRoutingTable.GetNthDestination(i);
					
					myRoutingTable.AddEntry(ithSenderRoutingTableDestination, senderAddress, senderRoutingTable.GetNthDistance(i) + 1);
				}
			}
		}
		if(pString[currEndMessageIndex + 1] == BEGINMESSAGECHAR[0])
		{
			currBeginMessageIndex = currEndMessageIndex + 1;
			currEndMessageIndex = pString.find(ENDMESSAGECHAR, currBeginMessageIndex);
			if(currEndMessageIndex == -1)
			{
				allMessagesParsed = true;
			}
		}
		else
		{
			allMessagesParsed = true;
		}
	}
}

int main(int argc, char **argv)
{
	srand(time(NULL));

	Address myAddress(getIpAddress(), stoi(argv[1]));
	Address broadcastAddress(getIpAddress(), 0);
	Point myLocation((rand() % 100) + 1, (rand() % 100) + 1);
	cout << myLocation.Serialize() << endl;
	RoutingTable myRoutingTable;
	Device myDeviceInfo(myAddress, myLocation, myRoutingTable);

	Socket routerCommunicator(myAddress.GetAddress(), atoi(argv[2]));
	routerCommunicator.set_non_blocking(true);
	string initialHandshake = argv[1];
	string reply;

	routerCommunicator << initialHandshake;
	sleep(3);

	try 
	{
		while (true) 
		{
			try
			{
				Message message(myLocation, myAddress, broadcastAddress, myRoutingTable);
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