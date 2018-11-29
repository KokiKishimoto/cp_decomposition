#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <random>
#include <set>
#include <string>
#include <vector>

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
	std::string name;
	int i;
	if ((i = ArgPos((char *)"-name", argc, argv)) > 0) name = argv[i + 1];
	std::cout << name << std::endl;

	if(name == "CP"){
		std::cout << "CP" << std::endl;
	}else if(name == "quantizeCP"){
		std::cout << "quantizeCP" << std::endl;
	}else if(name == "booleanCP"){
		std::cout << "quantizeCP" << std::endl;
	}else{
		std::cout << "error" << std::endl;
	}
}

