all: single_threaded.s multi_threaded.s barrier_test.s barriered.s

single_threaded.s: single_threaded.cpp
		g++ -Wall -g -O3 --std=c++11 -o single_threaded.s single_threaded.cpp  


multi_threaded.s: multi_threaded.cpp
		g++ -Wall -g -O3 --std=c++11 -pthread -o multi_threaded.s multi_threaded.cpp  

barriered.s: barriered.cpp barrier.hpp
		g++ -Wall -g -O3 --std=c++11 -pthread -o barriered.s barrier.hpp barriered.cpp  

barrier_test.s: barrier_test.cpp barrier.hpp
		g++ -Wall -g -O3 --std=c++11 -pthread -o barrier_test.s barrier.hpp barrier_test.cpp  

clear: 
	rm -f *.s *.dat
