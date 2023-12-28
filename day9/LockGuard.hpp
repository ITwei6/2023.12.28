#pragma once

#include <pthread.h>


class LockGuard
{
public:
    LockGuard(pthread_mutex_t *lock):_lock(lock)
    {
pthread_mutex_lock(_lock);//加锁                                                                                                                                                                               NNNNNNNNNNNNNNNNNNNNNNNNN NN N N NN N  NNNNNNNNNNNNNNNNNNNNNNNN                                                                                                                                                                                                                                                                          ngmNMN N N N N MN  
    }
     
    ~LockGuard()
    {
        pthread_mutex_unlock(_lock);//解锁
    }
private:
   pthread_mutex_t *_lock;
};