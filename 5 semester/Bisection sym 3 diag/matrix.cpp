#include "matrix.h"
#include "MyException.h"

Matrix::Matrix(int _size) {
	size = _size;
	data = new double[size*size];
}

Matrix::~Matrix() {
	if (data) delete[] data;
	data = NULL;
	size = 0;
}

void Matrix::init(int _size) {
	size = _size;
	data = new double[size*size];
}

Matrix::Matrix(const char *name) {
	FILE *fin;
	fin = fopen(name, "r");
	if (!fin) throw MyException(1, "Can't open file!");
	else
	{
		if (fscanf(fin, "%d", &size) != 1)
		{
			fclose(fin);
			throw MyException(2, "File is empty!");
		}

		data = new double[size*size];
		double temp;

		for (int i = 0; i < size*size; i++)
		{
			if (fscanf(fin, "%lf", &temp) != 1)
			{
				delete[] data;
				data = NULL;
				fclose(fin);
				throw MyException(3, "Incorrect data!");
			}
			data[i] = temp;
		}
		fclose(fin);
	}
}

void Matrix::init(const char *name) {
	FILE *fin;
	fin = fopen(name, "r");
	if (!fin) throw MyException(1, "Can't open file!");
	else
	{
		if (fscanf(fin, "%d", &size) != 1)
		{
			fclose(fin);
			throw MyException(2, "File is empty!");
		}

		data = new double[size*size];
		double temp;

		for (int i = 0; i < size*size; i++)
		{
			if (fscanf(fin, "%lf", &temp) != 1)
			{
				delete[] data;
				data = NULL;
				fclose(fin);
				throw MyException(3, "Incorrect data!");
			}
			data[i] = temp;
		}
		fclose(fin);
	}
}

int Matrix::getSize() const {
	return size;
}

Matrix& Matrix::operator=(const Matrix &M) {
	size = M.getSize();
	data = new double[size*size];
	for (int i = 0; i < size*size; i++)
		data[i] = M.data[i];
	return *this;
}


Matrix Matrix::operator*(const Matrix &M) {
	Matrix prod;
	if (size != M.getSize()) throw MyException(4, "Size mismatch!");
	prod.size = size;
	prod.data = new double[size*size];
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			prod.data[i*size + j] = 0;
			for (int k = 0; k < size; k++)
				prod.data[i*size + j] += data[i*size + k] * M.data[k*size + j];
		}
	return prod;
}

void Matrix::diag() {

	for (int i = 1; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++) {
			double x = data[i * size + i - 1];
			double y = data[j * size + i - 1];

			if (abs(y) < eps)
				continue;

			double r = sqrt(x * x + y * y);

			if (r < eps)
				continue;

			double cos = x / r, sin = -y / r;

			data[i * size + i - 1] = r;
			data[(i - 1) * size + i] = r;
			data[j * size + i - 1] = 0.0;
			data[(i - 1) * size + j] = 0.0;

			for (int k = i + 1; k < size; k++) {
				if (k == j)
					continue;

				x = data[i * size + k];
				y = data[j * size + k];
				data[k * size + i] = x * cos - y * sin;
				data[i * size + k] = x * cos - y * sin;
				data[k * size + j] = x * sin + y * cos;
				data[j * size + k] = x * sin + y * cos;
			}

			double a = data[i * size + i], b = data[i * size + j], c = data[j * size + i], d = data[j * size + j];
			double b_ii = a * cos - b * sin;
			double b_ji = a * sin + b * cos;
			double b_ij = c * cos - d * sin;
			double b_jj = c * sin + d * cos;

			data[i * size + i] = b_ii * cos - b_ij * sin;
			data[j * size + i] = b_ii * sin + b_ij * cos;
			data[i * size + j] = data[j * size + i];
			data[j * size + j] = b_ji * sin + b_jj * cos;
		}
	}
}

int Matrix::alter(double eig_val) {
	//p. 143
	//step 1
	double x = data[0] - eig_val;
	double y = 1.0;
	int count = x < eps;

	//step 2
	for (int i = 1; i < size; i++)
    {
		//a
		double a = data[i * size + i] - eig_val;
		double b = data[i * size + i - 1];

        //b
        double max = fabs(b * b * y);

        if (fabs(x) > max)
            max = fabs(x);

		if (max < eps)
            max = 1e-15;

        double g = 1e5 / max;
		//c
		double u = g * (a * x - b * b * y);
		double v = g * x;

		//d
		if (u * x < eps)
			count++;
		//e
		x = u;
        y = v;
	}

	return count;
}

int Matrix::findEigenvalues(double left, double right, double* values, bool fl) {

	diag();
	if (fl) {
		std::cout << "Matrix diag:\n";
		print(5);
	}

    right += eps;
    left -= eps;

	int count = alter(right) - alter(left);

	if (count == 0)
		return 0;

	int i = 0;
	int beforeLeftBorderCount = alter(left);
	double currentLeft = left, currentRight = right, currentMiddle;

    while (i < count) {
		while (currentRight - currentLeft > eps) {
			currentMiddle = 0.5 * (currentLeft + currentRight);

			if (alter(currentMiddle) < i + 1 + beforeLeftBorderCount)
				currentLeft = currentMiddle;
			else
				currentRight = currentMiddle;
		}

		currentMiddle = 0.5 * (currentLeft + currentRight);
		int new_count = alter(currentRight) - alter(currentLeft);

		for (int j = 0; j < new_count; j++) {
			values[i + j] = currentMiddle;
			if (fl) {
				std::cout << "New eigenvalue:\t" << values[i + j] << "\n";
			}
		}

		i += new_count;

		currentLeft = currentMiddle;
		currentRight = right;
	}
	return count;
}

double norm(const Matrix &A, double *values, int _size){
    double res = 0;
    for (int i = 0; i < _size; i++)
        res += A.data[i*A.size + i] - values[i];
    return res;
}

double dist(const Matrix &A, double *values, int _size){
    double res1 = 0;
    for (int i = 0; i < A.size; i++)
        for (int j = 0; j < A.size; j++)
            res1 = res1 + (A.data[i*A.size + j])*(A.data[i*A.size + j]);
    double res2 = 0;
    for (int i = 0; i < _size; i++)
        res2 = res2 + values[i]*values[i];
    return fabs(sqrt(res1)-sqrt(res2));
}

void Matrix::test_1(int size) {
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-1; j++) {
            if (i == j) data[i * size + j] = 1;
            else data[i * size + j] = 0;
		}

        for (int j = 0; j < size; j++) {
            data[(size-1)*size + j] = j+1;
            data[size-1 + j*size] = j+1;
		}
	}
}

void Matrix::test_2(int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < i + 1; j++) {
			data[i*size + j] = size - i;
		}
		for (int j = i + 1; j < size; j++) {
			data[i*size + j] = size - j;
		}
	}
}

void Matrix::print(int key) const {
	std::cout.precision(4);
	if (key > 0 && key < size - 1) {

		for (int i = 0; i < key; i++) {
			for (int j = 0; j < key; j++)
				std::cout << data[i*size + j] << "\t";
			std::cout << "...\t" << data[i*size + size - 1];
			std::cout << "\n";
		}
		for (int j = 0; j < key + 2; j++)
			std::cout << "...\t";
		std::cout << "\n";
		for (int j = 0; j < key; j++)
			std::cout << data[(size - 1)*size + j] << "\t";
		std::cout << "...\t" << data[size*size - 1];
		std::cout << "\n\n";
	}
	else {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++)
				std::cout << data[i*size + j] << "\t";
			std::cout << "\n";
		}
		std::cout << "\n\n";
	}
}

void Matrix::print(int key, const char* name) const {
	std::ofstream fout;
	fout.open(name);
	fout.precision(4);
	if (key > 0 && key < size - 1) {

		for (int i = 0; i < key; i++) {
			for (int j = 0; j < key; j++)
				fout << data[i*size + j] << "\t";
			fout << "...\t" << data[i*size + size - 1];
			fout << "\n";
		}
		for (int j = 0; j < key + 2; j++)
			fout << "...\t";
		fout << "\n";
		for (int j = 0; j < key; j++)
			fout << data[(size - 1)*size + j] << "\t";
		fout << "...\t" << data[size*size - 1];
		fout << "\n\n";
	}
	else {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++)
				fout << data[i*size + j] << "\t";
			fout << "\n";
		}
		fout << "\n\n";
	}
	fout.close();
}
