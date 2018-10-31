#pragma once
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <math.h>

class Matrix {
private:
	int size;
	double *data;
public:
	Matrix() : size(0), data(NULL) {}
	Matrix(const char* name);
	Matrix(int _size);
	~Matrix();
	Matrix& operator=(const Matrix&);
	Matrix operator*(const Matrix&);
	int getSize() const;
	double* getData() const;
	void init(int);
	void init(const char*);
	void print(int key) const;
	void print(int key, const char* name) const;
	//Matrix inverse(Matrix, int, int, bool);
	friend double norm(Matrix, Matrix);
	void test_1(int);
	void test_2(int);
};

void inverse(double*, double*, int, int, int, bool);
double norm(double*, double*, int, int, int);