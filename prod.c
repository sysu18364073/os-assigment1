#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t *empty;
sem_t *full;

int shm_fd;
const int BUFFER_SIZE = 20;
const char *name = "OS";
int *ptr = NULL;
int in = 0;     


void *producer(void *param){
     
     int item;
     int count = 0;
     int t;
     while(1){
          t = exp(count);
          sleep(t/100);
          count++;
          item = rand()%20 + 1;
          
          sem_wait(empty);
          pthread_mutex_lock(&mutex);
          
          ptr[in] = item;
          in = (in + 1) % BUFFER_SIZE;

          printf("producer pid %d tid %lu produced %d\n", getpid(), pthread_self(), item);

          pthread_mutex_unlock(&mutex);
          sem_post(full);

     }

}


int main(int argc, char *argv[]){

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, BUFFER_SIZE);
    ptr = mmap(0, BUFFER_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    empty = sem_open("EMPTY", O_CREAT, 0666, 20);
    full = sem_open("FULL", O_CREAT, 0666, 20);
    pthread_mutex_init(&mutex, NULL);
    
    pthread_t prod1,prod2,prod3;
    pthread_create(&prod1, NULL, producer, NULL);
    pthread_create(&prod2, NULL, producer, NULL);
    pthread_create(&prod3, NULL, producer, NULL);
    
    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(prod3, NULL);
    
    sem_destroy(empty);
    sem_destroy(full);
    pthread_mutex_destroy(&mutex);
    return 0;
}
