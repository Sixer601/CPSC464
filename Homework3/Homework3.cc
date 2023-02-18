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

// PRE: memoryID is a defined integer representing the identifying integer the
//      operating system will use to create and attach to shared memory. pKey
//      is a defined key type that is used to provide an identifier for the shared
//      memory. pSpaceRequired is the number of bytes that need to be allocated
//      in shared memory for the program.
// POST: memoryID reflects the identifier the operating system uses for attaching
//       to the shared memory.
void createSharedMemory(int &memoryID, key_t pKey, int pSpaceRequired)
{
	memoryID = shmget(pKey, pSpaceRequired, IPC_CREAT | 0666);
	// ASSERT: memoryID now reflects the identifier the operating system uses for
	//         attaching to the shared memory.
	if (memoryID < 0)
	// ASSERT: memoryID is less than 0, meaning there was an error creating the memory.
	{
		throw(Exception((char *)"Problem creating shared memory segment."));
	}
}

// PRE: memoryID is a defined integer representing the identifying integer the
//      operating system will use to create and attach to shared memory.
// POST: RV is a pointer to the shared memory the operating system allocated.
int *attachToSharedMemory(int memoryID)
{
	int *shm = (int *)shmat(memoryID, NULL, 0); // shared memory pointer.
	// ASSERT: shm is equal to an integer pointer casting of the RV of shmat.
	if (shm == (int *)-1)
	// ASSERT: shm is a null pointer.
	{
		throw(Exception((char *)"Problem attaching to shared memory segment."));
	}
	return (shm);
}

// PRE: pInputFile is a defined input stream representing the file where the data
//      is being input from.
// POST: RV is an integer array containing the data input from pInputFile.
IntArray inputData(istream &pInputFile)
{
	IntArray data; // integer array to contain the data.
	// ASSERT: data is a defaultly constructed IntArray.
	while (pInputFile.peek() != EOF)
	// ASSERT: the next character from the input file is not the end of file character.
	{
		int datum; // temporary variable to store single point of data.
		// ASSERT: datum is undefined.
		pInputFile >> datum;
		data.addInt(datum);
	}
	return (data);
}

// PRE: pMemoryID is a defined integer containing the identifier for the shared memory
//      where the contents of pIntArray will be stored. pKey is a defined key type that
//      is used to provide an identifier for the shared memory. pIntArray is a defined
//      IntArray that contains the information to be stored in shared memory.
// POST: 
int *storeIntArrayInSharedMemory(int &pMemoryID, key_t pKey, IntArray pIntArray)
{
	int sharedMemorySpaceRequired = (sizeof(int) * pIntArray.getContentLength());
	// Variable storing number of bytes required to store contents of pIntArray.
	// ASSERT: sharedMemorySpaceRequired is equal to the product of the size of an
	//         integer in bytes and the content length of pIntArray.
	int *sharedMemoryPointer; // pointer to point to shared memory.
	// ASSERT: sharedMemoryPointer is undefined.
	try
	{
		createSharedMemory(pMemoryID, pKey, sharedMemorySpaceRequired);
		sharedMemoryPointer = attachToSharedMemory(pMemoryID);
		// ASSERT: sharedMemoryPointer now points to the shared memory.
	}
	catch (Exception error)
	{
		error.handle();
	}
	for (int i = 0; i < pIntArray.getContentLength(); i++)
	// ASSERT: i is less than the content length of pIntArray.
	{
		sharedMemoryPointer[i] = pIntArray.getNthIntInArray(i);
		// ASSERT: the value at index i of sharedMemoryPointer is equal to the ith
		//         value in pIntArray.
	}
	return (sharedMemoryPointer);
}

// PRE: numChunks is a defined integer that represents the number of chunks to sort and merge.
// POST: RV is an IntArray containing the information the processes will use to handle
//       delegating work.
IntArray createJobBoard(int numChunks)
{
	IntArray jobBoard; // IntArray to store job board.
	// ASSERT: jobBoard is a defaultly constructed IntArray.
	for (int i = 0; i < numChunks; i++)
	// ASSERT: i is less than the number of chunks.
	{
		jobBoard.addInt(i);
		// ASSERT: the ith from chunk is added to the job board.
		jobBoard.addInt(i);
		// ASSERT: the ith to chunk is added to the job board.
		jobBoard.addInt(UNSORTED);
		// ASSERT: the ith job status is added to the job board.
	}
	return (jobBoard);
}

// PRE: numChildrenNeeded is a defined integer representing the number of children needed.
// POST: RV is if the process running is a child process if true, or a parent process if false.
//       numChildrenNeeded child processes are created.
bool createChildProcesses(int numChildrenNeeded)
{
	int numChildren = 0; // integer representing current number of children processes made.
	// ASSERT: numChildren begins at 0, as no children processes have been made at this point.
	bool child = false; // boolean representing if the current process is a child or not.
	// ASSERT: child begins as false, as the parent is the one running this code.
	while ((numChildren < numChildrenNeeded) && (!child))
	// ASSERT: both numChildren is less than numChildrenNeeded and child is false.
	{
		pid_t childPID = fork(); // process id for child process.
		// ASSERT: childPID is the RV of fork.
		if (childPID == -1)
		// ASSERT: childPID is equal to -1.
		{
			perror("fork");
		}
		else if (childPID == 0)
		// ASSERT: childPID is equal to 0.
		{
			child = true;
			// ASSERT: the current process is a child process.
		}
		numChildren++;
	}
	return (child);
}

// PRE: numJobs is a defined integer representing the number of jobs within the job board.
//      data is a defined array of integers that contains the data to sort. 
//      jobBoard is a defined array of integers that contains all job encodings.
//      sizeOfLastChunk is a defined integer that contains the number of numbers stored in
//      the last chunk of data. dataSize is a defined integer that represents the number of 
//      numbers stored within data.
// POST: data is sorted.
void beginWork(int numJobs, int dataMemoryID, int jobBoardMemoryID, int sizeOfLastChunk, int dataSize)
{
	int * data = attachToSharedMemory(dataMemoryID);
	int * jobBoard = attachToSharedMemory(jobBoardMemoryID);
	bool workDone = false; // bool to keep track of if all work needing to be done is finished.
	// ASSERT: workDone begins as false, as we assume data is not already sorted.
	int lastJob = (numJobs - 1); // helper variable to keep track of last job.
	// ASSERT: lastJob is equal to the difference of numJobs and 1.
	while (!workDone)
	// ASSERT: there is more work for the process running the code to do.
	{
		for (int i = 0; i < numJobs; i++)
		// ASSERT: i is less than the number of jobs.
		{
			int fromChunkLocation = ((JOBSPACE * (i + 1)) - FROMCHUNK); // index where ith fromChunk
															// is stored in jobBoard.
			// ASSERT: fromChunkLocation is equal to the difference of the product of JOBSPACE and
			//         the sum of i and 1, and FROMCHUNK.
			int toChunkLocation = ((JOBSPACE * (i + 1)) - TOCHUNK); // index where ith toChunk
														 // is stored in jobBoard.
			// ASSERT: toChunkLocation is equal to the difference of the product of JOBSPACE and
			//         the sum of i and 1, and TOCHUNK.
			int jobStatusLocation = ((JOBSPACE * (i + 1)) - JOBSTATUS); // index where ith jobStatus
															// is stored in jobBoard.
			// ASSERT: jobStatusLocation is equal to the difference of the product of JOBSPACE and
			//         the sum of i and 1, and JOBSTATUS.
			int fromChunk = jobBoard[fromChunkLocation]; // ith fromChunk in jobBoard.
			// ASSERT: fromChunk is equal to the fromChunkLocation-th index in jobBoard.
			int toChunk = jobBoard[toChunkLocation]; // ith toChunk in jobBoard.
			// ASSERT: toChunk is equal to the toChunkLocation-th index in jobBoard.
			int jobStatus = jobBoard[jobStatusLocation]; // ith jobStatus in jobBoard.
			// ASSERT: jobStatus is equal to the jobStatusLocation-th index in jobBoard.
			int adjFromChunkLocation = (toChunk + 1) * JOBSPACE; // adjacent fromChunk index in jobBoard.
			// ASSERT: adjFromChunkLocation is equal to the product of the sum of toChunk and 1, and JOBSPACE.
			int adjJobStatusLocation = adjFromChunkLocation + 2; // adjacent jobStatus index in jobBoard.
			// ASSERT: adjJobStatusLocation is equal to the sum of adjFromChunkLocation and 2.
			int adjFromChunk = jobBoard[adjFromChunkLocation]; // adjacent from chunk in jobBoard.
			// ASSERT: adjFromChunk is equal to the adjFromChunkLocation-th index of jobBoard.
			int adjToChunk = jobBoard[adjFromChunkLocation + 1]; // adjacent to chunk in jobBoard.
			// ASSERT: adjToChunk is equal to the sum of adjFromChunkLocation and 1-th index of jobBoard.
			int adjJobStatus = jobBoard[adjJobStatusLocation]; // adjacent job status in jobBoard.
			// ASSERT: adjJobStatus is equal to the adjJobStatusLocation-th index of jobBoard.
			if (jobStatus == UNSORTED)
			// ASSERT: jobStatus is unsorted. 
			{
				jobBoard[jobStatusLocation] = BUSY;
				// ASSERT: jobStatus is updated to busy while job is occurring.
				int start = (CHUNKSIZE * fromChunk); // index to start sorting at.
				// ASSERT: start is equal to the product of CHUNKSIZE and fromChunk.
				int end; // index to finish sorting at.
				// ASSERT: end is undefined.
				(fromChunk == lastJob) ? end = dataSize - 1 : end = ((CHUNKSIZE * toChunk) + CHUNKSIZE - 1);
				// ASSERT: if fromChunk is equal to lastJob, end is the last index in data. otherwise, its the
				//         sum of the product of CHUNKSIZE and toChunk, and the difference of CHUNKSIZE and 1.
				mergeSort(data, start, end);
				cout << "PID: " << getpid() << " sorted chunk: " << fromChunk << endl;
				(numJobs != 1) ? jobBoard[jobStatusLocation] = SORTED : jobBoard[jobStatusLocation] = COMPLETED;
				// ASSERT: if the number of jobs is not 1, set the jobStatus for this job to sorted. otherwise
				//         set the jobStatus for this job to completed.
			}
			else if ((jobStatus == SORTED) && (adjJobStatus == SORTED))
			// ASSERT: both jobStatus is SORTED and the adjacent job status is SORTED.
			{
				jobBoard[jobStatusLocation] = BUSY;
				// ASSERT: update the current job's status to busy while the job is being processed.
				int start = (CHUNKSIZE * fromChunk); // index to denote beginning of first part to merge.
				// ASSERT: start is equal to the product of CHUNKSIZE and fromChunk.
				int middle = (CHUNKSIZE * adjFromChunk); // index to denote beginning of second part 
												 // and end of first part to merge.
				// ASSERT: middle is equal to the product of CHUNKSIZE and adjFromChunk.
				int end; // index to denote the end of the second part to merge.
				// ASSERT: end is undefined.
				(adjToChunk == lastJob) ? end = (dataSize - 1) : end = ((CHUNKSIZE * (adjToChunk + 1)) - 1);
				// ASSERT: if the adjacent to chunk is the same as the last job, end is equal to the last index of 
				//         data, otherwise, end is equal to the difference of the product of CHUNKSIZE, and the sum
				//         of adjToChunk and 1, and 1.
				merge(data, start, middle, end);
				cout << "PID: " << getpid() << " merged chunks: " << fromChunk << " - " << adjFromChunk << endl;
				jobBoard[toChunkLocation] = adjToChunk;
				// ASSERT: toChunk is updated to be the adjacent to chunk.
				((fromChunk == 0) && (jobBoard[toChunkLocation] == lastJob)) ? (jobBoard[jobStatusLocation] = COMPLETED) : (jobBoard[jobStatusLocation] = SORTED);
				// ASSERT: if both fromChunk is 0, and the to chunk is the last job, then the job status is COMPLETED,
				//         otherwise the job status is SORTED.
				jobBoard[adjJobStatusLocation] = COMPLETED;
				// ASSERT: the adjacent job status is set to COMPLETED.
			}
		}
		if ((jobBoard[1] == lastJob) && (jobBoard[2] == COMPLETED))
		// ASSERT: both the first job's to chunk is the last job, and the first job's job status is COMPLETED.
		{
			workDone = true;
			// ASSERT: all work is completed.
		}
	}
}

// PRE: 
// POST: sorted contents of input file are output to standard output. 
int main(int argc, char *argv[])
{
	if (argc != 3)
	// ASSERT: 2 arguments were not passed to the program.
	{
		cout << "Invalid Number of Arguments." << endl;
	}
	else
	{
		ifstream inputFile(argv[1]); // input file passed to the program.
		// ASSERT: inputFile is the first argument.
		int numChildren = stoi(argv[2]); // number of children to farm out work to.
		// ASSERT: numChildren is equal to the integer equivalent to the second argument.
		int dataMemoryID; // represents the identifying integer the operating system will use
					   // to create and attach to shared memory for data.
		// ASSERT: dataMemoryID is undefined.
		IntArray data = inputData(inputFile); // IntArray to store data from file.
		// ASSERT: data is the RV of inputData.
		int numChunks = ceiling(data.getContentLength(), CHUNKSIZE); // number of chunks to sort.
		// ASSERT: numChunks is equal to ⌈(number of numbers in data) / CHUNKSIZE⌉.
		int sizeOfLastChunk = data.getContentLength() % CHUNKSIZE == 0 ? CHUNKSIZE : (data.getContentLength() % CHUNKSIZE);
		// number of numbers in the last chunk.
		// ASSERT: if the number of numbers in data is cleanly divisibly by CHUNKSIZE,
		//         the size of the last chunk is CHUNKSIZE, otherwise, it is the result of
		//         the number of numbers in data mod CHUNKSIZE.
		key_t dataMemoryKey = 010; // key to the data shared memory.
		// ASSERT: dataMemoryKey is equal to 010.
		int *pointerToDataSharedMemory = storeIntArrayInSharedMemory(dataMemoryID, dataMemoryKey, data); 
		// pointer to the shared memory where data is stored.
		// ASSERT: pointerToDataSharedMemory is equal to the RV of storeIntArrayInSharedMemory.
		int jobBoardMemoryID; // represents the identifying integer the operating system will use
					   	  // to create and attach to shared memory for the job board.
		// ASSERT: jobBoardMemoryID is undefined.
		IntArray jobBoard = createJobBoard(numChunks); // IntArray to store the job board.
		// ASSERT: jobBoard is equal to the RV of createJobBoard.
		key_t jobBoardMemoryKey = 1010101; // key to the job board shared memory.
		// ASSERT: jobBoardMemoryKey is equal to 1010101.
		int *pointerToJobBoardSharedMemory = storeIntArrayInSharedMemory(jobBoardMemoryID, jobBoardMemoryKey, jobBoard); 
		// pointer to the shared memory where job board is stored.
		// ASSERT: pointerToJobBoardSharedMemory is equal to the RV of storeIntArrayInSharedMemory.
		bool isChild = createChildProcesses(numChildren); // bool to track if process is child process.
		// ASSERT: isChild is equal to RV of createChildProcesses.
		beginWork(numChunks, dataMemoryID, jobBoardMemoryID, sizeOfLastChunk, data.getContentLength());
		printArray((char *)"Sorted List: ", pointerToDataSharedMemory, 0, data.getContentLength() - 1);
		if ((!isChild))
		// ASSERT: process is the parent process.
		{
			shmctl(dataMemoryID, IPC_RMID, NULL);
			// ASSERT: deallocates shared memory for data.
			shmctl(jobBoardMemoryID, IPC_RMID, NULL);
			// ASSERT: deallocates shared memory for job board.
		}
	}
	return (0);
}
