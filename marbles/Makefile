CXX_FLAG = -g -O2 -std=gnu++17 -static
CC = g++

marbles: marbles.o
	g++ marbles.o -o marbles 

marbles.o: marbles.cpp
	g++ -c $(CXX_FLAG) marbles.cpp
     
clean: 
	rm -f marbles *.o *.out *.gch