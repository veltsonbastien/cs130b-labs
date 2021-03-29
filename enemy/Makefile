CXX_FLAG = -g -O2 -std=gnu++17 -static
CC = g++

enemy: enemy.o
	g++ enemy.o -o enemy 

enemy.o: enemy.cpp
	g++ -c $(CXX_FLAG) enemy.cpp
     
clean: 
	rm -f enemy *.o *.out *.gch