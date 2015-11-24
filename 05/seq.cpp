#include <iostream>
#include <random>
#include <cmath>
#include "../inc/timer.hpp"
struct disc_t {
    double x;
    double y;
    double d;
} disc;

class Configuration{
    public:
        Configuration(int Nx_, int Ny_, double d0_, double a_): Nx(Nx_), Ny(Ny_), d0(d0_), a(a_) {
            init();
        }
        void performSweep(){
            for(int n=0; n<Nx*Ny; n++){
                int i = randomIndex(gen);
                disc_t temp = discs[i];
                double dx = randomDisplacement(gen);
                double dy = randomDisplacement(gen);
                temp.x += dx;
                temp.y += dy;
                if(!doesOverlap(temp, i)){
                    discs[i] = temp;
                }
            }
        }

    private:
        int Nx, Ny;
        double d0, a;
        double L = 1;
        std::vector<disc_t> discs;
        std::mt19937 gen;
        std::uniform_real_distribution<> randomDisplacement;
        std::uniform_int_distribution<> randomIndex;
        void init(){
            discs = std::vector<disc_t> (Nx*Ny);
            randomDisplacement = std::uniform_real_distribution<>(-a, a);
            randomIndex = std::uniform_int_distribution<> (0, Nx*Ny);

            for(int i=0; i<Nx; i++){
                for(int j=0; j<Ny; j++){
                    discs[i*Nx + j].x = L/Nx;
                    discs[i*Nx + j].y = std::sqrt(3)/2 * discs[i*Nx + j].x;
                    discs[i*Nx + j].d = d0;
                }
            }
            std::cout << "initialization done!" << std::endl;
        }

        bool doesOverlap(disc_t d, int allExcept){
            for(int i=0; i<Nx*Ny; i++){
                if(i==allExcept)
                    continue;
                if(std::sqrt((d.x-discs[i].x)*(d.x-discs[i].x)+
                            (d.y-discs[i].y)*(d.y-discs[i].y))>d.d)
                    return false;
            }
            return true;
        }
};

int main(int arg_c, char* arg_v[]){
    if(arg_c != 7){
        std::cout << "wrong usage!" << std::endl;
        return 1;
    }
    int Nx = std::stoi(arg_v[1]);
    int Ny = std::stoi(arg_v[2]);
    double d0 = std::stod(arg_v[3]);
    double a = std::stoi(arg_v[4]);
    int Sequi = std::stoi(arg_v[5]);
    int S = std::stoi(arg_v[6]);
    Configuration c (Nx, Ny, d0, a);
    // equilibrate the system
    for(int i=0;i<Sequi;i++){
        c.performSweep();
    }

    for(int i=0;i<S;i++){
        c.performSweep();
        //TODO
        //c.doAnalysis();
    }
    //TODO: get the output we're interested in!
    std::cout << "output:" << std::endl;
    return 0;
}
