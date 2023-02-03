#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include "IntArray.h"
#include "HelperFunctions.h"

using namespace std;

#define CHUNKSIZE 100

// PRE:
// POST:
void createSharedMemory(bool debugMode, IntArray pIntArray)
{
	if (debugMode)
	{
		cout << "Entered createSharedMemory." << endl;
	}

	int shared_mem_id;

	int *shm, *information;

	key_t key = 0601;

	int numChunks = ceiling(pIntArray.getContentLength(), CHUNKSIZE);
	
	if (debugMode)
	{
		cout << "Number of chunks/jobs to handle: " << numChunks << endl;
	}

	// TODO: Calculate space required for intArray
	int spaceRequired = (sizeof(char) * numChunks) + (sizeof(int) * pIntArray.getContentLength());

	if (debugMode)
	{
		cout << "Space Required: " << spaceRequired << endl;
	}

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
			information = shm;
			// TODO: Setup Job Encoding
			int numJobs = numChunks + ceiling(numChunks, 2);

			if (debugMode) 
			{
				cout << "Number of Jobs: " << numJobs << endl;
			}

			for (int i = 0; i < numJobs; i++) 
			{
				information[i] = i;
			}

			// Store data in memory for other processes to read
			for (int j = numJobs; j < (pIntArray.getContentLength() + numJobs); j++)
			{
				information[j] = pIntArray.getNthIntInArray(j - numJobs);
			}
		}
	}
}

// PRE:
// POST:
void inputData(bool debugMode, istream &pInputFile)
{
	if (debugMode)
	{
		cout << "Entered inputData." << endl;
	}

	// Input data from file
	IntArray data;
	while (pInputFile.peek() != EOF)
	{
		// ASSERT: 
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
	createSharedMemory(debugMode, data);
}

// PRE:
// POST:
void createChildProcesses(bool debugMode, int neededProcessesNum)
{
	if (debugMode)
	{
		cout << "Entered createChildProcesses which will make " << neededProcessesNum << " processes." << endl;
	}

	int children = 0;
	bool child = false;
	while (children < neededProcessesNum && !child)
	{
		// ASSERT:
		pid_t childPID = fork();
		if (childPID == -1)
		{
			perror("fork");
		}
		else if (childPID == 0)
		{
			// Child Process
			children++;
			child = true;
		}
		else
		{
			// Parent Process
			children++;
		}
	}
}

// PRE:
// POST:
void handleJobs(bool debugMode)
{
	if (debugMode)
	{
		cout << getpid() << " Entered handleJobs." << endl;
	}




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

			if (debugMode)
			{
				cout << "number of processes required: " << numProcesses << endl;
			}

			// Input file data into shared memory
			inputData(debugMode, inputFile);

			// Create child processes
			createChildProcesses(debugMode, (numProcesses - 1));

			// Begin handling chunks/jobs
			handleJobs(debugMode);
		}
		else
		{
			cout << "Enter a 1 or 0 for debug mode." << endl;
		}
	}

	return (0);
}
