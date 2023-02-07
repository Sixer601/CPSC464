#include "Exception.h"
#include <iostream>

using namespace std;

void insufficientCommandLineArguments(int argc)
{
	if(argc != 2)
	{
		throw(Exception((char *)"Incorrect number of command line arguments."));
	}
}

int main(int argc, char **argv)
{
	try
	{
		insufficientCommandLineArguments(argc);
	}
	catch(Exception w)
	{
		w.handle();
		return(0);
	}
	cout << "Did not run into an issue" << endl;

	return(0);
}