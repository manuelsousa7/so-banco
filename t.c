#include "stdio.h"
#include "unistd.h"

int main()
{
        printf("MAX CHILD ID IS :%ld\n",sysconf(_SC_CHILD_MAX));
	return 0;
}
