#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Socket.h"
#include "SocketException.h"
#include "Constants.h"

using namespace std;

// PRE: 
// POST: RV is a string containing the ip address of the computer running this program. 
//       
string getIpAddress()
{
  string ipAddress; // string to hold the computer's ip address.
  // ASSERT: ipAddress is a defaultly constructed string.
  char commandToAddIPaddressToLeaderIPFile[BASESTRINGSIZE]; // pseudo string to hold system command to get ip address.
  // ASSERT: this variable is an undefined character array of size BASESTRINGSIZE.
  snprintf(commandToAddIPaddressToLeaderIPFile, BASESTRINGSIZE, "hostname -I | awk '{print $1}' >> %s", LEADERIPFILE);
  int rvSystemCall = system(commandToAddIPaddressToLeaderIPFile); // return value of system call.
  // ASSERT: rvSystemCall is 0 if the program succeeded and not if it failed.
  if (rvSystemCall != 0)
  // ASSERT: rvSystemCall did not succeed.
    {
      cout << "Error in putting ip address of leader in " << LEADERIPFILE << endl;
    }
  else
    {
      ifstream leaderIpFile(LEADERIPFILE); // file that contains the leader daemon's ip address.
	 // ASSERT: leaderIpFile is a defined ifstream object containing LEADERIPFILE's contents.
      leaderIpFile >> ipAddress;
    }
  return(ipAddress);
}

// PRE: ipAddressFile is a defined file containing ipAddresses of computers running the daemon.
// POST: RV is a vector containing the ip addresses within ipAddressFile.
vector<string> inputIpAddresses(ifstream &ipAddressFile)
{
  vector<string> ipAddressList; // vector to contain all ip addresses within ipAddressFile.
  // ASSERT: ipAddressList is an empty vector of strings.
  while (ipAddressFile.peek() != EOF)
    // ASSERT: the next character from the input file is not the end of file character.
    {
      string address; // single ip address.
	 // ASSERT: address is a defaultly defined string.
      ipAddressFile >> address;
      ipAddressList.push_back(address);
    }
  return(ipAddressList);
}

// PRE: pRequest is a defined string that contains a request for a daemon.
// POST: RV is the number of subprocesses to run based upon the request's contents.
int parseRequestForSubProcesses(string pRequest)
{
  int numSubProcesses; // integer to hold the number of subprocesses to run.
  // ASSERT: numSubProcesses is an undefined integer.
  int firstDelimiterPos = pRequest.find(DELIMITER); // index within string that holds first delimiter.
  // ASSERT: firstDelimiterPos is the first index in pRequest that contains DELIMITER.
  int secondDelimiterPos = pRequest.find(DELIMITER, firstDelimiterPos + 1); // index within string that holds second delimiter.
  // ASSERT: secondDelimiterPos is the second index in pRequest that contains DELIMITER.
  numSubProcesses = stoi(pRequest.substr(firstDelimiterPos, secondDelimiterPos - firstDelimiterPos));
  // ASSERT: numSubProcesses is the integer equivalent of the substring of pRequest starting at firstDelimiterPos, 
  //         containing secondDelimiterPos - firstDelimiterPos digits.
  return(numSubProcesses);
}

// PRE: pRequest is a defined string that contains a request for a daemon.
// POST: RV is the program a daemon will need to run.
string parseRequest1ForProgram(string pRequest)
{
  string program; // string to hold the program that will be ran.
  // ASSERT: program is a defaultly defined string.
  int firstDelimiterPos = pRequest.find(DELIMITER); // index within string that holds first delimiter.
  // ASSERT: firstDelimiterPos is the first index in pRequest that contains DELIMITER.
  int secondDelimiterPos = pRequest.find(DELIMITER, firstDelimiterPos + 1); // index within string that holds second delimiter.
  // ASSERT: secondDelimiterPos is the second index in pRequest that contains DELIMITER.
  program = pRequest.substr(secondDelimiterPos + 1);
  // ASSERT: program is the substring of pRequest starting at the index equal to the sum of 1 and secondDelimiterPos.
  return(program);
}

// PRE: pRequest is a defined string that contains a request for a daemon.
// POST: RV is the program a daemon will need to run.
string parseRequest2ForProgram(string pRequest)
{
  string program; // string to hold the program that will be ran.
  // ASSERT: program is a defaultly defined string.
  int firstDelimiterPos = pRequest.find(DELIMITER); // index within string that holds first delimiter.
  // ASSERT: firstDelimiterPos is the first index in pRequest that contains DELIMITER.
  program = pRequest.substr(firstDelimiterPos + 1);
  // ASSERT: program is the substring of pRequest starting at the index equal to the sum of 1 and firstDelimiterPos.
  return(program);
}

// PRE: request is a defined string that contains a request for a daemon.
// POST: 
void handleRequest1(string request)
{
  ifstream ipAddressFile(IPADDRESSFILE); // file containing ip addresses of daemons.
  // ASSERT: ipAddressFile is a defined ifstream object containing the contents of IPADDRESSFILE.
  string myIP = getIpAddress(); // ip address of computer running this program.
  // ASSERT: myIP is the return value of getIpAddress.
  int numSubProcesses = parseRequestForSubProcesses(request); // number of sub processes to create to run passed command.
  // ASSERT: numSubProcesses is equal to the return value of parseRequestForSubProcesses.
  string programToRun = parseRequest1ForProgram(request); // program to run on each sub process.
  // ASSERT: programToRun is equal to the return value of parseRequest1ForProgram.
  vector<string> ipAddressList = inputIpAddresses(ipAddressFile); // vector containing ip addresses of daemons on the network.
  // ASSERT: ipAddressList contains all values within ipAddressFile.
  vector<string>::iterator it = ipAddressList.begin(); // iterator to go through ipAddressList.
  // ASSERT: it is equal to the beginning value of ipAddressList.
  for(int i = 0; i < numSubProcesses; i++)
  // ASSERT: i is less than the number of sub processes.
    {
      if(*it == myIP)
	 // ASSERT: the value it is pointing to is the ip address of the computer running this program.
	{
	  ++it;
	  // ASSERT: it now points to the next value in ipAddressList.
	}
      Socket requestLine(*it, PORTNUMBER); // Socket to send a request on.
	 // ASSERT: requestLine is a socket connected to the computer at the ip address pointed to by it on the port PORTNUMBER.
      string requestToSend = string(REQUEST2CHAR) + DELIMITER + programToRun + to_string(i).c_str(); 
	 // message to send over requestLine.
	 // ASSERT: requestToSend contains the character telling the daemon it is a type 2 request, the program to run,
	 //         and a unique number that helps the program the daemon will run know which file it must sort.
      try
	{
	  requestLine << requestToSend;
	}
      catch(SocketException&)
	{
	  cout << strerror(errno) << endl;
	} 
      ++it;
      if(it == ipAddressList.end())
	 // ASSERT: it has reached the end of ipAddressList.
	{
	  it = ipAddressList.begin();
	  // ASSERT: restart it at the beginning of ipAddressList.
	}
    }
}

// PRE: pString is a string containing arguments to parse. firstDelimiterPos is the index of the first delimiter in pString.
// POST: RV is a vector containing all of the arguments passed in pString for an executable.
vector<string> getCommandArguments(string pString, size_t firstDelimiterPos)
{
  vector<string> substrings; // vector to hold each argument.
  // ASSERT: substrings is an empty vector object.
  size_t index = pString.find(DELIMITER, firstDelimiterPos); // index within pString to denote where one of the delimiters is.
  // ASSERT: index is equal to the index of the first delimiter in pString.
  while(index != string::npos)
  // ASSERT: index does not equal the constant npos.
    {
      size_t pos = pString.find(DELIMITER, index + 1); // index within pString to denote where the delimiter following index is.
	 // ASSERT: pos is equal to the index of the delimiter following index.
      string substring = pString.substr(index + 1, pos - index - 1); // string to hold the argument in between index and pos.
	 // ASSERT: substring is the contents of pString starting at index + 1 and is the difference of the difference of pos, index, and 1.
      substrings.push_back(substring);
      index = pos;
	 // ASSERT: index now is equal to where position was within pString.
    }
  return(substrings);
}

// PRE: request is a defined string denoting a request made to the daemon.
// POST: the program outlined in request is ran on a child process.
void handleRequest2(string request)
{
  string programToRun = parseRequest2ForProgram(request); // string to hold the program for daemon to run.
  // ASSERT: programToRun is equal to the return value of parseRequest2ForProgram.
  pid_t childPID = fork();
  if (childPID == -1)
  // ASSERT: there was an error in forking the process.
    {
      perror("fork");
    }
  else if (childPID == 0)
  // ASSERT: the subsequent code is run by the child process.
    {
      size_t firstDelimiter = programToRun.find(DELIMITER, 0); // number denoting what index the first delimiter
	 											   // in programToRun is.
	 // ASSERT: firstDelimiter is equal to the index within programToRun where the DELIMITER string first appears.
      string programName = programToRun.substr(0, firstDelimiter); // string to contain the path and name of the executable.
	 // ASSERT: programName is equal to the substring in programToRun beginning at 0 and is firstDelimiter characters long.
      vector<string> programArgvVector = getCommandArguments(programToRun, firstDelimiter); 
	 // vector to hold the executable's arguments.
	 // ASSERT: programArgvVector is equal to the RV of getCommandArguments.
      char ** programArgv = new char *[programArgvVector.size() + 1]; // array of pseudostrings that will contain the arguments
	 													// to pass to execv.
	 // ASSERT: programArgv is an undefined array of  length of 1 more than the size of programArgvVectorm, containing character arrays.
      programArgv[programArgvVector.size()] = (char *) NULL;
	 // ASSERT: index programArgvVector.size() of programArgv is equal to the character array version of NULL.
      int err = execv(programName.c_str(), (char * const *)programArgv);
	 // ASSERT: err is the return value of execv, and only is relevant if execv fails.
      if (err == -1)
	 // ASSERT: execv did not run successfully.
	{
	  cout << "execv failed." << endl;
	  exit(1);
	}
      for(int j = 0; j < programArgvVector.size(); j++)
	 // ASSERT: j is less than the size of programArgvVector.
	{
	  delete [] programArgv[j];
	}
      delete [] programArgv;
    }
}

// PRE: 
// POST: this computer's ip address is added to IPADDRESSFILE. 
void addIpAddressToFile()
{
  char commandToAddIPaddressToNodesFile[BASESTRINGSIZE]; // pseudo string to contain the command run to put 
  											  // this computer's ip address in IPADDRESSFILE. 
  // ASSERT: this variable is an undefined character array of size BASESTRINGSIZE.
  snprintf(commandToAddIPaddressToNodesFile, BASESTRINGSIZE, "hostname -I | awk '{print $1}' >> %s", IPADDRESSFILE);
  int rvSystemCall = system(commandToAddIPaddressToNodesFile); // if putting ipAddress in IPADDRESSFILE was successful.
  // ASSERT: rvSystemCall is the return value of the system call to commandToAddIPaddresstoNodesFile.
  if (rvSystemCall != 0)
    // ASSERT: The command run had an RV that was not indicative of success.
    {
      cout << "Errors in putting ip address of computer in " << IPADDRESSFILE << endl;
    }
}


// PRE: listener is a defined socket object.
// POST: 
void listenForRequests(Socket &listener)
{
  while (true) 
  // ASSERT: loop indefinitely.
    {
      Socket newConnection; // socket to handle when a connection with another computer is made.
	 // ASSERT: newConnection is an empty socket.
      bool success = listener.accept(newConnection); // sanity check to ensure code runs 
	 									   // when accepting the connection is successful.
	 // ASSERT: success is true if listener accepted the socket and false when listener did not.
      if(success)
	 // ASSERT: accepting of newConnection by listener was successful.
	{
	  try
	    {
	      while(true)
		 // ASSERT: loop while the socket is still open.
		{			
		  string request; // string to hold request made by daemon client.
		  // ASSERT: request is a defaultly defined string.
		  newConnection >> request;
		  if(request[0] == REQUEST1CHAR[0])
		  // ASSERT: the first character in request is the same as the type 1 request character.
		    {
		      handleRequest1(request);
		    }
		  else if(request[0] == REQUEST2CHAR[0])
		  // ASSERT: the first character in request is the same as the type 2 request character.
		    {
		      handleRequest2(request);
		    }
		  else 
		    {
		      
		    }
		}
	    }
	  catch(SocketException &e)
	    {}
	}
   }
}


// PRE: 
// POST: 
int main(int argc, char **argv)
{
  if(argc != 1)
  // ASSERT: arguments to this program were passed, meaning end user did not understand how to run program.
    {
      cout << "Invalid number of arguments." << endl;
    }
  else
    {
      try 
	{
	  Socket listener(PORTNUMBER); // socket to listen for requests made to daemon.
	  // ASSERT: listener is a socket bound to port PORTNUMBER.
	  cout << "Listening on Port: " << PORTNUMBER << endl;
	  addIpAddressToFile();
	  listenForRequests(listener);
	} 
      catch (SocketException &e) 
	{
	  cerr << e.description() << endl << "Exiting." << endl;
	}
    }
  return(0);
}
