/*
 * =====================================================================================
 *
 *       Filename:  TaskQueue.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/30/2015 05:02:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dengbzh (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include"TaskQueue.h"
template<typename D>
TaskQueue<D>::TaskQueue(int shmid,void *shmptr,sem_t *dataSem,sem_t *memLock,sem_t *dataleftSem)
{
    m_shmid = shmid;
    m_shmptr = shmptr;
    m_jobInf = (memInf*)m_shmptr;
    m_head = m_shmptr + sizeof(memInf);
    m_jobSize = sizeof(D);
    m_dataPtr = NULL;
    m_dataSem = dataSem;
    m_memLock = memLock;
    m_dataleftSem = dataleftSem;
}

template<typename D>
TaskQueue<D>::~TaskQueue()
{

}


template<typename D>
int TaskQueue<D>::loadData(D data)
{
    sem_wait(m_dataleftSem);
    lock();
    if(m_jobInf->m_JobLeft == m_jobInf->m_JobMax)
    {
        unlock();
        return -1;
    }

    *(D*)(m_head + m_jobSize*m_jobInf->m_JobTailNo) = data;

    if(m_jobInf->m_JobTailNo == m_jobInf->m_JobMax)
    {
        m_jobInf->m_JobTailNo = 0;
    }
    else
    {
        m_jobInf->m_JobTailNo++;
    }

    m_jobInf->m_JobLeft++;
    printf("m_JobLeft:%d...\n",m_jobInf->m_JobLeft);
    sem_post(m_dataSem);
    unlock();
    return  0;
}

template<typename D>
int TaskQueue<D>::getData(D& data)
{
    sem_wait(m_dataSem);
    lock();
    
    data  = *(D*)(m_head + m_jobInf->m_JobHeadNo*m_jobSize);
    
    if(m_jobInf->m_JobHeadNo != m_jobInf->m_JobTailNo)
    {
        if(m_jobInf->m_JobHeadNo == m_jobInf->m_JobMax)
        {
            m_jobInf->m_JobHeadNo = 0;
        }
        else
        {
            m_jobInf->m_JobHeadNo++;
        }
    }

    m_jobInf->m_JobLeft--;
    printf("m_JobLeft:%d...\n",m_jobInf->m_JobLeft);
    sem_post(m_dataleftSem);
    unlock();
    return 0;
}

template<typename D>
void TaskQueue<D>::getTerminate(int &terminate)
{
    lock();
    if(m_jobInf->m_JobLeft !=0)
    {
        unlock();
        terminate = 0;
        return;
    }
    terminate = m_jobInf->m_terminate;
    if(m_jobInf->m_terminate != 0)
    {
        m_jobInf->m_terminateNum--;
        if(m_jobInf->m_terminateNum == 0)
        {
            m_jobInf->m_terminate = 0;
        }
    }
    unlock();
}

template<typename D>
void TaskQueue<D>::lock()
{
    sem_wait(m_memLock);
}

template<typename D>
void TaskQueue<D>::unlock()
{
    sem_post(m_memLock);
}

