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
void createJobBoard(bool debugMode, key_t jobKey, int &numChunks, int &jobBoardSpaceRequired)
{
	if (debugMode)
	{
		cout << "Entered createJobBoard" << endl;
	}

	int jobBoard_mem_id;

	char *shm, *jobs;

	int numJobs = numChunks + ceiling(numChunks, 2);

	if (debugMode)
	{
		cout << "Number of jobs to handle: " << numJobs << endl;
	}

	jobBoardSpaceRequired = (sizeof(char) * numJobs);

	if (debugMode)
	{
		cout << "Space Required for Job Board: " << jobBoardSpaceRequired << endl;
	}

	// Create shared memory segment
	jobBoard_mem_id = shmget(jobKey, jobBoardSpaceRequired, IPC_CREAT | 0666);
	if (jobBoard_mem_id < 0)
	{
		perror("shmget");
	}
	else
	{
		// Attach segment to our data space
		shm = (char *)shmat(jobBoard_mem_id, NULL, 0);
		if (shm == (char *)-1)
		{
			perror("shmat");
		}
		else
		{
			jobs = shm;

			for (int i = 0; i < numChunks; i++)
			{
				jobs[i] = 'S';
			}

			for (int j = numChunks; j < numJobs; j++)
			{
				jobs[j] = 'M';
			}
		}
	}
}

// PRE:
// POST:
void createSharedMemory(bool debugMode, IntArray pIntArray, key_t key, int &informationSpaceRequired, int &numChunks)
{
	if (debugMode)
	{
		cout << "Entered createSharedMemory." << endl;
	}

	int shared_mem_id;

	int *shm, *information;

	numChunks = ceiling(pIntArray.getContentLength(), CHUNKSIZE);

	if (debugMode)
	{
		cout << "Number of chunks to handle: " << numChunks << endl;
	}

	// TODO: Calculate space required for intArray
	informationSpaceRequired = ((sizeof(int) * (pIntArray.getContentLength())));

	if (debugMode)
	{
		cout << "Space Required: " << informationSpaceRequired << endl;
	}

	// Create shared memory segment
	shared_mem_id = shmget(key, informationSpaceRequired, IPC_CREAT | 0666);
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

			// Store data in memory for other processes to read
			for (int j = 0; j < pIntArray.getContentLength(); j++)
			{
				information[j] = pIntArray.getNthIntInArray(j);
			}

			if (debugMode)
			{
				cout << "Information: ";

				for (int k = 0; k < pIntArray.getContentLength(); k++)
				{
					cout << information[k] << " ";
				}
				cout << endl;
			}
		}
	}
}

// PRE:
// POST:
void inputData(bool debugMode, istream &pInputFile, key_t key, int &spaceRequired, int &numChunks)
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
	createSharedMemory(debugMode, data, key, spaceRequired, numChunks);
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
void handleJobs(bool debugMode, key_t jobKey, int &spaceRequired)
{
	int shmid;

	if (debugMode)
	{
		cout << getpid() << " Entered handleJobs with key: " << jobKey << endl;
	}

	char *shm, *s;

	/*
	 * Locate the segment.
	 */
	if ((shmid = shmget(jobKey, spaceRequired, 0666)) < 0)
	{
		perror("shmget");
	}
	else
	{
		/*
		 * Now we attach the segment to our data space.
		 */
		if ((shm = (char *)shmat(shmid, NULL, 0)) == (char *)-1)
		{
			perror("shmat");
			exit(1);
		}

		/*
		 * Now read what the server put in the memory.
		 */
		if (debugMode)
		{
			cout << "Jobs: ";

			for (int i = 0; i < (spaceRequired / sizeof(char)); i++)
			{
				cout << shm[i] << " ";
			}

			cout << endl;
		}


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
		key_t key = 914615;
		key_t jobKey = 10152;
		int informationSpaceRequired = 0;
		int jobBoardSpaceRequired = 0;
		int numChunks = 0;
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
				cout << "key: " << key << endl;
			}

			// Input file data into shared memory
			inputData(debugMode, inputFile, key, informationSpaceRequired, numChunks);

			// Create Job board to assign jobs to children
			createJobBoard(debugMode, jobKey, numChunks, jobBoardSpaceRequired);

			// Create child processes
			createChildProcesses(debugMode, (numProcesses - 1));

			// Begin handling chunks/jobs
			handleJobs(debugMode, jobKey, jobBoardSpaceRequired);
		}
		else
		{
			cout << "Enter a 1 or 0 for debug mode." << endl;
		}
	}

	return (0);
}
