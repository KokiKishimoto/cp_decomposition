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

std::vector< std::vector <std::string> > load(const std::string& file_name){
	relation_cnt = 0;
	std::fstream fs;
	std::vector<std::string> tmp(3);
	std::string l,m,n;
	fs.open(file_name, std::ios::in);
	while (fs >> l >> m >> n){
		tmp[0] = l;
		tmp[1] = m;
		tmp[2] = n;
		
		triple.push_back(tmp);
	}
	
	fs.close(); 
	return triple;
}
void add_rev(void){
	std::vector<std::string> tmp(3);
	std::string l, m, n;
	int triplesize = triple.size();
	for(int i=0; i<triplesize; i++){
		tmp[0] = triple[i][2];
		tmp[1] = "**" + triple[i][1];
		tmp[2] = triple[i][0];

		triple.push_back(tmp);

	}
}
