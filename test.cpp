#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>
#include <iterator>
#include <random>
int main(void){
	std::vector< std::vector< int > > vecto;
	vecto = std::vector<std::vector<int>> (5, std::vector<int>(10));
	for(int i=0;i<5;i++){
		for(int j=0;j<10;j++){
			vecto[i][j] = i + j;
			std::cout << vecto[i][j] << ' ';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(vecto.begin(), vecto.end(), engine);
	for(int i=0;i<5;i++){
		for(int j=0;j<10;j++){
			std::cout << vecto[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}
