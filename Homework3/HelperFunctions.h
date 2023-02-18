#ifndef INCLUDED_HelperFunctions
#define INCLUDED_HelperFunctions

#include <iostream>

using namespace std;
 
int ceiling(int a, int b);
 
void printArray(char *message, int *array, int start, int end);

void merge(int *array, int p, int q, int r);

void mergeSort(int *array, int p, int r);

#endif
