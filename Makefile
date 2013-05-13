nqueens:
		mpicxx -o nqueens Chessboard.cpp Main.cpp Message.cpp -O3 -ipo

clean:
		rm -f nqueens
		rm -f *.o
