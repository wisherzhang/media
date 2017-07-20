#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
using namespace std;
static pthread_key_t str_key;
static pthread_once_t str_alloc_key_once = PTHREAD_ONCE_INIT;
static void str_alloc_key();
static void str_alloc_destroy_accu(void *accu);
char *str_accumulate(const char *s)
{
	char *accu;
	pthread_once(&str_alloc_key_once,str_alloc_key);
	accu = (char *)pthread_getspecific(str_key);
	if(accu == NULL)
	{
		accu =(char *)malloc(1024);
		if(accu == NULL) return NULL;
		accu[0] = 0;
		pthread_setspecific(str_key,(void*)accu);
		cout<<"Thread "<<pthread_self()<<" : alloc buffer at "<<accu<<endl;
	}
	strcat(accu,s);
	return accu;
}
static void str_alloc_key()
{
	pthread_key_create(&str_key,str_alloc_destroy_accu);
	cout<<"Thread "<<pthread_self()<<": alloced key  "<<str_key<<endl;
}
static void str_alloc_destroy_accu(void *accu)
{
	cout<<"Thread "<<pthread_self()<<" freeing buffer at "<<accu<<endl;
	free(accu);
}
void *process(void *arg)
{
	char *res;
	res = str_accumulate("Result of ");
	if(strcmp((char*)arg,"first")==0)
           sleep(3);
	res=str_accumulate((char*)arg);
	res=str_accumulate(" thread");
	cout<<"Thread "<<pthread_self()<<": "<<res<<endl;
	return NULL;
}
int main(int argc,char* argv[])
{ 
    char* res;
    pthread_t th1,th2;
    res=str_accumulate("Result of ");
    pthread_create(&th1,NULL,process,(void*)"first");
    pthread_create(&th2,NULL,process,(void*)"second");
    res=str_accumulate("initial thread");
    cout<<"Thread "<<pthread_self()<<": "<<res<<endl;;
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    pthread_exit(0);
} 
