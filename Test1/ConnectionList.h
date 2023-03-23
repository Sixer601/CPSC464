#ifndef INCLUDED_ConnectionList
#define INCLUDED_ConnectionList

// ========================================
// Includes
// ========================================

#include "ServerSocket.h"

using namespace std;

// ========================================
// Non-Class Functions
// ========================================

// TODO: Ask Dr. Shende about if implementing a copy constructor makes sense, and if so, how to do = for ServerSocket.
void copy();

// ========================================
// Class Declaration
// ========================================

class ConnectionList
{
	/* Class Invariant (CI): 
	
	*/

	private: 
		ServerSocket * connectionArray;
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
			ServerSocket GetIthSocketInConnectionList(int i) const;

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
			void AddConnection(ServerSocket pSocket, bool pIsActive);

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

};

#endif