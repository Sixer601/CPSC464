#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "ConnectionList.h"
#include "ClientSocket.h"
#include "Constants.h"
#include "ServerSocket.h"
#include "SocketException.h"


using namespace std;

// TODO: Implement Non-Blocking Socket Implementation for Daemon.

// PRE: ipAddressList is a defined ifstream object that represents a file
// containing
//      a list of the ip addresses of the daemons active on the network.
// POST: RV is a Vector of strings that contains all ip addresses stored in
// ipAddressList
vector<string> inputIPadresses(ifstream &ipAddressList) 
{
	vector<string> data; // stores the data from ipAddressList.
	// ASSERT: data is a defaultly constructed Vector of strings.
	while (ipAddressList.peek() != EOF)
	// ASSERT: the next character from the input file is not the end of file
	// character.
  	{
	string datum; // temporary variable to store single point of data.
	// ASSERT: datum is a defaultly constructed string.
	ipAddressList >> datum;
	data.push_back(datum);
	}
	return (data);
}

// PRE: request is a defined string containing the request made from a client
// socket to the
//      daemon's server socket. That request is the first request outlined
//      inside of the document for the assignment.
// POST: The passed request has been handled.
void handleRequest1(string request) 
{
	ifstream ipAddressList(IPADDRESSFILE); // File containing list of ip addresses
								    // of the daemons on the network.
	// ASSERT: ipAddressList is the contents of the file "nodes.txt"
	vector<string> ipAddressVector = inputIPadresses(ipAddressList); // A vector of strings that contains all ip addresses
														// stored in ipAddressList.
	// ASSERT: ipAddressVector contains each address from ipAddressList.
	int firstDelimiterPos = request.find(" "); // Index of first delimiter in request.
	int secondDelimiterPos = request.find(" ", firstDelimiterPos + 1); // Index of second delimiter in request.
	int thirdDelimiterPos = request.find(" ", secondDelimiterPos + 1); // Index of third delimiter in request.
	int fourthDelimiterPos = request.find(" ", thirdDelimiterPos + 1); // Index of fourth delimiter in request.
	
	string requesterIpAddress = request.substr(firstDelimiterPos + 1, (secondDelimiterPos) - (firstDelimiterPos + 1));

	string requesterPreferredPort = request.substr(secondDelimiterPos + 1, (thirdDelimiterPos) - (secondDelimiterPos + 1));

	cout << "request 1 was made by ip address: " << requesterIpAddress  << " with a preferred port of: " << requesterPreferredPort << endl;
	
	int numProcesses = stoi(request.substr(thirdDelimiterPos + 1, (fourthDelimiterPos) - (thirdDelimiterPos + 1))); // number of processes to run in request.

	cout << "number of processes: " << numProcesses << endl;

	string programToRun = request.substr(fourthDelimiterPos + 1); // command to have processes run, with arguments.
	cout << "program to run: " << programToRun << endl;
	vector<string>::iterator it = ipAddressVector.begin(); // iterator to go through ipAddressVector.
	// ASSERT: it is a vector of strings iterator that begins at ipAddressVector.

	ConnectionList clients;

	bool isAllConnected = false; // represents if all processes have been connected to or not.
  	// ASSERT: isAllConnected begins at false, as we assume that we have not begun having connected to all the daemons.
	for (int i = 0; i < numProcesses; i++)
	// ASSERT: i begins at 0 and must be less than the number of processes to connect to.
  	{
		if (!isAllConnected)
		// ASSERT: All work has not been doled out.
		{
			Socket client; // client socket to connect to a daemon to farm out work to.
			cout << "attempting to connect to: " << *it << " on port: " << requesterPreferredPort << endl;
			bool success = client.connect((*it), stoi(requesterPreferredPort));
			if(success)
			{
				clients.AddConnection(client, true);
				++it;
				// ASSERT: the iterator for ipAddressVector is incremented.

				string idNum = to_string(i);
				if(i < DOUBLEDIGITCUTOFF)
				{
					cout << "Message sent to client: " << REQUEST2DENOTATION + programToRun + " " + FILEPREFIX + "0" + idNum + " " + idNum + " " + requesterIpAddress + " " + requesterPreferredPort << endl;
					client << (REQUEST2DENOTATION + programToRun + " " + FILEPREFIX + "0" + idNum + " " + idNum + " " + requesterIpAddress + " " + requesterPreferredPort);
				}
				else
				{
					cout << "Message sent to client: " << REQUEST2DENOTATION + programToRun + " " + FILEPREFIX + idNum + " " + idNum + " " + requesterIpAddress + " " + requesterPreferredPort << endl;
					client << (REQUEST2DENOTATION + programToRun + " " + FILEPREFIX + idNum + " " + idNum + " " + requesterIpAddress + " " + requesterPreferredPort);
				}
			}
			else
			{
				cout << "failed connection." << endl;
			}
		} 
		else 
		{
			clients.GetIthSocketInConnectionList(i % clients.GetNumConnections()) << (REQUEST2DENOTATION + programToRun);
		}

		if (it == ipAddressVector.end())
		// ASSERT: the iterator is at the end of the vector.
		{
			it = ipAddressVector.begin();
			// ASSERT: restart the iterator at the beginning.
			isAllConnected = true;
			// ASSERT: all daemons in IPADDRESSFILE have been farmed out. This means multiple jobs are given to some daemons.
			// This is due to the pigeonhole principle.
			cout << "all connected." << endl;
		}
  	}
	cout << "Request 1 Handled." << endl;
}

// PRE: request is a defined string containing the request made from a client
// socket to the
//      daemon's server socket. That request is the second request outlined
//      inside of the document for the assignment.
// POST: The passed request has been handled.
void handleRequest2(string request, bool &isChild) 
{
	int firstDelimiterPos = request.find(DELIMITER); // Index of first delimiter in request.
	string programToRun = request.substr(firstDelimiterPos + 1); // command to have processes run, with arguments.
	cout << "program to run: " << programToRun << endl;
	pid_t childPID = fork();    // process id for child process.
  	// ASSERT: childPID is the RV of fork.
	if (childPID == -1)
	// ASSERT: There was an error in the forking process.
	{
		perror("fork");
	}
	else if (childPID == 0)
  	// ASSERT: The code run inside this is for the child process.
  	{
		cout << "Child process running." << endl;
		int progFirstDelimiterPos = programToRun.find(DELIMITER); // Index of first delimiter in the program to run.
		string programName = programToRun.substr(0, progFirstDelimiterPos); // Name of the program to run.
		cout << "program name: " << programName << endl;
		const char * progName = programName.c_str();
		const char * progArgs[MAXARGS];
		bool areMoreArgs = true; //
		// ASSERT: 
		int nextDelimiterPos = programToRun.find(DELIMITER, progFirstDelimiterPos + 1); //
		// ASSERT: 
		int followingDelimiterPos = programToRun.find(DELIMITER, nextDelimiterPos + 1); //
		// ASSERT: 
		int argIterator = 0; //
		// ASSERT: 
		while (areMoreArgs) 
		// ASSERT: 
    		{
			if (nextDelimiterPos == programToRun.length()) 
			// ASSERT: 
			{
				areMoreArgs = false;
				// ASSERT: 
			} 
			else 
			{
				string programArg = programToRun.substr(nextDelimiterPos, followingDelimiterPos); //
				// ASSERT: 
				cout << "program argument: " << programArg << endl;
				const char *progArg = programArg.c_str(); //
				// ASSERT: 
				progArgs[argIterator] = progArg;
				// ASSERT: 
				nextDelimiterPos = followingDelimiterPos;
				followingDelimiterPos = programToRun.find(DELIMITER, nextDelimiterPos + 1);
				argIterator++;
			}
		}

		execv(progName, (char * const *)progArgs);
		isChild = true;
		// ASSERT:
	}
}

// PRE: server is a defined server socket used to listen for requests.
// POST:
void listenForRequests(ServerSocket server, bool &isChild) 
{
	ServerSocket clients[MAXSOCKS];
	bool active[MAXSOCKS];
	while (true && !isChild)
	// ASSERT: This loop will run until the program is forced to exit.
	{
		ServerSocket new_sock;
		server.accept(new_sock);
		try 
		{
			while (true && !isChild)
			// ASSERT: This loop will run until the program is forced to exit or there is an error.
			{
				string request; // string to hold the contents of a request made from a connection to the daemon.
				// ASSERT: request is a defaultly constructed string.
				new_sock >> request;
				cout << "request sent to me: " << request << endl;
				if (request[0] == REQUEST1CHARACTER)
				// ASSERT: Request 1 was made.
				{
					cout << "recieved a request of type 1." << endl;
					handleRequest1(request);
				}
				else if (request[0] == REQUEST2CHARACTER)
        			// ASSERT: Request 2 was made.
				{
					cout << "recieved a request of type 2." << endl;
          			handleRequest2(request, isChild);
				} 
				else
				// ASSERT: A faulty request was made.
				{
					cout << "Faulty request was made." << endl;
				}
			}
		} 
		catch (SocketException &)
		{}
	}
}

// PRE:
// POST: The ip address the daemon is running on is added to the file
// IPADDRESSFILE.
void addIPaddressToNodesFile() 
{
	char commandToAddIPaddressToNodesFile[BASESTRINGSIZE];
	snprintf(commandToAddIPaddressToNodesFile, BASESTRINGSIZE, "hostname -I | awk '{print $1}' >> %s", IPADDRESSFILE);
	int rvSystemCall = system(commandToAddIPaddressToNodesFile);
	// ASSERT: rvSystemCall is the return value of running the command in quotes
	if (rvSystemCall != 0)
	// ASSERT: The command run had an RV that was not indicative of success.
	{
		cout << "Errors in putting ip address of computer in nodes.txt" << endl;
	}
}

// PRE:
// POST:
int main(int argc, char **argv) 
{
	if (argc != 1)
	// ASSERT: The number of arguments passed to the program are not equal to 1.
	{
		cout << "Invalid Number of Arguments." << endl;
	} 
	else 
	{
		try 
		{
			ServerSocket server(PORTNUMBER); // A server socket to handle requests made to the daemon.
			// ASSERT: server is a server socket open on port PORT
			server.set_non_blocking(true);
			addIPaddressToNodesFile();
			cout << "Listening on Port: " << PORTNUMBER << endl;
			bool isChild = false; //
			// ASSERT:
			listenForRequests(server, isChild);
		} 
		catch (SocketException &e) 
		{
			cerr << e.description() << endl << "Exiting." << endl;
		}
	}
	return (0);
}