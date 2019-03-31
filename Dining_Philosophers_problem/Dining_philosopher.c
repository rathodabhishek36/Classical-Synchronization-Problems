//Abhishek Rathod B.Tech IIT Roorkee CSE
// Enroll. No.- 17114004

#include <stdio.h>
#include <pthread.h>
#include "my_semaphore.h"
#include <unistd.h>


// Number of philosphers
#define N 5

// States of philosphers
#define THINKING_STATE 0
#define HUNGRY_STATE 1
#define EATING_STATE 2

// Neighbouring philosphers
#define LEFT_PHILOSOPHER (ph_num+4)%N
#define RIGHT_PHILOSOPHER (ph_num+1)%N

// Semophore controls access to execute take_fork() and put_fork() 
sem_t mutex;

// Semophore for philosphers
sem_t S[N];

// Eat count for individual philosphers
int count[5];

// Total food count
int FOOD = 0;

// creates thread for each philospher
void * philospher(void *num);

void pick_fork(int);
void put_fork(int);

// checks the condition variable for taking the fork
void test(int);

// States for each philospher
int state[N];

int phil_num[N]={0,1,2,3,4};

int main()
{
    int i;

    pthread_t thread_id[N];

    // Initializing the semophores
    sem_init(&mutex,0,1);
    for(i=0;i<N;i++)
        sem_init(&S[i],0,0);

    // creates threads for all philosphers
    for(i=0;i<N;i++)
    {
        pthread_create(&thread_id[i],NULL,philospher,&phil_num[i]);
       
    }

    for(i=0;i<N;i++)
        pthread_join(thread_id[i],NULL);

    // outputs food count for each philosphers
    for(i=0;i<N;i++)
	printf("Philospher %d ate %d \n",i,count[i]);
    printf("\n");
}



//   philospher checks for availablity of food.
//   thinks for t ms
//   picks up fork and starts eating
//   puts fork down back

void *philospher(void *num)
{   
   
    while(FOOD <= 200)
    {
        int *i = num;
        usleep(10000);
        pick_fork(*i);
        put_fork(*i);
    }
}



//   waits for mutex lock to avoid race condition between put_fork() and pick_fork()
//   sets state as HUNGRY STATE
//   tests for condition variable to synchronise fork distribution
//   picks fork and eat

void pick_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = HUNGRY_STATE;
    test(ph_num);
    sem_post(&mutex);
    sem_wait(&S[ph_num]);
    usleep(10000);
}


//   checks if fork is available for philospher
//   and if free then signals philospher

void test(int ph_num)
{
    if (state[ph_num] == HUNGRY_STATE && state[LEFT_PHILOSOPHER] != EATING_STATE && state[RIGHT_PHILOSOPHER] != EATING_STATE)
    {
        state[ph_num] = EATING_STATE;
        usleep(20000);
        sem_post(&S[ph_num]);
    }
}


//   waits for mutex lock
//   sets state as THINKING STATE
//   increases food count
//   signals neighbours for availablity of fork
//   prints status of philosphers and then releases mutex lock

void put_fork(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = THINKING_STATE;

    count[ph_num]++;
    FOOD++;
  
    test(LEFT_PHILOSOPHER);
    test(RIGHT_PHILOSOPHER);

	printf("Total Eating Count = %d \n", FOOD);
	int i;
	for(i=0;i<5;i++){
		if(state[i]==EATING_STATE)
			printf("Philosopher %d is eating\n", i);
		else if(state[i]==HUNGRY_STATE)
			printf("Philosopher %d is waiting and calling pickup()\n", i);
		else if(state[i]==THINKING_STATE)
			printf("Philosopher %d is thinking\n", i);
	}

    sem_post(&mutex);
}