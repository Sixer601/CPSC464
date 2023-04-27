#include <iostream>

#include "MessageList.h"
#include "Message.h"
#include "Constants.h"

using namespace std;

	// ========================================
	// constructors
	// ========================================

	MessageList::MessageList()
	{
		messageList = new Message[MYMAXCONNECTIONS];
		numMessages = 0;
	}

	// ========================================
	// accessor functions
	// ========================================

	int MessageList::GetNumMessages() const
	{
		return(numMessages);
	}

	Message * MessageList::GetMessageList() const
	{
		return(messageList);
	}

	Message MessageList::GetNthMessage(int n) const
	{
		return(messageList[n]);
	}

	// ========================================
	// modifier functions
	// ========================================

	void MessageList::AddMessage(Message &pMessage)
	{
		messageList[numMessages] = pMessage;
		numMessages++;
	}

	// ========================================
	// predicate functions
	// ========================================

	bool MessageList::isEmpty()
	{
		return(numMessages == 0);
	}

	// ========================================
	// operators
	// ========================================



	// ========================================
	// Input/Output Member Functions
	// ========================================
