#include "HelperFunctions.h"
#include "IntArray.h"
#include <algorithm>

using namespace std;

// PRE: a and b are defined integers that represent the two
//      operands of the ceiling operation.
// POST: RV is the result of ⌈a/b⌉.
int ceiling(int a, int b)
{
	int result; // variable to store the result of the operation.
	// ASSERT: result begins as undefined.
	if ((a % b) > 0)
	// ASSERT: the result of a mod b is greater than 0.
	{
		result = (a + b) / b;
		// ASSERT: result is the rounded up version of a divided
		//         by b.
	}
	else
	{
		result = a / b;
		// ASSERT: result is the result of a divided by b.
	}
	return (result);
}

// PRE: message is a defined array of characters that represents
//      what should be output before the array. array is a defined
//      array of integers that contains the values to be printed.
//      start and end are both defined integers that represent the
//      beginning and ending indices of array to be printed
//      respectively.
// POST: message, followed by the contents of array from start to 
//       end are printed to standard output.
void printArray(char *message, int *array, int start, int end)
{
	cout << message;
	for(int i = start; i <= end; i++)
	// ASSERT: i is less than or equal to end.
	{
		cout << array[i] << " ";
	}
	cout << endl;
}

// PRE: array is a defined array of integers that contains 
//      two sorted lists, one from p to q, and the other from
//      q + 1 to r. p, q, and r are defined integers that represent
//      the beginning, middle, and end indices, respectively.
// POST: array[p, r] is sorted.
void merge(int *array, int p, int q, int r)
{
	int leftArraySize = q - p; // size of temporary array to copy
						  // first sorted list.
	// ASSERT: leftArraySize is equal to the difference of q and p.
	int rightArraySize = r - q + 1; // size of temporary array to copy
							  // second sorted list.
	// ASSERT: rightArraySize is equal to the sum of the difference of 
	//         r and q, and 1.
	int * leftArray = new int[leftArraySize]; // temporary storage for
									  // first sorted list.
	// ASSERT: leftArray is a new list of integers of size leftArraySize.
	int * rightArray = new int[rightArraySize]; // temporary storage for
									    // second sorted list.
	// ASSERT: rightArray is a new list of integers of size rightArraySize.
	for(int i = 0; i < leftArraySize; i++)
	// ASSERT: i is less than leftArraySize.
	{
		leftArray[i] = array[p + i];
		// ASSERT: the value at index i of leftArray is equal to
		//         the value at index p + i of array.
	}
	for(int j = 0; j < rightArraySize; j++)
	// ASSERT: j is less than rightArraySize.
	{
		rightArray[j] = array[q + j];
		// ASSERT: the value at index j of rightArray is equal to
		//         the value at index q + j of array.
	}
	int a = 0; // current index in leftArray.
	// ASSERT: a is equal to 0.
	int b = 0; // current index in rightArray.
	// ASSERT: b is equal to 0.
	int c = p; // current index in array.
	// ASSERT: c is equal to p.
	while((a < leftArraySize) && (b < rightArraySize))
	// ASSERT: both a is less than leftArraySize and 
	//         b is less than rightArraySize.
	{
		if (leftArray[a] <= rightArray[b])
		// ASSERT: the value at index a of leftArray is less than or equal to
		//         the value at index b of rightArray. 
		{
			array[c] = leftArray[a];
			// ASSERT: the value at index c of array is equal to
			//         the value at index a of leftArray.
			a++;
		}
		else
		{
			array[c] = rightArray[b];
			// ASSERT: the value at index c of array is equal to 
			//         the value at index b of rightArray.
			b++; 
		}
		c++;
	}
	while(a < leftArraySize)
	// ASSERT: a is less than leftArraySize.
	{
		array[c] = leftArray[a];
		// ASSERT: the value at index c of array is equal to
		//         the value at index a of leftArray.
		a++;
		c++;
	}
	while(b < rightArraySize)
	// ASSERT: b is less than rightArraySize.
	{
		array[c] = rightArray[b];
		// ASSERT: the value at index c of array is equal to
		//         the value at index b of rightArray.
		b++;
		c++;
	}
	delete []leftArray;
	delete []rightArray;
}

// PRE: array is a defined array of integers that represents data to be sorted.
//      p and r are the beginning and ending indices for the contents in array
//      to be sorted, respectively.
// POST: array[p, r] is sorted.
void mergeSort(int *array, int p, int r)
{
	if (p < r)
	// ASSERT: p is less than r.
	{
		int q = p + (r - p) / 2; // temporary integer that represents middle
		                    	// of p and r.
		// ASSERT: q is equal to the sum of the quotient of the difference of r and p, and 2, and p.
		mergeSort(array, p, q);
		mergeSort(array, q + 1, r);
		merge(array, p, q + 1, r);
	}
}
