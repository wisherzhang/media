#ifndef _THREAD_H_
#define _THREAD_H_
#include <vector>
#include <string>
#include <pthread.h>
using namespace std;
/**  
 * 执行任务的类，设置任务数据并执行  
*/  
class CTask
{
protected:
	string m_strTaskname; /** 任务的名称 */ 
	void *m_ptrdata;/** 设置任务数据 */

public:
	CTask(){}
	CTask(const string &taskname)
	{
		m_strTaskname = taskname;
		m_ptrdata = NULL;
	}
	virtual ~CTask(){}
	virtual int Run()=0;
	void setData(void *data);//设置任务数据
};
/**  
 * 线程池管理类的实现  
*/
class CThreadPool
{
private:
	static vector<CTask *> m_vecTaskList;
	static bool shutdown;
	int m_iThreadNum;
	pthread_t *pthread_id;
	static pthread_mutex_t m_pthreadMutex;
	static pthread_cond_t m_pthreadCond;
protected:
	static void *ThreadFun(void *threadData);
	static int MoveToIdle(pthread_t);
	static int MoveToBusy(pthread_t);
	int Create();
public:
	CThreadPool(int threadNum = 10);
	int AddTask(CTask *task);
	int StopAll();
	int getTaskSize();
};
#endif
