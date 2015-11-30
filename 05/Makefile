all:	seq.s multi.s

seq.s: seq.cpp
		g++ -Wall -g -O3 --std=c++11 -o seq.s ../inc/timer.hpp seq.cpp

multi.s: multi.cpp
		g++ -Wall -g -O3 -fopenmp --std=c++11 -o multi.s ../inc/timer.hpp multi.cpp
