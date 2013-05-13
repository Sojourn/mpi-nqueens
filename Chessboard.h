/*
 *	A class to hold the chessboard information.
 *	CprE 426 - Project 1
 *	Scott Connell && Joel Rausch
 */

#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <stdint.h>

//#define NO_MPI
#ifdef NO_MPI
#define MPI_CHAR char
#else
#include <mpi.h>
#endif

// A chessboard which can iterate over N-Queen solutions.
class Chessboard
{
public:
	// Constructor for a chessboard of with (non-zero) sides of length size.
	// Size represents the 'N' parameter in the N-Queens problem.
	Chessboard(size_t size);

	// Destructor.
	~Chessboard();

	// Returns the size of the chessboard.
	size_t getSize() const;

	// Return the raw board representation. This is used to set the board.
	std::vector<char> &getBoard();

	// Return a string representation of the board. Uses '1' based rows/cols.
	std::string toString() const;
	
	// Find the next partial solution (placing queens in columns from startX to endX).
	// startX is inclusive, endX is exclusive.
	//
	// Single threaded:
	//		Call with [0, n).
	// 
	// Parallel using Master/Slave(s):
	//		Master calls this with the first k columns [0, k).
	//		Slave(s) calls this takes [k, n).
	bool next(size_t startX, size_t endX);

private:
	// Try to place a queen at a position, and fail if a queen can attack it.
	bool tryPlace(size_t x, size_t y);
		
	// Recursive backtracking function for finding next partial solution.
	bool next(size_t startX, size_t endX, size_t x);

	// vector representing the board.
	std::vector<char> _queens;

	// vector representing the previous solution.
	std::vector<char> _previous;
};

#endif	//CHESSBOARD_H
