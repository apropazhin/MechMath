#include "getopt.h"
#include "MyException.h"
#include <unistd.h>

Params get_options(int argc, char *argv[]) {
	int i;
	Params par;
	while ((i = getopt(argc, argv, "k:s:f:i:o:v")) != -1) {
		switch ((char)i) {
		case 'k': {
			for (size_t j = 0; j < strlen(optarg); j++)
				if (!isdigit(optarg[j]))
					throw MyException(5, "Incorrect format of key!");

			char *next;
			par.k = strtol(optarg, &next, 10);
			break;
		}
		case 's': {
			for (size_t j = 0; j < strlen(optarg); j++)
				if (!isdigit(optarg[j]))
					throw MyException(5, "Incorrect format of key!");

			char *next;
			par.size = strtol(optarg, &next, 10);
			break;
		}
		case 'f': {
			par.formula = optarg;
			break;
		}
		case 'i': {
			par.in_name = optarg;
			break;
		}
		case 'o': {
			par.out_name = optarg;
			break;
		}
		case 'v': {
			par.flag = true;
			break;
		}

		}
	}
	return par;
}

void Params::print()
{
	std::cout << k << "\n" << formula << "\n" << in_name << "\n" << out_name << "\n" << flag << "\n";
}
