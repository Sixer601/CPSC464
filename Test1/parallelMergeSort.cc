#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "ClientSocket.h"
#include "HelperFunctions.h"
#include "IntArray.h"

using namespace std;

// PRE: pInputFile is a defined input stream representing the file where the data
//      is being input from.
// POST: RV is an integer array containing the data input from pInputFile.
IntArray inputData(istream &pInputFile)
{
	IntArray data; // integer array to contain the data.
	// ASSERT: data is a defaultly constructed IntArray.
	while (pInputFile.peek() != EOF)
	// ASSERT: the next character from the input file is not the end of file character.
	{
		int datum; // temporary variable to store single point of data.
		// ASSERT: datum is undefined.
		pInputFile >> datum;
		data.addInt(datum);
	}
	return (data);
}

void bossProcess(int numSubProcesses, ifstream &pInputFile)
{
	// TODO: Split file into C subfiles, where C is the number of subprocesses.
	freopen("errors.txt", "w", stderr);
	int i;
	// TODO: Determine how to do string interpolation in C++.
	i = system("split -l 10 -d tryexec.cc input");
	if (i != 0)
		printf("Errors in split.\n");

	// TODO: Connect to daemon on computer this process is running on.
	//       Send request to initiate N parallel daemons.
	

	// TODO: Wait for all employee processes to report back to boss. Once done
	//       print out a message stating that all files are sorted.
	bool employeeProcessesDone;
	while (!employeeProcessesDone)
	{

	}
}

void employeeProcess(fstream &pFile)
{
	IntArray data = inputData(pFile);
	mergeSort(data.getTheArray(), 0, data.getContentLength());
	for(int i = 0; i < data.getContentLength(); i++)
	{
		pFile << data.getNthIntInArray(i) << " ";
	}
	pFile << endl;
	// TODO: determine when to connect to boss process, then connect to boss process. 

	// TODO: Send Message to boss process that employee is done.

	// TODO: Wait for boss process to tell employee to end their shift.

	// TODO: End program.
}

int main(int argc, char **argv)
{
	if (argc < 2 || argc > 4)
	{
		cout << "Invalid number of arguments." << endl;
	}
	else
	{
		if (argv[1] == "boss")
		{
			ifstream inputFile(argv[2]);
			int numSubProcesses = stoi(argv[3]);
			bossProcess(numSubProcesses, inputFile);
		}
		else if (argv[1] == "employee")
		{
			fstream inputFile(argv[2], ios::in|ofstream::trunc);
			employeeProcess(inputFile);
		}
	}

	return (0);
}