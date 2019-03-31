//Abhishek Rathod B.Tech IIT Roorkee CSE
// Enroll. No.- 17114004

#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h> 

typedef struct 
{
  pthread_cond_t* element;
  struct node* next;
}node; // struct for each element in queue of a semaphore

typedef struct 
{
  node* head;
  node* tail;
  pthread_mutex_t queue_mutex;
  int len;

}queue; // struct for the queue in the semaphore 

void insert_element(queue *q, pthread_cond_t* val)  
{  
   	node* reg=(node*)malloc(sizeof(node));
    reg->element=val;
    if(q->head==NULL)
    {
    	q->head=reg;
    	q->tail=reg;
    	q->len=1; 
    }
    else
    {
    	q->tail->next=reg;
    	q->tail=reg;
	    q->len++;
    }
}



pthread_cond_t* remove_element(queue *q)  
{  
   if(q->len<=0)
	 {
    pthread_mutex_unlock(&q->queue_mutex);
	 	return NULL;
	 }
	 else
	 {
    pthread_cond_t* return_ele=q->head->element;	     
	 	if(q->len==1)
	 	{
	 		q->head=NULL;
	 		q->tail=NULL;
	 		q->len=0;
	 	}
	 	else
	 	{
	 		q->len--;
      q->head=q->head->next;
	 	}
    pthread_mutex_unlock(&q->queue_mutex);
	  
   	return return_ele;
	 } 
   
	 return NULL;
}

void push(queue* q ,pthread_cond_t* val) 
{
   pthread_mutex_lock(&q->queue_mutex);
   
   insert_element(q, val) ;

   pthread_mutex_unlock(&q->queue_mutex);
}


pthread_cond_t* pop(queue* q)
{
   pthread_mutex_lock(&q->queue_mutex);

   pthread_cond_t* return_ele=remove_element(q)  ;
   
   return return_ele; 
 }



typedef struct
{
   atomic_int s;  // semaphore varible
   queue Q;           // queue for storing blocked threads
   pthread_mutex_t lock1; 
   pthread_mutex_t lock2; 
   
} sem_t; // semaphore struct 
 


void sem_init( sem_t* reg,int pshared,unsigned int element) // for semaphore inilization
 {
    reg->s=element; // assign value
   // thread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
 }


 void sem_wait(sem_t* reg) //  same AS wait(S)
 {
 	pthread_mutex_lock(&reg->lock1); // saving from other threads   
    reg->s--;
    if(reg->s<0)
    {
      pthread_cond_t* cond_reg=(pthread_cond_t*)malloc(sizeof(pthread_cond_t));
      push(&reg->Q,cond_reg); 
      pthread_cond_wait(cond_reg,&reg->lock1); //block(reg) blocking reg thread 

            
    } 

    pthread_mutex_unlock(&reg->lock1);

  
}

 void sem_post(sem_t* reg)
 {
  pthread_mutex_lock(&reg->lock2); // saving from other threads

    reg->s++;    
    if(reg->s<=0)
    {
      
    	pthread_cond_t* cond=pop(&reg->Q);
    	if(cond!=NULL)
      {
      pthread_cond_signal(cond);   // wake up (selected thread)
      }

    }
  pthread_mutex_unlock(&reg->lock2);

 }