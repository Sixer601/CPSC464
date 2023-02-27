#include <iostream>
#include <string>
#include <stdlib.h>
#include "ServerSocket.h"
#include "SocketException.h"

using namespace std;

void runProgram()
{
	char *path = NULL;
	path = getcwd(path, 0);				   // get the current working directory. Since
									   // path is NULL, getcwd will allocate
									   // memory dynamically for the path
									   // string. Needs to be deleted.
									   // The returned path does not end in /
	char progName[] = "parallelMergeSort";	   // The name of the program to be executed.
	char program[100];					   // allocate space for the absolute path for
									   // the program.
	sprintf(program, "%s/%s", path, progName); // Add the / between the
									   // directory name and
									   // program name to make the
									   // correct absolute path
	free(path);						   // delete memory allocated for the path
									   // variable.
	char arg1[] = "boss";				   // Create a string for each command line
									   // argument to the program to be
									   // executed.
	char arg2[] = "testCase1.txt";
	char arg3[] = "1";
	char *nullP = (char *)NULL;			   // The arguments must end in a NULL
									   // pointer cast as a char *
	char *programArgv[4];				   // Create an array of pointers to the
									   // argument strings.
	programArgv[0] = arg1;				   // Populate the arguments.
	programArgv[1] = arg2;
	programArgv[2] = arg3;
	programArgv[3] = nullP;

	cout << "Calling " << program << " program" << endl;

	int err = execv(program, programArgv); // Execute the program with the
								    // arguments.
	if (err == -1)
	{ // This will be executed ONLY IF the execv
	  // failed, i.e., returned a -1.
		cout << "exec failed." << endl
			<< "Path = *" << path
			<< "* Command = *" << programArgv[0] << "*" << endl;
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

void handleRequest1()
{
	// TODO: Initiate N parallel daemons.

	// TODO: Contact N computers running this network daemon.

	// TODO: Lookup how to contact N computers using nodes.txt

	// TODO: Send request to run a program to each computer contacted.
}

void handleRequest2(ServerSocket &server)
{
	// TODO: fork a child process to run the requested program.
	pid_t childPID = fork(); // process id for child process.
	// ASSERT: childPID is the RV of fork.
	if (childPID == -1)
	// ASSERT: childPID is equal to -1.
	{
		perror("fork");
	}
	else if (childPID == 0)
	// ASSERT: childPID is equal to 0.
	{
		runProgram();
	}
	// TODO: go back to listening on port D.
	listenForRequests(server);
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
				if (data == "1")
				{
					handleRequest1();
				}
				else if (data == "2")
				{
					handleRequest2(server);
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
			cerr << e.description() << endl << "Exiting." << endl;
		}
	}
	return (0);
}