#include "thread.h"

void *inv_t(void *pa)
{
	Threads *args = (Threads*)pa;
	//long int t1;

	//t1 = get_time();
	args->B = args->A.inverse(args->B, args->my_rank, args->threads, args->flag);
	//InvMatrix(arg->n, arg->a, arg->x, arg->my_rank, arg->total_threads);
	//t1 = get_time() - t1;

	pthread_mutex_lock(&total_mutex);
	//thread_time += t1;
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