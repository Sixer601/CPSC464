#include <iostream>

using namespace std;

void handleRequest1()
{
	// TODO: Initiate N parallel demons.

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

void listenForRequests()
{
	while(true)
	{
		// TODO: Listen for a request. 

		// TODO: Check what request was made 
		(true) ? handleRequest1() : handleRequest2();
	}
}

int main(int argc, char **argv)
{
	// TODO: Open up a server socket.

	// TODO: Append IP Address to nodes.txt

	// TODO: Listen for requests on server socket.

		// TODO: Handle request 1 made on server socket.

		// TODO: Handle request 2 made on server socket.

	// TODO: Remove IP Address from nodes.txt

	return(0);
}