#include "Message.h"

std::ostream& operator<<(std::ostream& os, const Message& msg)
{
	// Print in the appropriate format
	if (msg._isLast) {
		os << msg._threadId << " finished" << std::endl;
	}
	else {
		os << msg._threadId << " sent: " << msg._data << std::endl;
	}
	return os;
}

void Message::write(std::thread::id threadId, double data, bool isLast)
{
	_threadId = threadId;
	_data = data;
	_isLast = isLast;
	// this is a new message - so set _isRead to false
	_isRead = false;
}

bool Message::wasLast()
{ 
	return _isLast; 
}

bool Message::isRead()
{
	return _isRead;
}

Message Message::read()
{
	_isRead = true;
	return Message(*this);
}
