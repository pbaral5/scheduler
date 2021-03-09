//Prashant Baral
//SIU856384505
//To compile: gcc -o timeout timeout.c
//To execute: ./timeout <time> <schedulingtype> eg ./timeout 50 schedule
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int a;
void signal1(){
	a=1;
  printf("Time is up\n");
}

void main(int argc ,char const *argv[])
{
	alarm(atoi(argv[1]));
	signal(SIGALRM,signal1);
	//signal(SIGALRM,signal1);
	pid_t pid = fork();

	if(pid==0)
	{	//alarm(atoi(argv[1]));
		execl(argv[2],argv[2],"50","p1","p2","p3","p4",NULL);
	}

	pause();
	if(a==1)
	{
		kill(pid,SIGKILL);
	}
}
