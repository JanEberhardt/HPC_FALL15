#include <iostream>
#include <random>
#include <cmath>
#include <fstream>
#include <cassert>
#include "../inc/timer.hpp"
struct disc_t {
    double x;
    double y;
    double d;
} disc;

class Configuration{
    public:
        Configuration(int Nx_, int Ny_, int M_, double d0_, double a_): Nx(Nx_), Ny(Ny_), M(M_), d0(d0_), a(a_) {
            init();
            histogram = std::vector<int> (M, 0);
        }

        /**
         * performs one sweep, which means Nx*Ny random updates
         */
        double performSweep(){
            int overlap = 0;
#pragma omp parallel for
            for(int n=0; n<Nx*Ny; n++){
                int i = randomIndex(gen);
                disc_t temp = discs[i];
                double dx = randomDisplacement(gen);
                double dy = randomDisplacement(gen);
                temp.x += dx;
                temp.y += dy;
                if(doesOverlap(temp, i))
                    overlap++;
                else
                    discs[i] = temp;
            }
            return ((double) overlap)/(Nx*Ny);
        }

        /**
         * updates the histogram
         */
        void updateHistogram(){
            double max_distance2 = 2. * (L/2.) * (L/2.);
            double distance2;
            int historyIndex;
#pragma omp parallel for
            for(int i=0; i<Nx*Ny; i++){
                for(int j=0; j<i; j++){
                    // get the minimal distance
                    distance2 = min_distance2(discs[i], discs[j]); 
                    // increment the histogram value that this corresponds to
                    historyIndex = (distance2/max_distance2) * (M-1);
                    assert(historyIndex < M && historyIndex >= 0);
                    histogram[historyIndex]++;
                }
            }
        }

        /**
         * prints the histogram to a file
         */
        void printHistToFile(std::string filename){
            std::ofstream ofs(filename);
            for(int i=0; i<M; i++){
                ofs << histogram[i] << ";" << std::endl;
            }
        }

        /**
         * prints the set of discs to a file
         */
        void printToFile(std::string filename){
            std::ofstream ofs(filename);
            for(int i=0; i<Nx*Ny; i++){
                ofs << discs[i].x << ", " << discs[i].y << ", " << discs[i].d << ";" << std::endl;
            }
        }

    private:
        int Nx, Ny, M;
        double d0, a;
        double L = 1.;
        std::vector<disc_t> discs;
        std::vector<int> histogram;
        std::mt19937 gen;
        std::uniform_real_distribution<> randomDisplacement;
        std::uniform_int_distribution<> randomIndex;

        /**
         * does initialization
         */
        void init(){
            discs = std::vector<disc_t> (Nx*Ny);
            randomDisplacement = std::uniform_real_distribution<>(-a, a);
            randomIndex = std::uniform_int_distribution<> (0, Nx*Ny);
            for(int i=0; i<Nx; i++){
                for(int j=0; j<Ny; j++){
                    discs[i*Nx + j].x = (L/Nx)*(i+1);
                    discs[i*Nx + j].y = (std::sqrt(3)/2 * (L/Nx)) * (j+1);
                    discs[i*Nx + j].d = d0;
                }
            }
            std::cout << "initialization done!" << std::endl;
        }

        /**
         * returns whether disc d overlaps with any of the other discs,
         * allExcept is just the index of disc d in the discs vector
         */
        bool doesOverlap(disc_t d, int allExcept){
            for(int i=0; i<Nx*Ny; i++){
                if(i==allExcept)
                    continue;
                //todo: this assumes all discs are the syme size
                if((d.x-discs[i].x)*(d.x-discs[i].x)+
                            (d.y-discs[i].y)*(d.y-discs[i].y) < (d.d*d.d+discs[i].d*discs[i].d))
                    return true;
            }
            return false;
        }

        /**
         * returns the minimal discance square of two points
         */
        double min_distance2 (disc_t a, disc_t b){
            double xdiff = a.x - b.x;
            double ydiff = a.y - b.y;
            if(xdiff > 0.5 * L)
                xdiff -= L;
            else if (xdiff < -0.5 * L)
                xdiff += L;
            if(ydiff > 0.5 * L)
                ydiff -= L;
            else if (ydiff < -0.5 * L)
                ydiff += L;
            xdiff = xdiff * xdiff;
            ydiff = ydiff * ydiff;
            return xdiff + ydiff;
        }
};

int main(int arg_c, char* arg_v[]){
    if(arg_c != 8){
        std::cout << "wrong usage!" << std::endl;
        return 1;
    }
    int Nx = std::stoi(arg_v[1]);
    int Ny = std::stoi(arg_v[2]);
    int M = std::stoi(arg_v[3]);
    double d0 = std::stod(arg_v[4]);
    double a = std::stoi(arg_v[5]);
    double overlapRatio;
    int Sequi = std::stoi(arg_v[6]);
    int S = std::stoi(arg_v[7]);
    timer t;
    t.start();
    Configuration c (Nx, Ny, M, d0, a);
    // equilibrate the system
    c.printToFile("config_initial.txt");
    for(int i=0;i<Sequi;i++){
        overlapRatio = c.performSweep();
        //std::cout << "sweep, overlap ratio: " << overlapRatio << std::endl;
    }

    for(int i=0;i<S;i++){
        overlapRatio = c.performSweep();
        c.updateHistogram();
        //std::cout << "sweep and update performed, overlap ratio: " << overlapRatio << std::endl;
    }
    c.printToFile("config_final.txt");
    //TODO: get the output we're interested in!
    c.printHistToFile("histogram.txt");
    t.stop();
    std::cout << "time:" << t.get_timing() << std::endl;
    return 0;
}
