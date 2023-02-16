#include "HelperFunctions.h"
#include "IntArray.h"

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
	return (result);
}

void merge(int *pIntArray, int p, int q, int r)
{
	/*
	int i = p;
	int j = q;
	int k = 0;
	int * temp = new int [r - p];
	while (i < q && j < r)
	{
		if (pIntArray[i] < pIntArray[j])
		{
			temp[k] = pIntArray[i];
			i++;
		}
		else
		{
			temp[k] = pIntArray[j];
			j++;
		}
		k++;
	}
	if (i == q)
	{
		while(j < r)
		{
			temp[k] = pIntArray[j];
			j++;
			k++;
		}
	}
	else if (j == r)
	{
		while(i < q)
		{
			temp[k] = pIntArray[i];
			i++;
			k++;
		}
	}
	for (int b = p; b < r; b++)
	{
		pIntArray[b] = temp[b];
	}
	*/
}

// PRE:
// POST:
void mergeSort(int *pIntArray, int p, int r)
{
	/*
	if (p < r)
	{
		int q = ((p + r) / 2);
		mergeSort(pIntArray, p, q);
		mergeSort(pIntArray, (q + 1), r);
		merge(pIntArray, p, q, r);
	}
	*/
}
