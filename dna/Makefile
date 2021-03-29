CXX_FLAG = --std=c++11 -g
CC = g++

dna: dna.o
	g++ dna.o -o dna 

dna.o: dna.cpp
	g++ -c $(CXX_FLAG) dna.cpp
     
clean: 
	rm -f dna *.o *.out *.gch