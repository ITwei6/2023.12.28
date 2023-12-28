#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include "LockGuard.hpp"
using namespace std;

#define NUM 4 // 创建多线程

//定义全局锁
//方式定义并且初始化
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
class threadData
{
public:
    threadData(int number)
    {
        _threadname = "thread-" + to_string(number);
    }

public:
    string _threadname;
};
int ticket=1000;//全局变量，共享资源
void * Getticket(void* args)
{

    threadData *td=static_cast<threadData*>(args);//可以知道是哪一个线程执行的
    const char* name=td->_threadname.c_str();
    //线程持续抢票
    while(true)
    {
        //加锁，锁共享资源，即临界区
        //LockGuard lg(&lock);//循环出来就释放
        pthread_mutex_lock(&lock);//线程申请成功锁后，才能往后执行，其他没有锁的线程就会在阻塞挂起
        if(ticket>0)
        {
            usleep(1000);//增加其他进程调度的机会
            printf("%s, get a ticket: %d\n",name,ticket);
            ticket--;
            pthread_mutex_unlock(&lock);//解锁
        }
        else 
        {
            pthread_mutex_unlock(&lock);
            break;
        }
        
        usleep(15);//强完票，我们还需要做一些事情，不是抢完立即再去强实多线程还要执行得到票之后的后续动作
    }
    printf("%s ...quit\n",name);
    return nullptr;
}
//多线程并发执行会存在问题----数据不一致问题
//如何解决呢？-->互斥锁,将共享资源加锁起来，不许多执行流一起访问

int main()
{
    // 1.如何创建多线程呢？--创建多线程，主线程要想找到每个线程，就需要保存每个线程的tid,用vector保存
    // 主线程在创建多线程之前，给每个线程都初始些属性，比如名字等
    vector<pthread_t> vp;//存储每个线程的tid
    vector<threadData*> vtd;//存储每个线程的基本属性--名字
    
    for (int i = 0; i < NUM; i++)//同时创建了四个线程,这四个线程都会执行GEtticket
    {
       
        pthread_t tid;
        threadData *td = new threadData(i);
        vtd.push_back(td);
        pthread_create(&tid, nullptr, Getticket,vtd[i]);
        vp.push_back(tid);
    }
    
     //多线程创建完，主线程还需要等待这些多线程,根据线程的tid等待
     for(int i=0;i<vp.size();i++)
     {
         pthread_join(vp[i],nullptr);
     }

     //还需要释放申请的资源
     for(int i=0;i<vtd.size();i++)
     {
        delete vtd[i];
     }

     return 0;
}