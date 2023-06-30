#include <iostream>
#include <thread>

/**
 * @brief Represents a message object that can be written by a producer thread and read by a consumer thread.
 */
class Message {
	std::thread::id _threadId;
	double _data;
	bool _isLast;
	bool _isRead = true;

public:
	/**
	 * @brief Writes a message to the message object.
	 * @param threadId The ID of the thread that is writing the message.
	 * @param data The message data to be written.
	 * @param isLast Flag indicating whether the message is the last one.
	 */
	void write(std::thread::id threadId, double data, bool isLast);

	/**
	 * @brief Checks if the message has been read by the consumer.
	 * @return True if the message has been read, false otherwise.
	 */
	bool isRead();


	/**
     * @brief Checks if the message was the last one.
	 * @return True if the message was the last one, false otherwise.
	 */
	bool wasLast();

	/**
	 * @brief Marks the message as read and returns a copy of it.
	 * @return A copy of the message that has been read.
	 */
	Message read();

	/**
	 * @brief Overloads the stream insertion operator to print the message details.
	 * @param os The output stream.
	 * @param msg The message object to be printed.
	 * @return The modified output stream.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Message& msg);
};


