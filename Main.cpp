#include <iostream>
#include <string>
#include <sstream>
#include "Chessboard.h"
#include "Message.h"

int main(int argc, char **argv)
{
	size_t no_sol = 0;	// number of solutions
	size_t n = 8;	        // size of chessboard. init at 8, just in case
	size_t k = 0;		// k parameter
	int err, rank, size;	

	/*  Give MPI it's command arguments  */
	err = MPI_Init( &argc, &argv );
	err = MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	err = MPI_Comm_size( MPI_COMM_WORLD, &size );

	/*  Okay, now we take back our command arguments  */
	if( argc != 3 )
	{
		std::cout << "\tUsage: " << argv[0] << " number_queens k" << std::endl;
		return EXIT_FAILURE;
	}
	n = atoi( argv[1] );
	k = atoi( argv[2] );

	/*  It is an invalid assumption to assume that k can be 1 or n-1  */
	if( 1 == k )
		k++;
	else if( (n-1) == k )
		k--;

	/*  MASTER PROCESSOR  */
	if( 0 == rank )
	{
		/*  The special case of only 1 queen  */
		if( 1 == n )
		{
			std::cout << "1" << std::endl;
			goto end;
		}

		Chessboard board( n );
		while( board.next(0, k) )
		{
			Message work_rqst_msg( n );
			int src_proc;
			Message::recvMessage( &work_rqst_msg, &src_proc );	// get a work request

			Message work_msg( n );
			work_msg.setTag( Message::work_response_e );
			work_msg.board = board.getBoard();
			Message::sendMessage( &work_msg, src_proc );	// give him something to do

		}

		/*  send kill signal to all slaves  */
		for( int i = 1; i < size; i++ )
		{
			Message work_rqst_msg( n );
			int src_proc;
			Message::recvMessage( &work_rqst_msg, &src_proc );	// get a work request

			Message kill_msg( n );
			kill_msg.setTag( Message::done_e );
			Message::sendMessage( &kill_msg, src_proc );	// send a kill command
		}

	}
	/*  EVERY OTHER PROCESSOR :: SLAVE(S)  */
	else
	{
		/*  special case of 1 queen  */
		if( 1 == n )
			goto end;

		for(;;)
		{
			Message work_rqst_msg( n );
			work_rqst_msg.setTag( Message::work_request_e );
			Message::sendMessage( &work_rqst_msg, 0 );	// send a request to master processor for work

			Message work_msg( n );
			Message::recvMessage( &work_msg, 0 );		// receive a work message
		
			if( Message::done_e == work_msg.getTag() )
			{
				break;
			}

			Chessboard partial( n );
			partial.getBoard() = work_msg.board;
	
			while( partial.next(k, n) )
			{
				std::string output = partial.toString();
				output.append( "\n" );
				std::cout << output;
			}
		}
	}
//	std::stringstream output;
//	output << "Proc " << rank << " quitting" << std::endl;
//	std::cout << output.str();
end:
	err = MPI_Finalize();
	return EXIT_SUCCESS;
}
