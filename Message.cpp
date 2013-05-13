#include "Message.h"

Message::Message(size_t boardSize)
{
	board.resize(boardSize);
}

void Message::setTag(MessageTag_e tag)
{
	_tag = tag;
}

Message::MessageTag_e Message::getTag() const
{
	return _tag;
}

bool Message::sendMessage(const Message *msg, int dst)
{
	int result = MPI_Send(
			(void*) (&msg->board[0]),
			msg->board.size(),
			MPI_CHAR,
			dst,
			static_cast<int>(msg->getTag()),
			MPI_COMM_WORLD);
		
	return true;	
}

bool Message::recvMessage(Message* msg, int *src)
{
	MPI_Status status;

	int result = MPI_Recv(
			(void*) (&msg->board[0]),
			msg->board.size(),
			MPI_CHAR,
			MPI_ANY_SOURCE,
			MPI_ANY_TAG,
			MPI_COMM_WORLD,
			&status);

	if( 0 != src )
		*src = status.MPI_SOURCE;

	msg->setTag(static_cast<MessageTag_e>(status.MPI_TAG));

	return true;
}

int Message::getRank() const
{
	static int rank = -1;
	if(rank == -1)
	{
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	}

	return rank;
}

int Message::getSize() const
{
	static int size = -1;
	if(size == -1)
	{
		MPI_Comm_size(MPI_COMM_WORLD, &size);
	}

	return size;
}
