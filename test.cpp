#include <algorithm>
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
int main(void){
	std::vector<std::vector<int>> v = std::vector<std::vector<int>>(10, std::vector<int>(3));
	for(int i = 0; i<10; i++){
		for(int j=0; j<3; j++){
			v[i][j] = i + j;	
			std::cout << v[i][j] << ' ';
		}
		std::cout << std::endl;
	}
	
	std::vector<int> v2 = std::vector<int>(10);
	
	for(int i=0; i<10; i++){
		v2[i] = i;
	}
	//std::iota(v.begin(), v.end(), 0); // 0?9 ???????
	
	//std::cout << "before: ";
	//std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout));
	//std::cout << std::endl;
	
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(v2.begin(), v2.end(), engine);
	
	for(auto i:v2){
		for(int j=0; j<3; j++){
			v[i][j] = i + j;	
			std::cout << v[i][j] << ' ';
		}
		std::cout << std::endl;
	}
	//std::cout << " after: ";
	//std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout));
	//std::cout << std::endl;
    return 0;
}
