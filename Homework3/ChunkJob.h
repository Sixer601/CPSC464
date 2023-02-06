#ifndef INCLUDED_ChunkJob
#define INCLUDED_ChunkJob

#include <iostream>

using namespace std;

class ChunkJob
{
	/*
		Class Invariant (CI):
		Job Statuses -
		0:no status
		1:busy
		2:unsorted
		3:sorted
		4:completed
	 */

private:
	int chunk1;
	int chunk2;
	int jobStatus;

public:
	// ==============================
	// Constructors
	// ==============================

	// Default Constructor

	// PRE: 
	// POST: 
	ChunkJob();

	// Copy Constructor

	// PRE: 
	// POST:
	ChunkJob(const ChunkJob &pChunkJob);

	// ==============================
	// Accessor Functions
	// ==============================

	// PRE: 
	// POST:
	int getChunk1() const;

	// PRE: 
	// POST:
	int getChunk2() const;

	// PRE: 
	// POST:
	int getJobStatus() const;

	// ==============================
	// Modifier Functions
	// ==============================

	// PRE: 
	// POST:
	void setChunk1(int pValue);

	// PRE: 
	// POST: 
	void setChunk2(int pValue);

	// PRE: 
	// POST: 
	void setJobStatus(int pValue);

};

#endif