#include "matrix.h"
#include "MyException.h"
#define eps 1e-10

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

Matrix Matrix::inverse(Matrix inv, bool fl) {
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (i == j) inv.data[i * size + j] = 1;
			else inv.data[i * size + j] = 0;
	if (fl) {
		std::cout << "inv:\n";
		inv.print(5);
	}

	for (int i = 0; i < size; i++)
	{
		double tmp1 = 0.0;
		for (int j = i + 1; j < size; j++)
			tmp1 += data[j * size + i] * data[j * size + i]; //(12)

		double tmp2 = sqrt(tmp1 + data[i * size + i] * data[i * size + i]); //(13)

        if (tmp2 < eps)
			throw MyException(7, "Can't invert - matrix is deteriorated!");

		data[i * size + i] -= tmp2; //(14)

		if (fl) {
            std::cout << "1. A:\ni = "<< i << "\n";
			print(5);
		}

		tmp1 = sqrt(tmp1 + data[i * size + i] * data[i * size + i]); //(15)

		if (tmp1 < eps)
		{
			data[i * size + i] += tmp2;
			continue;
		}

		for (int j = i; j < size; j++)
            data[j * size + i] = data[j * size + i]/(double)tmp1; //(16)

		if (fl) {
            std::cout << "2. A:\ni = "<< i << "\n";
			print(5);
		}

		for (int k = i + 1; k < size; k++)
		{
			tmp1 = 0.0;
			for (int j = i; j < size; j++)
				tmp1 += data[j * size + i] * data[j * size + k];

			tmp1 *= 2.0; //from formula
			for (int j = i; j < size; j++)
				data[j * size + k] -= tmp1 * data[j * size + i];
		}
		if (fl) {
            std::cout << "3. A:\ni = "<< i << "\n";
			print(5);
		}

		for (int k = 0; k < size; k++)
		{
			tmp1 = 0.0;
			for (int j = i; j < size; j++)
				tmp1 += data[j * size + i] * inv.data[k * size + j];

			tmp1 *= 2.0; //from formula
			for (int j = i; j < size; j++)
				inv.data[k * size + j] -= tmp1 * data[j * size + i];
		}
		if (fl) {
            std::cout << "4. inv:\ni = "<< i << "\n";
			inv.print(5);
		}

		data[i * size + i] = tmp2; //on diagonal - R
	}

	if (fl) {
        std::cout << "0. A:\n";
		print(5);
        std::cout << "0. inv:\n";
		inv.print(5);
	}

	for (int i = 0; i < size; ++i)
		for (int j = i + 1; j < size; j++)
		{
			double tmp1 = inv.data[i * size + j];
			inv.data[i * size + j] = inv.data[j * size + i];
			inv.data[j * size + i] = tmp1;
		}//Transpose Q

	if (fl) {
		std::cout << "A:\n";
		print(5);
		std::cout << "inv:\n";
		inv.print(5);
	}

	for (int k = 0; k < size; k++)
		for (int i = size - 1; i >= 0; i--)
		{
			double tmp1 = inv.data[i * size + k];
			for (int j = i + 1; j < size; j++)
				tmp1 -= data[i * size + j] * inv.data[j * size + k];
			inv.data[i * size + k] = tmp1 / data[i * size + i];
		}//Reverse Gauss

	if (fl) {
		std::cout << "0. A:\n";
		print(5);
		std::cout << "0. inv:\n";
		inv.print(5);
	}

	return inv;
}

double norm(Matrix A, Matrix inv) {
	double err = 0;
	Matrix E = A * inv;
	for (int i = 0; i < A.size; i++)
		err += E.data[i*E.size + i] - 1;
	return err;
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
