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

class Data{
public:
	std::vector< std::vector <std::string> > triple;
	std::map<std::string, int> entity_map;
	std::map<std::string, int> relation_map;
	std::vector< std::vector < int > > tripleID;
	int entity_num;
	int relation_num;
	Data(){}
	std::vector< std::vector <std::string> > load(const std::string& file_name){
        	std::fstream fs;
		std::vector<std::string> tmp(3);
		std::string l,m,n;
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
	std::map<std::string, int> entitydict(std::vector< std::vector < std::string > >& triple){
		int entity_cnt = 0;
		for(int i=0; i<triple.size(); i++){
			if(entity_map.find(triple[i][0]) == entity_map.end()){
				entity_map[triple[i][0]] = entity_cnt;
				entity_cnt++;
			}
			if(entity_map.find(triple[i][2]) == entity_map.end()){
				entity_map[triple[i][2]] = entity_cnt;
				entity_cnt++;
			}
		//std::cout << triple[i][0] << ':' << entity_map[triple[i][0]] << ' ';
		//std::cout << triple[i][2] << ':' << entity_map[triple[i][2]] << ' ';
		}
		//std::cout << std::endl;
		entity_num = entity_cnt;
		return entity_map;
	}
	std::map<std::string, int> relationdict(std::vector< std::vector < std::string > >& triple){
		int relation_cnt= 0;
		for(int i=0; i<triple.size(); i++){
			if(relation_map.find(triple[i][1]) == relation_map.end()){
				relation_map[triple[i][1]] = relation_cnt;
				relation_cnt++;
			}
		//std::cout << triple[i][1] << ':' << entity_map[triple[i][1]] << ' ';
		}
		//std::cout << std::endl;
		relation_num = relation_cnt;
		return relation_map;
	}
	std::vector< std::vector < int > > makeid(std::map<std::string, int>& entity_map, std::map<std::string, int>& relation_map, std::vector< std::vector < std::string > >& triple){
		std::vector<int> tmp(3);
		for(int i=0; i<triple.size(); i++){
    
			tmp[0] = entity_map[triple[i][0]];
			tmp[1] = relation_map[triple[i][1]];
			tmp[2] = entity_map[triple[i][2]];

			//std::cout << tmp[0] << ' ' << tmp[1] << ' ' << tmp[2];
			tripleID.push_back(tmp);
			//std::cout << std::endl;


		}
		return tripleID;
	}
};



class CP{
public:
	std::vector< std::vector <float> > subject;
	std::vector< std::vector <float> > relation;
	std::vector< std::vector <float> > object;
	int dim;
	//std::vector< std::vector <float> > object;
	//std::vector< std::vector <float> > relation;
	//std::vector<int> subject;
	Data data;
	//std::vector< std::vector <float> > subject(entity_num, std::vector<float>(dim) );
	//std::vector< std::vector <float> > object(entity_num, std::vector<float>(dim) );
	//std::vector< std::vector <float> > relation(entity_num, std::vector<float>(dim) );
	CP(const std::string& filename, const int& dimension){
		//Data data;
		dim= dimension;
		data.load(filename); 
		//for(int i=0; i<2; i++){
		//    for(int j=0; j<3;j++){
		//        std::cout << data.triple[i][j];
		//    }
		//std::cout << std::endl;
		//}
		data.entitydict(data.triple);
		data.relationdict(data.triple);
		data.makeid(data.entity_map, data.relation_map, data.triple);
		for(int i=0; i<2; i++){
		    for(int j=0; j<3;j++){
		        std::cout << data.tripleID[i][j];
		    }
		std::cout << std::endl;
		}
		std::cout << "entity_num:" << data.entity_num << std::endl;
		std::cout << "relation_num:" << data.relation_num<< std::endl;
	}
		
	void randominitialize(std::vector< std::vector< float > >& matrix, const int&size){
		//subject[0].push_back(1);
		//std::cout << "size:" << data.tripleID.size() << std::endl;
		matrix.push_back(std::vector<float>());
		std::random_device rnd;     // 非決定的な乱数生成器を生成
    		std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    		std::uniform_real_distribution<> rand100(-sqrt(6)/sqrt(2*data.tripleID.size()), sqrt(6)/sqrt(2*data.tripleID.size()));
		std::cout << "size:"<< size << std::endl;
		for(int i=0;i<size;i++){
			//std::cout << '[';
			for(int j=0;j<dim;j++){
				matrix[i].push_back(rand100(mt));
				//std::cout << matrix[i][j] << ',';
			}
			//std::cout << ']' << std::endl;
			matrix.push_back(std::vector<float>());
		//for(int i=0;i<data.tripleID.size();i++){
		//	for(int j=0;j<3;j++){
		//	std::cout << subject[i][j];
		//	}
		//	std::cout << std::endl;
		//}

		}
	}
	float scorefuntion(std::vector< float >& vector1, std::vector< float >& vector2, std::vector< float >& vector3){
		float score = 0;
		for (int i=0;i<dim;i++){
			score += vector1[i] * vector2[i] * vector3[i];
		}
		return score;
	}

	float computgradient(std::vector< float >& vector1, std::vector< float >& vector2, std::vector< float >& vector3){
		float score = scorefuntion(vector1, vector2, vector3);
		float sigmoidscore = sigmoid(score);
		std::vector< float > vecotor_product;
		float gradient;
		for(int i=0;i<dim;i++){
			vecotor_product = vector2[i] * vector3[i];
		}
		gradient = - std::exp(score) * sigmoidscore * vecotor_product;
		return gradient;
	}

	float sigmoid(float& score){
		float sigmoid;
		sigmoid = 1 / (1 + std::exp(-score));
		return sigmoid;
	}


	void train(void){
		randominitialize(subject, data.entity_num);
		randominitialize(relation, data.relation_num);
		randominitialize(object, data.entity_num);
		std::vector< float > subject_vector = subject[1];
		std::vector< float > relation_vector = relation[1];
		std::vector< float > object_vector = object[1];
		//std::cout << '[';
		//for(int i=0;i<dim;i++){
		//	std::cout << vector[i] << ' ';
		//}
		//std::cout << ']' << std::endl;
		//std::cout << vector.size();	

		for(int i=0; i<1000; i++){
			std::cout << computgradient(subject_vector, relation_vector, object_vector) << std::endl;
		}

	}
};
    //void initialive_vecotor(){
    //    std::random_device rnd;     // ?????????????
    //	std::mt19937 mt(rnd());     //  ???????????32??????????????
    //	std::uniform_int_distribution<> rand100(0, 99);        // [0, 99] ???????
    //	for (int i = 0; i < 20; ++i) {
    //	    std::cout << rand100(mt) << "\n";
    //}

    //}



int main(int argc, char *argv[]) {
	/*
	Data data;
	data.load(argv[1]); 
	for(int i=0; i<2; i++){
	    for(int j=0; j<3;j++){
	        std::cout << data.triple[i][j];
	    }
	std::cout << std::endl;
	}
	data.entitydict(data.triple);
	data.relationdict(data.triple);
	data.makeid(data.entity_map, data.relation_map, data.triple);
	for(int i=0; i<2; i++){
	    for(int j=0; j<3;j++){
	        std::cout << data.tripleID[i][j];
	    }
	std::cout << std::endl;
	}
	*/

	std::string filename = argv[1];
	int dimension = atoi(argv[2]);
	CP cp(filename, dimension);
	cp.train();
	/*
	for(int i=0; i<2; i++){
	    for(int j=0; j<3;j++){
	        std::cout << cp.data.tripleID[i][j];
	    }
	std::cout << std::endl;
	}
	*/

	return 0;
}

