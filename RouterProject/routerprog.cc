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

void setupConnection(Socket * newConnection, ConnectionsList &deviceConnections, ConnectionsList &routerConnections)
{
	try
	{
	  	string message;
		*newConnection >> message;
		if(message[0] == 'r')
		{
		  string foreignRouterAddress = message.substr(1);
		  Address routerAddress(foreignRouterAddress, ROUTERPORTNUMBER);
		  routerConnections.AddConnection(newConnection, routerAddress);
		  cout << "Routers Connected: " << endl;
		  routerConnections.PrintConnectionsList();
		}
		else if (message[0] == 'd')
		  {
		    int portNum = stoi(message.substr(1));
		    Address clientAddress(getIpAddress(), portNum);
		    deviceConnections.AddConnection(newConnection, clientAddress);
		    cout << "Devices Connected: " << endl;
		    deviceConnections.PrintConnectionsList();
		  }
		else
		  {
		    cout << "bad message." << endl;
		  }
	}
	catch(SocketException &e)
	  {cerr << e.description() << endl;}
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

void broadcastMessageNetworkWide(ConnectionsList &routerConnections, string message, ConnectionsList &deviceConnections)
{
	try 
	{
		for(int j = 0; j < routerConnections.GetNumConnections(); j++)
		{
			*routerConnections.GetNthConnection(j) << message;
		}
		broadcastMessage(deviceConnections, message);
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
				broadcastMessageNetworkWide(routerConnections, message, deviceConnections);
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

void updateRouterConnections(ConnectionsList &routerConnections, Address &myAddress)
{
	ifstream ipAddressList(ROUTERIPFILE);
	while (ipAddressList.peek() != EOF)
  	{
		string datum;
		ipAddressList >> datum;
		if(datum != "" && !(datum == myAddress.GetAddress()))
		{
 		  Address routerAddress(datum, ROUTERPORTNUMBER);
		  if(!routerConnections.HasConnectionWithAddress(routerAddress))
		  {
		    	Socket * routerCommunicator;
			routerCommunicator = new Socket(datum, ROUTERPORTNUMBER);
			routerCommunicator->set_non_blocking(true);
			*routerCommunicator << "r" + myAddress.GetAddress();
			routerConnections.AddConnection(routerCommunicator, routerAddress);
			routerConnections.PrintConnectionsList();
		  }
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

void listenForRequests(Socket &pListener, ConnectionsList &deviceConnections, ConnectionsList &routerConnections, Address &myAddress)
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
	  setupConnection(newConnection, deviceConnections, routerConnections);
	  newConnection->set_non_blocking(true);
	}
	else
	{
	  delete newConnection;
	}
	listenForClientMessages(deviceConnections, routerConnections, message);
	updateRouterConnections(routerConnections, myAddress);
	listenForRouterMessages(routerConnections, deviceConnections, message);
	sleep(2);
  }
}

void writeAddressToSharedFile()
{
  char commandToAddIPaddressToRoutersFile[BASESTRINGSIZE];
  snprintf(commandToAddIPaddressToRoutersFile, BASESTRINGSIZE, "hostname -I | awk '{print $1}' >> %s", ROUTERIPFILE);
  int rvSystemCall = system(commandToAddIPaddressToRoutersFile);
  if(rvSystemCall != 0)
    {
      cout << "Errors in putting ip address of computer in " << ROUTERIPFILE << endl;
    }
}

int main(int argc, char **argv)
{
	Socket listener(ROUTERPORTNUMBER);
	listener.set_non_blocking(true);
	ConnectionsList deviceConnections;
	ConnectionsList routerConnections;

	Address routerAddress(getIpAddress(), ROUTERPORTNUMBER);
	writeAddressToSharedFile();
	
	cout << "Listening on " << routerAddress.Serialize() << endl;
	listenForRequests(listener, deviceConnections, routerConnections, routerAddress);

	return(0);
}
