#include <iostream>
#include <string>
#include <stdlib.h>
#include "ServerSocket.h"
#include "SocketException.h"

using namespace std;

void addIPaddressToNodesFile()
{
	int rvSystemCall = system("hostname -I | awk '{print $1}' >> nodes.txt");
	if (rvSystemCall != 0)
	{
		cout << "Errors in putting ip address of computer in nodes.txt" << endl;
	}
}

void handleRequest1()
{
	// TODO: Initiate N parallel daemons.
	
	// TODO: Contact N computers running this network daemon.

	// TODO: Lookup how to contact N computers using nodes.txt

	// TODO: Send request to run a program to each computer contacted.
}

void handleRequest2()
{
	// TODO: Run specified program.

	// TODO: fork a child process to run the requested program.

	// TODO: go back to listening on port D.
}

void listenForRequests(ServerSocket &server)
{
	while (true)
	{
		ServerSocket new_sock;
		server.accept(new_sock);
		try
		{
			// TODO: Listen for a request.
			while (true)
			{
				string data;
				new_sock >> data;
				// TODO: Check what request was made.
				if(data == "1")
				{
					handleRequest1();
				}
				else if (data == "2")
				{
					handleRequest2();
				}
				else
				{
					cout << "Faulty Request." << endl;
				}
			}
		}
		catch (SocketException &)
		{
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 1)
	{
	}
	else
	{
		try
		{
			// TODO: Open up a server socket.
			ServerSocket server(30000);
			// TODO: Append IP Address to nodes.txt
			addIPaddressToNodesFile();
			// TODO: Listen for requests on server socket.
			listenForRequests(server);
		}
		catch (SocketException &e)
		{
			cerr << e.description() << "\nExiting.\n";
		}
	}
	return (0);
}