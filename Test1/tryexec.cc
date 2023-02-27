#include <unistd.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main (int argc, char * argv[]) {
  char * path = NULL;
  path = getcwd (path, 0); // get the current working directory. Since
			   // path is NULL, getcwd will allocate
			   // memory dynamically for the path
			   // string. Needs to be deleted.
                           // The returned path does not end in /
  char progName[] = "hello"; // The name of the program to be executed.
  char program[100];       // allocate space for the absolute path for
			   // the program.
  sprintf (program, "%s/%s", path, progName); // Add the / between the
					      // directory name and
					      // program name to make the
					      // correct absolute path
  free (path);            // delete memory allocated for the path
			  // variable. 
  char arg1[] = "Thursday!";  // Create a string for each command line
			     // argument to the program to be
			     // executed. 
  char * nullP = (char *) NULL;  // The arguments must end in a NULL
				 // pointer cast as a char *
  char * programArgv[2];        // Create an array of pointers to the
			 // argument strings.
  programArgv[0] = arg1;         // Populate the arguments.
  //  programArgv[0] = argv[1];         // Populate the arguments.  
  programArgv[1] = nullP;
    

  cout << "Calling " << program  << " program" << endl;
  
  int err = execv (program, programArgv);  // Execute the program with the
				    // arguments. 
  if (err == -1){      // This will be executed ONLY IF the execv
		       // failed, i.e., returned a -1.
    cout << "exec failed." << endl <<  "Path = *" << path
	 << "* Command = *" << programArgv[0] << "*" << endl; 
  
  }
  return (0);
    
}
