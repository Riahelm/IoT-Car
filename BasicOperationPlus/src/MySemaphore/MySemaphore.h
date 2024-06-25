// MySemaphore.h

#ifndef MY_SEMAPHORE_H
#define MY_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class MySemaphore{
    public:
        MySemaphore(unsigned long count);
        inline void release();
        inline void acquire();
    private:
        std::mutex mtx;
        std::condition_variable cv;
        unsigned long count;
};

#endif // MySemaphore.h