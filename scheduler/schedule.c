//Prashant Baral
//SIU856384505
//To compile: gcc -o schedule schedule.c
//To execute: ./schedule <qt> p1 p3 p6 p9 p12
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
int go=1;
typedef struct node
{
    pid_t data;
    struct node *next;
	//Struct describing the node of	the queue
}Listnode;
typedef struct queue
{	Listnode* first;
	Listnode* last;
	//Struct describing the queue
}queue1;
void enqueue(queue1* qp,pid_t t)
{
	Listnode* n =(Listnode *) malloc(sizeof(Listnode));
	if(n==NULL)
	{
    		printf("Out of Memory\n");
    		exit(1);
	}
	n->data = t;
	n->next = NULL;
	if(qp->last == NULL)
		qp->first=qp->last=n;
	else
    	{
    		qp->last->next = n;
    		qp->last = n;
	}
}
int dequeue(queue1* qp)
{
	Listnode* n;
	pid_t result;
	if(qp->first == NULL)
	{
		printf("empty queue\n");
		exit(1);
	}
	n = qp->first;
	result = qp->first->data;
	qp->first = qp->first->next;
	free(n);
	if(qp->first == NULL)
		qp->last = NULL;
	return 0;
}
void init(queue1 *lp)
{
	lp->first = NULL;
	lp->last = NULL;
}
void queueprint (queue1 q)
{
	printf("Now I will print the queue\n");
	Listnode * n;
	for (n = q.first; n != NULL; n = n->next) 
	{
		printf("%d\n", n->data);
	}
}
void termchild ( )
{
	go=0;
	signal(SIGCHLD,termchild);// Signal handler ,the termination of a process
        printf("Child terminated");
}
int main(int argc ,char const *argv[])
{
	pid_t mypid[10];
	Listnode *h;
	int N,qt;// Variable	initialization
	queue1 my_queue;
	init(&my_queue);
	if(argc < 2)
	{
	        printf("Wrong input");
	        return -1;
	}
// PART 1:Parse arguments from the user 
if( argc >2)
{
	qt = 1000*atoi(argv[1]); //Here you need the qt
}
 else
{
	printf ("%s qt prog1 [ prog2 ] . . . [ prog [N}\n" , argv[0]); 
	exit(1);
}
// PART 2: Forking the processes 
for(int i=2; i < argc;i++)
{
	printf ("Message from father : Creating program %s,%d\n", argv[i],argc) ; 
	//Fork , execl [ execl ( argv [ i ] , argv [ i ] ,NULL) ] processes and enqueue enqueue the process
	mypid[i]=fork();
	if (mypid[i]==0)
		{
			execl(argv[i],argv[i],NULL);
		}
	enqueue(&my_queue,mypid[i]);
}
sleep (1) ; // Small safe delay 
printf ("\nI am the Scheduler and I will now begin scheduling my programs:\n");
/// PART 3: Scheduling loop 
h = my_queue.first;
while (h!=NULL)  //Scheduling loop 
 {
	h = my_queue.first;
	kill(h->data,SIGCONT);//send the signal SIGCONT to the first element in queue 
	usleep ( qt ) ;
	signal(SIGCHLD,termchild);
	if(go!=0/*child has not finished*/ )
		{
		kill(h->data,SIGUSR1);//send the signal SIGUSR1 to the first element in queue 
		usleep (1000) ; // Small safe delay
		pid_t a= my_queue.first->data;
		dequeue(&my_queue);
		enqueue(&my_queue,a);//dequeue and re−enqueue
	} else
	{
		go=1;
		printf ("A child is dead \n") ;
		dequeue(&my_queue);//remove from list & mark child as dead
		h = my_queue.first;
	//if(h==NULL)
	//	return 0;
	}
	//if(&my_queue==NULL)
	//return 0;
	//h = my_queue.first;
	
 }
return 0;
}
