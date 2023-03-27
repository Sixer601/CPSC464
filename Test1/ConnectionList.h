#ifndef INCLUDED_ConnectionList
#define INCLUDED_ConnectionList

// ========================================
// Includes
// ========================================

#include "Socket.h"

using namespace std;

// ========================================
// Non-Class Functions
// ========================================



// ========================================
// Class Declaration
// ========================================

class ConnectionList
{
	/* Class Invariant (CI): 
	
	*/

	private: 
		Socket * connectionArray;
		bool * activeStatusArray;
		int numConnections;
		int arraySize;

	public:

		// ========================================
		// Constructors
		// ========================================

			// Default Constructor

				// PRE: 
				// POST: 
				ConnectionList();

			// Copy Constructor

				// PRE: 
				// POST: 
				ConnectionList(const ConnectionList &pConnection);

		// ========================================
		// Destructor
		// ========================================

			// PRE: 
			// POST: 
			~ConnectionList();

		// ========================================
		// Accessor Functions
		// ========================================

			// PRE: 
			// POST: 
			Socket GetIthSocketInConnectionList(int i) const;

			// PRE: 
			// POST: 
			bool GetIthActiveStatusInConnectionList(int i) const;

			// PRE: 
			// POST: 
			int GetNumConnections() const;

			// PRE: 
			// POST: 
			int GetCapacity() const;

		// ========================================
		// Modifier Functions
		// ========================================

			// PRE: 
			// POST: 
			void AddConnection(Socket pSocket, bool pIsActive);

			// PRE: 
			// POST: 
			void SetIthConnectionToStatus(int i, bool pNewStatus);

		// ========================================
		// Predicate Functions
		// ========================================

			// PRE: 
			// POST: 
			bool AllActive();

			// PRE: 
			// POST: 
			bool AllInactive();

		// ========================================
		// Operators
		// ========================================

			// PRE: 
			// POST: 
			Socket operator [](const int & i) const;

			// PRE: 
			// POST: 
			Socket & operator [](const int & i);




};

#endif