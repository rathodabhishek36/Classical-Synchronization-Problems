//Abhishek Rathod B.Tech IIT Roorkee CSE
// Enroll. No.- 17114004

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "my_semaphore.h"
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

sem_t smokers[3];
sem_t smoker_agent;

typedef void *(*smokers_t)(void *);

static int totalcount=20 ; //total number of times smokers will smoke a cigarette


// Assuming all the three smokers have an infinite supply of paper
 
void* smoker_1(void* arg)
{
	while (totalcount>0)
	{
		nanosleep((struct timespec[]){{0, rand() % 100000000}}, NULL);
		printf("Smoker one is waiting to smoke.\n");

		sem_wait(&smokers[0]);
		printf("Smoker one received matches and tobacco and is now making cigarette.\n");
		sem_post(&smoker_agent);
		totalcount-- ;
	}
	if(totalcount==0){
		exit(0) ;
	}
	return NULL;
}

void* smoker_2(void* arg)
{
	while (totalcount>0)
	{
		nanosleep((struct timespec[]){{0, rand() % 100000000}}, NULL);
		printf("Smoker two is waiting to smoke.\n");

		sem_wait(&smokers[1]);
		printf("Smoker two received matches and tobacco and is now making cigarette.\n");
		sem_post(&smoker_agent);
		totalcount-- ;
	}
	if(totalcount==0){
		exit(0) ;
	}
	return NULL;
}

void* smoker_3(void* arg)
{
	while (totalcount>0)
	{
		nanosleep((struct timespec[]){{0, rand() % 100000000}}, NULL);
		printf("Smoker three is waiting to smoke (has tobacco)\n");

		sem_wait(&smokers[2]);
		printf("Smoker three received matches and tobacco and is now making cigarette.\n");
		sem_post(&smoker_agent);
		totalcount-- ;
	}
	if(totalcount==0){
		exit(0) ;
	}
	return NULL;
}


// The main thread handles the agent's abritatin of items.

int main(int argc, char* arvg[])
{
	srand(time(NULL));

	// Initalize agent semaphore
	sem_init(&smoker_agent, 0, 1);

	// Setup our smoker threads and function references
	pthread_t smoker_threads[3];
	smokers_t smoker_functions[3] = { smoker_1, smoker_2, smoker_3 };

	// Create the smoker threads and initalize the semaphores
	for (int i = 0; i < 3; ++i)
	{
		sem_init(&smokers[i], 0, 0);

		if(pthread_create(&smokers[i], NULL, smoker_functions[i], NULL) == EAGAIN)
		{
			perror("Insufficent resources to create thread.\n");
			return 0;
		}
	}

	// Agent begins to distribute his items
	while (1)
	{
		// Lock the agent 
		sem_wait(&smoker_agent);

		// Distribute two items for one of the smokers
		sem_post(&smokers[rand() % 3]);
	}

	// Join all of the smoker threads on exit
	for (int i = 0; i < 3; ++i)
	{
		pthread_join(smoker_threads[i], NULL);
	}
}