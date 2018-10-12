#include <fstream>
#include <cstring>
#include <ctime>
#include <windows.h> 
#include "RBTree.h"
#include "RBTree.cpp"
#define SIZE 10000

int main() {

	std::ifstream file("requests.txt");
	if (!file) {
		srand(time(NULL));
		int* data = NULL;
		data = new int[SIZE];
		for (size_t i = 0; i < SIZE; i++) {
			Sleep(1);
			data[i] = rand() % 10000 + 1;
		}
		std::ofstream file("report.csv");
		file << "count" << "," << "time_add" << "," << "time_find" << "," << "time_del" << "\n";
		for (size_t i = 100; i <= 10000; i += 100) {
			RBTree<int> tree;
			double time_add = clock();
			for (size_t j = 0; j < i; j++) {
				tree.add(data[j]);
			}
			time_add = clock() - time_add;

			double time_find = clock();
			tree.search(data[i/10]);
			time_find = clock() - time_find;

			double time_del = clock();
			for (size_t j = 0; j < i; j++) {
				tree.del(tree.search(data[j]));
			}
			time_del = clock() - time_del;
			file << i << "," << time_add << "," << time_find << "," << time_del << "\n";
		}
		file.close();
		delete[] data;
	}
	else {
		RBTree<int> tree;
		char buf[255];
		size_t count_add = 0, count_del = 0;
		while (file >> buf) {
			if (strcmp(buf, "insert") == 0) {
				file >> buf;
				tree.add(atoi(buf));
				count_add++;
			}
			else if (strcmp(buf, "delete") == 0) {
				file >> buf;
				if (tree.search(atoi(buf))) {
					tree.del(tree.search(atoi(buf)));
					count_del++;
				}
				else std::cout << "Key " << buf << " not found\n";
			}
			else if (strcmp(buf, "find") == 0) {
				file >> buf;
				if (tree.search(atoi(buf))) {
					std::cout << "Finded node: ";
					tree.print(tree.search(atoi(buf)));
				}
				else std::cout << "Key " << buf << " not found\n";
			}
			else {
				std::cout << "Incorrect input\n";
				return -1;
			}

		}
		file.close();

		std::cout << "Succesfully added " << count_add << " nodes\n";
		std::cout << "Succesfully deleted " << count_del << " nodes\n";
		std::cout << std::endl;

		tree.print();
	}

	return 0;
}