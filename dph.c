#include<stdio.h>
#include<pthread.h>
 
int chops = 5;
 
struct timespec timeout;
 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockp = PTHREAD_MUTEX_INITIALIZER;
 
void *pcon(void *num)
{
	int flag = 1;
	while(1)
	{
		flag =(flag ? 0:1);
		if(!flag)
		{
			pthread_mutex_lock(&lock);
			if(chops > 0)
			{
				chops--;
				printf("philosopher %d is thinking\n",(int)num);
			}
			pthread_mutex_unlock(&lock);
		}
		else
		{
			pthread_mutex_lock(&lock);
			if(pthread_cond_timedwait(&cond, &lock, &timeout) == 0)
			{
				if(chops > 0)
				{
					printf("philpsopher %d is eating\n",(int)num); 
					sleep(rand()%5);
					chops++;
				}
			}
			else 
			{
				pthread_mutex_lock(&lockp);
				chops++;
				pthread_mutex_unlock(&lockp);
			}
			pthread_mutex_unlock(&lock);
			pthread_cond_broadcast(&cond);
		}
		usleep(100);
	}
}
 
int main(void)
{
	pthread_t tid[5];
	int i;
	timeout.tv_sec =rand()%3+1 ;
	timeout.tv_nsec = 0;
	for(i = 0; i < 5; i++)
		pthread_create(&tid[i], NULL, pcon,(void*)i);
	for(i = 0; i < 5; i++)
		pthread_join(tid[i], NULL);
 
	return 0;
}
