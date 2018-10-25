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
	//std::vector< std::vector <std::string> > triple_rev;
	std::map<std::string, int> entity_map;
	std::map<std::string, int> relation_map;
	std::vector< std::vector < int > > tripleID;
	int entity_num;
	int relation_num;
	int relation_cnt;
	Data(){}
	std::vector< std::vector <std::string> > load(const std::string& file_name){
		relation_cnt = 0;
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
	void add_rev(void){
		std::vector<std::string> tmp(3);
		std::string l, m, n;
		int triplesize = triple.size();
		for(int i=0; i<triplesize; i++){
			tmp[0] = triple[i][2];
			tmp[1] = std::to_string(relation_map[triple[i][1]]);
			//std::cout << "tmp1:"<< tmp[1] << std::endl;
			//std::cout << tmp[1] << std::endl;
			tmp[2] = triple[i][0];

			triple.push_back(tmp);

		}
		//for(int i=0; i<triple.size(); i++){
		//	for(int j=0; j<3; j++){
		//		std::cout << triple[i][j] << ' ';
		//	}
		//	std::cout << std::endl;
		//}
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
		for(int i=0; i<triple.size(); i++){
			if(relation_map.find(triple[i][1]) == relation_map.end()){
				std::cout << "dafadfadfasfadf:" << triple[i][1] << std::endl;
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

	void write_file(const std::vector < std::vector < int > >& tripleID, const std::string& filename){
		std::fstream fs;
		fs.open(filename, std::ios::out);
		//std::cout << "matrixsize:" << matrix.size() << std::endl;
		for(int i=0; i<tripleID.size(); i++){
			for(int j=0; j<3; j++){
				fs << tripleID[i][j] << ' ' << std::flush; 
				//std::cout << triple[i][j] << ' ';
			}
			fs << std::endl; 
			//std::cout << std::endl;
		}
		fs.close();
	}
};



class CP{
public:
	std::vector< std::vector <double> > subject;
	std::vector< std::vector <double> > relation;
	std::vector< std::vector <double> > object;
	int dim;
	//double learning_rate = 0.025;
	double objective_f;
	double learning_rate;
	double lam;
	Data data;
	Data testdata;
	CP(const std::string& filename, const std::string& testname, const int& dimension, const std::string& testidname, const double& rate, const double& lambda){
		objective_f= 0;
		dim= dimension;
		lam = lambda;
		learning_rate = rate;
		std::cout << "rate:" << learning_rate;
		data.load(filename); 
		data.entitydict(data.triple);
		data.relationdict(data.triple);
		data.add_rev();
		data.relationdict(data.triple);
		std::cout << "saaaaaize" << data.relation_map.size() << std::endl;
		for(auto x:data.relation_map){
			std::cout << "key:" << x.first << "value:" << x.second << std::endl;
		}
		data.makeid(data.entity_map, data.relation_map, data.triple);
		//for(int i=0; i<data.tripleID.size(); i++){
		//    for(int j=0; j<3;j++){
		//        std::cout << data.tripleID[i][j] << ' ';
		//    }
		//std::cout << std::endl;
		//}
		std::cout << "entity_num:" << data.entity_num << std::endl;
		std::cout << "relation_num:" << data.relation_num<< std::endl;
		testdata.load(testname); 
		//std::cout<< "fireeeeeeeeeeeeeeeeeeeeeeeeeeeeeee:" << testdata.triple[0][0] << std::endl;
		std::cout << "finish load" << std::endl;
		testdata.makeid(data.entity_map, data.relation_map, testdata.triple);
		std::cout << "finish makeid" << std::endl;
		//std::cout<< "fireeeeeeeeeeeeeeeeeeeeeeeeeeeeeee:" << testdata.tripleID[0][0] << std::endl;
		testdata.write_file(testdata.tripleID, testidname);
		std::cout << "finish write_file" << std::endl;
	}
		
	void randominitialize(std::vector< std::vector< double > >& matrix, const int&size){
		matrix = std::vector<std::vector<double>> (size, std::vector<double>(dim));
		std::random_device rnd;     // 非決定的な乱数生成器を生成
    		std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    		std::uniform_real_distribution<> rand100(-sqrt(6)/sqrt(2*dim), sqrt(6)/sqrt(2*dim));
    		//std::uniform_real_distribution<> rand100(-sqrt(6)/sqrt(2*10), sqrt(6)/sqrt(2*10));
		for(int i=0;i<size;i++){
			for(int j=0;j<dim;j++){
				matrix[i][j] = rand100(mt);
			}
		}

	}
	double scorefuntion(std::vector< double >& vector1, std::vector< double >& vector2, std::vector< double >& vector3){
		double score = 0;
		for (int i=0;i<dim;i++){
			score += vector1[i] * vector2[i] * vector3[i];
		}
		return score;
	}


	double sigmoid(double& score){
		//if(score > thre){
		//	return 1;
		//else if(score < -thre){
		//	return 0;
		//else{
			double sigmoid;
			sigmoid = 1 / (1 + std::exp(-score));
			return sigmoid;
	}

	std::vector< double > computgradient(std::vector< double >& vector1, std::vector< double >& vector2, std::vector< double >& vector3){
		double score = scorefuntion(vector1, vector2, vector3);
		double sigmoidscore = sigmoid(score);
		std::vector< double > vecotor_product(dim);
		std::vector< double > gradient(dim);
		for(int i=0;i<dim;i++){
			vecotor_product[i] = vector2[i] * vector3[i];
			gradient[i] = - std::exp(-score) * sigmoidscore * vecotor_product[i];// + lam * vector1[i];
			//std::cout << "gradient:" << gradient[i] << std::endl;
		}
		return gradient;
	}

	std::vector< double > updater(std::vector< double >& vector1, std::vector< double >& vector2, std::vector< double >& vector3){
	//void updater(std::vector< double >& vector1, std::vector< double >& vector2, std::vector< double >& vector3){
		std::vector< double > gradient2(dim);
		std::vector< double > vector(dim);
		gradient2 = computgradient(vector1, vector2, vector3);
		double norm = normfunction(gradient2);
		double threshold = 5;
		for(int i=0; i<dim; i++){
			if(norm >= threshold){
				double x = threshold / norm;
				vector[i] = vector1[i] - x * learning_rate * gradient2[i];
			}else{
				vector[i] = vector1[i] - learning_rate * gradient2[i];
				
			}

		}
		return vector;
	}

	std::vector< double > negative_computgradient(std::vector< double >& vector1, std::vector< double >& vector2, std::vector< double >& vector3){
		double score = scorefuntion(vector1, vector2, vector3);
		double sigmoidscore = sigmoid(score);
		std::vector< double > vecotor_product(dim);
		std::vector< double > gradient(dim);
		for(int i=0;i<dim;i++){
			vecotor_product[i] = vector2[i] * vector3[i];
			gradient[i] = sigmoidscore * vecotor_product[i];// + lam * vector1[i];
		}
		return gradient;
	}

	std::vector< double > negative_updater(std::vector< double >& vector1, std::vector< double >& vector2, std::vector< double >& vector3){
		std::vector< double > negative_gradient(dim);
		std::vector< double > vector(dim);
		negative_gradient = negative_computgradient(vector1, vector2, vector3);
		double norm = normfunction(negative_gradient);
		double threshold = 5;
		for(int i=0; i<dim; i++){
			double x = threshold / norm;
			if(norm >= threshold){
				double x = threshold / norm;
				vector[i] = vector1[i] - x * learning_rate * negative_gradient[i];
			}else{
				vector[i] = vector1[i] - learning_rate * negative_gradient[i];
				
			}
		}
		return vector;
	}
	double normfunction(std::vector< double >& vector){
		double norm;
		for(int i=0; i<vector.size(); i++){
			norm = norm + std::pow(vector[i], 2.0);
		}
		norm = sqrt(norm);
		return norm;
	}

	void objectivefunction(std::vector< double >& subject, std::vector< double >& object, std::vector< double >& relation){
		double score;
		double norm_sub, norm_obj, norm_re;
		double sigmoidscore;
		score = scorefuntion(subject, object, relation);
		sigmoidscore = sigmoid(score);
		norm_sub = normfunction(subject);
		norm_obj = normfunction(object);
		norm_re = normfunction(relation);
		objective_f = objective_f + log(sigmoidscore);
	}

	void objectivefunctionnegative(std::vector< double >& subject, std::vector< double >& object, std::vector< double >& relation){
		double score;
		double norm_sub, norm_obj, norm_re;
		double sigmoidscore;
		score = scorefuntion(subject, object, relation);
		sigmoidscore = sigmoid(score);
		norm_sub = normfunction(subject);
		norm_obj = normfunction(object);
		norm_re = normfunction(relation);
		objective_f = objective_f + log(1 - sigmoidscore);
	}

	//void objectivefunctionnegative(std::vector< double >& vector1, std::vector< double >& vector2, std::vector< double >& vector3){
	//	objective_f = objective_f + log(1 - sigmoidscore)
	//}


	void write_model(const std::vector < std::vector < double > >& matrix, const std::string& filename){
		std::fstream fs;
		fs.open(filename, std::ios::out);
		std::cout << "matrixsize:" << matrix.size() << std::endl;
		fs << matrix.size() << std::endl;
		fs << dim << std::endl;
		for(int i=0; i<matrix.size(); i++){
			for(int j=0; j<dim; j++){
				fs << matrix[i][j] << ' ' << std::flush; 
			}
			fs << std::endl; 
		}
		fs.close();
	}


	//void train(const int& iter, const std::string& subject_name, const std::string& object_name, const std::string& relation_name){
	void train(const int& iter){
		randominitialize(subject, data.entity_num);
		std::cout << "finish randominitialize subject" << std::endl;
		randominitialize(object, data.entity_num);
		std::cout << "finish randominitialize object" << std::endl;
		randominitialize(relation, data.relation_num);
		std::cout << "finish randominitialize relation" << std::endl;
		double score;
		double sigmoidscore;
		int random_sample = 5;
		std::vector< double > a, b, c;
		std::random_device rnd;     // 非決定的な乱数生成器を生成
    		std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    		std::uniform_int_distribution<> rand(0, data.entity_num-1);
		std::vector<std::vector <int> > tripleID_all = std::vector<std::vector<int>> (data.tripleID.size(), std::vector<int>(3 + random_sample*2));
		std::cout << tripleID_all.size() << std::endl;
		std::cout << tripleID_all.front().size() << std::endl;
		for(int i=0; i<data.tripleID.size(); i++){
			for(int j=0; j < 3; j++){
				tripleID_all[i][j] = data.tripleID[i][j];
				//std::cout << tripleID_all[i][j] << std::endl;
			}
		}
		std::cout << "finish tripleID" << std::endl;
		for(int i=0; i<data.tripleID.size(); i++){
			for(int j=3; j < 3 + random_sample ; j++){
				tripleID_all[i][j] = rand(mt);
				//std::cout << tripleID_all[i][j] << std::endl;
			}
		}
		std::cout << "finish negativesubject" << std::endl;
		for(int i=0; i<data.tripleID.size(); i++){
			for(int j=3+random_sample; j < 3 + 2 * random_sample ; j++){
				tripleID_all[i][j] = rand(mt);
			}
		}
		std::cout << "finish negativeobject" << std::endl;


		//for(int i=0; i<data.tripleID.size(); i++){
		//	for(int j=0; j<3+random_sample*2; j++){
		//		std::cout << tripleID_all[i][j] << ' ';
		//	}
		//	std::cout << std::endl;
		//}
		for(int i=0; i<iter; i++){
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());
			std::shuffle(tripleID_all.begin(), tripleID_all.end(), engine);

			int cnt = 0;
			for (auto j: tripleID_all){
				if(cnt % 1000 == 0){
					std::cout << cnt << ' ' << std::flush;
				}
				cnt++;
				a = updater(subject[j[0]], object[j[2]], relation[j[1]]);
				b = updater(object[j[2]], subject[j[0]], relation[j[1]]);
				c = updater(relation[j[1]], subject[j[0]], object[j[2]]);
				subject[j[0]] = a;
				object[j[2]] = b;
				relation[j[1]] = c;
				//std::cout << j[0] << j[1] << j[2];
				//score = scorefuntion(subject[j[0]], object[j[2]], relation[j[1]]);
				//score = scorefuntion(subject[0], object[1], relation[0]);

				//sigmoidscore = sigmoid(score);
				//std::cout << "score:" << score << std::endl;
				//std::cout << "sigmoidscore:" << sigmoidscore << std::endl;
				objectivefunction(subject[j[0]], object[j[2]], relation[j[1]]);

				for(int k=3;k<3+random_sample;k++){
					a = negative_updater(subject[j[k]], object[j[2]], relation[j[1]]);
					b = negative_updater(object[j[2]], subject[j[k]], relation[j[1]]);
					c = negative_updater(relation[j[1]], subject[j[k]], object[j[2]]);

					subject[j[k]] = a;
					object[j[2]] = b;
					relation[j[1]] = c;
					//score = scorefuntion(subject[j[k]], object[j[2]], relation[j[1]]);
					//sigmoidscore = sigmoid(score);
					//std::cout << "sigmoidscore:" << sigmoidscore << std::endl;
					objectivefunctionnegative(subject[j[k]], object[j[2]], relation[j[1]]);
				}
				for(int k=3+random_sample; k<3+random_sample*2; k++){
					a = negative_updater(subject[j[0]], object[j[k]], relation[j[1]]);
					b = negative_updater(object[j[k]], subject[j[0]], relation[j[1]]);
					c = negative_updater(relation[j[1]], subject[j[0]], object[j[k]]);
					subject[j[0]] = a;
					object[j[k]] = b;
					relation[j[1]] = c;
					score = scorefuntion(subject[j[0]], object[j[k]], relation[j[1]]);
					sigmoidscore = sigmoid(score);
					//std::cout << "sigmoidscore:" << sigmoidscore << std::endl;
					objectivefunctionnegative(subject[j[0]], object[j[k]], relation[j[1]]);
				}
			}
			//std::cout << "tripleID_all:" << tripleID_all[0][0] << ' ' << tripleID_all[0][1] << ' ' << tripleID_all[0][2];
			std::cout << std::endl;
			std::cout << "---------------" << std::endl;;
			std::cout << "---------------" << std::endl;;
			std::cout<< "---------" << i << "----------" << std::endl;
			//std::cout << "scorreeeee:" << scorefuntion(subject[tripleID_all[0][0]], object[tripleID_all[0][2]], relation[tripleID_all[0][1]]) << std::endl;
			std::cout << "scorreeeee:" << scorefuntion(subject[tripleID_all[0][0]], object[tripleID_all[0][2]], relation[tripleID_all[0][1]]) << std::endl;
			std::cout << "objectivefunction:" << objective_f << std::endl;
			std::cout << "---------------" << std::endl;;
			std::cout << "---------------" << std::endl;;
			if(i % 10 ==0){
				std::string subject_name = "./model/" + std::to_string(i) + "_subject.txt";
				std::string object_name = "./model/" + std::to_string(i) + "_object.txt";
				std::string relation_name = "./model/" + std::to_string(i) + "_relation.txt";
				write_model(subject, subject_name);
				write_model(object, object_name);
				write_model(relation, relation_name);
				//std::cout << "scorreeeee:" << scorefuntion(subject[tripleID_all[0][0]], object[tripleID_all[0][2]], relation[tripleID_all[0][1]]) << std::endl;

			}

		}

		//write_model(subject, subject_name);
		//write_model(object, object_name);
		//write_model(relation, relation_name);
	}
};

class Test{
public:
	std::vector< std::vector < int > > triple;
	Test(const std::string& testid, const std::string& subject){
		load(testid);
		//model_load(subject);

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
	
	//std::vector< std::vector <double> > model_load(const std::string& file_name){
	//void model_load(const std::string& file_name){
        	//std::fstream fs;
		//std::string l,m,n;
		//std::string str;
		//std::string dim;
		//fs.open(file_name, std::ios::in);
		//std::getline(fs, str); //? cin ( https://www.qoosky.io/techs/d5709e9878 ) ??????? fs
		//std::getline(fs, dim); //? cin ( https://www.qoosky.io/techs/d5709e9878 ) ??????? fs
		//std::vector< std::vector <double> > x = std::vector< std::vector< double > >(std::stoi(str), std::vector<double>(std::stoi(dim)));
		//if(! fs.is_open()) {
		//return EXIT_FAILURE;
		//std::string str;
		//cout << str << endl; //=> 1 10 100	//}
		//for(int i=0; i<std::stoi(str); i++){
		//	for(int j=0; j<std::stoi(dim); j++){
		//		//while (fs >> x[i][j]){
		//			fs >> x[i][j];
		//			std::cout << x[i][j] << ' ';
		//		//}
		//	}
		//	std::cout << std::endl;
		//}
		//fs.close(); 
		//return x;
	//}

};



int main(int argc, char *argv[]) {

	std::string filename = argv[1];
	std::string testname = argv[2];
	int dimension = atoi(argv[3]);
	int iteration = atoi(argv[4]);
	//std::string subject_name = argv[5];
	//std::string object_name = argv[6];
	//std::string relation_name = argv[7];
	std::string testidname= argv[5];
	double rate = std::stof(argv[6]);
	double lambda = std::stof(argv[7]);
	//std::cout << "atoi:" << rate;

	CP cp(filename, testname, dimension, testidname, rate, lambda);
	cp.train(iteration);

	//Test test("testid", "subject.txt");
	return 0;
}

