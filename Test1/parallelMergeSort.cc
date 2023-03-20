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
#include <unistd.h>

using namespace std;

// PRE: pInputFile is a defined input stream representing the file where the
// data
//      is being input from.
// POST: RV is an integer array containing the data input from pInputFile.
IntArray inputData(istream &pInputFile) {
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
void bossProcess(int numSubProcesses, ifstream &pInputFile) {
  // TODO: Split file into C subfiles, where C is the number of subprocesses.
  freopen("errors.txt", "w", stderr);
  char inputFileFullName[BASESTRINGSIZE];
  // TODO: Determine what the file being split will be named.
  snprintf(inputFileFullName, BASESTRINGSIZE, "%s.txt", "");
  char splitCommand[BASESTRINGSIZE];
  snprintf(splitCommand, BASESTRINGSIZE, "split -l %d -d %s sort",
           numSubProcesses, inputFileFullName);
  int i = system(splitCommand); // i is the return value of the command to split the
						  // input file into their different components.
  if (i != 0)
  // ASSERT: The return value after splitting the input file was an error.
  {
    printf("Errors in split.\n");
  }

  // TODO: Connect to daemon on computer this process is running on.

  //       Send request to initiate N parallel daemons.
  ClientSocket cSock(LOCALHOSTIPADDRESS,
                     PORTNUMBER); // Client socket to connect to daemon running
                                  // on this computer.
  // ASSERT: cSock is a client socket connected to the computer at ip address
  // LOCALHOSTIPADDRESS on port PORTNUMBER.
  char request[BASESTRINGSIZE]; // request string to send over socket.
  // ASSERT: request is a character array of size BASESTRINGSIZE.

  // TODO: Ask Dr. Shende how to tell request 1 to handle the different file names.
  // NOTE: Use employee ID in order to determine which file to use. local Daemon
  //       will tell the other daemon which employee ID to use for running employee
  //       program.

  snprintf(request, BASESTRINGSIZE, "1 %d ./parallelMergeSort employee",
           numSubProcesses);
  cSock << request;
  int employeeProcessesDone = 0; // integer to represent the number of employee processes that have
         					   // reported to the boss that they have completed the job given to them.
  // ASSERT: it is assumed that no employee processes are done before we have
  // given them work.

  // TODO: Determine how to listen for an employee process.
  ServerSocket employeeCommunicator(5000);
  while (employeeProcessesDone < numSubProcesses)
  // ASSERT: there are employee processes still at work.
  {
    ServerSocket new_sock; //
    // ASSERT:
    employeeCommunicator.accept(new_sock);
    try {
      bool singleEmployeeDone = false; //
      // ASSERT:
      while (!singleEmployeeDone)
      // ASSERT:
      {
        string request; //
        // ASSERT:
        new_sock >> request;
        if (request == "finished")
        // ASSERT:
        {
          singleEmployeeDone = true;
          employeeProcessesDone++;
        }
      }
    } catch (SocketException &) {
    }
  }
  cout << "All files have been sorted." << endl;
  // TODO: Determine how to tell an employee its shift is over.
}

// PRE: pFile is a defined fstream object that contains the information to sort
// using merge sort. POST: pFile contains a sorted list of numbers. This
// employee was told that its shift is over.
void employeeProcess(fstream &pFile, int employeeID) {
  IntArray data =
      inputData(pFile); // integer array to contain the numbers stored in pFile
  // ASSERT: data contains the information stored in pFile.
  mergeSort(data.getTheArray(), 0, data.getContentLength());
  for (int i = 0; i < data.getContentLength(); i++)
  // ASSERT: i begins at 0 and must be less than the content length of the
  // integer array data.
  {
    pFile << data.getNthIntInArray(i) << " ";
  }
  pFile << endl;
  // TODO: determine when and how to connect to the boss process.

  // TODO: Send Message to boss process that employee is done.

  // TODO: Wait for boss process to tell employee to end their shift.
  bool shiftOver = false; // reflects if the employee has gotten confirmation
                          // that their shift is over.
  // ASSERT: shiftOver begins as false due to the fact that no information was
  // given to the employee
  //         from the boss that the shift is over.
  while (!shiftOver)
  // ASSERT: The employee's shift is not over.
  {
  }
}

// PRE:
// POST:
int main(int argc, char **argv) {
  if (argc < 2 || argc > 4)
  // ASSERT: The number of arguments are not between 2 and 4.
  {
    cout << "Invalid number of arguments." << endl;
  } else {
    if (strcmp(argv[1], "boss") == 0)
    // ASSERT: the first argument passed to this function is
    //         to denote this merge sort program will be the
    //         boss of some employee programs.
    {
      ifstream inputFile(
          argv[2]); // ifstream that contains the file of numbers to sort.
      int numSubProcesses = stoi(argv[3]); // int that represents the number of
                                           // processes to farm the work out to.
      // ASSERT: numSubProcesses is equal to the 3rd argument passed to the
      // program.
      bossProcess(numSubProcesses, inputFile);
    } else if (strcmp(argv[1], "employee") == 0)
    // ASSERT: the first argument passed to this function is
    //         to denote this merge sort program will be the
    //         employee of some boss program.
    {
	 int employeeID = stoi(argv[3]);
      fstream inputFile(argv[2], ios::in | ofstream::trunc); // fstream that contains the section of
                                      					 // the overall document that the employee
                                      					 // is supposed to sort.
      employeeProcess(inputFile, employeeID);
    }
  }

  return (0);
}