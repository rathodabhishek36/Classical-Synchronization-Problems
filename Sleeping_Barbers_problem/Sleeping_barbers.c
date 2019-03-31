//Abhishek Rathod B.Tech IIT Roorkee CSE
// Enroll. No.- 17114004

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "my_semaphore.h"
#include <unistd.h>

#define NUM_THREADS 100
pthread_t threads[NUM_THREADS];

int number_of_cuts = 0;// NUM_THREADS;
int number_of_seats = 0;
pthread_mutex_t barber_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t customer_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t customers;
pthread_cond_t barber;

int max_seats = 0;

void *barberBob(void *threadid){
  int haircuts = NUM_THREADS - 1;
  while(haircuts > 0 || number_of_seats != max_seats){
    pthread_mutex_lock(&barber_mutex);
    printf("1) Barber locks barber_mutex, wait for signal on customers\n");
    pthread_cond_wait(&customers, &barber_mutex);
    printf("4) Barber received customers signal, should have lock on barber_mutex\n");
    pthread_cond_signal(&barber);
    printf("5) Barber signals that he's ready\n");
    number_of_seats++;
    printf("6) Barber incremented the empty seats\n");
    pthread_mutex_unlock(&barber_mutex);
    printf("7) Barber unlocks barber_mutex\n");
    //cutting hair
    haircuts--;
    printf("\nBarber starts cutting hair!: seats left = %d, haircuts before close = %d\n",number_of_seats,haircuts);
  }
  printf(" All haircuts done for today, shop closed!\n");
  pthread_exit(NULL);
}

void *customer(void *threadid)
{
    int haircuts = 1;
    while(haircuts > 0){
      sleep(1);
    pthread_mutex_lock(&barber_mutex);
    if(number_of_seats > 1){
      number_of_seats--;
      printf("2) Customer decremented the empty seats\n");
      haircuts--;
      pthread_cond_signal(&customers);
      //      pthread_mutex_unlock(&barber_mutex);// might be shady
      printf("3) Custmoer signals barber,waits for barber \n");
      pthread_cond_wait(&barber, &barber_mutex);
      printf("8) signaled barber, got barber_mutex back\n");
      if(number_of_seats < max_seats){pthread_cond_signal(&customers);}
      pthread_mutex_unlock(&customer_mutex);
      printf("9) custmers getting cut, no longer has barber_mutex lock\n");
      //getting cut
      number_of_cuts++;
      printf("\nCustomer gets hair cut! %d empty seats with %d threadid and %d cuts done total, %d haircuts left for customer \n",number_of_seats,(int)threadid,number_of_cuts,haircuts);
      exit(0) ;
    }
    else{
      printf("\n No empty seats, thread %d \n",(int)threadid);
      pthread_mutex_unlock(&barber_mutex);
      printf(" customer no longer has mutex lock\n");
    }
    //    sleep(1);
  }
  pthread_exit(NULL);
}



int main()
{
    printf("Enter number of seats : ");
    scanf("%d",&number_of_seats) ;
  if(number_of_seats<=1 || number_of_seats>999){
    printf("Invalid value.\n");
    exit(0) ;
  }
  max_seats = number_of_seats;
  printf("number of seats %d \n", number_of_seats);
  pthread_cond_init(&customers,NULL);
  pthread_cond_init(&barber,NULL);
  int t;
  for(t = 0; t < NUM_THREADS; t++){
    printf("Creating thread %d\n", t);
    if(t == 0){
      pthread_create(&threads[t], NULL, barberBob, (void *)t);
    }
    else{
      pthread_create(&threads[t], NULL, customer, (void *)t);
    }
  }
    for(t = 0; t < NUM_THREADS; t++)
    pthread_join(threads[t],NULL);

  pthread_exit(NULL);
}