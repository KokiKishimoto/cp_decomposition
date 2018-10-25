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

class Test{
public:
	std::vector< std::vector < int > > triple;
	std::vector< std::vector <float> > subject;
	std::vector< std::vector <float> > object;
	std::vector< std::vector <float> > relation;
	Test(const std::string& testid, const std::string& subjectname, const std::string& objectname, const std::string& relationname){
		load(testid);
		model_load(subjectname, subject);
		model_load(objectname, object);
		model_load(relationname, relation);
	}

	std::vector< std::vector <int> > load(const std::string& file_name){
        	std::fstream fs;
		std::vector<int> tmp(3);
		int l,m,n;
		fs.open(file_name, std::ios::in);
		//if(! fs.is_open()) {
		//return EXIT_FAILURE;
		//}
		while (fs >> l >> m >> n){
			tmp[0] = l;
			tmp[1] = m;
			tmp[2] = n;
			
			//std::cout << tmp[0] << tmp[1] << tmp[2] << std::endl;
			triple.push_back(tmp);
		}
		
		fs.close(); 
		return triple;
	}
	
	void model_load(const std::string& file_name, std::vector< std::vector< float > >&matrix){
		std::fstream fs;
		std::string l,m,n;
		int str;
		int dim;
		fs.open(file_name, std::ios::in);
		fs >> str >> dim;
		//std::getline(fs, str); //? cin ( https://www.qoosky.io/techs/d5709e9878 ) ??????? fs
		//std::getline(fs, dim); //? cin ( https://www.qoosky.io/techs/d5709e9878 ) ??????? fs
		matrix = std::vector< std::vector< float > >(str, std::vector<float>(dim));
		std::cout << str << std::endl; //=> 1 10 100	//}
		for(int i=0; i<str; i++){
			for(int j=0; j<dim; j++){
				fs >> matrix[i][j];
				//std::cout << matrix[i][j] << ' ';
			}
			//std::cout << std::endl;
		}
		fs.close(); 
		std::cout << "size:" << matrix.size() << std::endl;
		std::cout << "frontsize:" << matrix.front().size() << std::endl;
	}

	//void rank(std::vector< std::vector< int > >& triple, std::vector< std::vector< float > >& subject, std::vector< std::vector< float > >& object, std::vector< std::vector< float > >& relation){
	void rank(void){
		int ranking;
		int cnt = 0;
		int all = subject.size();
		std::vector < float > score = std::vector < float > (subject.size());
		for(auto x:triple){
		//std::vector < int > x = triple[0];
			//std::cout << "x:" << x[0] << ' ' << x[1] << ' '<< x[2] << std::endl;
			for(int i=0; i<subject.size(); i++){
				//for(int j=0; j<subject.front().size(); j++){
				//	score[j] = 0;
				score[i] = 0;
				//}
				for(int j=0; j<subject.front().size(); j++){
					score[i] = score[i] + subject[i][j] * object[x[2]][j] * relation[x[1]][j];
					
					//std::cout << score[i] << std::endl;
				}
				//for(int j=0; j<subject.front().size(); j++){
				//	highscore = highscore + subject[0][j] * object[1][j] * relation[0][j];
				//}
			}
			float scoree = 0;
			for(int j=0; j<subject.front().size(); j++){
				scoree = scoree + subject[x[0]][j] * object[x[2]][j] * relation[x[1]][j];
			}
			//std::sort(score.begin(),score.end(),std::greater<float>());
			//std::sort(score.begin(),score.end());
			//for(int j=0; j<100; j++){
			//	std::cout << score[j] << ' ';
			//}
			//std::cout << std::endl;
			//std::cout << sigmoid(scoree) << std::endl;
			ranking = 1;
			for(int i=0; i<subject.size(); i++){
				if(score[x[0]] < score[i]){
					ranking = ranking + 1;
					//std::cout << ranking << std::endl;
				}else{
					//std::cout << "aiueo";
				}
				
			}
			if(ranking < 10){
				cnt = cnt + 1;
			}
		}
		std::cout << "cnt:" << cnt << std::endl;
	}
	float sigmoid(float& score){
		float sigmoid;
		sigmoid = 1 / (1 + std::exp(-score));
		return sigmoid;
	}

};
int main(int argc, char *argv[]){
	std::string testid = argv[1];
	std::string subjectname = argv[2];
	std::string objectname = argv[3];
	std::string relationname = argv[4];
	//Test test("testid", "subject.txt");
	Test test(testid, subjectname, objectname, relationname);
	test.rank();
}
