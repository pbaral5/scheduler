#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/resource.h>
void main()
{
int a,t;
t=fork();
if(t==0)
{
	execl("p","p",NULL);
}
wait(&a);
}
