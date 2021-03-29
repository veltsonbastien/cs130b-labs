CXX_FLAG = --std=c++11 -g
CC = g++

compromise: compromise.o
	g++ compromise.o -o compromise 

compromise.o: compromise.cpp
	g++ -c $(CXX_FLAG) compromise.cpp
     
clean: 
	rm -f compromise *.o *.out *.gch