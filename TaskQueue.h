/*
 * =====================================================================================
 *
 *       Filename:  TaskQueue.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/30/2015 01:19:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dengbzh (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _TASKQUEUE_H
#define _TASKQUEUE_H
#include"Utility.h"
#include"ProcessPool.h"
#define MAXSIZE_QUEUE 4096
#define DEFAULT_QUEUE 1024
#define TERMINATE_INITAL 0
#define TERMINATE_ACTIVE 1 
#define _POSIX_NAME_MAX  64

struct memInf;

template<typename D>
class TaskQueue{
private:
    TaskQueue();
    memInf *m_jobInf;
    void *m_shmptr;
    void *m_head;
    void *m_dataPtr;
    int m_shmid;
    int m_jobSize;
    sem_t *m_dataSem;
    sem_t *m_memLock;
    sem_t *m_dataleftSem;
public:
    TaskQueue(int shmid,void *shmptr,sem_t *dataSem,sem_t *memLock,sem_t *dataleftSem);
    virtual ~TaskQueue();
    int getData(D& data);
    void lock();
    void unlock();
    int loadData(D data);
    void getTerminate(int &terminate);

};
#endif
