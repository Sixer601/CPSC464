#include "ClientSocket.h"
#include "Constants.h"
#include "HelperFunctions.h"
#include "IntArray.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>

using namespace std;

// PRE: pInputFile is a defined input stream representing the file where the
// data
//      is being input from.
// POST: RV is an integer array containing the data input from pInputFile.
IntArray inputData(istream &pInputFile)
{
	IntArray data; // integer array to contain the data.
	// ASSERT: data is a defaultly constructed IntArray.
	while (pInputFile.peek() != EOF)
	// ASSERT: the next character from the input file is not the end of file
	// character.
	{
		int datum; // temporary variable to store single point of data.
		// ASSERT: datum is undefined.
		pInputFile >> datum;
		data.addInt(datum);
	}

	return (data);
}

// PRE: numSubProcesses is a defined integer denoting the number of jobs to farm
// out to
//      other daemons. pInputFile is a defined ifstream that denotes the file to
//      be sorted.
// POST: pInputFile has been broken into numSubProcesses components
//       and those components have been sorted. All employees created have
//       terminated after being told that their shifts are over.
void bossProcess(int numSubProcesses, string pFileName, int pPortNum) 
{
	freopen("errors.txt", "w", stderr);
	char inputFileFullName[BASESTRINGSIZE]; // 
	// ASSERT: 
	snprintf(inputFileFullName, BASESTRINGSIZE, "%s", pFileName.c_str());
	char splitCommand[BASESTRINGSIZE]; //
	// ASSERT: 
	snprintf(splitCommand, BASESTRINGSIZE, "split -l %d -d %s sort", numSubProcesses, inputFileFullName);
	int i = system(splitCommand); // i is the return value of the command to split the
							// input file into their different components.
  	if (i != 0)
  	// ASSERT: The return value after splitting the input file was an error.
	{
		printf("Errors in split.\n");
	}

	// Send request to initiate N parallel daemons.
	ClientSocket cSock(LOCALHOSTIPADDRESS, PORTNUMBER); // Client socket to connect to daemon running on this computer.
	// ASSERT: cSock is a client socket connected to the computer at ip address LOCALHOSTIPADDRESS on port PORTNUMBER.
	char request[BASESTRINGSIZE]; // request string to send over socket.
	// ASSERT: request is a character array of size BASESTRINGSIZE.

	// TODO: Ask Dr. Shende how to tell request 1 to handle the different file names.
	// NOTE: Use employee ID in order to determine which file to use. local Daemon will tell the other daemon which 
	//       employee ID to use for running employee program.

	char commandToSnagIPaddressOfBoss[BASESTRINGSIZE];

	snprintf(commandToSnagIPaddressOfBoss, BASESTRINGSIZE, "hostname -I | awk '{print $1}' >> %s", TEMPIPADDRESSFILE);

	snprintf(request, BASESTRINGSIZE, "%c %d %s %s", REQUEST1CHARACTER, numSubProcesses, PARALLELMERGESORTPROGPATH, EMPLOYEEARG);
	cSock << request;

	bool employeesDone = false; //
	// ASSERT: 

	bool * employeesStatuses = new bool[numSubProcesses];

	ServerSocket employeeCommunicator(pPortNum);
	while (!employeesDone)
	// ASSERT: there are employee processes still at work.
	{
		ServerSocket new_sock; //
		// ASSERT:
		employeeCommunicator.accept(new_sock);
		try 
		{
			bool singleEmployeeDone = false; //
			// ASSERT:
			while (!singleEmployeeDone)
			// ASSERT:
			{
				string employeeMessage; //
				// ASSERT:
				new_sock >> employeeMessage;
				int delimiterPos = employeeMessage.find(DELIMITER);
				int employeeID = stoi(employeeMessage.substr(0, delimiterPos - 1));
				string employeeStatement = employeeMessage.substr(delimiterPos + 1);
				if (employeeStatement == "finished")
				// ASSERT:
				{
					singleEmployeeDone = true;
					employeesStatuses[employeeID] = true;
				}
				int numEmployeesFinished = 0;
				for(int p = 0; p < numSubProcesses; p++)
				{
					if(employeesStatuses[p])
					{
						numEmployeesFinished++;
					}
				}
				if(numEmployeesFinished == numSubProcesses)
				{
					employeesDone = true;
				}
			}
		}

		catch (SocketException &) {}
	}

	cout << "All files have been sorted." << endl;
	// TODO: Determine how to tell an employee its shift is over.
	// NOTE: Make sure ip address is passed as well as port, shende said so.
}

// PRE: pFile is a defined fstream object that contains the information to sort
// using merge sort. POST: pFile contains a sorted list of numbers. This
// employee was told that its shift is over.
void employeeProcess(fstream &pFile, int employeeID, string bossIpAddress, int bossPort) 
{
	IntArray data = inputData(pFile); // integer array to contain the numbers stored in pFile
	// ASSERT: data contains the information stored in pFile.
	mergeSort(data.getTheArray(), 0, data.getContentLength());
	for (int i = 0; i < data.getContentLength(); i++)
	// ASSERT: i begins at 0 and must be less than the content length of the integer array data.
	{
		pFile << data.getNthIntInArray(i) << " ";
	}
	pFile << endl;
	
	ClientSocket bossCommunicator(bossIpAddress, bossPort); //
	// ASSERT: 
	char doneMessage[BASESTRINGSIZE];
	snprintf(doneMessage, BASESTRINGSIZE, "%d finished", 7);
	bossCommunicator << doneMessage;

	bool shiftOver = false; // reflects if the employee has gotten confirmation that their shift is over.
	// ASSERT: shiftOver begins as false due to the fact that no information was given to the employee
	//         from the boss that the shift is over.
	string reply; //
	// ASSERT: 
	while (!shiftOver) 
	// ASSERT: 
	{
		bossCommunicator >> reply;
		if(reply == "shift over.")
		// ASSERT: 
		{
			shiftOver = true;
			// ASSERT: 
		}
	}
}

// STRUCTURE TO RUN: 
//      BOSS PROCESS: ./parallelMergeSort boss test-case1.txt 4 40000
//                       programName status fileToSplit subprocesses port
//
//      EMPLOYEE PROCESS: ./parallelMergeSort employee sort01 1 192.168.1.101 40000 
//                               programName status fileToSort ID bossIP bossPort


// PRE:
// POST:
int main(int argc, char **argv)
{
	if (argc < 2 || argc > 4)
	// ASSERT: The number of arguments are not between 2 and 4.
	{
		cout << "Invalid number of arguments." << endl;
	} 
	else 
	{
		if (strcmp(argv[1], "boss") == 0)
		// ASSERT: The first argument passed to this function is to denote this merge sort program will be the
		//         boss of some employee programs.
		{
			string fileName = argv[2];
			ifstream inputFile(argv[2]); // ifstream that contains the file of numbers to sort.
			int numSubProcesses = stoi(argv[3]); // int that represents the number of processes to farm the work out to.
			// ASSERT: numSubProcesses is equal to the 3rd argument passed to the program.
			bossProcess(numSubProcesses, fileName, stoi(argv[4]));
		} 
		else if (strcmp(argv[1], "employee") == 0)
		// ASSERT: the first argument passed to this function is
		//         to denote this merge sort program will be the
		//         employee of some boss program.
		{
			fstream inputFile(argv[2], ios::in | ofstream::trunc); // fstream that contains the section of
														// the overall document that the employee
														// is supposed to sort.
			int employeeID = stoi(argv[3]);
			string bossIpAddress = argv[4];
			int bossPort = stoi(argv[5]);
			
			employeeProcess(inputFile, employeeID, bossIpAddress, bossPort);
		}
	}

	return (0);
}