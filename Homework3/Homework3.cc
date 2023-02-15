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

// PRE:
// POST:
void createSharedMemory(key_t pKey, int pSpaceRequired, int &memoryID)
{
	memoryID = shmget(pKey, pSpaceRequired, IPC_CREAT | 0666);
	if (memoryID < 0)
	// ASSERT:
	{
		throw(Exception((char *)"Problem allocating shared memory segment."));
	}
}

// PRE:
// POST:
void accessSharedMemory(key_t pKey, int pSpaceRequired, int &memoryID)
{
	memoryID = shmget(pKey, pSpaceRequired, 0666);
	if (memoryID == -1)
	// ASSERT:
	{
		throw(Exception((char *)"Problem accessing shared memory segment."));
	}
}

// PRE:
// POST:
int *attachToSharedMemory(int &memoryID)
{
	int * shm = (int *)shmat(memoryID, NULL, 0);
	if (shm == (int *)-1)
	{
		throw(Exception((char *)"Problem attaching to shared memory segment."));
	}
	return (shm);
}

// PRE: jobKey is a defined key that represents the identifier for the shared
//      memory that the job board resides at. numChunks is a defined integer
//      that represents the number of chunks that the information being sorted
//      can be broken into. jobBoardSpaceRequired is a defined integer that
//      represents the number of bytes required to store the jobBoard.
// POST: There is a location in shared memory that is sized at the number of
//       bytes equal to jobBoardSpaceRequired. This memory's key is equal to
//       jobKey.
void createJobBoard(key_t jobKey, int &numChunks, int &jobBoardSpaceRequired)
{
	int jobBoard_mem_id; // identifier that aids in creating a space in shared
					 // memory creation.
	// ASSERT: jobBoard_mem_id is undefined.
	int *shm; // pointer to shared memory for once it is created.
	// ASSERT: shm is undefined.
	int *jobs; // pointer to reference items in shared memory.
	// ASSERT: jobs is undefined.
	jobBoardSpaceRequired = (sizeof(int) * numChunks) * 3;
	// ASSERT: jobBoardSpaceRequired is the size of a character for each job.
	try
	{
		createSharedMemory(jobKey, jobBoardSpaceRequired, jobBoard_mem_id);
		shm = attachToSharedMemory(jobBoard_mem_id);
	}
	catch (Exception error)
	{
		error.handle();
	}
	jobs = shm;
	// ASSERT: jobs points to the same location in memory that shm does.
	for (int i = 1; i <= numChunks; i++)
	// ASSERT: i is less than the number of chunks in the information.
	{
		int fromChunkLocation = (JOBSPACE * i) - FROMCHUNK;
		int toChunkLocation = (JOBSPACE * i) - TOCHUNK;
		int jobStatusLocation = (JOBSPACE * i) - JOBSTATUS;
		jobs[fromChunkLocation] = i;
		jobs[toChunkLocation] = i;
		jobs[jobStatusLocation] = UNSORTED;
	}
}

// PRE: pIntArray is a defined IntArray object that contains the stored information. infoKey is a defined
//      key object that represents the identifier to access the shared memory that will contain the
//      information. informationSpaceRequired is a defined integer that represents the number of bytes
//      required to store what pIntArray contains. numChunks is a defined integer that represents the
//      number of chunks the information stored in pIntArray can be broken into.
// POST: There is a location in shared memory that is sized at the number of
//       bytes equal to informationSpaceRequired. This memory's key is equal to infoKey.
void createInformationSpace(IntArray pIntArray, key_t infoKey, int &informationSpaceRequired, int &numChunks)
{
	int shared_mem_id; // identifier that aids in shared memory creation.
	// ASSERT: shared_mem_id is undefined.
	int *shm; // pointer to shared memory.
	// ASSERT: shm is undefined.
	int *information; // pointer to information being stored in shared memory.
	// ASSERT: information is undefined.
	numChunks = ceiling(pIntArray.getContentLength(), CHUNKSIZE);
	// ASSERT: numChunks is equal to the ceiling of the quotient of the RV of getContentLength for
	//         pIntArray and the predefined constant CHUNKSIZE.
	informationSpaceRequired = ((sizeof(int) * (pIntArray.getContentLength())));
	// ASSERT: informationSpaceRequired is equal to the size in bytes of an int for each int in
	//         pIntArray.
	try
	{
		createSharedMemory(infoKey, informationSpaceRequired, shared_mem_id);
		shm = attachToSharedMemory(shared_mem_id);
	}
	catch (Exception error)
	{
		error.handle();
	}
	information = shm;
	cout << hex << (void *)shm << dec << endl;
	// ASSERT: information points to the same location in memory as shm.
	for (int i = 0; i < pIntArray.getContentLength(); i++)
	// ASSERT: j is less than the number of items in pIntArray.
	{
		information[i] = pIntArray.getNthIntInArray(i);
		// ASSERT: the jth index of information is equal to the jth integer in
		//         pIntArray.
	}
}

// PRE: pInputFile is a defined input stream that contains the input file. infoKey is a defined
//      key that represents the identifier for the shared information in which the information
//      within pInputFile will be stored. spaceRequired is a defined integer that represents
//      the number of bytes required to store the information from pInputFile. numChunks is a
//      defined integer that represents the number of chunks the information can be broken up
//      into.
// POST: In shared memory, identified by infoKey, the contents of pInputFile are stored.
//       spaceRequired and numChunks are updated to represent the number of bytes that the
//       file contents take up in shared memory, and the number of chunks the information can
//       be broken up into.
void inputData(istream &pInputFile, key_t infoKey, int &spaceRequired, int &numChunks)
{
	IntArray data; // array of integers to store data from file.
	// ASSERT: data is an integer array constructed using the default constructor.
	while (pInputFile.peek() != EOF)
	// ASSERT: the next character in the file is not the end of file character.
	{
		int datum; // a single number from the input file.
		// ASSERT: datum is undefined.
		pInputFile >> datum;
		data.addInt(datum);
	}
	createInformationSpace(data, infoKey, spaceRequired, numChunks);
}

// PRE: neededChildrenNum is a defined integer that represents the number of child processes
//      requested by the user. childNum is a defined integer that represents a unique number
//      each process is given. The parent process contains a childNum of 0.
// POST: There are x+1 processes running the code following this function, where x is equal
//       to neededChildrenNum. Each of these processes contains a unique number, starting at
//       0, to denote which child they are relative to each other.
void createChildProcesses(int neededChildrenNum)
{
	int numChildren = 0;
	bool child = false; // identifier for if the process is a child process.
	// ASSERT: child begins as false, as the only process running this code is the parent
	//         process.
	while (numChildren < neededChildrenNum && !child)
	// ASSERT: the current identifier of the process is less than the number of children
	//         needed, and the current process does not have the child flag.
	{
		pid_t childPID = fork(); // childPID contains 0 if it is a child process, the
							// process ID of the parent process based on what the
							// computer gave it at the time of initialization, or
							// -1 if there was an error.
		if (childPID == -1)
		// ASSERT: childPID has a process number of -1.
		{
			perror("fork");
		}
		else if (childPID == 0)
		// ASSERT: the current process is a child process.
		{
			child = true;
			// ASSERT: the child flag is set to true.
		}
		numChildren++;
		// ASSERT: childNum is incremented by 1.
	}
}

// PRE:
// POST:
void findAndDoJob(int numChunks, int *jobBoard, int *infoSHM, bool &allComplete)
{
	int numComplete = 0; //
	// ASSERT:
	for (int i = 0; i < numChunks; i++)
	// ASSERT:
	{
		int fromChunkLocation = (JOBSPACE * i) - FROMCHUNK; //
		// ASSERT:
		int toChunkLocation = (JOBSPACE * i) - TOCHUNK; //
		// ASSERT:
		int jobStatusLocation = (JOBSPACE * i) - JOBSTATUS; //
		// ASSERT:
		int jobStatus = jobBoard[jobStatusLocation]; //
		// ASSERT:
		cout << "Process " << getpid() << " is looking at a job with a jobStatus: " << jobStatus << endl;
		if (jobStatus == UNSORTED)
		// ASSERT: Job is to mergesort an unsorted chunk.
		{
			jobBoard[jobStatusLocation] = BUSY;
			// ASSERT: job i is set to busy while being worked on.
			int startingLocation = (jobBoard[fromChunkLocation] * CHUNKSIZE); //
			// ASSERT:
			int *temp = new int[CHUNKSIZE]; //
			// ASSERT:
			for (int j = startingLocation; j < (startingLocation + CHUNKSIZE); j++)
			// ASSERT:
			{
				temp[j] = infoSHM[j];
			}
			mergeSort(temp, 0, CHUNKSIZE);
			cout << "completed overall mergesort for chunk." << endl;
			jobBoard[jobStatusLocation] = SORTED;
			// ASSERT: job i is sorted, and its status reflects this.
		}
		else if (jobStatus == SORTED)
		// ASSERT: Job is to merge two sorted chunks.
		{
			int adjFromChunkLocation = JOBSPACE * jobBoard[toChunkLocation]; //
			// ASSERT:
			int adjToChunkLocation = adjFromChunkLocation + 1; //
			// ASSERT:
			int adjJobStatusLocation = adjFromChunkLocation + 2; //
			// ASSERT:
			if (adjJobStatusLocation < ((JOBSPACE * numChunks) - JOBSTATUS) && jobBoard[adjJobStatusLocation] == SORTED)
			// ASSERT:
			{
				merge(infoSHM, (i * CHUNKSIZE), ((i + 1) * CHUNKSIZE), (((i + 2) * CHUNKSIZE) - 1));
				jobBoard[toChunkLocation] = jobBoard[adjToChunkLocation];
				jobBoard[adjJobStatusLocation] = COMPLETED;
				if (jobBoard[fromChunkLocation] == 0 && jobBoard[toChunkLocation] == (numChunks - 1))
				{
					jobBoard[jobStatusLocation] = COMPLETED;
				}
			}
		}
		else if (jobStatus == COMPLETED)
		// ASSERT:
		{
			numComplete++;
			cout << "numComplete: " << numComplete << endl;
		}
	}
	if (numComplete == numChunks)
	// ASSERT:
	{
		allComplete == true;
		cout << "all jobs completed." << endl;
	}
}

// PRE: childNum is a defined integer that represents which child the current process running this function is. numChunks is a defined integer
//      that represents the number of chunks needing to be sorted. numProcesses is a defined integer that represents the number of processes
//      the overall program is utilizing. jobKey is a defined key that is used in accessing the memory that stores the job information for the
//      processes. infoKey is a defined key that is used in accessing the numbers that need to be handled by the different jobs.
// POST: the information at shared memory accessed with key "infoKey" is in sorted order. all ChunkJobs stored at shared memory accessed with
//       key "jobKey" have a job status that denotes completion.
void beginWork(int numChunks, int numProcesses, key_t jobKey, key_t infoKey, int &jobBoardSpaceRequired, int &infoSpaceRequired)
{
	int jobBoardSHMid; // the identifier for the job board shared memory.
	// ASSERT: jobBoardSHMid is undefined.
	int infoSHMid; // the indentifier for the information shared memory.
	// ASSERT: infoSHMid is undefined.
	int *jobBoard; // the pointer to the job board shared memory.
	// ASSERT: jobSHM points to nothing.
	int *infoSHM; // the pointer to the information shared memory.
	// ASSERT: infoSHM points to nothing.
	bool allComplete = false; //
	// ASSERT: allComplete begins as false, as none of the jobs are assumed to be completed.
	try
	{
		accessSharedMemory(jobKey, jobBoardSpaceRequired, jobBoardSHMid);
		jobBoard = attachToSharedMemory(jobBoardSHMid);
		accessSharedMemory(infoKey, infoSpaceRequired, infoSHMid);
		infoSHM = attachToSharedMemory(infoSHMid);

		while (!allComplete)
		// ASSERT:
		{
			findAndDoJob(numChunks, jobBoard, infoSHM, allComplete);
			allComplete = true;
		}
	}
	catch (Exception error)
	{
		error.handle();
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
		ifstream inputFile(argv[1]); //
		// ASSERT:
		int numChildren = stoi(argv[2]); //
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
		inputData(inputFile, informationKey, informationSpaceRequired, numChunks);
		createJobBoard(jobKey, numChunks, jobBoardSpaceRequired);
		createChildProcesses(numChildren);
		beginWork(numChunks, (numChildren + 1), jobKey, informationKey, jobBoardSpaceRequired, informationSpaceRequired);
	}
	return (0);
}
