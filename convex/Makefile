CXX_FLAG = --std=c++11 -g
CC = g++

convex: convex.o
	g++ convex.o -o convex 

convex.o: convex.cpp
	g++ -c $(CXX_FLAG) convex.cpp
     
clean: 
	rm -f convex *.o *.out *.gch