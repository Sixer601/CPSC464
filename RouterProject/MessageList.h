#ifndef INCLUDED_MessageList
#define INCLUDED_MessageList

#include <iostream>

#include "Message.h"
#include "Constants.h"

using namespace std;

class MessageList
{

private:
	int numMessages;
	Message * messageList;

public:

	// ========================================
	// constructors
	// ========================================

	MessageList();

	// ========================================
	// accessor functions
	// ========================================

	int GetNumMessages() const;

	Message * GetMessageList() const;

	Message GetNthMessage(int n) const;

	// ========================================
	// modifier functions
	// ========================================

	void AddMessage(Message &pMessage);

	// ========================================
	// predicate functions
	// ========================================

	bool isEmpty();

	// ========================================
	// operators
	// ========================================



	// ========================================
	// Input/Output Member Functions
	// ========================================



	
};

#endif
