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

// PRE: newConnection is a defined pointer to a socket object that points to a new connection that has been made to
//      this router. deviceConnections is a defined connections list object containing the list of connections made 
//      by devices to this router. routerConnections is a defined connections list object containing the list of 
//      connections made by routers to this router.
// POST: the connection made to this router is now stored within the appropriate list, based upon if the connection
//       was made by a router or by a device.
void setupConnection(Socket * newConnection, ConnectionsList &deviceConnections, ConnectionsList &routerConnections)
{
	try
	{
		string message; // the initial handshake message made by the new connection.
		// ASSERT: message begins as a defaultly constructed string object.
		*newConnection >> message;
		if(message[0] == ROUTERINITIALMESSAGEHEADERCHARACTER)
		// ASSERT: the first character of the message received denotes this message originated from a router.
		{
			string foreignRouterAddress = message.substr(1); // the string form of the IP address of the router this
		  										    // initial handshake message came from.
			// ASSERT: foreignRouterAddress is the contents of message except for the character denoting the mesage
			//         came from a router.
			Address routerAddress(foreignRouterAddress, ROUTERPORTNUMBER); // address object denoting the address of 
		  													   // the router that connected to this router.
		  // ASSERT: routerAddress is an address object containing foreignRouterAddress as the address and 
		  //         ROUTERPORTNUMBER as the port.
		  routerConnections.AddConnection(newConnection, routerAddress);
		  cout << "Routers Connected: " << endl;
		  routerConnections.PrintConnectionsList();
		}
		else if (message[0] == DEVICEINITIALMESSAGEHEADERCHARACTER)
		// ASSERT: the first character of the message received denotes this message originated from a device.
		{
			int portNum = stoi(message.substr(1)); // the port of the device that connected to this router.
			// ASSERT: portNum is equal to the integer equivalent of the contents of message except for 
			//         the character denoting the message originated from a device.
			Address deviceAddress(getIpAddress(), portNum); // address object denoting the address of the device that
												   // connected to this router.
			// ASSERT: deviceAddress is an address object containing this router's ip address as the address and 
			//         portNum as the port.
			deviceConnections.AddConnection(newConnection, deviceAddress);
			cout << "Devices Connected: " << endl;
			deviceConnections.PrintConnectionsList();
		}
		else
		// ASSERT: the message is foreign to this router.
		{
			cout << "bad message received." << endl;
		}
	}
	catch(SocketException &e)
	{
		cerr << e.description() << endl;
	}
}

// PRE: connections is a defined connections list object. message is a defined string object that contains the message
//      to be broadcast.
// POST: message is sent to every connection to the router in connections.
void broadcastMessage(ConnectionsList &connections, string message)
{
	try
	{
		for(int j = 0; j < connections.GetNumConnections(); j++)
		// ASSERT: j is less than the number of connections in connections.
		{
			*connections.GetNthConnection(j) << message;
		}
	}
	catch (SocketException &e)
	{}
}

// PRE: routerConnections is a defined connections list object containing the list of routers connected to this router.
//      message is a defined string object containing the message to be broadcast to every device on the network.
//      deviceConnections is a defined connections list object containing the list of devices connected to this router.
// POST: message is sent to every device locally and to all routers, with information to have them broadcast the message
//       to all of their local devices.
void broadcastMessageNetworkWide(ConnectionsList &routerConnections, string message, ConnectionsList &deviceConnections)
{
	try 
	{
		for(int j = 0; j < routerConnections.GetNumConnections(); j++)
		// ASSERT: j is less than the number of routers connected to this router.
		{
			*routerConnections.GetNthConnection(j) << message;
		}
		broadcastMessage(deviceConnections, message);
	} 
	catch (SocketException &e)
	{}
}

// PRE: deviceConnections is a defined connections list object containing the list that contains the device to send message to.
//      message is a defined string object containing the message to be sent to a specific device on the network.
//      recipientAddress is a defined address object denoting the address of the device to send the message to.
// POST: the connected device 
void sendMessage(ConnectionsList &deviceConnections, string message, Address recipientAddress, ConnectionsList &routerConnections)
{
	try
	{
		if(recipientAddress.GetAddress() == getIpAddress())
		// ASSERT: the recipient of the message belongs to this router's subnetwork.
		{
			*deviceConnections.GetConnectionWithAddress(recipientAddress) << message;
		}
		else if(routerConnections.HasConnectionWithAddress(recipientAddress))
		// ASSERT: the recipient of the message belongs outside of this router's subnetwork.
		{
			*routerConnections.GetConnectionWithAddress(recipientAddress) << message;
		}
		else 
		// ASSERT: the recipient of the message belongs outside of the entirety of the network.
		{
			cout << "This message is meant for a device outside of the network." << endl;
		}
	}
	catch (SocketException &e)
	{}
}

// PRE: deviceConnections is a defined connections list object containing the devices that are connected locally to this router.
//      routerConnections is a defined connections list object containing the routers that are connected to this router.
//      message is a defined string object that will hold messages from clients.
// POST: any messages sent by devices are handled appropriately.
void listenForClientMessages(ConnectionsList &deviceConnections, ConnectionsList &routerConnections, string message)
{
	for(int i = 0; i < deviceConnections.GetNumConnections(); i++)
	// ASSERT: i is less than the number of devices connected to this router.
	{
		try
		{
			*(deviceConnections.GetNthConnection(i)) >> message;
			Message messageContents(message); // the message object holding the contents of message.
			// ASSERT: messageContents is a constructed Message object having been passed message as its parameter.
			Address recipientAddress(messageContents.GetRecipientAddress()); // Address object to contain the address of who
																// the message is intended for.
			// ASSERT: recipientAddress is constructed using the copy constructor, being passed the recipient address stored in
			//         messageContents as its parameter.
			if(recipientAddress.GetAddress() == BROADCASTIPADDRESS)
			// ASSERT: the address provided is the broadcast address to send the message to every device on the network.
			{
				broadcastMessageNetworkWide(routerConnections, message, deviceConnections);
			}
			else if(recipientAddress.GetPort() == BROADCASTPORT)
			// ASSERT: the port provided is the broadcast port to send the message to every device on this router's subnetwork.
			{
				broadcastMessage(deviceConnections, message);
			}
			else
			// ASSERT: this message is intended for a particular device. 
			{
				sendMessage(deviceConnections, message, recipientAddress, routerConnections);
			}
		}
		catch (SocketException &e)
		{}
	}
}

// PRE: routerConnections is a defined connections list object containing the list of routers connected to this router.
//      myAddress is a defined address object containing the IP and port of this router.
// POST: routerConnections contains all routers listed in ROUTERIPFILE.
void updateRouterConnections(ConnectionsList &routerConnections, Address &myAddress)
{
	ifstream ipAddressList(ROUTERIPFILE); // defined ifstream object containing the file for the list of routers that are
								   // a part of the network.
	// ASSERT: ipAddressList contains the contents of ROUTERIPFILE.
	while (ipAddressList.peek() != EOF)
  	// ASSERT: ipAddressList has not reached the end of file character.
	{
		string datum; // string that will contain a single IP address.
		// ASSERT: datum is a defaultly constructed string object.
		ipAddressList >> datum;
		if(datum != "" && !(datum == myAddress.GetAddress()))
		// ASSERT: datum is neither empty nor this router's IP address. 
		{
 			Address routerAddress(datum, ROUTERPORTNUMBER); // address object that contains the router that is listed in 
		  										   // ipAddressList.
		  	// ASSERT: routerAddress is an Address object passed datum and ROUTERPORTNUMBER as its 2 constructor parameters.
		  	if(!routerConnections.HasConnectionWithAddress(routerAddress))
		  	// ASSERT: routerConnections does not have a connection with the address stored in routerAddress.
		  	{
				Socket * routerCommunicator; // socket to communicate with the router listed in ipAddressList.
				// ASSERT: routerCommunicator is a pointer to a socket object.
				routerCommunicator = new Socket(datum, ROUTERPORTNUMBER);
				// ASSERT: routerCommunicator is a clientSocket passed datum and ROUTERPORTNUMBER as its constructor parameters.
				routerCommunicator->set_non_blocking(true);
				// ASSERT: routerCommunicator is a non-blocking socket.
				*routerCommunicator << ROUTERINITIALMESSAGEHEADERCHARACTER + myAddress.GetAddress();
				routerConnections.AddConnection(routerCommunicator, routerAddress);
				routerConnections.PrintConnectionsList();
		  	}
		}
	}
}

// PRE: routerConnections is a defined ConnectionsList object containing the list of routers connected to this router.
//      deviceConnections is a defined ConnectionsList object containing the list of devices connected to this router.
//      message is a defined string object that will store messages received from routers in routerConnections.
// POST: any messages sent by routers are handled appropriately.
void listenForRouterMessages(ConnectionsList &routerConnections, ConnectionsList &deviceConnections, string message)
{
	for(int i = 0; i < routerConnections.GetNumConnections(); i++)
	// ASSERT: i is less than the number of connected routers.
	{
		try
		{
			*(routerConnections.GetNthConnection(i)) >> message;
			Message messageContents(message); // message object to store information contained in message.
			// ASSERT: messageContents is a defined message object passed message as its constructor parameter.
			Address recipientAddress = messageContents.GetRecipientAddress(); // address of the recipient of message.
			// ASSERT: recipientAddress is the recipient address in messageContents.
			if(recipientAddress.GetPort() == BROADCASTPORT)
			// ASSERT: The message received is meant to be broadcast on this router's subnetwork.
			{
				broadcastMessage(deviceConnections, message);
			}
			else
			// ASSERT: The message received is meant to be sent to a particular device. 
			{
				sendMessage(deviceConnections, message, recipientAddress, routerConnections);
			}
		}
		catch (SocketException &e)
		{}
	}
}

// PRE: pListener is a defined socket object that contains the listener server socket.
//      deviceConnections is a defined connections list object that contains a list of the devices connected to this router.
//      routerConnections is a defined connections list object that contains a list of the routers connected to this router.
//      myAddress is a defined address object containing the address of this router.
// POST: Any requests made to this router are handled appropriately.
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
			// ASSERT: newConnection is a non-blocking connection.
		}
		else
		// ASSERT: accepting of newConnection by listener was not successful.
		{
	  		delete newConnection;
		}
		listenForClientMessages(deviceConnections, routerConnections, message);
		updateRouterConnections(routerConnections, myAddress);
		listenForRouterMessages(routerConnections, deviceConnections, message);
		sleep(2);
  	}
}

// PRE: 
// POST: This router's IP Address is written to ROUTERIPFILE. 
void writeAddressToSharedFile()
{
	char commandToAddIPaddressToRoutersFile[BASESTRINGSIZE]; // character array to store system command.
	// ASSERT: commandToAddIPaddressToRoutersFile is a defaultly constructed character array of size BASESTRINGSIZE.
  	snprintf(commandToAddIPaddressToRoutersFile, BASESTRINGSIZE, "hostname -I | awk '{print $1}' >> %s", ROUTERIPFILE);
	// ASSERT: commandToAddIPaddressToRoutersFile contains the system command to add this router's ip address to ROUTERIPFILE.
  	int rvSystemCall = system(commandToAddIPaddressToRoutersFile); // integer to store the RV of the system command.
	// ASSERT: int is 0 if the system command succeeded, and another number if it failed.
  	if(rvSystemCall != 0)
     // ASSERT: rvSystemCall is not equal to 0.
	{
     	cout << "Errors in putting ip address of computer in " << ROUTERIPFILE << endl;
    	}
}

// PRE: 
// POST: 
int main(int argc, char **argv)
{
	Socket listener(ROUTERPORTNUMBER); // socket object to listen for connections.
	// ASSERT: listener is a server with ROUTERPORTNUMBER as its open port.
	listener.set_non_blocking(true);
	// ASSERT: listener is non-blocking.
	ConnectionsList deviceConnections; // list of devices connected to this router.
	// ASSERT: deviceConnections is a defaultly constructed ConnectionsList object.
	ConnectionsList routerConnections; // list of routers connected to this router.
	// ASSERT: routerConnections is a defaultly constructed ConnectionsList object.
	Address routerAddress(getIpAddress(), ROUTERPORTNUMBER); // this router's IP address and port.
	// ASSERT: routerAddress is a defined Address object with the RV of getIpAddress and ROUTERPORTNUMBER as its constructer parameters.
	writeAddressToSharedFile();
	cout << "Listening on " << routerAddress.Serialize() << endl;
	listenForRequests(listener, deviceConnections, routerConnections, routerAddress);
	return(0);
}
