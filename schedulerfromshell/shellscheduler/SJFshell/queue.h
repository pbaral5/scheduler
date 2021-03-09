typedef struct list_tag {
	int data;
	struct list_tag * next1;
} ListNode;

typedef ListNode * slist;
void slPrint (slist *lp){
	printf("Now I will print\n");
	while ((*lp) != NULL)
	{
		printf("%d\n",(*lp)->data);
		lp = &((*lp)->next1);
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
		lp = &((*lp)->next1);
	
	n->next1 = *lp;
	*lp = n;
}
struct node {				
       int pid;
	char *name;
	int quantum;
       struct node *next;
	
};


struct queue {				
       struct node *head;
       struct node *tail ;
};

     
void enqueue(int item, char *s,int quant, struct queue *q) {		
     struct node *p;						

     p= (struct node *)malloc(sizeof(struct node));
     p->pid=item;
     p->name=s;	
     p->quantum=quant;
     p->next=NULL;
     if (q->head==NULL) q->head=q->tail=p;
     else {
          q->tail->next=p;
          q->tail=p;
     }

}


int dequeue(struct queue *q) {		
    int item;				
    struct node *p;
    
    item=q->head->pid;
    p=q->head;
    q->head=q->head->next;
    free(p);
    return item;
}


void delete(struct queue *q, int key)
{
     if (q->head->pid == key)
     {
        struct node *p = q->head;
        q->head = q->head->next;
        free(p);
        return;
    }
    struct node *current = q->head->next;
    struct node *previous = q->head;
    while (current != NULL && previous != NULL)
    {
      if (current->pid == key)
      {
        struct node *tmp = current;
        if (current == q->tail)
          q->tail = previous;
        previous->next = current->next;
        free(tmp);
        return;
      }
      previous = current;
      current = current->next;
    }
    return;  
  }





