#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> 
#include <set>
#include <map>
#include <stdlib.h>
#include <random>
#include <math.h>
#include <cmath>
#include <limits>

int ArgPos(std::string str, int argc, char **argv) {
	int a;
	for (a = 1; a < argc; a++){
		if (str == argv[a]) {
			if (a == argc - 1) {
				std::cout << "Argument missing for" << str << std::endl;
				exit(1);
			}
			return a;
		}
	}
	return -1;
}

int main(int argc, char **argv){
	std::string file_name;
	std::string rev_filename;
	int i;
	if ((i = ArgPos((char *)"-file_name", argc, argv)) > 0) file_name = argv[i + 1];
	if ((i = ArgPos((char *)"-rev_filename", argc, argv)) > 0) rev_filename = argv[i + 1];

	std::ifstream fin;
	std::ofstream fout;

	fin.open(file_name);
	fout.open(rev_filename);

	std::string word;
	std::vector<std::string> tmp;
	while (getline(fin, word, '\t')) {
		tmp.push_back(word);
		if ((int)tmp.size() == 3) {
			fout << tmp[2] << '\t' << "**" + tmp[1] << '\t' << tmp[0] << std::endl;
			tmp.clear();
		}
	}

	fin.close();
	fout.close();
	return 0;
}
