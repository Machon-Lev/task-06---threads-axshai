#include <iostream>
#include <exception>
#include "Message.h"
#include <mutex>
#include <condition_variable>

using std::cout;
using std::exception;

// Function declarations
void consumer(Message& messageContainer);
void producer(Message& messageContainer);

// Global variables
std::mutex g_messageMtx;                  // Mutex for protecting shared data
std::condition_variable cond;             // Condition variable for synchronization

int main()
{
	Message messageContainer; 
	std::thread tConsumer(consumer, std::ref(messageContainer));
	std::thread tProducer1(producer, std::ref(messageContainer));
	std::thread tProducer2(producer, std::ref(messageContainer));

	tConsumer.join();                     // Wait for the consumer thread to finish
	tProducer1.join();                    // Wait for producer thread 1 to finish
	tProducer2.join();                    // Wait for producer thread 2 to finish
}

// Consumer function
void consumer(Message& messageContainer)
{
	Message recivedMsg;
	int lastCounter = 0; 

	// Keep consuming messages until receiving the last message twice
	while (lastCounter < 2)
	{
		{
			std::unique_lock lck(g_messageMtx);
			// Wait until a new unread message is available in the message container
			cond.wait(lck, [&messageContainer]() {return !messageContainer.isRead(); });
			recivedMsg = messageContainer.read();
		}
		cond.notify_all();                  // Notify all waiting threads that the container is available
		cout << recivedMsg;
		lastCounter += recivedMsg.wasLast();   // Check if the received message was the last one
	}
}

// Producer function
void producer(Message& messageContainer)
{
	auto thisThreadId = std::this_thread::get_id();
	double dataToSend = (double)std::hash<std::thread::id>{}(thisThreadId);   // Generate data to send based on the thread ID

	while (true)
	{
		{
			std::unique_lock lck(g_messageMtx);
			// Wait until the message container is read by the consumer
			cond.wait(lck, [&messageContainer]() {return messageContainer.isRead(); });
			messageContainer.write(thisThreadId, dataToSend, dataToSend == 0);
		}
		cond.notify_all();                  // Notify all waiting threads that the container is available
		if (!dataToSend)
		{
			break;                          // Stop producing messages if the data to send is 0
		}
		dataToSend /= 10;
	}
}