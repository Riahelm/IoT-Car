// MySemaphore.h

#ifndef MYSEMAPHORE_H
#define MYSEMAPHORE_H

#include <mutex>
#include <condition_variable>

class MySemaphore{
    public:
        MySemaphore(int count);
        ~MySemaphore();
        void release();
        void acquire();
    private:
        std::mutex mtx;
        std::condition_variable cv;
        int count;
};

#endif // MySemaphore.h