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

void beginWork(int numJobs, int *data, int *jobBoard, int sizeOfLastChunk, int dataSize)
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
			int adjFromChunkLocation = (toChunk + 1) * JOBSPACE;
			int adjToChunkLocation = adjFromChunkLocation + 1;
			int adjJobStatusLocation = adjFromChunkLocation + 2;
			int adjFromChunk = jobBoard[adjFromChunkLocation];
			int adjToChunk = jobBoard[adjToChunkLocation];
			int adjJobStatus = jobBoard[adjJobStatusLocation];
			if (jobStatus == UNSORTED)
			{
				jobBoard[jobStatusLocation] = BUSY;
				int start = (CHUNKSIZE * fromChunk);
				int end;
				(fromChunk == lastJob) ? end = dataSize - 1 : end = ((CHUNKSIZE * toChunk) + CHUNKSIZE - 1);
				mergeSort(data, start, end);
				//printArray((char *)"After merge sort: ", data, start, end);
				jobBoard[jobStatusLocation] = SORTED;
				//printArray((char *)"Job Board after merge sort: ", jobBoard, 0, (JOBSPACE * numJobs) - 1);
			}
			else if ((jobStatus == SORTED) && (adjJobStatus == SORTED))
			{
				jobBoard[jobStatusLocation] = BUSY;
				int start = (CHUNKSIZE * fromChunk);
				int middle = (CHUNKSIZE * adjFromChunk);
				int end;
				(adjToChunk == lastJob) ? end = (dataSize - 1) : end = ((CHUNKSIZE * (adjToChunk + 1)) - 1);
				merge(data, start, middle, end);
				//printArray((char *)"After merge sort: ", data, start, end);
				jobBoard[toChunkLocation] = adjToChunk;
				((fromChunk == 0) && (jobBoard[toChunkLocation] == lastJob)) ? (jobBoard[jobStatusLocation] = COMPLETED) : (jobBoard[jobStatusLocation] = SORTED);
				jobBoard[adjJobStatusLocation] = COMPLETED;
				//printArray((char *)"Job Board after merge: ", jobBoard, 0, (JOBSPACE * numJobs) - 1);
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
		beginWork(numChunks, pointerToDataSharedMemory, pointerToJobBoardSharedMemory, sizeOfLastChunk, data.getContentLength());
		printArray((char *)"Sorted List: ", pointerToDataSharedMemory, 0, data.getContentLength() - 1);
		if ((!isChild))
		{
			shmctl(dataMemoryID, IPC_RMID, NULL);
			shmctl(jobBoardMemoryID, IPC_RMID, NULL);
		}
	}
	return (0);
}
