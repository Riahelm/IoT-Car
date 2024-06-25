#include "MySemaphore.h"

MySemaphore::MySemaphore(int count){
    this->count = count;
};

MySemaphore::~MySemaphore(){};

void MySemaphore::release(){
    std::unique_lock<std::mutex> lock(mtx);
    count++;
    cv.notify_one();
};

void MySemaphore::acquire(){
    std::unique_lock<std::mutex> lock(mtx);
    while(count == 0){
        cv.wait(lock);
    }
    count--;
};
