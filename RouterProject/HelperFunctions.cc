#include "HelperFunctions.h"
#include "Constants.h"
#include <fstream>

using namespace std;

string getIpAddress()
{
  string ipAddress; // string to hold the computer's ip address.
  // ASSERT: ipAddress is a defaultly constructed string.
  char commandToAddIPaddressToLeaderIPFile[BASESTRINGSIZE]; // pseudo string to hold system command to get ip address.
  // ASSERT: this variable is an undefined character array of size BASESTRINGSIZE.
  snprintf(commandToAddIPaddressToLeaderIPFile, BASESTRINGSIZE, "hostname -I | awk '{print $1}' >> %s", TEMPFILE);
  int rvSystemCall = system(commandToAddIPaddressToLeaderIPFile); // return value of system call.
  // ASSERT: rvSystemCall is 0 if the program succeeded and not if it failed.
  if (rvSystemCall != 0)
  // ASSERT: rvSystemCall did not succeed.
    {
      cout << "Error in putting ip address in " << TEMPFILE << endl;
    }
  else
    {
      ifstream routerIpFile(TEMPFILE); // file that contains the leader daemon's ip address.
	 // ASSERT: leaderIpFile is a defined ifstream object containing LEADERIPFILE's contents.
      routerIpFile >> ipAddress;
    }
  char commandToRemoveTempFile[BASESTRINGSIZE]; // pseudo string to hold system command to remove temp file.
  // ASSERT: this variable is an undefined character array of size BASESTRINGSIZE.
  snprintf(commandToRemoveTempFile, BASESTRINGSIZE, "rm %s", TEMPFILE);
  int rvSystemCall2 = system(commandToRemoveTempFile); // return value of system call.
  // ASSERT: rvSystemCall2 is 0 if the program succeeded and not if it failed.
  if (rvSystemCall2 != 0)
  // ASSERT: rvSystemCall2 did not succeed.
    {
      cout << "Error in removing " << TEMPFILE << endl;
    }
  return(ipAddress);
}

int findNextDelimiterIndex(string pString, int currDelimIndex)
{
	return(pString.find(DELIMITER, currDelimIndex + 1));
}