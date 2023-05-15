#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>

int pipe(int fildes[2]); // 0 read 1 write
sem_t mutex[5];

int main(void)
{
	pid_t child1_pid1, child_pid2;
	int fd[2];
	FILE *pipe_file;
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(1);
	}

	// create mutex
	for (int i = 1; i <= 3; i++)
	{
		mutex[i] = *sem_open("my_sem", O_CREAT, S_IWUSR, 1);
		sem_post(&mutex[i]);
	}
	sem_wait(&mutex[1]);
	sem_wait(&mutex[2]);

	child1_pid1 = fork();
	if (child1_pid1 == 0) // child1 write
	{
		close(fd[0]);
		pipe_file = fdopen(fd[1], "w");
		fprintf(pipe_file, "Child process 1 is sending a message!\n");
		fclose(pipe_file);
		sem_post(&mutex[1]);
		exit(0);
	}
	child_pid2 = fork();
	if(child_pid2 == 0) // child2 write
	{
		sem_wait(&mutex[1]);
		close(fd[0]);
		pipe_file = fdopen(fd[1], "w");
		fprintf(pipe_file, "Child process 2 is sending a message!\n");
		fclose(pipe_file);
		sem_post(&mutex[2]);
		sem_post(&mutex[1]);
		exit(0);
	}
	
	// parent read
	close(fd[1]);
	sem_wait(&mutex[2]);
	pipe_file = fdopen(fd[0], "r");
	char buf[1024];
	fgets(buf, 1024, pipe_file);
	printf("%s", buf);
	fgets(buf, 1024, pipe_file);
	printf("%s", buf);
	fclose(pipe_file);
	sem_post(&mutex[2]);
	return 0;
}