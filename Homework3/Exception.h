#ifndef INCLUDED_Exception
#define INCLUDED_Exception

#include <iostream>

using namespace std;

class Exception
{

	/*
	     Class Invariant (CI):
		The object is a defined exception such that msg contains
		a message detailing the exception.
	*/

	private:
		char * msg;

	public:

		// PRE: message is a defined array of characters.
		//      representing the exception message.
		// POST: msg now holds message.
		Exception (char * message)
		{
			msg = message;
		};

		// PRE: msg contains some exception message.
		// POST: msg is output to Standard Output.
		void handle()
		{
			cout << "Exception: " << msg << endl;
		};

};

#endif