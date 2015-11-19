all: single_threaded.s multi_threaded.s

single_threaded.s: steady_state_diffusion.cpp
	g++ -Wall -g -O3 --std=c++11 -o single_threaded.s steady_state_diffusion.cpp  

multi_threaded.s: steady_state_diffusion_openmp.cpp
	g++ -Wall -g -O3 --std=c++11 -fopenmp -o multi_threaded.s steady_state_diffusion_openmp.cpp  
