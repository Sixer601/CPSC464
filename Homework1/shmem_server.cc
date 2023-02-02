#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define SHMSZ     27    // Size of shared memory in bytes.

int main()
{
    int shared_mem_ID;  // identifier for the shared memory that will
			// be allocated.  
    key_t key;          // We refer to the shared memory segment by
			// this key.
    char *shm, *message;      // pointers to hold addresses in shared memory.

    bool error = false;
    
    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 5678;

    /*
     * Create the shared memory segment.
     */
    shared_mem_ID = shmget(key, SHMSZ, IPC_CREAT | 0666);
    if (shared_mem_ID < 0) {
        perror("shmget");
    }
    else {
      /*
       * Now we attach the segment to our data space.
       */
      shm = (char *) shmat(shared_mem_ID, NULL, 0);
      if (shm == (char *) -1) { 
        perror("shmat");
      }
      else {

	cout << hex << (void *)shm << dec<< endl;
	/*
	 * Now put some things into the memory for the
	 * other process to read.
	 */
	message = shm;

	char nextChar = 'a';
	for (int charNo = 0; (charNo < 26); charNo++) {
	  message[charNo] = nextChar;
	  nextChar++;
	}
	message[26] = '\0';

	/*
	 * Finally, we wait until the other process 
	 * changes the first character of our memory
	 * to '*', indicating that it has read what 
	 * we put there.
	 */
	while (shm[0] != '*')
	  sleep(1);
	cout << "Client done." << endl;
      }
    }

    return (0);
}
