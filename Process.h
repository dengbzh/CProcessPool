/*
 * =====================================================================================
 *
 *       Filename:  process.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/29/2015 11:15:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dengbzh (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _PROCESS_H
#define _PROCESS_H
#include"Utility.h"
template<typename D>
class TaskQueue;
template<typename T,typename D>
class Task;
template<typename T,typename D>
class ProcessPool;

template<typename T,typename D>
class Process{
private:    
    void *m_shmptr;
    int  m_shmid;
    Process();
    TaskQueue<D> *m_taskQueue;
    Task<T,D> *m_task;
    T *m_job;
    ProcessPool<T,D> *m_processPool;
public:
    Process(int shmid,sem_t *dataSem,sem_t *memLock,sem_t *dataleftSem);
    virtual ~Process();
    void run();
    void loadData(D data);
    void terminate();
    void freeProcess();
    int getShmid(){return m_shmid;}
    void setProcessPool(ProcessPool<T,D> *processpool){m_processPool = processpool;}
};

#endif
