//Abhishek Rathod B.Tech IIT Roorkee CSE
// Enroll. No.- 17114004

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "my_semaphore.h"
#include <time.h>
#include <unistd.h>

typedef int buffer_queue;
#define BUFFER_SIZE 10

buffer_queue buffer[BUFFER_SIZE];

/* counter for the buffer*/
int counter;

/*mutex lock */
pthread_mutex_t mutex;

/* semaphore variables */
sem_t full, empty;

pthread_t thread_id;       
pthread_attr_t thread_attr; //thread attributes

void *producer(void *param); // producer thread 
void *consumer(void *param); // consumer thread
int insert_data(buffer_queue item) ;
int remove_data(buffer_queue *item) ;

void initializeData() {
   pthread_mutex_init(&mutex, NULL);
   /* Create the full semaphore and initialize to 0 */
   sem_init(&full, 0, 0);
   /* Create the empty semaphore and initialize to BUFFER_SIZE */
   sem_init(&empty, 0, BUFFER_SIZE);
   /* Get the default attributes */
   pthread_attr_init(&thread_attr);
   /* init buffer */
   counter = 0;
}

void *producer(void *param) {
   buffer_queue item;
   while(1) {
      /* sleep for a random period of time */
      sleep(rand()%4+1);
      /* generate a random number to insert inside the buffer*/
      item = rand()%100; // 
      /* acquire the empty lock */
      sem_wait(&empty);
      /* acquire the mutex lock */
      pthread_mutex_lock(&mutex);
      if(insert_data(item)) {
         fprintf(stderr, "Error condition in producer\n");
      }
      else {
         printf("Producer has produced the element : %d\n", item);
      }
      /* release the mutex lock */
      pthread_mutex_unlock(&mutex);
      /* signal full */
      sem_post(&full);
   }
}

void *consumer(void *param) {
   buffer_queue item;
   while(1) {
      /* sleep for a random period of time */
      sleep(rand()%4+1);
      /* aquire the full lock */
      sem_wait(&full);
      /* aquire the mutex lock */
      pthread_mutex_lock(&mutex);
      if(remove_data(&item)) {
         fprintf(stderr, "Error condition in consumer\n");
      }
      else {
         printf("Consumer consumed the element : %d\n", item);
      }
      /* release the mutex lock */
      pthread_mutex_unlock(&mutex);
      /* signal empty */
      sem_post(&empty);
   }
}

int insert_data(buffer_queue item) {
   if(counter < BUFFER_SIZE) {
      buffer[counter] = item;
      counter++;
      return 0;
   }
   else { /* Error the buffer is full */
      return -1;
   }
}

int remove_data(buffer_queue *item) {
   if(counter > 0) {
      *item = buffer[(counter-1)];
      counter--;
      return 0;
   }
   else { /* Error buffer empty */
      return -1;
   }
}

int main() {
   int i;
   /* Verify the correct number of arguments were passed in */
   
   int sleepTime ;
   int numProd ;
   int numCons ;

   printf("Enter time in seconds for main thread to sleep : ") ;
   scanf("%d",&sleepTime) ;
   printf("Enter number of producers : ") ;
   scanf("%d",&numProd) ;
   printf("Enter number of consumers : ") ;
   scanf("%d",&numCons) ;

   initializeData();

   /* Create producer threads */
   for(i = 0; i < numProd; i++) {
      pthread_create(&thread_id,&thread_attr,producer,NULL);
    }

   /* Create consumer threads */
   for(i = 0; i < numCons; i++) {
      pthread_create(&thread_id,&thread_attr,consumer,NULL);
   }

   sleep(sleepTime);

   printf("Exit the program\n");
   exit(0);
}