#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include "IntArray.h"

using namespace std;

// PRE: 
// POST: 
void inputData(istream &pInputFile) {
    cout << "Entered inputData." << endl;

    IntArray data;
    while(pInputFile.peek() != EOF) {
        int datum;
        pInputFile >> datum;
        data.addInt(datum);
    }

    for(int i = 0; i < data.getContentLength(); i++) {
        cout << data.getNthIntInArray(i) << " ";
    }
    cout << endl;

}

// PRE: 
// POST: 
void createChildProcesses() {
    cout << "Entered createChildProcesses." << endl;

}

// PRE: 
// POST: 
void handleJobs() {
    cout << "Entered handleJobs." << endl;

}

// PRE: 
// POST: 
int main(int argc, char **argv) {
    
    if (argc != 2) {
        cout << "Invalid number of arguments." << endl;
    }
    else {

        ifstream inputFile(argv[1]);

        // Input file data into shared memory
        inputData(inputFile);

        // Create child processes
        createChildProcesses();

        // Begin handling chunks/jobs
        handleJobs();

    }
    

    return(0);

}
