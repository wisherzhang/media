
#include "death_handler.hpp"
int main(int argc,char*argv[])
{
        Debug::DeathHandler dh;
        dh.set_thread_safe (true);
        dh.set_color_output (true);
        int *p = NULL;
        *p = 10;
	return 0;
}
