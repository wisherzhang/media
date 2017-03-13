#include "death_handler.hpp"
#include <signal.h>
#include <iostream>
using namespace std;
bool flag = true;
static void
signal_handler (int signo)
{
  static unsigned int __terminated = 0;

  switch (signo) {
  case SIGINT:
	cout<<"receive Ctrl+c signal"<<endl;
	flag = false;
	break;
  case SIGTERM:
    /*if (__terminated == 0) {
      GST_DEBUG ("Terminating.");
      loop->quit ();
    }

    __terminated = 1;*/
	cout<<"receive kill singal "<<endl;
    break;

  case SIGPIPE:
    //GST_DEBUG ("Ignore sigpipe signal");
    cout<<"receive SIGPIPE singal "<<endl;
    break;

  default:
    break;
  }
}

int main(int argc,char*argv[])
{
        Debug::DeathHandler dh;
        dh.set_thread_safe (true);
        dh.set_color_output (true);

	struct sigaction signalAction;

 	/* Install our signal handler */
  	signalAction.sa_handler = signal_handler;

  	sigaction (SIGINT, &signalAction, NULL);
  	sigaction (SIGTERM, &signalAction, NULL);
  	sigaction (SIGPIPE, &signalAction, NULL);
	while(flag);
	return 0;
}
