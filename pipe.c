#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include "DataStructures.h"

/* Structs */
typedef struct Resources {// 0 = false, 1 = true
  int resource_A;
  int resource_B;
  int resource_C;
  int resource_D;
}Resources;

typedef struct Job {
  int number;
  struct tm *creation_date;
  int execution_time;
  int memory_requirement;
  Resources resources;
}Job;


/*functions prototype*/
void Generator();
void SchedulerJobs();
int Possibility();
int Random();
struct tm *getDate();
void handler();
void* Processor();
int checkAvaliableResources();
void reserveResources();

/* defined Constant*/
#define MAX_JOBS 100
int memory = 2000;
int PROCESSORS = 100;/*number of Processors avaliable*/
/*Intializes resources*/
Resources resourcesManager = {0,0,0,0};

/* declare Queue*/
Queue* JobsQueue;

/* set Handler*/
void handler(){
  exit(0);
}

void Generator(){
  int num, fd;
  mkfifo("/Users/Donat/csc320/project/pipe" , 0666);
  printf("waiting for readers...\n");
  fd = open("/Users/Donat/csc320/project", O_WRONLY);
  while(1){
    int i, id;
    int generates_maximum = Random(0,5);
    Job job;
    for (i = 0; i < generates_maximum; i++) {
      if(id < MAX_JOBS){
        job.number = id++;
        job.creation_date = getDate();
        job.execution_time = Random(10,1000);
        job.memory_requirement = Random(1,100);
        job.resources.resource_A = Possibility();
        job.resources.resource_B = Possibility();
        job.resources.resource_C = Possibility();
        job.resources.resource_D = Possibility();
      }else{
        if(id >= MAX_JOBS)
          exit(0);
      }
      //TODO write log here
      /* Send to Scheduler Jobs */
      // SchedulerJobs(job);
      if ((num = write(fd, &job, sizeof(job))) == -1)
        perror("cannot write in pipe");
      else
        printf("producer: wrote %d bytes\n", num);
    }
    sleep(1);
  }
}

void SchedulerJobs(){
  Job job;
  int num, fd;
  mkfifo ("/Users/Donat/csc320/project", 0666);
  fd = open("/Users/Donat/csc320/project", O_RDONLY);
  printf("got a writer\n");
  if ((num = read(fd, &job, sizeof(job))) == -1)
    perror("cannot read");
  else {
    printf("=====\n");
    printf("%d\n", job.number);
    printf("%d\n", job.execution_time);
    printf("%d\n", job.memory_requirement);
    printf("A=%d\tB=%d\tC=%d\tD=%d\n", job.resources.resource_A,job.resources.resource_B,job.resources.resource_C,job.resources.resource_D);
    printf("=====\n");
  }
  enqueue(JobsQueue, &job);
  Job j = *((Job*)dequeue(JobsQueue));
  printf("ID: %d\n", j.number);

}

int Possibility(){
  int possibility;
  possibility = Random(1,10);
  if(possibility == 1){
    return 1;
  }else{
    return 0;
  }
}

int Random(int min, int max){
  return (min + (rand() % (max - min + 1)));
}

struct tm *getDate(){
  time_t t;
  struct tm *date;
  time(&t);
  date = localtime(&t);
  return date;
}

int main(int argc, char *argv[]){

  /*set srand (seed) once to prevent repeate values in seconds*/
  time_t t;
  srand((unsigned) time(&t));

  /* start run program */
  Generator();

  return 0;
}
