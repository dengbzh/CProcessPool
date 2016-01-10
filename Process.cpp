/*
 * =====================================================================================
 *
 *       Filename:  process.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/29/2015 11:23:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dengbzh (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include"Process.h"
#include"TaskQueue.h"
#include"Task.h"

template<typename T,typename D>
Process<T,D>::Process(int shmid,sem_t *dataSem, sem_t *memLock,sem_t *dataleftSem)
{
    m_shmid = shmid;
    if((m_shmptr = shmat(m_shmid,0,0)) == (void *)-1)
        err_sys("shmat error...\n");


    m_taskQueue = new TaskQueue<D>(m_shmid,m_shmptr,dataSem,memLock,dataleftSem);
    m_job = new T;
    m_task = new Task<T,D>;
    
    m_task->setTaskQueue(m_taskQueue);
    m_task->setTask(m_job);
    printf("Process %ld create successful...\n",getpid());
}

template<typename T,typename D>
Process<T,D>::~Process()
{
    freeProcess();
}

template<typename T,typename D>
void Process<T,D>::run()
{
    m_task->run();
}

template<typename T,typename D>
void Process<T,D>::terminate()
{
    m_taskQueue->terminate();
}

template<typename T,typename D>
void Process<T,D>::loadData(D data)
{
    m_taskQueue->loadData(data);
}

template<typename T,typename D>
void Process<T,D>::freeProcess()
{
//    printf("Process %ld delete %ld...\n",getpid(),m_pid);
    if(m_task != NULL)
    {
       delete m_task;
    }
    if(m_job != NULL)
    {
        delete m_job;
    }

    if(m_taskQueue != NULL)
    {
        delete m_taskQueue;
    }

    if(shmdt(m_shmptr) == -1)
        err_sys("shmdt error in %ld...\n",getpid());
}
