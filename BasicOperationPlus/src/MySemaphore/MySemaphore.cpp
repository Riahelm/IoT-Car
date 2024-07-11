#include "MySemaphore.h"

/* Semaphore initializer, holds a value "count" in order to determine how many permits it has */
/* INPUT: Count, starting amount of permits */
MySemaphore::MySemaphore(int count){
    this->count = count;
};

/* Semaphore destructor */
MySemaphore::~MySemaphore(){};

/* Function to release permits */
void MySemaphore::release(){
    std::unique_lock<std::mutex> lock(mtx);
    count++;
    cv.notify_one();
};

/* Function to acquire a permit */
void MySemaphore::acquire(){
    std::unique_lock<std::mutex> lock(mtx);
    while(count == 0){
        cv.wait(lock);
    }
    count--;
};
