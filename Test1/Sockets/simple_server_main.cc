#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;
#define MAXCLIENTS 5

int main ( int argc, char * argv[] )
{
  std::cout << "running....\n";

  try
    {
      std::string finishString("finish");
      // Create the socket
      ServerSocket server ( 30000 );
      server.set_non_blocking (true);
	 ServerSocket clients[MAXCLIENTS];
	 bool active[MAXCLIENTS];
	 for (int i = 0; i < MAXCLIENTS; i++) {
		clients[i].set_non_blocking(true);
		active[i] = false;
	 }
	 int numClients = 0;
      while ( true ) {
	  ServerSocket client;
	  //client.set_non_blocking(true);
	  try {
	    bool success = server.accept (clients[numClients]);
	    if(success)
	    {
		 cout << "Adding client to list of clients." << endl;
		 active[numClients] = true;
		 numClients++;
		 cout << "Size of client list: " << numClients << endl;
	    }
	  }
	  catch ( SocketException& )
	    {}
	  for (int c = 0; c < numClients; c++) {
		//cout << "Size of client list in for loop: " << clients.size() << endl;
	    if(active[c])
	    {
	    try
	      {
		// std::cout << c << " ";
		std::string data;
		clients[c] >> data;
		cout << "Got message " << data << endl;
		clients[c] << finishString;
		active[c] = false;
	      }
	    catch ( SocketException& )
	      {}
	  }
	  }
      }
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}
