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
	std::vector< std::vector <double> > subject;
	std::vector< std::vector <double> > object;
	std::vector< std::vector <double> > relation;
	int test_triple_size;
	int sum_triple_size;
	Test(const std::string& testid, const std::string& subjectname, const std::string& objectname, const std::string& relationname){
		load(testid);
		model_load(subjectname, subject);
		model_load(objectname, object);
		model_load(relationname, relation);
	}

	std::vector< std::vector <int> > load(const std::string& file_name){
	int d;
	std::ifstream fin(file_name, std::ios::in | std::ios::binary);
	fin.read( ( char * ) &test_triple_size, sizeof(int));
	std::cout << test_triple_size << std::endl;
	fin.read( ( char * ) &sum_triple_size, sizeof(int));
	std::cout << sum_triple_size << std::endl;
	std::cout << "tripleeeeeeeeeeesizeeeeeeeeeeeeee:" << test_triple_size << std::endl;
	triple = std::vector<std::vector<int>>(sum_triple_size, std::vector<int>(3));
	for(int i=0; i<sum_triple_size; i++){
		for(int j=0; j<3; j++){
			fin.read( ( char * ) &d, sizeof(int));
			triple[i][j] = d;
			//std::cout << d << ' ';
		}
		//std::cout << std::endl;
			
	}
	fin.close();
	return triple;
	}

	void model_load(const std::string& file_name, std::vector< std::vector< double > >&matrix){
		
		int matrix_size;
		int dim;
		double d;
		std::ifstream fin(file_name, std::ios::in | std::ios::binary);
		fin.read( ( char * ) &matrix_size, sizeof(int));
		fin.read( ( char * ) &dim, sizeof(int));
		std::cout << "aiueo" << matrix_size << std::endl;
		std::cout << "kakikukeko" << dim << std::endl;
		matrix = std::vector< std::vector< double > >(matrix_size, std::vector<double>(dim));
		for(int i=0; i<matrix_size; i++){
			for(int j=0; j<dim; j++){
				fin.read( ( char * ) &d, sizeof(double));
				matrix[i][j] = d;
				//std::cout << 'a' << std::endl;
			}
				
		}
		fin.close();
	}
	double scorefunction(const std::vector< double >& vector1, const std::vector< double >& vector2, const std::vector< double >& vector3){
		double score = 0;
		for(int j = 0; j < vector1.size(); j++){
			score = score + vector1[j] * vector2[j] * vector3[j];
			//std::cout << vector1.size();
		}
		return score;
	}

	void srank(void){
		int ranking;
		int cnt = 0;
		int relation_number = relation.size() / 2;
		double score_test;
		double score_test2;
		double score;
		double rate;
		int subject_elem;
		int object_elem;
		int relation_elem;
		for(int i=0; i<test_triple_size; i++){
			std::set<int> remove_set;
			subject_elem = triple[i][0];
			object_elem = triple[i][2];
			relation_elem = triple[i][1];
			for(int j=0; j<sum_triple_size; j++){
				if(subject_elem != triple[j][0] && object_elem == triple[j][2] && relation_elem == triple[j][1]){
					remove_set.insert(triple[j][0]);
				}
			}
			ranking = 1;
			score_test = scorefunction(subject[subject_elem], object[object_elem], relation[relation_elem]);
			score_test2 = scorefunction(subject[object_elem], object[subject_elem], relation[relation_elem+relation_number]);
			for(int i=0; i<subject.size(); i++){
				if(remove_set.find(i) != remove_set.end()){
					continue;
				}
				score = scorefunction(subject[i], object[object_elem], relation[relation_elem]);
				if(score_test+score_test2 < score){
					ranking = ranking + 1;
				}
			}
			
			if(ranking < 10){
				cnt = cnt + 1;
			}
		}
		rate = (float)cnt / (float)test_triple_size;
		std::cout << "cnt:" << cnt << std::endl;
		std::cout << "test triple number:" << test_triple_size << std::endl;
		std::cout << "rate:" << rate << std::endl;
	}
	void orank(void){
		int ranking;
		int cnt = 0;
		int all = subject.size();
		int relation_number = relation.size() / 2;
		std::cout << "nuuuuuuuuuuuuuuuuuuumber relation" << relation_number << std::endl;
		//int relation_number = 11;
		double score_test;
		double score_test2;
		double score;
		double score2;
		double rate;
		int subject_elem;
		int object_elem;
		int relation_elem;
		std::cout << "aiueo" << std::endl;
		for(int i=0; i<test_triple_size; i++){
			subject_elem = triple[i][0];
			object_elem = triple[i][2];
			relation_elem = triple[i][1];
			ranking = 1;
			score_test = scorefunction(subject[subject_elem], object[object_elem], relation[relation_elem]);
			score_test2 = scorefunction(subject[object_elem], object[subject_elem], relation[relation_elem+relation_number]);
			for(int i=0; i<subject.size(); i++){
				score = scorefunction(subject[subject_elem], object[i], relation[relation_elem]);
				if(score_test+score_test2 < score){
					ranking = ranking + 1;
				}
			}
			
			if(ranking < 10){
				cnt = cnt + 1;
			}
		}
		rate = (float)cnt / (float)test_triple_size;
		std::cout << "cnt:" << cnt << std::endl;
		std::cout << "test triple number:" << test_triple_size << std::endl;
		std::cout << "rate:" << rate << std::endl;
	}

};
int main(int argc, char *argv[]){
	std::string testid = argv[1];
	std::string subjectname = argv[2];
	std::string objectname = argv[3];
	std::string relationname = argv[4];
	Test test(testid, subjectname, objectname, relationname);
	test.srank();
	//test.orank();
}
