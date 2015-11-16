#include "barrier.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>

int main(int arg_c, char* arg_v[]){
    if(arg_c != 2){
        std::cout << "usage: barriertest n (number of threads)" << std::endl;
        return 1;
    }

    int n = std::atoi(arg_v[1]);

    std::cout << "testing with n=" << n << " threads!";
    std::vector<std::thread> threads(n);
    std::vector<int> ints (n);
    MyBarrier b (n);
    for(int i=0;i<n;i++){
        //TODO: what is this & for???
        threads[i] = std::thread([&, i, n](){
                //inside the thread:
                for(int x=0; x<200; x++)
                    ints[i]++;
                b.wait();
                if(i==0){
                    for(int x=0;x<n;x++){
                        assert(ints[x]==200);
                        ints[x]=0;
                    }
                }
                b.wait();

                //and let's do that again!
                for(int x=0; x<200; x++)
                    ints[i]++;
                b.wait();
                if(i==0){
                    for(int x=0;x<n;x++){
                        assert(ints[x]==200);
                        ints[x]=0;
                    }
                }
            });
    }
    for(int i=0;i<n;i++)
        threads[i].join();

    std::cout << "successfully!" << std::endl;
    return 0;
}
