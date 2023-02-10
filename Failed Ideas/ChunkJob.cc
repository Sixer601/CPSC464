#include "ChunkJob.h"
#include <iostream>

using namespace std;

// ==============================
// Constructors
// ==============================

	// Default Constructor

	// PRE: 
	// POST: 
	ChunkJob::ChunkJob()
	{
		chunk1 = -1;
		chunk2 = -1;
		jobStatus = 0;
	}

	// Informed Constructor

	// PRE: 
	// POST: 
	ChunkJob::ChunkJob(int pChunk1, int pChunk2, int pJobStatus)
	{
		chunk1 = pChunk1;
		chunk2 = pChunk2;
		jobStatus = pJobStatus;
	}

	// Copy Constructor

	// PRE: 
	// POST: 
	ChunkJob::ChunkJob(const ChunkJob &pChunkJob)
	{
		chunk1 = pChunkJob.chunk1;
		chunk2 = pChunkJob.chunk2;
		jobStatus = pChunkJob.jobStatus;
	}

// ==============================
// Accessor Functions
// ==============================

	// PRE: 
	// POST:
	int ChunkJob::getChunk1() const
	{
		return(chunk1);
	}

	// PRE: 
	// POST:
	int ChunkJob::getChunk2() const
	{
		return(chunk2);
	}

	// PRE: 
	// POST:
	int ChunkJob::getJobStatus() const
	{
		return(jobStatus);
	}

// ==============================
// Modifier Functions
// ==============================

	// PRE: 
	// POST: 
	void ChunkJob::setChunk1(int pValue)
	{
		chunk1 = pValue;
	}

	// PRE: 
	// POST: 
	void ChunkJob::setChunk2(int pValue)
	{
		chunk2 = pValue;
	}

	// PRE: 
	// POST: 
	void ChunkJob::setJobStatus(int pValue)
	{
		jobStatus = pValue;
	}



















