#include "MySemaphore.h"

MySemaphore::MySemaphore(unsigned long count = 0) : count(count){};

inline void MySemaphore::release(){
    std::unique_lock<std::mutex> lock(mtx);
    count++;
    cv.notify_one();
}

inline void MySemaphore::acquire(){
    std::unique_lock<std::mutex> lock(mtx);
    while(count == 0){
        cv.wait(lock);
    }
    count--;
}
