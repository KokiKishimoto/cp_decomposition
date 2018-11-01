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
	std::cout << "tripleeeeeeeeeeesizeeeeeeeeeeeeee:" << test_triple_size * 2 << std::endl;
	triple = std::vector<std::vector<int>>(sum_triple_size, std::vector<int>(3));
	for(int i=0; i<sum_triple_size; i++){
		for(int j=0; j<3; j++){
			fin.read( ( char * ) &d, sizeof(int));
			triple[i][j] = d;
		}
			
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
		}
		return score;
	}

	void rank(void){
		int s_ranking;
		int o_ranking;
		int progress_cnt = 0;
		int progress_percent = 0;
		int cnt_top1 = 0;
		int cnt_top5 = 0;
		int cnt_top10 = 0;
		int top1 = 1;
		int top5 = 5;
		int top10 = 10;
		int relation_number = relation.size() / 2;
		double score_test;
		double score_test2;
		double score;
		int subject_elem;
		int object_elem;
		int relation_elem;
		double MRR = 0;
		for(int i=0; i<test_triple_size; i++){
			std::set<int> s_remove_set;
			std::set<int> o_remove_set;
			subject_elem = triple[i][0];
			object_elem = triple[i][2];
			relation_elem = triple[i][1];
			for(int j=0; j<sum_triple_size; j++){
				if(subject_elem != triple[j][0] && object_elem == triple[j][2] && relation_elem == triple[j][1]){
					s_remove_set.insert(triple[j][0]);
				}
				if(subject_elem == triple[j][0] && object_elem != triple[j][2] && relation_elem == triple[j][1]){
					o_remove_set.insert(triple[j][2]);
				}
			}
			s_ranking = 1;
			score_test = scorefunction(subject[subject_elem], object[object_elem], relation[relation_elem]);
			score_test2 = scorefunction(subject[object_elem], object[subject_elem], relation[relation_elem+relation_number]);
			for(int i=0; i<subject.size(); i++){
				if(s_remove_set.find(i) != s_remove_set.end()){
					continue;
				}
				score = scorefunction(subject[i], object[object_elem], relation[relation_elem]);
				if(score_test+score_test2 < score){
					s_ranking = s_ranking + 1;
				}
			}
			MRR = MRR + (double)1 / (double)s_ranking;
			
			if(s_ranking <= top10){
				cnt_top10 = cnt_top10 + 1;
			}
			if(s_ranking <= top5){
				cnt_top5 = cnt_top5 + 1;
			}
			if(s_ranking <= top1){
				cnt_top1 = cnt_top1 + 1;
			}

			o_ranking = 1;
			for(int i=0; i<object.size(); i++){
				if(o_remove_set.find(i) != o_remove_set.end()){
					continue;
				}
				score = scorefunction(subject[subject_elem], object[i], relation[relation_elem]);
				if(score_test+score_test2 < score){
					o_ranking = o_ranking + 1;
				}
			}
			
			if(o_ranking <= top10){
				cnt_top10 = cnt_top10 + 1;
			}
			if(o_ranking <= top5){
				cnt_top5 = cnt_top5 + 1;
			}
			if(o_ranking <= top1){
				cnt_top1 = cnt_top1 + 1;
			}
			MRR = MRR + (double)1 / (double)o_ranking;
			if(progress_cnt % 10 == 0){
				std::cout << "progress:" << (double)progress_cnt / test_triple_size * 100 << "%" << std::endl;
				progress_percent = progress_percent + 1;
			}
			progress_cnt = progress_cnt + 1;

		}
		MRR = MRR / test_triple_size / 2;
		std::cout << "cnt_top10:" << cnt_top10 << std::endl;
		std::cout << "cnt_top5:" << cnt_top5 << std::endl;
		std::cout << "cnt_top1:" << cnt_top1 << std::endl;
		std::cout << "test triple number:" << test_triple_size * 2 << std::endl;
		std::cout << "rate_top10:" << (float)cnt_top10 / (float)test_triple_size / 2 << std::endl;
		std::cout << "rate_top5:" << (float)cnt_top5 / (float)test_triple_size / 2 << std::endl;
		std::cout << "rate_top1:" << (float)cnt_top1 / (float)test_triple_size / 2 << std::endl;
		std::cout << "MRR:" << MRR << std::endl;
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
	std::string testid;
	std::string subjectname;
	std::string objectname;
	std::string relationname;
	int i;
	if ((i = ArgPos((char *)"-testfile", argc, argv)) > 0) testid = argv[i + 1];
	if ((i = ArgPos((char *)"-subject_model", argc, argv)) > 0) subjectname = argv[i + 1];
	if ((i = ArgPos((char *)"-object_model", argc, argv)) > 0) objectname = argv[i + 1];
	if ((i = ArgPos((char *)"-relation_model", argc, argv)) > 0) relationname = argv[i + 1];
	Test test(testid, subjectname, objectname, relationname);
	test.rank();

	return 0;
}
