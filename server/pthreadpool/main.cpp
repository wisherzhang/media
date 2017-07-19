#include "Thread.h"     
#include <iostream>     
#include <unistd.h>     
#include <stdlib.h> 
class CMyTask: public CTask    
{    
public:    
    CMyTask(){}    
        
    inline int Run()    
    {    
        cout<< (char*)this->m_ptrdata;    
        sleep(10);    
        return 0;    
    }    
};
int main()    
{    
    CMyTask taskObj;    
        
    char szTmp[] = "this is the new thread running";    
    taskObj.setData((void*)szTmp);    
    CThreadPool threadPool(10);    
        
    for(int i = 0; i < 20; i++)    
    {    
        threadPool.AddTask(&taskObj);    
    }    
        
    while(1)    
    {    
        cout<<"there are still "<<threadPool.getTaskSize()<<" tasks need to handle"<<endl;    
        if (threadPool.getTaskSize() == 0)    
        {    
            if (threadPool.StopAll() == -1)    
            {       
                cout<<"Now I will exit from main"<<endl;    
                exit(0);    
            }    
        }    
        sleep(2);    
    }    
        
    return 0;    
}
