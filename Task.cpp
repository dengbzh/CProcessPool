/*
 * =====================================================================================
 *
 *       Filename:  Task.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/29/2015 11:57:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dengbzh (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include"Task.h"
#include"TaskQueue.h"
template<typename T,typename D>
Task<T,D>::Task()
{

}

template<typename T,typename D>
Task<T,D>::~Task()
{
    freetask();
}

template<typename T,typename D>
void Task<T,D>::setNoDeley(bool nodelay)
{

}

template<typename T,typename D>
void Task<T,D>::setTaskQueue(TaskQueue<D> *taskqueue)
{
    m_taskQueue = taskqueue;
}

template<typename T,typename D>
void Task<T,D>::setTask(T *task)
{
    m_task = task;
}

template<typename T,typename D>
int  Task<T,D>::getData()
{
   return  m_taskQueue->getData(m_taskdata);
}

template<typename T,typename D>
void Task<T,D>::getTerminate()
{
    m_taskQueue->getTerminate(m_terminate);
}

template<typename T,typename D>
void Task<T,D>::run()
{
    if(m_task != NULL)
    {
        while(1)
        {
            printf("Task %ld waitting data...\n",getpid());
            getData();
            
            m_task->run(m_taskdata);
            
            getTerminate();
            
            if(m_terminate == 1)
            {
                break;
            }
        }
    }
    else{
        printf("No such task...\n");
    }
}

template<typename T,typename D>
void Task<T,D>::freetask()
{

}
