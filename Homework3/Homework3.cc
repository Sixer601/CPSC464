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
	char *shm; // pointer to shared memory for once it is created.
	// ASSERT: shm is undefined.
	char *jobs; // pointer to reference items in shared memory.
	// ASSERT: jobs is undefined.
	int numJobs = numChunks + ceiling(numChunks, 2); // represents the number
										    // of jobs to be completed.
	// ASSERT: numJobs is equal to the sum of the number of chunks and the 
	//         ceiling of half of the number of chunks.
	jobBoardSpaceRequired = (sizeof(char) * numJobs);
	// ASSERT: jobBoardSpaceRequired is the size of a character for each job.
	jobBoard_mem_id = shmget(jobKey, jobBoardSpaceRequired, IPC_CREAT | 0666);

	if (jobBoard_mem_id < 0)
	// ASSERT: jobBoard_mem_id is less than 0.
	{
		perror("shmget");
	}
	else
	{
		shm = (char *)shmat(jobBoard_mem_id, NULL, 0);
		// ASSERT: shm is a pointer to the memory location at jobBoard_mem_id.
		if (shm == (char *)-1)
		// ASSERT: shm is a NULL pointer.
		{
			perror("shmat");
		}
		else
		{
			jobs = shm;
			// ASSERT: jobs points to the same location in memory that shm does.
			for (int i = 0; i < numChunks; i++)
			// ASSERT: i is less than the number of chunks in the information.
			{
				jobs[i] = 'S';
				// ASSERT: the ith job is a sorting job.
			}

			for (int j = numChunks; j < numJobs; j++)
			// ASSERT: j is in the range (numChunks, numJobs].
			{
				jobs[j] = 'M';
				// ASSERT: the ith job is a merging job.
			}
		}
	}
}

// PRE: pIntArray is a defined IntArray object that contains the stored information. infoKey is a defined
//      key object that represents the identifier to access the shared memory that will contain the
//      information. informationSpaceRequired is a defined integer that represents the number of bytes
//      required to store what pIntArray contains. numChunks is a defined integer that represents the
//      number of chunks the information stored in pIntArray can be broken into.
// POST: There is a location in shared memory that is sized at the number of 
//       bytes equal to informationSpaceRequired. This memory's key is equal to infoKey.
void createSharedMemory(IntArray pIntArray, key_t infoKey, int &informationSpaceRequired, int &numChunks)
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
	shared_mem_id = shmget(infoKey, informationSpaceRequired, IPC_CREAT | 0666);
	// ASSERT: shared_mem_id is the RV of shmget.
	if (shared_mem_id < 0)
	// ASSERT: shared_mem_id is less than 0
	{
		perror("shmget");
	}
	else
	{
		shm = (int *)shmat(shared_mem_id, NULL, 0);
		// ASSERT: shm points to the memory stored at the address that shared_mem_id holds.
		if (shm == (int *)-1)
		// ASSERT: shm is a NULL pointer.
		{
			perror("shmat");
		}
		else
		{
			information = shm;
			// ASSERT: information points to the same location in memory as shm.
			for (int j = 0; j < pIntArray.getContentLength(); j++)
			// ASSERT: j is less than the number of items in pIntArray.
			{
				information[j] = pIntArray.getNthIntInArray(j);
				// ASSERT: the jth index of information is equal to the jth integer in 
				//         pIntArray.
			}
		}
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
	createSharedMemory(data, infoKey, spaceRequired, numChunks);
}

// PRE: neededChildrenNum is a defined integer that represents the number of child processes
//      requested by the user. childNum is a defined integer that represents a unique number
//      each process is given. The parent process contains a childNum of 0.
// POST: There are x+1 processes running the code following this function, where x is equal
//       to neededChildrenNum. Each of these processes contains a unique number, starting at
//       0, to denote which child they are relative to each other.
int createChildProcesses(int neededChildrenNum, int &childNum)
{
	bool child = false; // identifier for if the process is a child process.
	// ASSERT: child begins as false, as the only process running this code is the parent
	//         process.
	while (childNum < neededChildrenNum && !child)
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
		childNum++;
		// ASSERT: childNum is incremented by 1.
	}
	if (!child)
	// ASSERT: the current process is the parent process.
	{
		childNum = 0;
		// ASSERT: childNum is reset to 0.
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
	int jobBoardSHMid; // the identifier for the job board shared memory.
	// ASSERT: jobBoardSHMid is undefined.
	int infoSHMid; // the indentifier for the information shared memory.
	// ASSERT: infoSHMid is undefined.
	char *jobSHM; // the pointer to the job board shared memory.
	// ASSERT: jobSHM points to nothing.
	int *infoSHM; // the pointer to the information shared memory.
	// ASSERT: infoSHM points to nothing.
	if ((jobBoardSHMid = shmget(jobKey, jobBoardSpaceRequired, 0666)) < 0)
	// ASSERT: jobBoardSHMid is less than 0.
	{
		perror("shmget");
	}
	else
	{
		jobSHM = (char *)shmat(jobBoardSHMid, NULL, 0);
		// ASSERT;
		if (jobSHM == (char *)-1)
		// ASSERT: jobSHM is a NULL pointer. 
		{
			perror("shmat");
		}
		else
		{
			if ((infoSHMid = shmget(infoKey, infoSpaceRequired, 0666)) < 0)
			// ASSERT: infoSHMid
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
