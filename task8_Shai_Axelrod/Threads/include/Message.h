#include <iostream>
#include <thread>
class Message {
	std::thread::id _threadId;
	double _data;
	bool _isLast;
	bool _isRead = true;
public:
	void write(std::thread::id threadId, double data, bool isLast);
	bool isRead();
	bool wasLast() {return _isLast;}
	Message read();
	friend std::ostream& operator<<(std::ostream& os, const Message& msg);
};


