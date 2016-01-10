/*
 * =====================================================================================
 *
 *       Filename:  Task.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/29/2015 11:29:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dengbzh (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _TASK_H
#define _TASK_H
#include"Utility.h"
template<typename D>
class TaskQueue;

template<typename T,typename D>
class Task{
private:
    TaskQueue<D> *m_taskQueue;
    D m_taskdata;
    T *m_task;
   int m_terminate;
public:
    Task();
    virtual ~Task();
    void setTaskQueue(TaskQueue<D> *taskqueue);
    void setTask(T *task);
    int getData();
    void setNoDeley(bool nodelay);
    void run();
    void getTerminate();
    void freetask();
};

#endif
