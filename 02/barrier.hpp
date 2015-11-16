#ifndef HPCSE_BARRIER_HPP
#define HPCSE_BARRIER_HPP

#include <thread>
#include <mutex>
#include <cassert>
#include <limits>
#include <iostream>

class MyBarrier{
private: int n,generation,count;
         std::mutex m;

// constructor
public: MyBarrier(int n_) : n(n_){
            generation = 0;
            count = 0;
        }

// all threads must call wait before we can proceed!
        void wait(){
            std::unique_lock<std::mutex> lock (m);
            int local_gen = generation;

            count++;
            if(count == n){
                generation++;
                count = 0;
                lock.unlock();
            }
            else {
                lock.unlock();
                while(true){
                    lock.lock();
                    if(local_gen != generation)
                        break;
                    lock.unlock();
                }
            } 
        }
};

#endif //HPCSE_BARRIER_HPP
