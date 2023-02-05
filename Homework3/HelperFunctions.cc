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
	return(result);
}

void merge(IntArray pIntArray, int p, int q, int r)
{
	int i = p;
	int j = q;
	int k = 0;
	int * temp = new int[r - p];
	for (int a = p; a < r; a++)
	{
		temp[a] = pIntArray.getNthIntInArray(a);
	}
	while (i < q && j < r)
	{
		if (pIntArray.getNthIntInArray(i) < pIntArray.getNthIntInArray(j))
		{
			temp[k] = pIntArray.getNthIntInArray(i);
			i++;
		}
		else
		{
			temp[k] = pIntArray.getNthIntInArray(j);
			j++;
		}
		k++;
	}
	if (i == q)
	{
		while(j < r)
		{
			temp[k] = pIntArray.getNthIntInArray(j);
			j++;
			k++;
		}
	}
	else if (j == r)
	{
		while(i < q)
		{
			temp[k] = pIntArray.getNthIntInArray(i);
			i++;
			k++;
		}
	}
	for (int b = p; b < r; b++)
	{
		pIntArray.replaceInt(b, temp[b]);
	}
}

// PRE: 
// POST: 
void mergeSort(IntArray pIntArray, int p, int r)
{
	if (p != r)
	{
		int q = ((p + r) / 2);
		mergeSort(pIntArray, p, (q - 1));
		mergeSort(pIntArray, q, r);
		merge(pIntArray, p, q, r);
	}
}





















