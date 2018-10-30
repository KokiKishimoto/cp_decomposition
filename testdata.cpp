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
	Test(const std::string& testid, const std::string& subjectname, const std::string& objectname, const std::string& relationname){
		triple = std::vector<std::vector<int>>(3134, std::vector<int>(3));
		load(testid);
		model_load(subjectname, subject);
		model_load(objectname, object);
		model_load(relationname, relation);
	}

	std::vector< std::vector <int> > load(const std::string& file_name){

	int d;
	int triple_size;
	std::ifstream fin(file_name, std::ios::in | std::ios::binary);
	fin.read( ( char * ) &triple_size, sizeof(int));
	std::cout << "tripleeeeeeeeeeesizeeeeeeeeeeeeee:" << triple_size << std::endl;
	for(int i=0; i<2000; i++){
		
		for(int j=0; j<3; j++){
			fin.read( ( char * ) &d, sizeof(int));
			triple[i][j] = d;
			std::cout << triple[i][j] << ' ';
		}
		std::cout << std::endl;
			
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

	//void srank(void){
	//	int ranking;
	//	int cnt = 0;
	//	int all = subject.size();
	//	double score_test;
	//	double score;
	//	for(auto x:triple){
	//		ranking = 1;
	//		score_test = 0;
	//		score_test = scorefunction(subject[x[0]], object[x[2]], relation[x[1]]);
	//		for(int i=0; i<subject.size(); i++){
	//			score = 0;
	//			score = scorefunction(subject[i], object[x[2]], relation[x[1]]);
	//			if(score_test < score){
	//				ranking = ranking + 1;
	//			}
	//		}
	//		
	//		if(ranking < 10){
	//			cnt = cnt + 1;
	//		}
	//	}
	//	std::cout << "cnt:" << cnt << std::endl;
	//}
	void srank(void){
		int ranking;
		int cnt = 0;
		int all = subject.size();
		int relation_number = 11;
		double score_test;
		double score_test2;
		double score;
		double score2;
		for(auto x:triple){
			ranking = 1;
			score_test = 0;
			score_test2 = 0;
			score_test = scorefunction(subject[x[0]], object[x[2]], relation[x[1]]);
			score_test2 = scorefunction(object[x[2]], subject[x[0]], relation[x[1]+relation_number]);
			for(int i=0; i<subject.size(); i++){
				score = 0;
				score = scorefunction(subject[i], object[x[2]], relation[x[1]]);
				if(score_test+score_test2 < score){
					ranking = ranking + 1;
				}
			}
			
			if(ranking < 10){
				cnt = cnt + 1;
			}
		}
		std::cout << "cnt:" << cnt << std::endl;
		std::cout << "test triple number:" << triple.size() << std::endl;
	}

	void orank(void){
		int ranking;
		int cnt = 0;
		int all = subject.size();
		int relation_number = 11;
		double score_test;
		double score_test2;
		double score;
		double score2;
		double sum_score;
		for(auto x:triple){
			ranking = 1;
			score_test = 0;
			score_test2 = 0;
			score_test = scorefunction(subject[x[0]], object[x[2]], relation[x[1]]);
			score_test2 = scorefunction(object[x[2]], subject[x[0]], relation[x[1]+relation_number]);
			sum_score = score_test + score_test2;
			for(int i=0; i<subject.size(); i++){
				score = 0;
				score = scorefunction(subject[x[0]], object[i], relation[x[1]]);
				if(sum_score < score){
					ranking = ranking + 1;
				}
			}
			
			if(ranking < 20){
				cnt = cnt + 1;
			}
		}
		std::cout << "cnt:" << cnt << std::endl;
	}

	double sigmoid(double& score){
		double sigmoid;
		sigmoid = 1 / (1 + std::exp(-score));
		return sigmoid;
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
