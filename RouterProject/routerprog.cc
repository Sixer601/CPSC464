#include <cerrno>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

#include "Address.h"
#include "Socket.h"
#include "SocketException.h"
#include "Constants.h"
#include "HelperFunctions.h"
#include "ConnectionsList.h"
#include "Point.h"
#include "RoutingTable.h"
#include "Message.h"

using namespace std;

void setupConnection(Socket * newConnection, ConnectionsList &connections)
{
	newConnection->set_non_blocking(true);
	string clientMessage;
	*newConnection >> clientMessage;
	int portNum = stoi(clientMessage.substr(0, clientMessage.find(DELIMITER)));
	Address clientAddress(getIpAddress(), portNum);
	connections.AddConnection(newConnection, clientAddress);	
	connections.PrintConnectionsList();
}

void broadcastMessageNetworkWide(ConnectionsList &routerConnections, string message)
{
	try 
	{
		for(int j = 0; j < routerConnections.GetNumConnections(); j++)
		{
			*routerConnections.GetNthConnection(j) << message;
		}
	} 
	catch (SocketException &e)
	{}
}

void broadcastMessage(ConnectionsList &connections, string message)
{
	try
	{
		for(int j = 0; j < connections.GetNumConnections(); j++)
		{
			*connections.GetNthConnection(j) << message;
		}
	}
	catch (SocketException &e)
	{}
}

void sendMessage(ConnectionsList &connections, string message, int recipientPortNum)
{
	try
	{
		string messageContents = message.substr(message.find(DELIMITER) + 1);
		*connections.GetConnectionWithPort(recipientPortNum) << messageContents;
	}
	catch (SocketException &e)
	{}
}

void listenForClientMessages(ConnectionsList &deviceConnections, ConnectionsList &routerConnections, string message)
{
	for(int i = 0; i < deviceConnections.GetNumConnections(); i++)
	{
		try
		{
			*(deviceConnections.GetNthConnection(i)) >> message;
			Message messageContents(message);
			
			Address recipientAddress(messageContents.GetRecipientAddress());
			if(recipientAddress.GetAddress() == to_string(BROADCASTIPADDRESS))
			{
				broadcastMessageNetworkWide(routerConnections, message);
			}
			else if(recipientAddress.GetPort() == 0)
			{
				broadcastMessage(deviceConnections, message);
			}
			else if(deviceConnections.HasConnectionWithAddress(recipientAddress))
			{
				sendMessage(deviceConnections, message, recipientAddress.GetPort());
			}
		}
		catch (SocketException &e)
		{}
	}
}

void updateRouterConnections(ConnectionsList &routerConnections)
{
	ifstream ipAddressList(ROUTERIPFILE);
	while (ipAddressList.peek() != EOF)
  	{
		string datum;
		ipAddressList >> datum;
		Address routerAddress(datum, ROUTERPORTNUMBER);
		Socket * routerCommunicator;
		routerCommunicator = new Socket(datum, ROUTERPORTNUMBER);
		routerCommunicator->set_non_blocking(true);
		if(!routerConnections.HasConnectionWithAddress(routerAddress))
		{
			routerConnections.AddConnection(routerCommunicator, routerAddress);
		}
	}
}

void listenForRouterMessages(ConnectionsList &routerConnections, ConnectionsList &deviceConnections, string message)
{
	for(int i = 0; i < routerConnections.GetNumConnections(); i++)
	{
		try
		{
			*(routerConnections.GetNthConnection(i)) >> message;
			Message messageContents(message);
			
			int recipientPortNum = messageContents.GetRecipientAddress().GetPort();
			if(recipientPortNum == 0)
			{
				broadcastMessage(deviceConnections, message);
			}
			else if(deviceConnections.HasConnectionWithAddress(messageContents.GetRecipientAddress()))
			{
				sendMessage(deviceConnections, message, recipientPortNum);
			}
		}
		catch (SocketException &e)
		{}
	}
}

void listenForRequests(Socket &pListener, ConnectionsList &deviceConnections, ConnectionsList &routerConnections)
{
	while (true) 
  	// ASSERT: loop indefinitely.
     {
		string message;
     	Socket * newConnection = new Socket(); // socket to handle when a connection with another computer is made.
		// ASSERT: newConnection is an empty socket.
     	bool success = pListener.accept(*newConnection); // sanity check to ensure code runs 
	 										   // when accepting the connection is successful.
		// ASSERT: success is true if listener accepted the socket and false when listener did not.
     	if(success)
		// ASSERT: accepting of newConnection by listener was successful.
		{
			setupConnection(newConnection, deviceConnections);
		}
		listenForClientMessages(deviceConnections, routerConnections, message);
		updateRouterConnections(routerConnections);
		listenForRouterMessages(routerConnections, deviceConnections, message);
		int numRouters;
		sleep(2);
   	}
}

void writeAddressToSharedFile(Address &myAddress, string sharedFilePath)
{
	ofstream sharedFile(sharedFilePath);
	sharedFile << myAddress.Serialize() << endl;
	sharedFile.close();
}

int main(int argc, char **argv)
{
	Socket listener(atoi(argv[1]));
	listener.set_non_blocking(true);
	ConnectionsList deviceConnections;
	ConnectionsList routerConnections;

	Address routerAddress(getIpAddress(), atoi(argv[1]));
	writeAddressToSharedFile(routerAddress, ROUTERIPFILE);
	cout << "Listening on " << routerAddress.Serialize() << endl;
	listenForRequests(listener, deviceConnections, routerConnections);

	return(0);
}