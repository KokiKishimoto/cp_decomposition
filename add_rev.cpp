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

class AddRev{
public:
	std::vector< std::vector <std::string> > triple;
	std::string triple_file;
	AddRev(const std::string& file_name){
		triple_file = file_name;	
		load();
	}

	std::vector< std::vector <std::string> > load(void){
		std::fstream fs;
		std::vector<std::string> tmp(3);
		std::string l,m,n;
		fs.open(triple_file, std::ios::in);
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
	void write_file(const std::string& rev_filename){

		std::ofstream fout;
		fout.open(rev_filename, std::ios::out);

		for(int i=0; i<triple.size(); i++){
			fout << triple[i][0] << '\t' << triple[i][1] << '\t' << triple[i][2] << std::endl; // ??????????
		}
		fout.close();

	}
};

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
	AddRev addrev(file_name);
	addrev.add_rev();
	addrev.write_file(rev_filename);
	std::cout << addrev.triple.size() << std::endl;
	std::cout << addrev.triple.front().size() << std::endl;

	return 0;
}
