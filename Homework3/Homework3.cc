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
void createJobBoard(key_t jobKey, int &numChunks, int &jobBoardSpaceRequired)
{
	int jobBoard_mem_id; //
	// ASSERT: 
	char *shm; //
	// ASSERT: 
	char *jobs; //
	// ASSERT: 
	int numJobs = numChunks + ceiling(numChunks, 2); //
	// ASSERT: 
	jobBoardSpaceRequired = (sizeof(char) * numJobs);
	// ASSERT: 
	jobBoard_mem_id = shmget(jobKey, jobBoardSpaceRequired, IPC_CREAT | 0666);
	// ASSERT: 
	if (jobBoard_mem_id < 0)
	// ASSERT: 
	{
		perror("shmget");
	}
	else
	{
		shm = (char *)shmat(jobBoard_mem_id, NULL, 0);
		// ASSERT: 
		if (shm == (char *)-1)
		// ASSERT: 
		{
			perror("shmat");
		}
		else
		{
			jobs = shm;
			// ASSERT: 
			for (int i = 0; i < numChunks; i++)
			// ASSERT: 
			{
				jobs[i] = 'S';
				// ASSERT: 
			}

			for (int j = numChunks; j < numJobs; j++)
			// ASSERT: 
			{
				jobs[j] = 'M';
				// ASSERT: 
			}
		}
	}
}

// PRE:
// POST:
void createSharedMemory(IntArray pIntArray, key_t key, int &informationSpaceRequired, int &numChunks)
{
	int shared_mem_id; //
	// ASSERT: 
	int *shm; //
	// ASSERT: 
	int *information; //
	// ASSERT: 
	numChunks = ceiling(pIntArray.getContentLength(), CHUNKSIZE);
	// ASSERT: 
	informationSpaceRequired = ((sizeof(int) * (pIntArray.getContentLength())));
	// ASSERT: 
	shared_mem_id = shmget(key, informationSpaceRequired, IPC_CREAT | 0666);
	// ASSERT: 
	if (shared_mem_id < 0)
	// ASSERT: 
	{
		perror("shmget");
	}
	else
	{
		shm = (int *)shmat(shared_mem_id, NULL, 0);
		// ASSERT: 
		if (shm == (int *)-1)
		// ASSERT: 
		{
			perror("shmat");
		}
		else
		{
			information = shm;
			// ASSERT: 
			for (int j = 0; j < pIntArray.getContentLength(); j++)
			// ASSERT: 
			{
				information[j] = pIntArray.getNthIntInArray(j);
				// ASSERT: 
			}
		}
	}
}

// PRE:
// POST:
void inputData(istream &pInputFile, key_t key, int &spaceRequired, int &numChunks)
{
	IntArray data; //
	// ASSERT: 
	while (pInputFile.peek() != EOF)
	// ASSERT: 
	{
		int datum; //
		// ASSERT: 
		pInputFile >> datum;
		data.addInt(datum);
	}
	createSharedMemory(data, key, spaceRequired, numChunks);
}

// PRE:
// POST:
int createChildProcesses(int neededChildrenNum, int &childNum)
{
	bool child = false; // 
	// ASSERT: 
	while (childNum < neededChildrenNum && !child)
	// ASSERT: 
	{
		pid_t childPID = fork(); //
		// ASSERT: 
		if (childPID == -1)
		// ASSERT: 
		{
			perror("fork");
		}
		else if (childPID == 0)
		// ASSERT: 
		{
			child = true;
			// ASSERT: 
		}
		childNum++;
		// ASSERT: 
	}
	if (!child)
	// ASSERT: 
	{
		childNum = 0;
		// ASSERT: 
	}
	return (childNum);
}

// PRE: childNum is a defined integer that represents which child the current process running this function is. numChunks is a defined integer
//      that represents the number of chunks needing to be sorted. numProcesses is a defined integer that represents the number of processes
//      the overall program is utilizing. jobKey is a defined key that is used in accessing the memory that stores the job information for the
//      processes. infoKey is a defined key that is used in accessing the numbers that need to be handled by the different jobs.
// POST:
void handleJobs(int childNum, int numChunks, int numProcesses, key_t jobKey, key_t infoKey, int &jobBoardSpaceRequired, int &infoSpaceRequired)
{
	int jobBoardSHMid; //
	// ASSERT:
	int infoSHMid; //
	// ASSERT:
	char *jobSHM; //
	// ASSERT:
	int *infoSHM; //
	// ASSERT:
	if ((jobBoardSHMid = shmget(jobKey, jobBoardSpaceRequired, 0666)) < 0)
	// ASSERT:
	{
		perror("shmget");
	}
	else
	{
		if ((jobSHM = (char *)shmat(jobBoardSHMid, NULL, 0)) == (char *)-1)
		// ASSERT:
		{
			perror("shmat");
		}
		else
		{
			if ((infoSHMid = shmget(infoKey, infoSpaceRequired, 0666)) < 0)
			// ASSERT:
			{
				perror("shmget");
			}
			else
			{
				if ((infoSHM = (int *)shmat(infoSHMid, NULL, 0)) == (int *)-1)
				// ASSERT:
				{
					perror("shmat");
				}
				else
				{
					int startingChunk = ceiling(numChunks, numProcesses) * childNum; //
					// ASSERT:
					if (startingChunk < numChunks)
					// ASSERT:
					{
						if (((startingChunk * (CHUNKSIZE)) + CHUNKSIZE) > (infoSpaceRequired / sizeof(int)))
						// ASSERT:
						{
						}
						else
						{
						}
					}
				}
			}
		}
	}
}

// PRE: argc contains the number of arguments passed to the program. argv are the arguments passed to the program.
// POST:
int main(int argc, char **argv)
{
	if (argc != 3)
	// ASSERT:
	{
		cout << "Invalid number of arguments." << endl;
	}
	else
	{
		int numChildren = stoi(argv[2]); //
		// ASSERT:
		ifstream inputFile(argv[1]); //
		// ASSERT:
		key_t informationKey = 914615; //
		// ASSERT:
		key_t jobKey = 10152; //
		// ASSERT:
		int informationSpaceRequired = 0; //
		// ASSERT:
		int jobBoardSpaceRequired = 0; //
		// ASSERT:
		int numChunks = 0; //
		// ASSERT:
		int childNum = 0; //
		// ASSERT:

		inputData(inputFile, informationKey, informationSpaceRequired, numChunks);
		createJobBoard(jobKey, numChunks, jobBoardSpaceRequired);
		createChildProcesses(numChildren, childNum);
		handleJobs(childNum, numChunks, (numChildren + 1), jobKey, informationKey, jobBoardSpaceRequired, informationSpaceRequired);
	}
	return (0);
}
