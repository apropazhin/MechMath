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

	for (int i = 1; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			double x = data[i * size + i - 1];
			double y = data[j * size + i - 1];

			if (abs(y) < eps)
				continue;

			double r = sqrt(x * x + y * y);

			if (r < eps)
				continue;

			double cosPhi = x / r;
			double sinPhi = -y / r;

			data[i * size + i - 1] = r;
			data[(i - 1) * size + i] = r;
			data[j * size + i - 1] = 0.0;
			data[(i - 1) * size + j] = 0.0;

			for (int k = i + 1; k < size; k++)
			{
				if (k == j)
					continue;

				x = data[i * size + k];
				y = data[j * size + k];
				data[k * size + i] = data[i * size + k] = x * cosPhi - y * sinPhi;
				data[k * size + j] = data[j * size + k] = x * sinPhi + y * cosPhi;
			}

			x = data[i * size + i];
			y = data[j * size + j];
			r = data[i * size + j];
			double s = data[j * size + i];
			double a_ii = x * cosPhi - s * sinPhi;
			double a_ji = x * sinPhi + s * cosPhi;
			double a_ij = r * cosPhi - y * sinPhi;
			double a_jj = r * sinPhi + y * cosPhi;

			data[i * size + i] = a_ii * cosPhi - a_ij * sinPhi;
			data[j * size + i] = a_ii * sinPhi + a_ij * cosPhi;
			data[i * size + j] = data[j * size + i];
			data[j * size + j] = a_ji * sinPhi + a_jj * cosPhi;
		}
	}
}

int Matrix::alter(double eig_val) {

	double x = data[0 * size + 0] - eig_val;
	double y = 1.0;
	int count = x < eps;

	for (int i = 1; i < size; i++)
	{
		double a_k = data[i * size + i] - eig_val;
		double b_k1 = data[i * size + i - 1];

		double tmp = abs(b_k1 * b_k1 * y);

		if (abs(x) > tmp)
			tmp = abs(x);

		if (tmp < eps)
			tmp = 1e-15;

		double gamma = 1e15 / tmp;
		double u = gamma * (a_k * x - b_k1 * b_k1 * y);
		double v = gamma * x;

		if (u * x < eps)
			count++;

		x = u;
		y = v;
	}

	return count;
}

int Matrix::findEigenvalues(double left, double right, double* values, bool fl) {
	
	diag();
	right += eps;
	left -= eps;

	int count = alter(right) - alter(left);

	if (count == 0)
		return 0;

	int i = 0;
	int beforeLeftBorderCount = alter(left);
	double currentLeft = left, currentRight = right, currentMiddle;

	while (i < count)
	{
		while (currentRight - currentLeft > eps)
		{
			currentMiddle = 0.5 * (currentLeft + currentRight);

			if (alter(currentMiddle) < i + 1 + beforeLeftBorderCount)
				currentLeft = currentMiddle;
			else
				currentRight = currentMiddle;
		}

		currentMiddle = 0.5 * (currentLeft + currentRight);
		int new_count = alter(currentRight) - alter(currentLeft);

		for (int j = 0; j < new_count; j++)
			values[i + j] = currentMiddle;

		i += new_count;

		currentLeft = currentMiddle;
		currentRight = right;
	}
	return count;
}

void Matrix::test_1(int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < i; j++) {
			data[i*size + j] = 0;
		}
		data[i*size + i] = 1;
		for (int j = i + 1; j < size; j++) {
			data[i*size + j] = -1;
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
