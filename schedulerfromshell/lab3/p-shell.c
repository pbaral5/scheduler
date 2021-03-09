#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/resource.h>

void cont (int sig_num) {
     
}
void cont1()
{
signal(SIGCONT,cont1);
//printf("STARTING PROCESS");
}

void stop (int sig_num) {
printf("quantum time reached\n");
     pause();
}
void stop1()
{
pause();
}

int main (char *argc[], char *argv[]) {


	int i,num,sltime;
	
	signal(SIGQUIT,cont);
	signal(SIGTSTP,stop);
	signal(SIGUSR1,stop1);
	signal(SIGCONT,cont1);
	num =atoi(argv[1]);
	sltime = 1000*atoi(argv[2]);
	pause();
	for (i=1; i<=num; i++){
		printf("This is program %s and it prints for the %d time of %d...\n",argv[0],i,num);
		usleep(sltime);
	}
}
