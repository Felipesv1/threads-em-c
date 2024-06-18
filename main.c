#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// void	*routine(void)
// {
// 	printf("test from threads\n");
// 	sleep(3);
// 	printf("Ending thread\n");
// }

// int	main(int ac, char **av)
// {
// 	pthread_t t1, t2;
// 	if (pthread_create(&t1, NULL, &routine, NULL) != 0)
// 		return (1);
// 	if (pthread_create(&t2, NULL, &routine, NULL) != 0)
// 		return (2);
// 	if(pthread_join(t1, NULL) != 0)
// 		return (3);
// 	if(pthread_join(t2, NULL) != 0)
// 		return (4);
// 	return (0);
// }

// race conditions ----------------------------------------------------

// int mails = 0;
// void *routine()
// {
// 	for (int i = 0; i < 100000000; i++)
// 		mails++;
// }

// int main(int ac, char **av)
// {
// 	pthread_t t1, t2;
// 	if (pthread_create(&t1, NULL, &routine, NULL) != 0)
// 		return (1);
// 	if (pthread_create(&t2, NULL, &routine, NULL) != 0)
// 		return (2);
// 	if (pthread_join(t1, NULL) != 0)
// 		return (3);
// 	if (pthread_join(t2, NULL) != 0)
// 		return (4);
// 	printf("NUmber of mails:%d\n", mails);
// 	return (0);
// }

// MUTEX -------------------------------------------------------

// int mails = 0;
// pthread_mutex_t mutex;
// void *routine()
// {
// 	for (int i = 0; i < 1000000; i++)
// 	{
// 		pthread_mutex_lock(&mutex);
// 		mails++;
// 		pthread_mutex_unlock(&mutex);
// 		//read mails
// 		//increment
// 		//write mails
// 	}
// }

// int main(int ac, char **av)
// {
// 	pthread_t t1, t2;
// 	pthread_mutex_init(&mutex, NULL);
// 	if (pthread_create(&t1, NULL, &routine, NULL) != 0)
// 		return (1);
// 	if (pthread_create(&t2, NULL, &routine, NULL) != 0)
// 		return (2);
// 	if (pthread_join(t1, NULL) != 0)
// 		return (3);
// 	if (pthread_join(t2, NULL) != 0)
// 		return (4);
// 	pthread_mutex_destroy(&mutex);
// 	printf("NUmber of mails:%d\n", mails);
// 	return (0);
// }

// pthreads_create in loop ---------------------------------------

// int mails = 0;
// pthread_mutex_t mutex;
// void *routine()
// {
// 	for (int i = 0; i < 1000000; i++)
// 	{
// 		pthread_mutex_lock(&mutex);
// 		mails++;
// 		pthread_mutex_unlock(&mutex);
// 		// read mails
// 		// increment
// 		// write mails
// 	}
// }

// int main(int ac, char **av)
// {
// 	pthread_t th[4];
// 	int i;
// 	pthread_mutex_init(&mutex, NULL);
// 	for (i = 0; i < 4; i++)
// 	{
// 		if (pthread_create(th + i, NULL, &routine, NULL) != 0)
// 		{
// 			perror("Failed to create thread");
// 			return (1);
// 		}
// 		printf("thread %d has started\n", i);
// 	}
// 	for (i = 0; i < 4; i++)
// 	{
// 		if (pthread_join(th[i], NULL) != 0)
// 			return (2);
// 		printf("thread %d has finished execution\n", i);
// 	}
// 	pthread_mutex_destroy(&mutex);
// 	printf("NUmber of mails:%d\n", mails);
// 	return (0);
// }

// Deadlocks ----------------------------------------------------------

#define THREAD_NUM 8

pthread_mutex_t	mutexFuel;
int				fuel = 50;
pthread_mutex_t	mutexWater;

int				water = 50;
void	*routine()
{
	// if (rand() % 2 == 0)
	// {
		pthread_mutex_lock(&mutexFuel);
		// sleep(1); //ocorre um deadlock
		pthread_mutex_lock(&mutexWater);
	// }
	fuel += 50;
	water = fuel;
	printf("incremented fuel to: %d set water to %d\n", fuel, water);
	pthread_mutex_unlock(&mutexFuel);
	pthread_mutex_unlock(&mutexWater);
}

int	main(int ac, char **av)
{
	pthread_t	th[THREAD_NUM];
	int			i;

	pthread_mutex_init(&mutexFuel, NULL);
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_create(th + i, NULL, &routine, NULL) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
	}
	for (i = 0; i < THREAD_NUM; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
			return (2);
	}
	printf("Fuel: %d\n", fuel);
	pthread_mutex_destroy(&mutexFuel);
	pthread_mutex_destroy(&mutexWater);
	return (0);
}
