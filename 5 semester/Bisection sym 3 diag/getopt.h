#pragma once
#include <iostream>
#include <stdlib.h>
#include <string.h>

class Params {
public:
	int k;
	int size;
	double left;
	double right;
	char *formula;
	char *in_name;
	char *out_name;
	bool flag;
	Params() : k(0), size(0), left(0), right(0), formula(NULL), in_name(NULL), out_name(NULL), flag(false) {}
	void print();
};

Params get_options(int argc, char *argv[]);