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
	float learning_rate = 0.025;
	Data data;
	CP(const std::string& filename, const int& dimension){
		dim= dimension;
		data.load(filename); 
		data.entitydict(data.triple);
		data.relationdict(data.triple);
		data.makeid(data.entity_map, data.relation_map, data.triple);
		for(int i=0; i<data.tripleID.size(); i++){
		    for(int j=0; j<3;j++){
		        std::cout << data.tripleID[i][j] << ' ';
		    }
		std::cout << std::endl;
		}
		std::cout << "entity_num:" << data.entity_num << std::endl;
		std::cout << "relation_num:" << data.relation_num<< std::endl;
	}
		
	void randominitialize(std::vector< std::vector< float > >& matrix, const int&size){
		matrix = std::vector<std::vector<float>> (size, std::vector<float>(dim));
		std::random_device rnd;     // 非決定的な乱数生成器を生成
    		std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    		std::uniform_real_distribution<> rand100(-sqrt(6)/sqrt(2*data.tripleID.size()), sqrt(6)/sqrt(2*data.tripleID.size()));
    		//std::uniform_real_distribution<> rand100(-sqrt(6)/sqrt(2*10), sqrt(6)/sqrt(2*10));
		for(int i=0;i<size;i++){
			for(int j=0;j<dim;j++){
				matrix[i][j] = rand100(mt);
			}
		}

	}
	float scorefuntion(std::vector< float >& vector1, std::vector< float >& vector2, std::vector< float >& vector3){
		float score = 0;
		for (int i=0;i<dim;i++){
			score += vector1[i] * vector2[i] * vector3[i];
		}
		return score;
	}


	float sigmoid(float& score){
		float sigmoid;
		sigmoid = 1 / (1 + std::exp(-score));
		return sigmoid;
	}

	std::vector< float > computgradient(std::vector< float >& vector1, std::vector< float >& vector2, std::vector< float >& vector3){
		float score = scorefuntion(vector1, vector2, vector3);
		float sigmoidscore = sigmoid(score);
		std::vector< float > vecotor_product(dim);
		std::vector< float > gradient(dim);
		for(int i=0;i<dim;i++){
			vecotor_product[i] = vector2[i] * vector3[i];
			gradient[i] = - std::exp(-score) * sigmoidscore * vecotor_product[i];
		}
		return gradient;
	}

	std::vector< float > updater(std::vector< float >& vector1, std::vector< float >& vector2, std::vector< float >& vector3){
	//void updater(std::vector< float >& vector1, std::vector< float >& vector2, std::vector< float >& vector3){
		std::vector< float > gradient2(dim);
		std::vector< float > vector(dim);
		gradient2 = computgradient(vector1, vector2, vector3);
		for(int i=0; i<dim; i++){
			vector[i] = vector1[i] - learning_rate * gradient2[i];
		}
		return vector;
	}

	std::vector< float > negative_computgradient(std::vector< float >& vector1, std::vector< float >& vector2, std::vector< float >& vector3){
		float score = scorefuntion(vector1, vector2, vector3);
		float sigmoidscore = sigmoid(score);
		std::vector< float > vecotor_product(dim);
		std::vector< float > gradient(dim);
		for(int i=0;i<dim;i++){
			vecotor_product[i] = vector2[i] * vector3[i];
			gradient[i] = sigmoidscore * vecotor_product[i];
		}
		return gradient;
	}

	std::vector< float > negative_updater(std::vector< float >& vector1, std::vector< float >& vector2, std::vector< float >& vector3){
		std::vector< float > negative_gradient(dim);
		std::vector< float > vector(dim);
		negative_gradient = negative_computgradient(vector1, vector2, vector3);
		for(int i=0; i<dim; i++){
			vector[i] = vector1[i] - learning_rate * negative_gradient[i];
			//std::cout << vector1[i];
		}
		return vector;
	}




	void train(const int& iter){
		randominitialize(subject, data.entity_num);
		randominitialize(object, data.entity_num);
		randominitialize(relation, data.relation_num);
		float score;
		float sigmoidscore;
		int random_sample = 5;
		std::vector< float > a, b, c;
		std::random_device rnd;     // 非決定的な乱数生成器を生成
    		std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    		std::uniform_int_distribution<> rand(0, data.entity_num-1);
		//std::vector<std::vector <int> >  random_subject = std::vector<std::vector<int>> (data.entity_num, std::vector<int>(random_sample));
		//std::vector<std::vector <int> >  random_object = std::vector<std::vector<int>> (data.entity_num, std::vector<int>(random_sample));
		std::vector<std::vector <int> > tripleID_all = std::vector<std::vector<int>> (data.tripleID.size(), std::vector<int>(3 + random_sample*2));
		std::cout << tripleID_all.size() << std::endl;
		std::cout << tripleID_all.front().size() << std::endl;
		for(int i=0; i<data.tripleID.size(); i++){
			for(int j=0; j < 3; j++){
				tripleID_all[i][j] = data.tripleID[i][j];
				//std::cout << tripleID_all[i][j] << std::endl;
			}
		}
		for(int i=0; i<data.tripleID.size(); i++){
			for(int j=3; j < 3 + random_sample ; j++){
				tripleID_all[i][j] = rand(mt);
				//std::cout << tripleID_all[i][j] << std::endl;
			}
		}
		for(int i=0; i<data.tripleID.size(); i++){
			for(int j=3+random_sample; j < 3 + 2 * random_sample ; j++){
				tripleID_all[i][j] = rand(mt);
			}
		}


		//for(int i=0; i<data.tripleID.size(); i++){
		//	for(int j=0; j<3+random_sample*2; j++){
		//		std::cout << tripleID_all[i][j] << ' ';
		//	}
		//	std::cout << std::endl;
		//}
		for(int i=0; i<iter; i++){
			for (auto j: tripleID_all){
				a = updater(subject[j[0]], object[j[2]], relation[j[1]]);
				b = updater(object[j[2]], subject[j[0]], relation[j[1]]);
				c = updater(relation[j[1]], subject[j[0]], object[j[1]]);
				subject[j[0]] = a;
				object[j[2]] = b;
				relation[j[1]] = c;
				score = scorefuntion(subject[j[0]], object[j[2]], relation[j[1]]);
				sigmoidscore = sigmoid(score);
				//std::cout << "score:" << score << std::endl;
				//std::cout << "sigmoidscore:" << sigmoidscore << std::endl;

				for(int k=3;k<3+random_sample;k++){
					score = scorefuntion(subject[j[k]], object[j[2]], relation[j[1]]);
					a = negative_updater(subject[j[k]], object[j[2]], relation[j[1]]);
					b = negative_updater(object[j[2]], subject[j[k]], relation[j[1]]);
					c = negative_updater(relation[j[1]], subject[j[k]], object[j[2]]);
					subject[j[k]] = a;
					object[j[2]] = b;
					relation[j[1]] = c;
					//score = scorefuntion(subject[x], object[j[2]], relation[j[1]]);
					//sigmoidscore = sigmoid(score);
					//std::cout << "sigmoidscore:" << sigmoidscore << std::endl;
				}
				for(int k=3+random_sample; k<3+random_sample*2; k++){
					score = scorefuntion(subject[j[0]], object[j[k]], relation[j[1]]);
					a = negative_updater(subject[j[0]], object[j[k]], relation[j[1]]);
					b = negative_updater(object[j[k]], subject[j[0]], relation[j[1]]);
					c = negative_updater(relation[j[1]], subject[j[0]], object[j[k]]);
					subject[j[0]] = a;
					object[j[k]] = b;
					relation[j[1]] = c;
					score = scorefuntion(subject[j[0]], object[j[k]], relation[j[1]]);
					sigmoidscore = sigmoid(score);
					std::cout << "sigmoidscore:" << sigmoidscore << std::endl;
				}
			}
			std::cout<< "---------" << i << "----------" << std::endl;

		}

	}
};

//class test{
//public:
//		CP(const std::string& filename, const int& dimension){
//
//};



int main(int argc, char *argv[]) {

	std::string filename = argv[1];
	int dimension = atoi(argv[2]);
	int iteration = atoi(argv[3]);
	CP cp(filename, dimension);
	cp.train(iteration);

	return 0;
}

