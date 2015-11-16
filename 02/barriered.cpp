#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <thread>
#include "timer.hpp"
#include "barrier.hpp"

class Diffusion2D {
private:
    std::vector<std::thread> threads;
    int num_threads;

    // initialized the grid
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


public: double D, L, dt, dxy;
    int N;
    std::vector<std::vector<double>> grid;
    std::vector<std::vector<double>> grid_next;
    Diffusion2D(double D_, double L_, int N_, double dt_) : D(D_), L(L_), dt(dt_), N(N_){
        initialize_grid();
    }


    // runs euler until t_current = t_max
    void run_foreward_euler(int n, int t_max){
        num_threads = n;
        // start the threads and do this in a function
        threads = std::vector<std::thread> (n);
        MyBarrier b (n);
        int size = 2*N / n;
        int from_line, to_line;
        for (int i=0; i<n; i++){
            from_line = i*size;
            to_line = (i+1)*size;
            threads[i] = std::thread([&, this, t_max](){
                    //this is the stuff we do inside the thread
                    int t_current = 0;
                    while(t_current < t_max){
                        double x_part, y_part;
                        for(int i=from_line; i<=to_line; i++){
                            for(int j=0; j<=2*N; j++){
                                x_part = ((i==2*N)?0.0:grid[i+1][j]) - 
                                    2*grid[i][j] + ((i==0)?0.0:grid[i-1][j]);
                                x_part = x_part / dxy;
                                y_part = ((j==2*N)?0.0:grid[i][j+1]) - 
                                    2*grid[i][j] + ((j==0)?0.0:grid[i][j-1]);
                                y_part = y_part / dxy;
                                grid_next[i][j] = dt * D * (x_part + y_part) + grid[i][j];
                            }
                        }
                        b.wait();
                        //the first thread is responsible for swapping out the vectors...
                        if(i == 0)
                            std::swap(grid, grid_next); 
                        b.wait();
                        t_current += dt;
                    }
            });
        }
    }

    // joins the threads
    void join_threads(){
        for(int i=0; i<num_threads; i++)
            threads[i].join();
    }

    // prints the grid
    void print_grid(std::string const& filename){
        std::ofstream out_file(filename, std::ios::out);
        for(int i = 0; i <= 2*N; i++) {
            for(int j = 0; j <= 2*N; j++)
                out_file << (i*dxy-L) << '\t' << (j*dxy-L) << '\t' << grid[i][j] << "\n";
            out_file << "\n";
        }
        out_file.close();
    }
};

int main(int arg_c, char* arg_v[]){
    // command line input arguments stuff
    if(arg_c != 6){
        std::cout << "usage: D (diffusion parameter) L (size of quadrant) N (#steps in 1 dimension) dt (timestep) n #threads" << std::endl; 
        return 1;
        }
    double D = std::stod(arg_v[1]);
    double L = std::stod(arg_v[2]);
    int N = std::stoi(arg_v[3]);
    double dt = std::stod(arg_v[4]);
    int n = std::stoi(arg_v[5]);

    std::cout << "barriered multi-threaded version with " << n << " threads." << std::endl;

    Diffusion2D system(D, L, N, dt); 

    double tmax = 10000 * dt;

    timer t;

    t.start();
    system.run_foreward_euler(n, tmax);
    t.stop();

    std::cout << "    time passed: " << t.get_timing();

    std::string filename_str = "b";
    filename_str += std::to_string(n);
    filename_str += "-";
    filename_str += std::to_string(N);
    filename_str += ".dat";
    const char* filename = filename_str.c_str();
    std::cout << " results written in: " << filename << std::endl;
    system.print_grid(filename);
    return 0;
}
