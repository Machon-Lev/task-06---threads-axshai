#include "Message.h"

std::ostream& operator<<(std::ostream& os, const Message& msg)
{
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
	_isRead = false;
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
