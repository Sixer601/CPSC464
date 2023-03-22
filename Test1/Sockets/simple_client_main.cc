#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

int main ( int argc, char * argv[] )
{
  try
    {

      ClientSocket client_socket ( "localhost", 30000 );
      //client_socket.set_non_blocking (true);

      std::string reply;
      bool done = false;
      while (!done) {
      char msg[40];
      try
	{
	  std::cin.getline(msg, 40);
	  client_socket << msg;
	  client_socket >> reply;
	  if (reply.compare("finish") == 0) {
	    done = true;
	  }
	}
      catch ( SocketException& ) {}
      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;
      }
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return 0;
}
