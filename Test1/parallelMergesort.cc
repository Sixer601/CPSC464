#include <iostream>

using namespace std;

void bossProcess()
{
	// TODO: Split file into C subfiles, where C is the number of subprocesses.

	// TODO: Connect to daemon on computer this process is running on.
	//       Send request to initiate N parallel daemons. 

	// TODO: Wait for all employee processes to report back to boss. Once done
	//       print out a message stating 	
}

void employeeProcess()
{
	// TODO: Mergesort information provided in file handed to employee.

	// TODO: Send Message to boss process that employee is done.

	// TODO: Wait for boss process to tell employee to end their shift.

	// TODO: End program.
}

int main(int argc, char **argv)
{
	if(argc != 0)
	// ASSERT: 
	{
		cout << "Invalid number of arguments." << endl;
	}
	else
	{
		bool isBoss;
		// TODO: Check if process should be a boss process or employee process.
		(isBoss) ? (bossProcess()) : (employeeProcess());
	}

	return(0);
}