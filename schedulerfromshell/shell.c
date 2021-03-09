#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <sys/resource.h>
#include "queue.h"


int fg_pid=0;
int fg_suspended=0;
int run=1,GONE=1;

struct queue pid_list,pid_list2;
slist my_queue;
pid_t mypid[10];
void childdead();
void help() {
	printf("This is manual page\n");
	printf("This shell supports the following commands:\n");
	printf("\tver\n\texec\n\tps\n\tkill\thelp\n\texit\n");
	printf("For more details please type 'help command'\n");
}
void termchild ( )
{
	//go=0;
	signal(SIGCHLD,termchild);// Signal handler ,the termination of a process
        printf("Child dead");
}

void helpcmd(char *cmd) {
	printf("This is manual page\n\n");
	if (strcmp(cmd,"ver")==0)
	{
		printf("\nver:\tShows details about the shell version\n");
	}
	else if (strcmp(cmd,"exec"==0)) 
	{
		printf("\nexec p1(n1,qt1) p2(n2,qt2) ...:\nExecutes the programs p1, p2 ...\nEach program types a message for n times and it is given a time quantum of qt msec.\n");
		printf("If parameter (&) is given the program will be executed in the background\n");
	}
	else if (strcmp(cmd,"ps")==0)
	{
		printf("\nps:\tShows the living process with the given pid\n");
	}
	else if (strcmp(cmd,"kill")==0)
	{
		printf("\nkill pid:\tEnds the process with the given pid\n");
	}
	else if (strcmp(cmd,"help")==0)
	{
		printf("\nhelp:\tYou should know this command by now\n");
	}
	else if (strcmp(cmd,"exit")==0)
	{
		printf("\nexit:\tEnds the experience of working in the new shell\n");
	}
	else 
	{
		printf("\nNo Such command. Type help to see a list of commands\n");
	}
}

void ver() {
	printf("\nNew Shell. Works properly!!\n");
}

void ps() {
	struct node *p;
	printf("\nNEW SHELL presents the following living processes\n");
	printf("\tPID\tNAME\n");
	int y=1;
	while(y<3)
	{
		printf("\t%d\n",mypid[y]);
	y=y+1;
	}
}

void mykill(int pid) {
	kill(pid,SIGTERM);
	printf("You have just killed process %d\n",pid );
}


void exec(char *input) {
	int i,t,status;
	char *args[10];
	char *temp;
	struct node *p;

	for (i = 0; i < 10; i++)
	{
		args[i]=(char *)malloc(10*sizeof(char));
	}

	strcpy(args[0],strtok(input,"(,"));
	for (i=1; (temp=strtok(NULL,",)"))!=NULL; i++) 
		strcpy(args[i],temp);
	printf("\n");
	if (strcmp(args[i-1],"&")==0)
	{
		args[i-1]=NULL;
	}
	else
		args[i]=NULL;
	slInsert(&my_queue,atoi(args[1]));
	/*if ((t=fork())==0)
	{
		execv(args[0],args);
	}
	enqueue(t,args[0],atoi(args[2]),&pid_list);*/

}

void myexit() {
	char yesno;
	if (pid_list.head==pid_list.tail)
	{
		run=0;
	}
	else {
		printf("There are still living processes Do you want to kill them? (y/n): ");
		yesno=getchar();
		if (yesno == 'y')
		{
			while(pid_list.head!=pid_list.tail)
			{
				mykill(pid_list.tail->pid);
				usleep(10000);
			}
			run=0;
		}
	}
}

void childdead(int signum){
	int dead_pid, status;
	dead_pid=wait(&status);
	printf("The child %d is dead\n",dead_pid );
	GONE=0;
	delete(&pid_list,dead_pid);
	printf("\n");
	if (dead_pid==fg_pid)
	{
		fg_pid=0;
	}
}
void RR(){
struct node* h;
h= pid_list.head->next;
while (h!=NULL)  //Scheduling loop 
 {
	h= pid_list.head->next;
	kill(h->pid,SIGCONT);//send the signal SIGCONT to the first element in queue 
	usleep ( 1000*(h->quantum) ) ;
	signal(SIGCHLD,childdead);
	if(GONE==1/*child has not finished*/ )
		{
		kill(h->pid,SIGTSTP);//send the signal SIGUSR1 to the first element in queue 
		usleep (1000) ; // Small safe delay
		pid_t a= h->pid;
		//dequeue(a,h->name,&pid_list);
		delete(&pid_list,a);
		enqueue(a,h->name,h->quantum,&pid_list);//dequeue and re−enqueue
	} else
	{
		delete(&pid_list,h->pid);
		h= pid_list.head->next;
	}
	
 }
}

void SJF( int argnumber)
{

int t,q,i=1,k;
int a =1;
char *buf[10];
for (k = 0; k < 10; k++)
	{
		buf[k]=(char *)malloc(10*sizeof(char));
	}
buf[0]="p";
	while(my_queue!=NULL)
	{	sprintf(buf[i], "%d",my_queue->data);
		mypid[i]=fork();
		if ((mypid[i])==0)
		{
		execl("p","p",buf[i],NULL);
		exit(0);
		}
		i=i+1;
		my_queue =((my_queue)->next1);
	}
	for(int j=1;j<=argnumber;j++){
		usleep(1000);
		kill(mypid[j],SIGCONT);
		usleep (1000);
		wait(&q);
		
		}
}

void MFQ()
{
	struct node *h,*k;
	h = pid_list.head->next;
	while((h!=NULL)||(k!=NULL))
	{	h= pid_list.head->next;
		k=pid_list2.head;
	if(h!=NULL)
	{
		kill(h->pid,SIGCONT);
		usleep (1000*(h->quantum));
		signal(SIGCHLD,childdead);
		if(GONE==1)
		{
			kill(h->pid,SIGTSTP);//send the signal SIGUSR1 to the first element in queue 
			usleep (1000) ; // Small safe delay
			pid_t a= h->pid;
			//dequeue(a,h->name,&pid_list2);
			delete(&pid_list,a);
			enqueue(a,h->name,h->quantum,&pid_list2);//dequeue and re−enqueue
		}
		else
		{
			delete(&pid_list,h->pid);
			h= pid_list.head->next;
		}
	}
	else
	{
		k=pid_list2.head;
		kill(k->pid,SIGCONT);
		usleep(2000*(k->quantum));
		signal(SIGCHLD,childdead);
		if(GONE==1)
		{
			kill(k->pid,SIGTSTP);//send the signal SIGUSR1 to the first element in queue 
			usleep (1000); // Small safe delay
			pid_t b= k->pid;
			//dequeue(a,k->name,&pid_list2);
			delete(&pid_list2,b);
			enqueue(b,k->name,k->quantum,&pid_list2);//dequeue and re−enqueue
		}
		else
		{
			delete(&pid_list2,k->pid);
			k=pid_list2.head;
		}
	}
	}
}
void susp (int signum) {
	fg_suspended=1;
	printf("All processes supspended\n");
}

void cont (int signum) {
	fg_suspended=0;
	printf("Waking all processes...\n");
	while (fg_pid!=0 && fg_suspended!=1)
		pause();
}

int main(int argc, char const *argv[])
{	
	slInit(&my_queue);
	char input[15][30];
	char type[15];
	int argnum, i,m;
	//struct node* h;
	pid_list.head=NULL;
	pid_list.tail=NULL;
	enqueue(getppid(),"NEW SHELL",0,&pid_list);
	//signal(SIGCHLD,termchild);
	signal(SIGTSTP,susp);
	signal(SIGQUIT,cont);
	setpriority(PRIO_PROCESS,0,-20);

	ver();
	
	while (run){
		printf("=>");
		for (argnum=0; (scanf("%s",&input[argnum]))==1;argnum++)
			if (getchar()=='\n') break;
		if (strcmp(input[0],"ver")==0 && argnum==0) ver();
		else if (strcmp(input[0],"help")==0 && argnum==0) help();
		else if (strcmp(input[0],"help")==0 && argnum==1) helpcmd(input[argnum]);
		else if (strcmp(input[0],"ps")==0 && argnum==0) ps();
		else if (strcmp(input[0],"kill")==0 && argnum==1) mykill(atoi(input[1]));
		else if (strcmp(input[0],"exec")==0 && argnum!=0){ 
			for (i=1; i<=argnum; i++) exec(input[i]);
			//FIFO();
			printf("For shortest job first type SJF \nEnter Scheduling type:\n ");
			scanf("%s",&type);
		if(strcmp(type,"SJF")==0) SJF(argnum); 
 			
}
		else if (strcmp(input[0],"exit")==0 && argnum==0) myexit();
	    else printf("No such command. Check help for help.\n");
	}
}
