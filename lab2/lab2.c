#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


pthread_t consumer_t[10], producer_t[10];
sem_t empty, full, mutex;

int in, out, buffer_pool[4], data;
FILE *in_file ;

void *producer(int i)
{
	while (1)
	{
		sem_wait(&empty);
		sem_wait(&mutex);

		if(fscanf(in_file, "%d\n", &data) == EOF)
		{
			fseek(in_file, 0, SEEK_SET);
			fscanf(in_file, "%d\n", &data);
		}
		buffer_pool[in] = data;
		in = (in + 1) % 4;
		printf("producer %d produces %d\n", i, data);

		sem_post(&mutex);
		sem_post(&full);
		sleep(1);
	}
}

void *consumer(int i)
{
	while (1)
	{
		sem_wait(&full);
		sem_wait(&mutex);

		data = buffer_pool[out];
		out = (out + 1) % 4;
		printf("consumer %d consumes %d\n", i, data);

		sem_post(&mutex);
		sem_post(&empty);
		sleep(1);
	}
}

int main(void)
{
	in_file = fopen("lab2_data.txt", "r"); // read only
	// create mutex
	sem_init(&empty, 0, 4);
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);

	// create thread
	for (int i = 1; i <= 4; i++)
	{
		void *ptr = (void *)(long)i;
		pthread_create(&consumer_t[i], NULL, (void *)consumer, (void *)ptr);
		printf("consumer %d is created\n", i);
	}

	for (int i = 1; i <= 3; i++)
	{
		void *ptr = (void *)(long)i;
		pthread_create(&producer_t[i], NULL, (void *)producer, ptr);
		printf("producer %d is created \n", i);
	}

	// join thread
	for (int i = 1; i <= 4; i++)
	{
		pthread_join(consumer_t[i], NULL);
	}
	for (int i = 1; i <= 3; i++)
	{
		pthread_join(producer_t[i], NULL);
	}
	fclose(in_file);
	return 0;
}