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
void createSharedMemory(IntArray pIntArray)
{
	cout << "Entered createSharedMemory." << endl;

	int shared_mem_id;

	int *shm, *information;

	key_t key = 0601;

	// TODO: Calculate space required for intArray
	int spaceRequired = sizeof(int) * pIntArray.getContentLength();

	cout << "Space Required: " << spaceRequired << endl;

	// Create shared memory segment
	shared_mem_id = shmget(key, spaceRequired, IPC_CREAT | 0666);
	if (shared_mem_id < 0)
	{
		perror("shmget");
	}
	else
	{
		// Attach segment to our data space
		shm = (int *)shmat(shared_mem_id, NULL, 0);
		if (shm == (int *)-1)
		{
			perror("shmat");
		}
		else
		{
			// Store data in memory for other processes to read
			information = shm;
			for (int i = 0; i < pIntArray.getContentLength(); i++)
			{
				information[i] = pIntArray.getNthIntInArray(i);
			}
		}
	}
}

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

	// Debug Mode Output
	if (debugMode)
	{
		cout << "data: ";
		for (int i = 0; i < data.getContentLength(); i++)
		{
			// ASSERT:
			cout << data.getNthIntInArray(i) << " ";
		}
		cout << endl;
	}

	// Store intArray in shared memory
	createSharedMemory(data);
}

// PRE:
// POST:
void createChildProcesses(int neededProcessesNum)
{
	cout << "Entered createChildProcesses which will make " << neededProcessesNum << " processes." << endl;
	int children = 0;
	bool child = false;
	while(children < neededProcessesNum && !child) {
		// ASSERT: 
		pid_t childPID = fork();
		if(childPID == -1) {
			perror("fork");
		}
		else if (childPID == 0)
		{
			children++;
			// Child Process
			child = true;
			cout << "I am child: " << getpid() << " of parent: " << getppid() << endl;
		}
		else 
		{
			children++;
			// Parent Process
			cout << "I am: " << getpid() << endl;
		}
	}
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
		if (thirdArg == 1 || thirdArg == 0)
		{
			if (thirdArg == 1)
			{
				debugMode = true;
			}
			else
			{
				debugMode = false;
			}

			cout << "number of processes required: " << numProcesses << endl;

			// Input file data into shared memory
			inputData(inputFile, debugMode);

			// Create child processes
			createChildProcesses(numProcesses - 1);

			// Begin handling chunks/jobs
			handleJobs();
		}
		else
		{
			cout << "Enter a 1 or 0 for debug mode." << endl;
		}
	}

	return (0);
}
