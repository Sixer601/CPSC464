#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include "IntArray.h"
#include "HelperFunctions.h"
#include "Exception.h"
#include "Constants.h"

using namespace std;

void createSharedMemory(int &memoryID, key_t pKey, int pSpaceRequired)
{
	memoryID = shmget(pKey, pSpaceRequired, IPC_CREAT | 0666);
	if (memoryID < 0)
	{
		throw(Exception((char *)"Problem allocating shared memory segment."));
	}
}

int *attachToSharedMemory(int memoryID)
{
	int *shm = (int *)shmat(memoryID, NULL, 0);
	if (shm == (int *)-1)
	{
		throw(Exception((char *)"Problem attaching to shared memory segment."));
	}
	return (shm);
}

IntArray inputData(istream &pInputFile)
{
	IntArray data;
	while (pInputFile.peek() != EOF)
	{
		int datum;
		pInputFile >> datum;
		data.addInt(datum);
	}
	return (data);
}

int *storeIntArrayInSharedMemory(int &pMemoryID, key_t pKey, IntArray pIntArray)
{
	int sharedMemorySpaceRequired = (sizeof(int) * pIntArray.getContentLength());
	int *sharedMemoryPointer;
	try
	{
		createSharedMemory(pMemoryID, pKey, sharedMemorySpaceRequired);
		sharedMemoryPointer = attachToSharedMemory(pMemoryID);
	}
	catch (Exception error)
	{
		error.handle();
	}
	for (int i = 0; i < pIntArray.getContentLength(); i++)
	{
		sharedMemoryPointer[i] = pIntArray.getNthIntInArray(i);
	}
	return (sharedMemoryPointer);
}

IntArray createJobBoard(int numChunks)
{
	IntArray jobBoard;
	for (int i = 0; i < numChunks; i++)
	{
		jobBoard.addInt(i);
		jobBoard.addInt(i);
		jobBoard.addInt(UNSORTED);
	}
	return (jobBoard);
}

bool createChildProcesses(int numChildrenNeeded)
{
	int numChildren = 0;
	bool child = false;
	while ((numChildren < numChildrenNeeded) && (!child))
	{
		pid_t childPID = fork();
		if (childPID == -1)
		{
			perror("fork");
		}
		else if (childPID == 0)
		{
			child = true;
		}
		numChildren++;
	}
	return (child);
}

void beginWork(int numJobs, int *data, int *jobBoard, int sizeOfLastChunk)
{
	bool workDone = false;
	while (!workDone)
	{
		int numJobsCompleted = 0;
		for (int i = 0; i < numJobs; i++)
		{
			int lastJob = (numJobs - 1);
			int fromChunkLocation = ((JOBSPACE * (i + 1)) - FROMCHUNK);
			int toChunkLocation = ((JOBSPACE * (i + 1)) - TOCHUNK);
			int jobStatusLocation = ((JOBSPACE * (i + 1)) - JOBSTATUS);
			int fromChunk = jobBoard[fromChunkLocation];
			int toChunk = jobBoard[toChunkLocation];
			int jobStatus = jobBoard[jobStatusLocation];
			int adjFromChunkLocation = toChunk * JOBSPACE;
			int adjToChunkLocation = adjFromChunkLocation + 1;
			int adjJobStatusLocation = adjFromChunkLocation + 2;
			int adjFromChunk = jobBoard[adjFromChunkLocation];
			int adjToChunk = jobBoard[adjToChunkLocation];
			int adjJobStatus = jobBoard[adjJobStatusLocation];
			if (jobStatus == UNSORTED)
			{
				jobBoard[jobStatusLocation] = BUSY;
				(fromChunk == lastJob) ? mergeSort(data, (CHUNKSIZE * fromChunk), ((CHUNKSIZE * toChunk) + sizeOfLastChunk - 1)) : mergeSort(data, (CHUNKSIZE * fromChunk), ((CHUNKSIZE * toChunk) + CHUNKSIZE - 1));
				if (fromChunk != lastJob)
				{
					jobBoard[toChunkLocation] = fromChunk + 1;
				}
				jobBoard[jobStatusLocation] = SORTED;
			}
			else if (jobStatus == SORTED)
			{
				jobBoard[jobStatusLocation] = BUSY;
				if ((i != lastJob) && (adjJobStatus == SORTED))
				{
					cout << "adjacentChunkInformation: [" << adjFromChunk << ", " << adjToChunk << ", " << adjJobStatus << "]" << endl;
					merge(data, (CHUNKSIZE * fromChunk), (CHUNKSIZE * adjFromChunk), ((CHUNKSIZE * adjToChunk) - 1));
					jobBoard[toChunkLocation] = adjToChunk;
					jobBoard[adjJobStatusLocation] = COMPLETED;
				}
				if (fromChunk == 0 && adjToChunk == lastJob)
				{
					if(sizeOfLastChunk != CHUNKSIZE)
					{					
						merge(data, (CHUNKSIZE * fromChunk), (CHUNKSIZE * lastJob), ((CHUNKSIZE * (lastJob + 1)) - sizeOfLastChunk - 1));
					}
					else
					{
						merge(data, (CHUNKSIZE * fromChunk), (CHUNKSIZE * lastJob), ((CHUNKSIZE * (lastJob)) - 1));
					}
					jobBoard[jobStatusLocation] = COMPLETED;
				}
				else
				{
					jobBoard[jobStatusLocation] = SORTED;
				}
			}
			else if (jobStatus == COMPLETED)
			{
				numJobsCompleted++;
			}
		}
		if (numJobsCompleted == numJobs)
		{
			workDone = true;
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "Invalid Number of Arguments." << endl;
	}
	else
	{
		ifstream inputFile(argv[1]);
		int numChildren = stoi(argv[2]);
		int dataMemoryID;
		IntArray data = inputData(inputFile);
		int numChunks = ceiling(data.getContentLength(), CHUNKSIZE);
		int sizeOfLastChunk = data.getContentLength() % CHUNKSIZE == 0 ? CHUNKSIZE : (data.getContentLength() % CHUNKSIZE);
		key_t dataMemoryKey = 1234;
		int *pointerToDataSharedMemory = storeIntArrayInSharedMemory(dataMemoryID, dataMemoryKey, data);
		int jobBoardMemoryID;
		IntArray jobBoard = createJobBoard(numChunks);
		key_t jobBoardMemoryKey = 5678;
		int *pointerToJobBoardSharedMemory = storeIntArrayInSharedMemory(jobBoardMemoryID, jobBoardMemoryKey, jobBoard);
		bool isChild = createChildProcesses(numChildren);
		beginWork(numChunks, pointerToDataSharedMemory, pointerToJobBoardSharedMemory, sizeOfLastChunk);
		cout << "Sorted List: ";
		for(int i = 0; i < data.getContentLength(); i++)
		{
			cout << pointerToDataSharedMemory[i] << " ";
		}
		cout << endl;
		if ((!isChild))
		{
			shmctl(dataMemoryID, IPC_RMID, NULL);
			shmctl(jobBoardMemoryID, IPC_RMID, NULL);
		}
	}
	return (0);
}
