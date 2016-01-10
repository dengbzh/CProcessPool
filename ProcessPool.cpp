/*
 * =====================================================================================
 *
 *       Filename:  ProcessPool.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/02/2016 11:49:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dengbzh (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include"ProcessPool.h"

template<typename T,typename D>
ProcessPool<T,D>::ProcessPool(int maxJobNum,int maxProcessNum,int initProcessNum,int incProcessNum,
                    int jobUpperBond,int jobLowerBound)
{
    m_memSize = maxJobNum*sizeof(D) + sizeof(memInf);
    m_initProcessNum = initProcessNum;
    m_incProcessNum = incProcessNum;
    m_maxProcessNum = maxProcessNum;
    m_jobLowerBound = jobLowerBound;
    m_jobUpperBound = jobUpperBond;
    
    if(m_initProcessNum > m_maxProcessNum)
    {
        printf("initProcessNum must smaller than maxProcessNum...\n");
        exit(1);
    }
    if(m_jobLowerBound >= m_jobUpperBound)
    {
        printf("jobLowerBound must smaller than jobUpperBond...\n");
        exit(1);
    }


    m_totProcess = 0;
    
    m_pid = getpid();

    snprintf(&m_dataSemName[0],sizeof(m_dataSemName),"/ProcessPoolDataSem.%ld",m_pid);
    sem_unlink(&m_dataSemName[0]);
    m_dataSem = sem_open(&m_dataSemName[0],O_CREAT,0666,0);
    if(m_dataSem == SEM_FAILED)
    {
        printf("process pool init sem_t set failed...\n");
    }

    snprintf(&m_dataleftSemName[0],sizeof(m_dataleftSemName),"/ProcessPoolDataSem.%ld",m_pid);
    sem_unlink(&m_dataleftSemName[0]);
    m_dataleftSem = sem_open(&m_dataleftSemName[0],O_CREAT,0666,maxJobNum);
    if(m_dataleftSem == SEM_FAILED)
    {
        printf("process pool init sem_t set failed...\n");
    }



    snprintf(&m_memLockName[0],sizeof(m_memLockName),"/ProcessPoolmemMutex.%ld",m_pid);
    sem_unlink(&m_memLockName[0]);
    m_memLock = sem_open(&m_memLockName[0],O_CREAT,0666,1);
    if(m_memLock == SEM_FAILED)
    {
        printf("process pool init sem_t set failed...\n");
    }
    createSharedMem();

    m_shmInf = (memInf*)m_shmptr;
    m_shmInf->m_JobHeadNo = 0;
    m_shmInf->m_JobTailNo = 0;
    m_shmInf->m_JobMax = maxJobNum;
    m_shmInf->m_JobLeft = 0;
    m_shmInf->m_Jobsize = sizeof(D);
    m_shmInf->m_terminate = 0;
    m_shmInf->m_terminateNum = 0;

    m_process = new Process<T,D>(m_shmid,m_dataSem,m_memLock,m_dataleftSem);
    createProcess(m_initProcessNum);
}

template<typename T,typename D>
ProcessPool<T,D>::~ProcessPool()
{    
    if(m_process != NULL)
        delete m_process;
    unlink();
}


template<typename T,typename D>
void ProcessPool<T,D>::createSharedMem()
{
    if((m_shmid = shmget(IPC_PRIVATE,m_memSize,0600)) < 0)
        err_sys("shmget error...\n");
    if((m_shmptr = shmat(m_shmid,0,0)) == (void *)-1)
        err_sys("shmat error...\n");
}

template<typename T,typename D>
void ProcessPool<T,D>::createProcess(int createNum)
{
    printf("Process %ld create %d child process...\n",getpid(),createNum);
int processcnt = 0;
 forkProcess:  
    if((m_childID = fork())<0)
    {
        err_sys("open error\n");
    }else if(m_childID > 0){
        //parent
        printf("Create process %ld...\n",m_childID); 
        
        m_totProcess++;
        processcnt++;
        if(processcnt < createNum)
        {
            goto forkProcess;
        }
    }else{
        //child
        printf("Process %ld create successful...\n",getpid());
        m_process->run();
        delete m_process;
        printf("Process %ld exit...\n",getpid());
        exit(0);   
    }
}


template<typename T,typename D>
void ProcessPool<T,D>::invoker(D data)
{
    printf("Process %ld assigning jobs %d...\n",getpid(),data);
    if(m_totProcess < m_maxProcessNum)
    {
        lockMem();
        if(m_shmInf->m_JobLeft >=  m_jobUpperBound)
        {
            unlockMem();
            if(m_totProcess + m_incProcessNum < m_maxProcessNum)
            {
                createProcess(m_incProcessNum);
            }
            else
            {
                createProcess(m_maxProcessNum - m_totProcess);
            }
        }
        else if(m_shmInf->m_JobLeft <= m_jobLowerBound)
        {
            unlockMem();
            if(m_totProcess - m_incProcessNum > m_initProcessNum)
            {
                terminate(m_totProcess - m_incProcessNum);
            }
            else if(m_totProcess > m_initProcessNum)
            {
                terminate(m_totProcess - m_initProcessNum);
            }
        }
        else
        {
            unlockMem();
        }
    }
    m_process->loadData(data);

}



template<typename T,typename D>
int ProcessPool<T,D>::terminate(int num)
{
    printf("Proess %ld terminate %d process...\n",getpid(),num);
    lockMem(); 
    m_shmInf->m_terminate = 1;
    m_shmInf->m_terminateNum += num;
    m_totProcess -= num;
    unlockMem();
    return 0;
}

template<typename T,typename D>
void ProcessPool<T,D>::unlink()
{
    //sem_close(m_dataSem);
    shmdt(m_shmptr);
    if(m_pid == getpid())
    {
        sem_close(m_dataSem);
        sem_unlink(&m_dataSemName[0]);
        sem_close(m_memLock);
        sem_unlink(&m_memLockName[0]);
        sem_close(m_dataleftSem);
        sem_unlink(&m_dataleftSemName[0]);
        shmctl(m_shmid,IPC_RMID,0);
    }
}

template<typename T,typename D>
void ProcessPool<T,D>::lockMem()
{
    sem_wait(m_memLock);
}

template<typename T,typename D>
void ProcessPool<T,D>::unlockMem()
{
    sem_post(m_memLock);
}
