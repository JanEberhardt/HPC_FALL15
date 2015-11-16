#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <math.h>
#include <mutex>
#include "timer.hpp"

std::mutex iolock;

double f(double a){
    return sqrt(a)*log(a);
}



//simply runs midpoint-riemann sum...
double single_threaded(double a, double b, long num_steps, double (*f)(double)){
    double delta, sum; 
    double x_is, x_i, x_im1;

    sum = 0;
    delta = (b-a)/((double) num_steps);
    x_im1 = a;
    x_i = a+delta;
    for(int i=0; i<num_steps; i++){
        x_is = (x_im1 + x_i)/2; 
        sum += f(x_is)*delta;
        x_im1 = x_i;
        x_i = x_i + delta;
        if(i==num_steps-1)
            x_i = b;
    }
    return sum;
}

//starts given number of threads and uses the single_threaded version to calc a part...
double multi_threaded(double a, double b, long num_steps, int num_threads, double (*f)(double)){
    double x_l, x_r, delta;
    long n;
    n = num_steps / num_threads;
    delta = (b-a)/((double) num_threads); 
    std::vector<std::future<double>> results(num_threads); 
    x_l = a;
    x_r = a+delta;
    for(int i=0;i<num_threads;i++){
        std::packaged_task<double()> pt(std::bind(single_threaded, x_l, x_r, n, f));
        results[i] = pt.get_future();
        std::thread t(std::move(pt));
        t.detach();
        x_l = x_r;
        x_r += delta;
        if(i == num_threads-1)
            x_r = b;
    }

    for(int i=0;i<num_threads;i++){
        results[i].wait();
    }

    double sum = 0;
    for (int i=0;i<num_threads;i++){
        sum += results[i].get();
    }

    return sum;
}


int main(int argc, char *argv[]){
    double a = 1.0;
    double b = 4.0;

    long n = 100;
    int m;
    if(argc == 2){
        n = std::stol(argv[1]);
    }
    if(argc == 3){
        n = std::stol(argv[1]);
        m = std::stoi(argv[2]);
    }
        

    std::cout << "#threads: "<< m << "\t#steps: " << n;

    double res;

    timer t;
    t.start();
    if(m == 0)
        res = single_threaded(a,b,n,*f);
    else
        res = multi_threaded(a,b,n,m,*f);
    t.stop();

    std::cout << "\tresult: " << res << "\ttime [s]: " << t.get_timing() << std::endl;

    return 0;
}
