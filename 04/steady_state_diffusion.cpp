#include <iostream>
#include <math.h>
#include <time.h>
#include <random>
#include "../inc/timer.hpp"
#define PI 3.14159265

class RandomWalker{
    public:
        RandomWalker(double x_, double y_, double d_) : x(x_), y(y_), d(d_){}

        double walk(){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0, 1);
            double a;
            while(!boundaryHit(x,y)){
                a = dis(gen);
                x=getX(x, a);
                y=getY(y, a);
            }
            return g(x,y);
        }
        
    private:
        double x, y, d;

        double getX(double oldx, double a){
            double dx = d * cos(2*PI*a);
            return oldx + dx;
        }

        double getY(double oldy, double a){
            double dy = d * sin(2*PI*a);
            return oldy + dy;
        }

        double boundaryHit(double x, double y){
            if(x<=0 || y<=0 || x<=1 || y<=1)
                return true;
            else
                return false;
        }

        // TODO: give this function as a parameter!
        double g(double x, double y){
            return x;
        }


};

int main(int arg_c, char* arg_v[]){
    if(arg_c != 5){
        std::cout << "wrong usage" << std::endl;
        return 1;
    }
    double x_0 = std::stod(arg_v[1]);
    double y_0 = std::stod(arg_v[2]);
    double d = std::stod(arg_v[3]);
    int N = std::stoi(arg_v[4]);
    double sum = 0;

    timer t;
    t.start();
    for(int i=0;i<N;i++){
        RandomWalker rw(x_0, y_0, d);
        sum += rw.walk();
    }
    double res = sum / N;
    t.stop();
    std::cout << "ST and the result is: " << res << " calculated in: " << t.get_timing() <<std::endl;


}
