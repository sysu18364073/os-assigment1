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

const int BUFFER_SIZE = 20;
const char *name = "OS";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t *empty;
sem_t *full;

int shm_fd;
void *ptr = NULL;
int out = 0;

void *consumer(void *param){
     int count = 0;
     int t;
     while(1){
          t = exp(count);
          sleep(t/100);
          count++;
          
          sem_wait(full);
          pthread_mutex_lock(&mutex);
          
          printf("consumer pid %d tid %lu consumed %d\n", getpid(), pthread_self(), *((int*)ptr+out));      
          out = (out + 1) % BUFFER_SIZE;
          
          pthread_mutex_unlock(&mutex);
          sem_post(empty);

     }

}


int main(int argc, char *argv[]){
    
    empty = sem_open("EMPTY", O_CREAT, 0666, 20);
    full = sem_open("FULL", O_CREAT, 0666, 20);
    pthread_mutex_init(&mutex, NULL);

    shm_fd = shm_open(name, O_RDONLY, 0666);
    ptr = mmap(0, BUFFER_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    pthread_t cons1,cons2,cons3;
    pthread_create(&cons1, NULL, consumer, NULL);
    pthread_create(&cons2, NULL, consumer, NULL);
    pthread_create(&cons3, NULL, consumer, NULL);
 
    pthread_join(cons1, NULL);
    pthread_join(cons2, NULL);
    pthread_join(cons3, NULL);

    sem_destroy(empty);
    sem_destroy(full);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
