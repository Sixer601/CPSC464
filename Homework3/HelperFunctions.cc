#include "HelperFunctions.h"
#include "IntArray.h"
#include <algorithm>

using namespace std;

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
	return (result);
}

void printArray(char *message, int *array, int start, int end)
{
	cout << message;
	for(int i = start; i <= end; i++)
	{
		cout << array[i] << " ";
	}
	cout << endl;
}

void merge(int *array, int p, int q, int r)
{
	int leftArraySize = q - p;
	int rightArraySize = r - q + 1;
	int * leftArray = new int[leftArraySize];
	int * rightArray = new int[rightArraySize];
	for(int i = 0; i < leftArraySize; i++)
	{
		leftArray[i] = array[p + i];	
	}
	for(int j = 0; j < rightArraySize; j++)
	{
		rightArray[j] = array[q + j];
	}
	int a = 0;
	int b = 0;
	int c = p;
	while((a < leftArraySize) && (b < rightArraySize))
	{
		if (leftArray[a] <= rightArray[b])
		{
			array[c] = leftArray[a];
			a++;
		}
		else
		{
			array[c] = rightArray[b];
			b++; 
		}
		c++;
	}
	while(a < leftArraySize)
	{
		array[c] = leftArray[a];
		a++;
		c++;
	}
	while(b < rightArraySize)
	{
		array[c] = rightArray[b];
		b++;
		c++;
	}
	delete []leftArray;
	delete []rightArray;
}

// PRE:
// POST:
void mergeSort(int *array, int p, int r)
{
	if (p < r)
	{
		int q = p + (r - p) / 2;
		// Sort first and second arrays
		mergeSort(array, p, q);
		mergeSort(array, q + 1, r);
		merge(array, p, q + 1, r);
	}
}
