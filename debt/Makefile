CXX_FLAG = --std=c++11 -g
CC = g++

debt: debt.o
	g++ debt.o -o debt 

debt.o: debt.cpp
	g++ -c $(CXX_FLAG) debt.cpp
     
clean: 
	rm -f debt *.o *.out *.gch