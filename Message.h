#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <vector>
#include <mpi.h>

class Message
{
public:
	enum MessageTag_e
	{
		work_request_e = 0,
		work_response_e = 1,
		done_e = 2
	};

	Message(size_t boardSize);

	void setTag(MessageTag_e tag);
	MessageTag_e getTag() const;

	static bool sendMessage(const Message* msg, int dst);
	static bool recvMessage(Message* msg, int* src);

	std::vector<char> board;

private:
	int getRank() const;
	int getSize() const;
	MessageTag_e _tag;
};

#endif // MESSAGE_H
