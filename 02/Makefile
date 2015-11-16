all: single_threaded multi_threaded barrier_test barriered

single_threaded: single_threaded.cpp
		g++ -Wall -g -O3 --std=c++11 -o single_threaded single_threaded.cpp  


multi_threaded: multi_threaded.cpp
		g++ -Wall -g -O3 --std=c++11 -pthread -o multi_threaded multi_threaded.cpp  

barriered: barriered.cpp barrier.hpp
		g++ -Wall -g -O3 --std=c++11 -pthread -o barriered barrier.hpp barriered.cpp  

barrier_test: barriertest.cpp barrier.hpp
		g++ -Wall -g -O3 --std=c++11 -pthread -o barriertest barrier.hpp barriertest.cpp  
		
