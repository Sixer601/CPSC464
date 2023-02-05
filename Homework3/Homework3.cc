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
	int jobBoard_mem_id;

	char *shm, *jobs;

	int numJobs = numChunks + ceiling(numChunks, 2);

	jobBoardSpaceRequired = (sizeof(char) * numJobs);

	jobBoard_mem_id = shmget(jobKey, jobBoardSpaceRequired, IPC_CREAT | 0666);
	if (jobBoard_mem_id < 0)
	{
		perror("shmget");
	}
	else
	{
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
	int shared_mem_id;

	int *shm, *information;

	numChunks = ceiling(pIntArray.getContentLength(), CHUNKSIZE);

	informationSpaceRequired = ((sizeof(int) * (pIntArray.getContentLength())));

	shared_mem_id = shmget(key, informationSpaceRequired, IPC_CREAT | 0666);
	if (shared_mem_id < 0)
	{
		perror("shmget");
	}
	else
	{
		shm = (int *)shmat(shared_mem_id, NULL, 0);
		if (shm == (int *)-1)
		{
			perror("shmat");
		}
		else
		{
			information = shm;

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

	IntArray data;
	while (pInputFile.peek() != EOF)
	{
		int datum;
		pInputFile >> datum;
		data.addInt(datum);
	}

	createSharedMemory(debugMode, data, key, spaceRequired, numChunks);
}

// PRE:
// POST:
int createChildProcesses(bool debugMode, int neededChildrenNum, int &childNum)
{
	bool child = false;
	while (childNum < neededChildrenNum && !child)
	{
		pid_t childPID = fork();
		if (childPID == -1)
		{
			perror("fork");
		}
		else if (childPID == 0)
		{
			childNum++;
			child = true;
		}
		else
		{
			childNum++;
		}
	}
	if (!child)
	{
		childNum = 0;
	}
	return (childNum);
}

// PRE:
// POST:
void handleJobs(bool debugMode, int childNum, int numChunks, int numProcesses, key_t jobKey, key_t infoKey, int &jobBoardSpaceRequired, int &infoSpaceRequired)
{
	int jobBoardSHMid, infoSHMid;

	char *jobSHM;
	int *infoSHM;

	if ((jobBoardSHMid = shmget(jobKey, jobBoardSpaceRequired, 0666)) < 0)
	{
		perror("shmget");
	}
	else
	{
		if ((jobSHM = (char *)shmat(jobBoardSHMid, NULL, 0)) == (char *)-1)
		{
			perror("shmat");
		}
		else
		{
			if ((infoSHMid = shmget(infoKey, infoSpaceRequired, 0666)) < 0)
			{
				perror("shmget");
			}
			else
			{
				if ((infoSHM = (int *)shmat(infoSHMid, NULL, 0)) == (int *)-1)
				{
					perror("shmat");
				}
				else
				{
					int startingChunk = ceiling(numChunks, numProcesses) * childNum;
					if (startingChunk < numChunks)
					{
						cout << "Child: " << childNum << " working on chunk: " << startingChunk + 1 << " with data: ";
						if (((startingChunk * (CHUNKSIZE)) + CHUNKSIZE) > (infoSpaceRequired / sizeof(int)))
						{
							for (int i = startingChunk * CHUNKSIZE; i < (infoSpaceRequired / sizeof(int)); i++)
							{
								cout << infoSHM[i] << " ";
							}
							cout << endl;
						}
						else
						{
							for (int i = startingChunk * CHUNKSIZE; i < ((startingChunk * (CHUNKSIZE)) + CHUNKSIZE); i++)
							{
								cout << i << " ";
							}
							cout << endl;
						}
					}
				}
			}
		}
	}
}

// PRE:
// POST:
int main(int argc, char **argv)
{

	if (argc != 3)
	{
		cout << "Invalid number of arguments." << endl;
	}
	else
	{
		bool debugMode;
		int numChildren = stoi(argv[2]);
		ifstream inputFile(argv[1]);
		key_t informationKey = 914615;
		key_t jobKey = 10152;
		int informationSpaceRequired = 0;
		int jobBoardSpaceRequired = 0;
		int numChunks = 0;
		int childNum = 0;

		inputData(debugMode, inputFile, informationKey, informationSpaceRequired, numChunks);
		createJobBoard(debugMode, jobKey, numChunks, jobBoardSpaceRequired);
		createChildProcesses(debugMode, numChildren, childNum);
		handleJobs(debugMode, childNum, numChunks, (numChildren + 1), jobKey, informationKey, jobBoardSpaceRequired, informationSpaceRequired);
	}

	return (0);
}
