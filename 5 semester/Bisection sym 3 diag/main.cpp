#include "matrix.h"
#include "getopt.h"
#include "MyException.h"

int main(int argc, char *argv[]) {
	try {
		if (argc == 1) {
			std::cout << "Help:\n -f formula (1 or 2)\n -s size of matrix\n -l left border of interval (integer)\n -r right border of interval (integer)\n -i input file\n -o output file\n -v debug\n -k format of output\n";
		}
		else {
			Params par = get_options(argc, argv);

			if (par.right - par.left < eps)
				throw MyException(10, "Incorrect borders!");

			Matrix A;
			double* values = NULL;

			if (par.formula && par.size) {
				A.init(par.size);
				if (strcmp("1", par.formula) == 0) {
					std::cout << "Test 1\n";
					A.test_1(par.size);
				}
				else if (strcmp("2", par.formula) == 0) {
					std::cout << "Test 2\n";
					A.test_2(par.size);
				}
				else throw MyException(8, "No formula");
			}
			else if (par.in_name) {
				std::cout << "Read from file\n";
				A.init(par.in_name);
			}
			else throw MyException(9, "Incorrect input");
			std::cout << "Matrix:\n";
			A.print(5);
			values = new double[A.getSize()];

			//timespec mt1, mt2;
			//clock_gettime(CLOCK_MONOTONIC, &mt1);
			int count_values = A.findEigenvalues(par.left, par.right, values, par.flag);
			//clock_gettime(CLOCK_MONOTONIC, &mt2);
			//double tt = (1e9*(mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec)) / 1e9;

			if (count_values > 0) {
				if (par.out_name) {
					std::ofstream fout;
					fout.open(par.out_name);
					fout.precision(4);
					if (par.k > 0 && par.k < count_values - 1) {
						for (int i = 0; i < par.k; i++)
							fout << values[i] << "\t";

						fout << "...\t" << values[count_values - 1];
						fout << "\n\n";
					}
					else {
						for (int i = 0; i < count_values; i++)
							fout << values[i] << "\t";
						fout << "\n\n";
					}
					fout.close();
				}
				else {
					std::cout << "Eigenvalues:\n";
					std::cout.precision(4);
					if (par.k > 0 && par.k < count_values - 1) {
						for (int i = 0; i < par.k; i++)
							std::cout << values[i] << "\t";

						std::cout << "...\t" << values[count_values - 1];
						std::cout << "\n\n";
					}
					else {
						for (int i = 0; i < count_values; i++)
							std::cout << values[i] << "\t";
						std::cout << "\n\n";
					}
				}
			}
			else
				std::cout << "Eigenvalues doesn't exist on this interval\n";


			//double err = norm(A, C);
			//std::cout << "Norm of residual: " << err << std::endl;
			//std::cout << "Time: " << tt << " sec" << std::endl;
			if (values) delete[] values;
		}
	}
	catch (MyException ex) {
		std::cout << ex.ID() << " - " << ex.Message() << std::endl;
	}
	return 0;
}
