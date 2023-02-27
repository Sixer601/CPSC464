#ifndef INCLUDED_HelperFunctions
#define INCLUDED_HelperFunctions

#include <iostream>

using namespace std;

// PRE: a and b are defined integers that represent the two
//      operands of the ceiling operation.
// POST: RV is the result of ⌈a/b⌉.
int ceiling(int a, int b);
 
// PRE: message is a defined array of characters that represents
//      what should be output before the array. array is a defined
//      array of integers that contains the values to be printed.
//      start and end are both defined integers that represent the
//      beginning and ending indices of array to be printed
//      respectively.
// POST: message, followed by the contents of array from start to 
//       end are printed to standard output.
void printArray(char *message, int *array, int start, int end);

// PRE: array is a defined array of integers that contains 
//      two sorted lists, one from p to q, and the other from
//      q + 1 to r. p, q, and r are defined integers that represent
//      the beginning, middle, and end indices, respectively.
// POST: array[p, r] is sorted.
void merge(int *array, int p, int q, int r);

// PRE: array is a defined array of integers that represents data to be sorted.
//      p and r are the beginning and ending indices for the contents in array
//      to be sorted, respectively.
// POST: array[p, r] is sorted.
void mergeSort(int *array, int p, int r);

#endif
