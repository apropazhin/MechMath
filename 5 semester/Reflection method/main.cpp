#include "matrix.h"
#include "getopt.h"
#include "MyException.h"

int main(int argc, char *argv[]) {
	try {
		if (argc == 1) {
			std::cout << "Help:\n -f formula (1 or 2)\n -s size of matrix\n -i input file\n -o output file\n -v debug\n -k format of output\n";
		}
		else {
			Params par = get_options(argc, argv);
			Matrix A, B, C;
			if (par.formula && par.size) {
                A.init(par.size);
				if (strcmp("1", par.formula) == 0) {
					std::cout << "Test 1\n";
					A.test_1(par.size);
				}
                else if (strcmp("2", par.formula) == 0) {
					std::cout << "Test 2\n";
					A.test_2(par.size);
                } else throw MyException(8, "No formula");
                C.init(par.size);
				B = A;
			}
            else if (par.in_name) {
                std::cout << "Read from file\n";
				A.init(par.in_name);
				B = A;
				C.init(A.getSize());
            } else throw MyException(9, "Incorrect input");
            std::cout << "Matrix:\n";
            A.print(5);

			timespec mt1, mt2;
			clock_gettime(CLOCK_MONOTONIC, &mt1);
			C = B.inverse(C, par.flag);
			clock_gettime(CLOCK_MONOTONIC, &mt2);
			double tt = (1e9*(mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec)) / 1e9;

			if (par.out_name) {
				C.print(par.k, par.out_name);
			}
            else {
                std::cout << "Inverse matrix:\n";
                C.print(par.k);
            }
			double err = norm(A, C);
			std::cout << "Norm of residual: " << err << std::endl;
            std::cout << "Time: " << tt << " sec" << std::endl;
		}
	}
	catch (MyException ex) {
		std::cout << ex.ID() << " - " << ex.Message() << std::endl;
	}
	return 0;
}
