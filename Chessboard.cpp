/* 	The implementation of the Chessboard class to hold the chessboard information.
 *      CprE 426 - Project 1
 *      Scott Connell && Joel Rausch
 */
#include "Chessboard.h"
#include <algorithm>
#include <cstdlib>
#include <string.h>
#include <sstream>
#include <cassert>
#include <cmath>

Chessboard :: Chessboard(size_t size_in) :
	_queens(size_in)
{
	assert(size_in > 0);

	// Initialize the board with all queens in the first row
	memset(&_queens[0], 0, sizeof(char) * size_in);
}

Chessboard::~Chessboard()
{
}

size_t Chessboard::getSize() const
{
	return _queens.size();
}

std::vector<char> &Chessboard::getBoard()
{
	return _queens;
}

std::string Chessboard::toString() const
{
	std::stringstream ss;
	
	//Release chessboard representation
	for(size_t i = 0; i < getSize(); i++)
	{
		ss << ((int) (_queens[i] + 1));
	}

	//Debug chessboard representation
	//for(size_t y = 0; y < getSize(); y++)
	//{
	//	for(size_t x = 0; x < getSize(); x++)
	//	{
	//		ss << ((_queens[x] == y) ? "X" : ".");
	//		ss << " ";
	//	}

	//	ss << "\n";
	//}

	return ss.str();
}

bool Chessboard::next(size_t startX, size_t endX)
{
	assert(startX < endX);
	assert(startX < getSize());
	assert(endX <= getSize());
	
	_previous = _queens;
	return next(startX, endX, startX);
}

bool Chessboard::next(size_t startX, size_t endX, size_t x)
{
	assert(x >= startX && x < endX);

	// Try to place in each of the rows <= previous position
	for(size_t y = _queens[x]; y < _queens.size(); y++)
	{
		// If we can't place a queen in the row, continue searching
		if(!tryPlace(x, y))
		{
			continue;
		}

		// Try to place a queen in the next column
		if((x + 1) < endX)
		{
			if(next(startX, endX, x + 1))
			{
				return true;
			}
		}

		// In the last row, make sure the solution is unique
		else if(_queens != _previous)
		{
			return true;
		}
	}

	// Backtrack
	_queens[x] = 0;
	return false;
}

bool Chessboard::tryPlace(size_t x, size_t y)
{
	// Check for conflicts in columns [0, x)
	for(size_t xi = 0; xi < x; xi++)
	{
		size_t difference = x - xi;

		// Check in row
		if(_queens[xi] == y)
		{
			return false;
		}

		// Check upper diagonal
		else if(_queens[xi] == (y - difference))
		{
			return false;
		}

		// Check lower diagonal
		else if(_queens[xi] == (y + difference))
		{
			return false;
		}
	}

	// No conflicts; place queen
	_queens[x] = y;
	return true;
}
