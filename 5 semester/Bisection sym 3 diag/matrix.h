#pragma once
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <math.h>

#define eps 1e-10

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
	void init(int);
	void init(const char*);
	void print(int key) const;
	void print(int key, const char* name) const;
	void diag();
	int alter(double);
	int findEigenvalues(double, double, double*, bool);
	friend double norm(const Matrix &A, double *values, int _size);
	friend double dist(const Matrix &A, double *values, int _size);
	void test_1(int);
	void test_2(int);
};