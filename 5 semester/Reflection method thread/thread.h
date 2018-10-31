#pragma once
#include <pthread.h>
#include <stdlib.h>

class Threads {
public:
	double* a;
	double* b;
	int size;
	int my_rank;
	int threads;
	double time_t;
	double norm;
	bool flag;
	Threads() :
		a(NULL), b(NULL), size(0), my_rank(0), threads(0), time_t(0), norm(0), flag(false) {}
};

void *inv_t(void*);
void synchronize(int);
