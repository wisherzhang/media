#include "Thread.h"
#include <iostream>
#include <stdlib.h>
void CTask::setData(void *data)
{
	m_ptrdata = data;
}
vector<CTask *> CThreadPool::m_vecTaskList;
bool CThreadPool::shutdown = false;
pthread_mutex_t CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;
CThreadPool::CThreadPool(int threadNum)
{
	this->m_iThreadNum = threadNum;
	cout << "I will create " << threadNum << " threads" << endl; 
	Create();
}
void *CThreadPool::ThreadFun(void *threadData)
{
     while(1)
     {
	pthread_t tid = pthread_self();
	while(m_vecTaskList.size() == 0&&!shutdown)
	{
		pthread_mutex_lock(&m_pthreadMutex);
		pthread_cond_wait(&m_pthreadCond,&m_pthreadMutex);
	}
	if(shutdown)
	{
		pthread_mutex_unlock(&m_pthreadMutex);
		cout<<"thread "<<pthread_self()<<" will exit"<<endl;
		pthread_exit(NULL);
	}
	cout<<"tid "<<tid<<cout<<" run"<<endl;
	vector<CTask *>::iterator iter =  m_vecTaskList.begin();
	CTask *task = *iter;
	if (iter != m_vecTaskList.end())    
        {    
            task = *iter;    
            m_vecTaskList.erase(iter);    
        } 
	pthread_mutex_unlock(&m_pthreadMutex);
	task->Run(); /** 执行任务 */    
        cout<<"tid:"<<tid<<" idle"<<endl;   
     }
   return (void*)0;
}
int CThreadPool::AddTask(CTask *task)
{
	pthread_mutex_lock(&m_pthreadMutex);
	this->m_vecTaskList.push_back(task);    
        pthread_mutex_unlock(&m_pthreadMutex);    
	pthread_cond_signal(&m_pthreadCond);
	return 0;
}
int CThreadPool::Create()    
{  
	pthread_id = (pthread_t *)malloc(sizeof(pthread_t) * m_iThreadNum);
	for(int i =0;i<m_iThreadNum;i++)
	{
		pthread_create(&pthread_id[i],NULL,ThreadFun,NULL);
	}
	return 0;
}
int CThreadPool::StopAll()    
{
	/** 避免重复调用 */    
	if (shutdown)    
	{    
		return -1;      
	}  
	cout<<"Now I will end all threads!!"<<endl;;    
	shutdown = true;
	pthread_cond_broadcast(&m_pthreadCond);
	/** 阻塞等待线程退出，否则就成僵尸了 */    
	for (int i = 0; i < m_iThreadNum; i++)    
	{ 
		pthread_join(pthread_id[i], NULL);      
	}   	
	free(pthread_id);    
	pthread_id = NULL;
	pthread_mutex_destroy(&m_pthreadMutex);
	pthread_cond_destroy(&m_pthreadCond);
	return 0;
}
/**  
 * 获取当前队列中任务数  
 */    
int CThreadPool::getTaskSize()    
{    
	return m_vecTaskList.size();        
}  



