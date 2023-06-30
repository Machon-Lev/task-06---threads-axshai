#include <iostream>
#include <exception>
#include "Message.h"
#include <mutex>
#include <condition_variable>

using std::cout;
using std::exception;

void consumer(Message& messageContainer);
void producer(Message& messageContainer);


std::mutex g_messageMtx;
std::condition_variable cond;

int main()
{
	
	Message messageContainer;
	std::thread tConsumer(consumer, std::ref(messageContainer));
	std::thread tProducer1(producer, std::ref(messageContainer));
	std::thread tProducer2(producer, std::ref(messageContainer));

	tConsumer.join();
	tProducer1.join();
	tProducer2.join();
}

void consumer(Message &messageContainer)
{
	Message recivedMsg;
	int lastCounter = 0;
	while (lastCounter < 2)
	{
		{
			std::unique_lock lck(g_messageMtx);
			cond.wait(lck, [&messageContainer]() {return !messageContainer.isRead(); });
			recivedMsg = messageContainer.read();
		}
		cond.notify_all();
		cout << recivedMsg;
		lastCounter += recivedMsg.wasLast();
	}

}

void producer(Message& messageContainer)
{
	auto thisThreadId = std::this_thread::get_id();
	double dataToSend = (double)std::hash<std::thread::id>{}(thisThreadId);
	while (true)
	{
		{
			std::unique_lock lck(g_messageMtx);
			cond.wait(lck, [&messageContainer]() {return messageContainer.isRead(); });
			messageContainer.write(thisThreadId, dataToSend, dataToSend == 0);
		}
		cond.notify_all();
		if (!dataToSend)
		{
			break;
		}
		dataToSend /= 10;
	}
	
}
