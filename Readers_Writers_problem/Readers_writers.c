//Abhishek Rathod B.Tech IIT Roorkee CSE
// Enroll. No.- 17114004

#include<stdio.h>
#include<pthread.h>
#include"my_semaphore.h"
#include<unistd.h>

sem_t mutex,writebuffer;
int data = 0,read_count = 0;

void *reader(void *arg)
{
  int var;
  var = ((int) arg);
  sem_wait(&mutex);
  read_count = read_count + 1;
  if(read_count==1)
   sem_wait(&writebuffer);
  sem_post(&mutex);
  printf("Data is read by the reader number %d is %d\n",var,data);
  sleep(1);
  sem_wait(&mutex);
  read_count = read_count - 1;
  if(read_count==0)
   sem_post(&writebuffer);
  sem_post(&mutex);
}

void *writer(void *arg)
{
  int var;
  var = ((int) arg);
  sem_wait(&writebuffer);
  data++;
  printf("Data is written by the writer number %d is %d\n",var,data);
  sleep(1);
  sem_post(&writebuffer);
}

int main()
{
  int i,b; 
  pthread_t reader_id[100],writer_id[100];
  sem_init(&mutex,0,1);
  sem_init(&writebuffer,0,1);
  int number_of_readers=0;
  int number_of_writers=0;

  printf("Enter the number of readers(At max 100) : ") ;
  scanf("%d",&number_of_readers) ;

  printf("Enter the number of writers(At max 100) : ") ;
  scanf("%d",&number_of_writers) ;

  for(i=0;i<number_of_writers;i++)
  {
    pthread_create(&writer_id[i],NULL,writer,(void *)i);
  }
  for(i=0;i<number_of_readers;i++)
  {
    pthread_create(&reader_id[i],NULL,reader,(void *)i);
  }
  for(i=0;i<number_of_writers;i++)
  {
    pthread_join(writer_id[i],NULL);
  }
  for(i=0;i<number_of_readers;i++)
  {
    pthread_join(reader_id[i],NULL);
  }
  
  return 0;
}