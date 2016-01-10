/*
 * =====================================================================================
 *
 *       Filename:  ProcessPool.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/02/2016 11:02:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dengbzh (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _PROCESSPOOL_H
#define _PROCESSPOOL_H
#include"Utility.h"
#include"TaskQueue.h"
#include"Task.h"
#include"Process.h"

#define _PROCESSPOOL_SEM_LEN 80
template<typename D>
class TaskQueue;

template<typename T,typename D>
class Task;

template<typename T,typename D>
class Process;

 
struct memInf{
public:
    int m_JobHeadNo;
    int m_JobTailNo;
    int m_JobMax;
    int m_JobLeft;
    int m_Jobsize;
    int m_terminate;
    int m_terminateNum;
};


template<typename T,typename D>
class ProcessPool{
private:
    ProcessPool();
    int m_memSize;
    int m_initProcessNum;
    int m_incProcessNum;
    int m_maxProcessNum;
    int m_jobUpperBound;
    int m_jobLowerBound;
    int m_totProcess;

    pid_t m_childID;
    pid_t m_pid;
    void *m_shmptr;
    int m_shmid;
    memInf *m_shmInf;
    Process<T,D> *m_process;
    sem_t *m_dataSem;
    sem_t *m_memLock;
    sem_t *m_dataleftSem;
    char m_dataSemName[40];
    char m_memLockName[40];
    char m_dataleftSemName[40];
public:
    ProcessPool(int maxJobNum,int maxProcessNum, int initProcessNum,int incProcessNum,
                int jobUpperBond,int jobLowerBound);
    virtual ~ProcessPool();
    void createSharedMem();
    void createProcess(int createNum);
    void invoker(D data);
    void terminateAll(){terminate(m_totProcess);}
    int terminate(int num);
    void lockMem();
    void unlockMem();
    void unlink();
};



#endif
