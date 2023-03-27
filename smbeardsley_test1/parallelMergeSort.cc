#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include "Constants.h"
#include "Socket.h"
#include "SocketException.h"
#include "HelperFunctions.h"

using namespace std;

// PRE: 
// POST: RV is a string containing the ip address of the computer this program is running on.
string getIpAddress()
{
  string ipAddress; // string to hold the ip address of the computer this program is running on.
  // ASSERT: ipAddress is a defaultly constructed string object.
  char commandToAddIPaddressToBossIPFile[BASESTRINGSIZE]; // pseudo string to hold the command to add ip address to BOSSIPFILE.
  // ASSERT: this variable is an undefined character array of size BASESTRINGSIZE.
  snprintf(commandToAddIPaddressToBossIPFile, BASESTRINGSIZE, "hostname -I | awk '{print $1}' >> %s", BOSSIPFILE);
  int rvSystemCall = system(commandToAddIPaddressToBossIPFile); // integer holding the RV of system call.
  // ASSERT: rvSystemCall is equal to the return value of system.
  if (rvSystemCall != 0)
  // ASSERT: there was an error in the system call.
    {
      cout << "Error in putting ip address of boss in " << BOSSIPFILE << endl;
    }
  else
    {
      ifstream bossIpFile(BOSSIPFILE); // file containing this computer's ip address.
	 // ASSERT: bossIpFile is a defined ifstream object containing the contents of BOSSIPFILE.
      bossIpFile >> ipAddress;
    }
  return(ipAddress);
}

// PRE: executable path is a defined string containing the path to the executable to have employees run.
//      numSubProcesses is a defined integer containing the number of employees to assign work to. 
//      inputFile is a defined ifstream object containing the file of numbers to sort, and inputFileName
//      is a defined string containing the name of the file of numbers to sort to aid in the diving of the file.. 
// POST: All employees shifts are over and all files have been processed.
void bossProcess(string executablePath, int numSubProcesses, ifstream &inputFile, string inputFileName)
{
  char * pathPrefix = NULL; // character array to hold path to inputFile.
  // ASSERT: pathPrefix is a null pointer.
  pathPrefix = getcwd(pathPrefix, 0);
  
  string ipAddress = getIpAddress(); // string to contain the ip address of the computer running this program.
  // ASSERT: ipAddress is the RV of getIpAddress.
  char splitCommand[BASESTRINGSIZE]; // pseudo string to hold the command used in dividing the main file to sort.
  // ASSERT: splitCommand is an undefined character array of size BASESTRINGSIZE.
  snprintf(splitCommand, BASESTRINGSIZE, "split -n %d -d %s \"%s\"", numSubProcesses, inputFileName.c_str(), (string(pathPrefix) + "/" + FILEPREFIX).c_str());
  int rvSystemCommand = system(splitCommand); // integer containing if the system command had an error in running or not.
  // ASSERT: rvSystemCommand is equal to the return value of the system command.
  if (rvSystemCommand != 0)
  // ASSERT: there was an error in running the system command.
    {
      printf("Errors in split.\n");
    }

  Socket sSock(BOSSPORT); // listener socket to handle employee communication.
  // ASSERT: sSock is a socket bound to port BOSSPORT.
  int numEmployeesDone = 0; // integer representing the number of employees who have completed their shift.
  // ASSERT: numEmployeesDone is assumed to be 0 until we know an employee is done.
  Socket cSock(LOCALHOSTIP, PORTNUMBER); // client socket containing the request to the local daemon to farm out work to employees.
  // ASSERT: cSock is a defined socket object that is connected to the computer this program is running on, on port PORTNUMBER.
  char request[BASESTRINGSIZE]; // pseudo string containing the request to be sent to the local daemon.
  // ASSERT: request is an undefined character array of size BASESTRINGSIZE.
  snprintf(request, BASESTRINGSIZE, "1 %d %s %s %s %d %s ", numSubProcesses, executablePath.c_str(), EMPLOYEEARG, ipAddress.c_str(), BOSSPORT, (string(pathPrefix) + "/" + FILEPREFIX).c_str());
  cout << request << endl;
  cSock << request;
  
  while(numEmployeesDone < numSubProcesses)
  // ASSERT: the number of employees done is less than the number of employees.
    {
      cout << "num employees done: " << numEmployeesDone << endl;
      Socket newConnection; // socket to handle when a connection with another computer is made.
	 // ASSERT: newConnection is an empty socket.
      bool success = sSock.accept(newConnection); // sanity check to ensure code runs 
	 									// when accepting the connection is successful.
	 // ASSERT: success is true if sSock accepted the socket and false when sSock did not.
      
      if(success)
	 // ASSERT: accepting of newConnection by sSock was successful.
	{
	  try
	    {
	      string employeeMessage; // string to hold message from employee.
		 // ASSERT: employeeMessage is a defaultly constructed string.
	      newConnection >> employeeMessage;
	      if(employeeMessage == EMPLOYEESHIFTENDMESSAGE)
		 // ASSERT: employee claims to have finished their shift.
		{
		  newConnection << "exit";
		  numEmployeesDone++;
		  // ASSERT: there is one more employee done.
		}
	    }
	  catch(SocketException &e)
	    {}
	}
    }
    cout << "num employees done: " << numEmployeesDone << endl;
}

// PRE: 
// POST: 
vector<int> readInput(string fileToSort)
{
  vector<int> input; //
  // ASSERT: 
  ifstream file(fileToSort); //
  // ASSERT: 
  int num; //
  // ASSERT: 
  while (file >> num)
  // ASSERT: 
    {
      input.push_back(num);
    }

  file.close();

  return(input);
}

// PRE: bossIP is a defined string containing the ip address of the boss process of this employee.
//      bossPort is a defined integer containing the port of the boss process of this employee.
//      filePrefix is a defined string object containing the prefix for the file the employee is to sort.
//      employeeID is an integer representing a unique identifier for this employee. 
// POST: the file assigned to this employee is sorted, and the boss process has allowed the employee to terminate.
void employeeProcess(string bossIP, int bossPort, string filePrefix, int employeeID)
{
  string fileToSort; // string to hold the name of the file to sort.
  // ASSERT: fileToSort is a defaultly constructed string object.
  if (employeeID <= 9)
  // ASSERT: the employee id is a single digit.
    {
      fileToSort = filePrefix + "0" + to_string(employeeID);
	 // ASSERT: clean up formatting of fileToSort.
    }
  else
    {
      fileToSort = filePrefix + to_string(employeeID);
	 // ASSERT: retain formatting of fileToSort.
    }

  vector<int> listToSort = readInput(fileToSort); // vector containing all numbers to sort in employee's file.
  // ASSERT: listToSort contains RV of readInput.
  mergeSort(listToSort.data(), 0, listToSort.size() - 1);
  
  // ASSERT: The sorted list will be output to standard output. Largely to prove program functioned.
  cout << "Sorted List: ";
  for(int i = 0; i < listToSort.size(); i++)
  {
	cout << listToSort[i] << " ";
  }
  cout << endl;

  Socket cSock(bossIP, bossPort); // socket to communicate to boss process using.
  // ASSERT: cSock is a socket connected to the computer at ip address bossIP bound to port bossPort.
  string reply; // string to hold reply of what boss sends back.
  // ASSERT: reply is a defaultly constructed string.
  bool isComplete = false; // bool to represent if employee is acknowledged as finished by boss process.
  // ASSERT: isComplete is assumed false until the boss process communicates otherwise.
  try
    {
      while(!isComplete)
	 // ASSERT: employee is not acknowledged as finished by boss process.
	{
	  cSock << EMPLOYEESHIFTENDMESSAGE;
	  cSock >> reply;
	  if(reply == BOSSACK)
	  // ASSERT: reply from boss was acknowledgement as finished.
	    {
	      isComplete = true;
		 // ASSERT: employee's shift is now over.
	    }
	}
    }
  catch(SocketException &)
    {
      cout << strerror(errno) << endl;
    }
}

// PRE: 
// POST: 
int main(int argc, char **argv)
{
  if(argc != 4 && argc != 5)
  // ASSERT: 3 or 4 arguments were not passed to program.
    {
      cout << "Invalid number of arguments." << endl;
    }
  else
    {
      if(strcmp(argv[1], BOSSARG) == 0)
	{
	  char *executablePath = realpath(argv[0], NULL);
	  // ASSERT: executablePath is equal to the real path of the 0th argument passed to the program.
	  if (executablePath == NULL)
	  // ASSERT: executablePath was not defined correctly.
	    {
	      throw runtime_error("Failed to get program path");
	    }
	  
	  int numSubProcesses = stoi(argv[2]); // integer representing the number of employees to hire.
	  // ASSERT: numSubProcesses is equal to the 2nd argument passed to program.
	  ifstream inputFile(argv[3]); // file containing numbers to sort.
	  // ASSERT: inputFile is a defined ifstream object containing the contents of the 3rd argument passed to program.
	  char *inputFileName = realpath(argv[3], NULL); // pseudo string pointing to name of file passed as argument.
	  // ASSERT: inputFileName is the RV of realpath.

	  bossProcess(executablePath, numSubProcesses, inputFile, inputFileName);
	  free(executablePath);
	  free(inputFileName);
	  inputFile.close();
	}
      else if(strcmp(argv[0], EMPLOYEEARG) == 0)
	{
	  string bossIP = argv[1];
	  int bossPort = stoi(argv[2]);
	  string filePrefix = argv[3];
	  int employeeID = stoi(argv[4]);
	  employeeProcess(bossIP, bossPort, filePrefix, employeeID);
	}
      else
	{
	  cout << "Faulty call to program. Recieved: ";
	  // Output faulty call for debugging purposes.
	  for(int i = 0; i < argc; i++)
	    {
	      cout << argv[i] << " ";
	    }
	  cout << endl;
	}
    }
  return(0);
}
