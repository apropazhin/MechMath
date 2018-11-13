#include "thread.h"
#include "matrix.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *inv_t(void *pa)
{
	Threads *args = (Threads*)pa;

	//timespec mt1, mt2, mt3, mt4;
	//clock_gettime(CLOCK_THREAD_CPUTIME_ID, &mt1);
	inverse(args->a, args->b, args->size, args->my_rank, args->threads, args->flag);
	//clock_gettime(CLOCK_THREAD_CPUTIME_ID, &mt2);

	//clock_gettime(CLOCK_THREAD_CPUTIME_ID, &mt3);
	args->norm = norm(args->c, args->b, args->size, args->my_rank, args->threads);
	//clock_gettime(CLOCK_THREAD_CPUTIME_ID, &mt4);
	
	pthread_mutex_lock(&mutex);
	//args->time_t = (1e9*(mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec)) / 1e9;
	//args->time_n = (1e9*(mt4.tv_sec - mt3.tv_sec) + (mt4.tv_nsec - mt3.tv_nsec)) / 1e9;
	pthread_mutex_unlock(&mutex);

	return NULL;
}

void synchronize(int total_threads)
{
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	static pthread_cond_t condvar_in = PTHREAD_COND_INITIALIZER;
	static pthread_cond_t condvar_out = PTHREAD_COND_INITIALIZER;
	static int threads_in = 0;
	static int threads_out = 0;

	pthread_mutex_lock(&mutex);

	threads_in++;
	if (threads_in >= total_threads)
	{
		threads_out = 0;
		pthread_cond_broadcast(&condvar_in);
	}
	else
		while (threads_in < total_threads)
			pthread_cond_wait(&condvar_in, &mutex);

	threads_out++;
	if (threads_out >= total_threads)
	{
		threads_in = 0;
		pthread_cond_broadcast(&condvar_out);
	}
	else
		while (threads_out < total_threads)
			pthread_cond_wait(&condvar_out, &mutex);

	pthread_mutex_unlock(&mutex);
}
