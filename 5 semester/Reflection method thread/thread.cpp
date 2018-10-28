#include "thread.h"
#include "matrix.h"

void *inv_t(void *pa)
{
	Threads *args = (Threads*)pa;

	//timespec mt1, mt2;
	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, CLOCK_THREAD_CPUTIME_ID, &mt1);
	inverse(args->a, args->b, args->size, args->my_rank, args->threads, args->flag);
	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, CLOCK_THREAD_CPUTIME_ID, &mt2);

	pthread_mutex_lock(&total_mutex);
	//args->time_t = (1e9*(mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec)) / 1e9;
	pthread_mutex_unlock(&total_mutex);
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