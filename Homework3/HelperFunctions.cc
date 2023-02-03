#include "HelperFunctions.h"
#include <iostream>
#include <fstream>

using namespace std;

// PRE: 
// POST: 
int ceiling(int a, int b)
{
	int result;
	if ((a % b) > 0)
	{
		result = (a + b) / b;
	}
	else
	{
		result = a / b;
	}
	return(result);
}






















