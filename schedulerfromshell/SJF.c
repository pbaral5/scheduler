//Prashant Baral
//siu856384505
//To Compile: gcc -o SJF SJF.c
//To execute: SJF p1 p3 p6 p9 p12
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int go=1;
typedef struct list_tag {
	int data;
	struct list_tag * next;
} ListNode;

typedef ListNode * slist;

void slInsert (slist * lp, int t);
void slPrint (slist * lp);
void slInit(slist * qp);

void termchild ( )
{
	go=0;
	signal(SIGCHLD,termchild);// Signal handler ,the termination of a process
        printf("Child dead");
}
void slPrint (slist *lp){
	printf("Now I will print\n");
	while ((*lp) != NULL)
	{
		printf("%d\n",(*lp)->data);
		lp = &((*lp)->next);
	}
}
void slInit(slist * qp){
	*qp = NULL;
}

void slInsert (slist * lp, int t){
	ListNode * n = (ListNode *) malloc(sizeof(ListNode));
	if (n == NULL)
	{
		printf("Out of memory\n");
		exit(1);
	}
	
	n->data = t;
	while (*lp != NULL && (*lp)->data < t)
		lp = &((*lp)->next);
	
	n->next = *lp;
	*lp = n;
}
/*void main(){
	slist sl;
	int data;
	slInit(&sl);
	printf("Give me numbers. 0 = exit\n");
	
	scanf("%d",&data);
	while (data != 0)
	{
		slInsert(&sl, data);
		scanf("%d",&data);
	}

	slPrint(&sl);
	printf("Bye\n");
}*/
int main(int argc ,char *argv[])
{
	pid_t mypid[10];
	//Listnode *h;
	int N,qt;// Variable	initialization
	slist my_queue;
	slInit(&my_queue);
	if(argc < 2)
	{
	        printf("Wrong input");
		printf("Input as ./executable <process1> <process2>........<processn>");
	        return -1;
	}
// PART 1:Parse arguments from the user 
if( argc >2)
{
	qt = 5000*atoi(argv[1]); //Here you need the qt
}
 else
{
	printf ("%s  prog1 [ prog2 ] . . . [ prog [N}\n" , argv[0]); 
	exit(1);
}
for(int i=1; i < argc;i++)
	{
	char *token = strtok(argv[i], "p");
	int l = atoi(token);
	slInsert(&my_queue,l);
	}
// PART 2: Forking the processes 
for(int i=1; i < argc;i++)
	{
	printf ("Message from father : Creating program %s,%d\n", argv[i],argc) ; 
	//Fork , execl [ execl ( argv [ i ] , argv [ i ] ,NULL) ] processes and enqueue enqueue the process
	int k;
        char buf[20];
        snprintf(buf, 20, "p%d", my_queue->data); // puts string into buffer
	mypid[i]=fork();
	if (mypid[i]==0)
		{
			execl(buf,buf,NULL);
		}
	my_queue = ((my_queue)->next);
	//slInsert(&my_queue1,mypid[i]);
}
sleep (1) ; // Small safe delay 
printf ("\nI am the Scheduler and I will now begin scheduling my programs:\n");
/// PART 3: Scheduling loop
int a=1,p; 
while (a<argc)  //Scheduling loop 
 {
	kill(mypid[a],SIGCONT);//send the signal SIGCONT to the first element in queue 
	//usleep ( qt ) ;
	wait(&p);
	signal(SIGCHLD,termchild);
	a=a+1;
	//printf ("A child is dead \n") ;
	//my_queue = ((my_queue)->next);
 }
return 0;
}




