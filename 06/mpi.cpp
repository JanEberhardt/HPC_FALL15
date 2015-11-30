#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mpi.h>
#include "../inc/timer.hpp"

int main(int arg_c, char* arg_v[]){
    //initialize mpi
    MPI_Init(&arg_c, &arg_v);

    if(arg_c != 5){
        std::cout << "usage: D (diffusion parameter) L (size of quadrant) N (#steps in 1 dimension) dt (timestep)" << std::endl; 
        return 1;
        }
    double D = std::stod(arg_v[1]);
    double L = std::stod(arg_v[2]);
    int N = std::stoi(arg_v[3]);
    double dt = std::stod(arg_v[4]);
   

    int n, rank;

    //get the mpi parameters n and rank
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0)
        std::cout << "mpi version with " << n << " 'threads'." << std::endl;

    
    // slice the grid into parts and give the parts to the solvers
    // create the grid-part!
    int sliceHeight = 2*N / n;
    double x,y;
    double dxy = L/N;
    std::vector<std::vector<double>> grid(sliceHeight);
    std::vector<std::vector<double>> grid_next(sliceHeight);
    for(int i = 0; i<sliceHeight; i++){
        x = dxy * (i+rank*sliceHeight-N);
        grid[i] = std::vector<double> (2*N+1);
        grid_next[i] = std::vector<double> (2*N+1);
        for(int j = 0; j<=N*2; j++){
            y = dxy * (j-N);
            if(std::sqrt(x*x + y*y) < 0.5)
                grid[i][j] = 1;
        }
    }

    double tmax = 10000 * dt;
    double tcurrent = 0;
    double y_part, x_part;

    timer t;
    t.start();
    while(tcurrent < tmax){
        tcurrent += dt;
        
        // do the communication



        // solve that part
        for(int i=1; i<sliceHeight-1; i++){
            for(int j=0; j<=2*N; j++){
                x_part = grid[i+1][j] - 2*grid[i][j] + grid[i-1][j];
                x_part = x_part / dxy;
                y_part = ((j==2*N)?0.0:grid[i][j+1]) - 2*grid[i][j] + 
                        ((j==0)?0.0:grid[i][j-1]);
                y_part = y_part / dxy;
                grid_next[i][j] = dt * D * (x_part + y_part) + grid[i][j];
            }
        }
        //do the corner-cases
        
        //swap
        std::swap(grid, grid_next);
    }
    t.stop();
    std::cout << "in rank " << rank << " time passed: " << t.get_timing();
    MPI_Finalize();
    return 0;
}
