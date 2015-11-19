all: openmp.s

openmp.s: openmp.cpp
		g++ -Wall -g -O3 -fopenmp --std=c++11 -o openmp.s ../inc/timer.hpp openmp.cpp

clear: 
	rm -f *.s *.dat
