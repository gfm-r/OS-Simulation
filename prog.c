#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
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

/* defined Constant*/
#define MAX_JOBS 5
char memoryManager[2000];
Resources resourcesManager;

/* declare Queue*/
Queue* JobsQueue;

/* set Handler*/
void handler(){
  Job* job;
  job = (Job*) dequeue(JobsQueue);
  if(job)
    printf("%d\n", job->number);
  exit(0);
}

void Generator(){
  while(1){
    int i, id;
    int generates_maximum = Random(0,5);
    Job job;
    for (i = 0; i < generates_maximum; i++) {
      if(id < MAX_JOBS){
        job.number = id++;
      }
      job.creation_date = getDate();
      job.execution_time = Random(10,1000);
      job.memory_requirement = Random(1,100);
      job.resources.resource_A = Possibility();
      job.resources.resource_B = Possibility();
      job.resources.resource_C = Possibility();
      job.resources.resource_D = Possibility();
      SchedulerJobs(job);
    }
    sleep(1);
  }
}
void SchedulerJobs(Job job){

  printf("=====\n");
  printf("%d\n", job.number);
  printf("%d\n", job.execution_time);
  printf("%d\n", job.memory_requirement);

  printf("A=%d\tB=%d\tC=%d\tD=%d\n", job.resources.resource_A,job.resources.resource_B,job.resources.resource_C,job.resources.resource_D);

  printf("=====\n");

  /*enqueue(JobsQueue, &job);
   ((Job)dequeue(JobsQueue))*/

}
void MemoryManager(Job job){
  if(job.memory_requirement <= sizeof(memoryManager)){
    memoryManager -= job.memory_requirement;
  }
}
void ResourceManager(){

}
void Processors(){

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
void writeLog(const char * filename){

}
int main(int argc, char *argv[]){

  /*set srand (seed) once to prevent repeate values in seconds*/
  time_t t;
  srand((unsigned) time(&t));

  /* Intializes jobs queue for SchedulerJobs*/
  JobsQueue = new_queue();

  /*set sigmal to force quit programm (try without it! and press CTRL+C)*/
  signal(SIGINT,handler);
  printf("%lu\n", sizeof(memory));
  /* start run program */
  Generator();

  return 0;
}
