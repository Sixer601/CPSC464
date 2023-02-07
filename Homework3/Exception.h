#ifndef INCLUDED_Exception
#define INCLUDED_Exception

#include <iostream>

using namespace std;

class Exception
{
	private:
		char * msg;

	public:

		// PRE: 
		// POST: 
		Exception (char * message)
		{
			msg = message;
		};

		// PRE: 
		// POST: 
		void handle()
		{
			cout << "Exception: " << msg << endl;
		};

};

#endif