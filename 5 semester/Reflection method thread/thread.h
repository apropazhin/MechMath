#pragma once
#include <pthread.h>
#include "matrix.h"

class Threads {
public:
	Matrix A;
	Matrix B;
	int my_rank;
	int threads;
	bool flag;
};

void *inv_t(void*);
void synchronize(int);

pthread_mutex_t total_mutex = PTHREAD_MUTEX_INITIALIZER;