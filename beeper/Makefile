CXX_FLAG = --std=c++11 -g
CC = g++

beeper: beeper.o
	g++ beeper.o -o beeper 

beeper.o: beeper.cpp
	g++ -c $(CXX_FLAG) beeper.cpp
     
clean: 
	rm -f beeper *.o *.out *.gch