#include <iostream>
#include <fstream>

using namespace std;

// PRE: 
// POST: 
void inputData(istream &pInputFile) {
    

}

// PRE: 
// POST: 
void createChildProcesses() {


}

// PRE: 
// POST: 
void handleJobs() {


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
