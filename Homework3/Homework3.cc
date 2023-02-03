#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include "IntArray.h"

using namespace std;

// PRE:
// POST:
void inputData(istream &pInputFile, bool debugMode)
{
	cout << "Entered inputData." << endl;

	// Input data from file
	IntArray data;
	while (pInputFile.peek() != EOF)
	{
		int datum;
		pInputFile >> datum;
		data.addInt(datum);
	}

	if (debugMode)
	{
		cout << "data: ";
		for (int i = 0; i < data.getContentLength(); i++)
		{
			cout << data.getNthIntInArray(i) << " ";
		}
		cout << endl;
	}
}

// PRE:
// POST:
void createChildProcesses()
{
	cout << "Entered createChildProcesses." << endl;
}

// PRE:
// POST:
void handleJobs()
{
	cout << "Entered handleJobs." << endl;
}

// PRE:
// POST:
int main(int argc, char **argv)
{

	if (argc != 4)
	{
		cout << "Invalid number of arguments." << endl;
	}
	else
	{
		bool debugMode;
		int numProcesses = stoi(argv[1]);
		ifstream inputFile(argv[2]);
		int thirdArg = stoi(argv[3]);
		if(thirdArg == 1 || thirdArg == 0) {
			if(thirdArg == 1) {
				debugMode = true;
			}
			else {
				debugMode = false;
			}

			cout << "number of processes required: " << numProcesses << endl;

			// Input file data into shared memory
			inputData(inputFile, debugMode);

			// Create child processes
			createChildProcesses();

			// Begin handling chunks/jobs
			handleJobs();
		}
		else {
			cout << "Enter a 1 or 0 for debug mode." << endl;
		}
	}

	return (0);
}
