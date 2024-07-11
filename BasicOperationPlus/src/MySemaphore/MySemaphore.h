// MySemaphore.h

#ifndef MYSEMAPHORE_H
#define MYSEMAPHORE_H

/* Concurrency libraries */
#include <mutex>                
#include <condition_variable>

/* The following class implements a generic semaphore, usable as both a binary and counting semaphore depending 
   on how the release and acquire functions are used */
class MySemaphore{
    public:
        MySemaphore(int count);     /* Semaphore constructor                                */
        ~MySemaphore();             /* Semaphore destructor                                 */
        void release();             /* Function to release permits                          */
        void acquire();             /* Function to acquire permits, waits if count is == 0  */
    private:
        std::mutex mtx;             /* Sinchronization variable */
        std::condition_variable cv; /* Sinchronization variable */
        int count;                  /* Number of permits        */
};

#endif // MySemaphore.h