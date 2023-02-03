#include <iostream>

using namespace std;

int main(int argc, char **argv) {
	if (argc != 3)
	{
		cout << "Invalid number of arguments." << endl;
	}
	else
	{	
		cout << stoi(argv[1]) % stoi(argv[2]) << endl;
	}
	return(0);
}