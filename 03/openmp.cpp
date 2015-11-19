#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <omp.h>
#include "../inc/timer.hpp"

class Diffusion2D {
private: 
    double D, L, dt, dxy;
    int N;
    std::vector<std::vector<double>> grid;
    std::vector<std::vector<double>> grid_next;

public: 
    Diffusion2D(double D_, double L_, int N_, double dt_) : D(D_), L(L_), dt(dt_), N(N_){
        initialize_grid();
    }

    void foreward_euler(){
        double x_part, y_part;

        #pragma omp parallel for
        for(int i=0;i<=2*N;i++){
            for(int j=0;j<=2*N;j++){
                x_part = ((i==2*N)?0.0:grid[i+1][j]) - 2*grid[i][j] 
                    + ((i==0)?0.0:grid[i-1][j]);
                x_part = x_part / dxy;
                y_part = ((j==2*N)?0.0:grid[i][j+1]) - 2*grid[i][j] 
                    + ((j==0)?0.0:grid[i][j-1]);
                y_part = y_part / dxy;
                grid_next[i][j] = dt * D * (x_part + y_part) + grid[i][j];
            }
        }

        std::swap(grid, grid_next); 
    }

    void print_grid(std::string const& filename){
        std::ofstream out_file(filename, std::ios::out);
        for(int i = 0; i <= 2*N; i++) {
            for(int j = 0; j <= 2*N; j++)
                out_file << (i*dxy-L) << '\t' << (j*dxy-L) << '\t' << grid[i][j] << "\n";
            out_file << "\n";
        }
        out_file.close();
    }
private: 
    void initialize_grid(){
        double x, y;
        dxy = L/N;
        grid = std::vector<std::vector<double>> (N*2 + 1);
        grid_next = std::vector<std::vector<double>> (N*2 + 1);
        for(int i=0;i<=2*N;i++){
            x = dxy * (i-N);
            grid[i] = std::vector<double> (N*2 + 1);
            grid_next[i] = std::vector<double> (N*2 + 1);
            for(int j=0;j<=2*N;j++){
                y = dxy * (j-N);
                if(std::sqrt(x*x + y*y) < 0.5)
                    grid[i][j] = 1;
            }
        }
    }
};

int main(int arg_c, char* arg_v[]){
    if(arg_c != 6){
        std::cout << "usage: D (diffusion parameter) L (size of quadrant) N (#steps in 1 dimension) dt (timestep) n (# threads)" << std::endl; 
        return 1;
        }
    double D = std::stod(arg_v[1]);
    double L = std::stod(arg_v[2]);
    int N = std::stoi(arg_v[3]);
    double dt = std::stod(arg_v[4]);
    int n = std::stoi(arg_v[5]);

    omp_set_num_threads(n);

    std::cout << "openmp version with " << n << " threads" << std::endl;

    Diffusion2D system(D, L, N, dt); 

    double tmax = 10000 * dt;
    double tcurrent = 0;

    timer t;
    t.start();
    while(tcurrent < tmax){
        tcurrent += dt;
        system.foreward_euler();
    }
    t.stop();
    std::cout << "    time passed: " << t.get_timing();

    std::string filename_str = "s-";
    filename_str += std::to_string(N);
    filename_str += ".dat";
    const char* filename = filename_str.c_str();
    std::cout << " results written in: " << filename << std::endl;
    system.print_grid(filename);
    return 0;
}
