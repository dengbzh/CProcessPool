/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/29/2015 04:27:25 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dengbzh (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include"Utility.h"
#include"TaskQueue.h"
#include"TaskQueue.cpp"
#include"Task.h"
#include"Task.cpp"
#include"ProcessPool.h"
#include"ProcessPool.cpp"
#include"Process.cpp"

template<typename D>
class job{
public:
    void run(D data)
    {
        printf("process %ld running job with data %d...\n",getpid(),data);
        sleep(1);
    }
};

int main()
{
    int totJobNum = 20;
    int maxProcessNum = 10;
    int initProcessNum = 5;
    int incProcessNum = 2;
    int jobUpperBond = 10;
    int jobLowerBound = 3;
    int loop = 100;
    
    ProcessPool<job<int>,int> *ppool = new ProcessPool<job<int>,int>(totJobNum,maxProcessNum,initProcessNum,incProcessNum,jobUpperBond,jobLowerBound);
    for(int i =0;i<loop;i++)
    {
        ppool->invoker(i);
    }
    ppool->terminateAll();
    delete ppool;
    printf("Process %ld exit...\n",getpid());
    exit(0);

}


