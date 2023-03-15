#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <string.h>
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "SocketException.h"

using namespace std;

vector<string> inputIPadresses(ifstream& ipAddressList)
{
	vector<string> data;
	while (ipAddressList.peek() != EOF)
	// ASSERT: the next character from the input file is not the end of file character.
	{
		string datum; // temporary variable to store single point of data.
		// ASSERT: datum is undefined.
		ipAddressList >> datum;
		data.push_back(datum);
	}
	return (data);
}

void handleRequest1(string request)
{
	ifstream ipAddressList("nodes.txt");
	vector<string> ipAddressVector = inputIPadresses(ipAddressList);
	int firstDelimiterPos = request.find(" ");
	int secondDelimiterPos = request.find(" ", firstDelimiterPos + 1);

	int numProcesses = stoi(request.substr(firstDelimiterPos + 1, secondDelimiterPos));
	string programToRun = request.substr(secondDelimiterPos + 1);

	vector<string>::iterator it = ipAddressVector.begin();
	vector<ClientSocket> clientSocketVector; 
	
	bool isAllConnected = false;

	for (int i = 0; i < numProcesses; i++)
	{
		if(!isAllConnected)
		{
			ClientSocket socket( (*it).c_str(), 30000 );
			++it;
			clientSocketVector.push_back(socket);
			
			socket << ("2 " + programToRun);
		}
		else
		{
			clientSocketVector[i % clientSocketVector.size()] << ("2 " + programToRun);
		}

		if(it == ipAddressVector.end())
		{
			it = ipAddressVector.begin();
			isAllConnected = true;
		}	
	}
}

void handleRequest2(string request)
{
	int firstDelimiterPos = request.find(" ");
	string programToRun = request.substr(firstDelimiterPos + 1);
	pid_t childPID = fork(); // process id for child process.
	// ASSERT: childPID is the RV of fork.
	if (childPID == -1)
	// ASSERT: There was an error in the forking process.
	{
		perror("fork");
	}
	else if (childPID == 0)
	// ASSERT: The code run inside this is for the child process.
	{
		// TODO: Ask shende about how to handle path to program.
		int progFirstDelimiterPos = programToRun.find(" ");
		string progName = programToRun.substr(0, progFirstDelimiterPos);
		string progArgs = programToRun.substr(progFirstDelimiterPos + 1);
		// TODO: Ask shende about how to handle arguments for program.
		//execv(progName.c_str(), progArgs.c_str());
		// TODO: Ask shende about how to kill child process "gracefully".
	}
}

void listenForRequests(ServerSocket server)
{
	while (true)
	{
		ServerSocket new_sock;
		server.accept(new_sock);
		try
		{
			while (true)
			{
				string request;
				new_sock >> request;
				if (request[0] == '1')
				// ASSERT: Request 1 was made.
				{
					handleRequest1(request);
				}
				else if (request[0] == '2')
				// ASSERT: Request 2 was made.
				{
					handleRequest2(request);
				}
				else
				// ASSERT: A faulty request was made.
				{
					cout << "Faulty request was made." << endl;
				}
			}
		}
		catch (SocketException &)
		{
		}
	}
}

void addIPaddressToNodesFile()
{
	int rvSystemCall = system("hostname -I | awk '{print $1}' >> nodes.txt");
	if (rvSystemCall != 0)
	{
		cout << "Errors in putting ip address of computer in nodes.txt" << endl;
	}
}

int main(int argc, char **argv)
{
	if (argc != 1)
	{
		cout << "Invalid Number of Arguments." << endl;
	}
	else
	{
		try
		{
			ServerSocket server(30000);
			addIPaddressToNodesFile();
			cout << "Listening for requests on port 30000" << endl;
			listenForRequests(server);
		}
		catch (SocketException &e)
		{
			cerr << e.description() << endl
				<< "Exiting." << endl;
		}
	}
	return (0);
}