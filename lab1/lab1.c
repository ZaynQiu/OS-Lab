#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex[10];
pthread_t philosopher_t[10];
int philosopher_t_id[10];
long long philosopher_times[10];

void *philosopher(int i)
{
	while(1)
	{
		// think ...
		printf("Philosopher %d is thinking\n", i);
		// get mutex
		if (i % 2 == 0)
		{
			pthread_mutex_lock(&mutex[i]);
			pthread_mutex_lock(&mutex[i - 1]);
		}
		else
		{
			if (i - 1 == 0)
				pthread_mutex_lock(&mutex[5]);
			else
				pthread_mutex_lock(&mutex[i - 1]);
			pthread_mutex_lock(&mutex[i]);
		}
		// eat ...
		philosopher_times[i]++;
		if(philosopher_times[i] > 1000)	
			philosopher_times[i] = 0;
		printf("Philosopher %d is eating, times:%d\n", i, philosopher_times[i]);
		// release mutex
		pthread_mutex_unlock(&mutex[i]);
		if (i - 1 == 0)
			pthread_mutex_unlock(&mutex[5]);
		else
			pthread_mutex_unlock(&mutex[i - 1]);
	}
}

int main(void)
{
	// create mutex
	for (int i = 1; i <= 5; i++)
	{
		pthread_mutex_init(&mutex[i], NULL);
	}
	// create thread
	for(int i=1; i<=5; i++)
	{
		philosopher_times[i] = 0;
		printf("Philosopher %d is created\n", i);
		pthread_create(&philosopher_t[i], NULL, (void *)philosopher, i);
	}
	printf("yes\n");
	// join thread
	for(int i=1; i<=5; i++)
	{
		pthread_join(philosopher_t[i], NULL);
	}
	return 0;
}