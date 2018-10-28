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
	int relation_numkari;
	int relation_cnt;
	Data(){}
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
			//tmp[1] = std::to_string(relation_map[triple[i][1]]);
			tmp[1] = "**" + triple[i][1];
			//std::cout << "tmppppp" << tmp[1] << std::endl;
			tmp[2] = triple[i][0];

			triple.push_back(tmp);

		}
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
		}
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
		}
		relation_numkari = relation_cnt;
		return relation_map;
	}

	std::map<std::string, int> rev_dict(){
		int count = 0;
		std::string revkey;
		for(auto itr = relation_map.begin(); itr != relation_map.end(); ++itr) {
        		std::cout << "keyyeeeeee = " << itr->first << ", vallllleeeee = " << itr->second << "\n";    // ????
			revkey = "**" + itr->first;
			relation_map[revkey] = itr->second + relation_numkari;
    		}
		for(auto itr = relation_map.begin(); itr != relation_map.end(); ++itr) {
			count = count + 1;
    		}
		relation_num = count;
		for(auto itr = relation_map.begin(); itr != relation_map.end(); ++itr) {
        		std::cout << "keyyeeeeee = " << itr->first << ", vallllleeeee = " << itr->second << "\n";    // ????
    		}
		return relation_map;
	}
	std::vector< std::vector < int > > makeid(std::map<std::string, int>& entity_map, std::map<std::string, int>& relation_map, std::vector< std::vector < std::string > >& triple){
		std::vector<int> tmp(3);
		for(int i=0; i<triple.size(); i++){
    
			tmp[0] = entity_map[triple[i][0]];
			tmp[1] = relation_map[triple[i][1]];
			tmp[2] = entity_map[triple[i][2]];

			tripleID.push_back(tmp);


		}
		//for(int i=85000; i<87000; i++){
		//	std::cout << tripleID[i][0] << ' ' << tripleID[i][1] << ' ' << tripleID[i][2] << std::endl;
		//}
		return tripleID;
	}

	void write_file(const std::vector < std::vector < int > >& tripleID, const std::string& filename){
		int triple_size = tripleID.size();

		std::ofstream fout;
		fout.open(filename, std::ios::out|std::ios::binary|std::ios::trunc);
		fout.write((char * ) &triple_size,sizeof(int));
		for(int i=0; i<tripleID.size(); i++){
			for(int j=0; j<tripleID.front().size(); j++){
				fout.write(( char * ) &tripleID[i][j], sizeof( int ) );
			}
		}
		fout.close();

	}
};



class CP{
public:
	std::vector< std::vector <double> > subject;
	std::vector< std::vector <double> > relation;
	std::vector< std::vector <double> > object;
	int dim;
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
		data.rev_dict();
		std::cout << "saaaaaize" << data.relation_map.size() << std::endl;
		for(auto x:data.relation_map){
			std::cout << "key:" << x.first << "value:" << x.second << std::endl;
		}
		data.makeid(data.entity_map, data.relation_map, data.triple);
		//for(int i=0; i<4000; i++){
		//	for(int j=0; j<3; j++){
		//		std::cout << data.tripleID[i][j] << ' ';
		//	}
		//	std::cout << std::endl;
		//}
		std::cout << "entity_num:" << data.entity_num << std::endl;
		std::cout << "relation_num:" << data.relation_num<< std::endl;
		testdata.load(testname); 
		std::cout << "finish load" << std::endl;
		testdata.makeid(data.entity_map, data.relation_map, testdata.triple);
		std::cout << "finish makeid" << std::endl;
		testdata.write_file(testdata.tripleID, testidname);
		std::cout << "finish write_file" << std::endl;
	}
		
	void randominitialize(std::vector< std::vector< double > >& matrix, const int&size){
		matrix = std::vector<std::vector<double>> (size, std::vector<double>(dim));
		std::random_device rnd;     
    		std::mt19937 mt(rnd());     
    		std::uniform_real_distribution<> rand100(-sqrt(6)/sqrt(2*dim), sqrt(6)/sqrt(2*dim));
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
		}
		return gradient;
	}

	std::vector< double > updater(std::vector< double >& vector1, std::vector< double >& vector2, std::vector< double >& vector3){
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


	void write_model(const std::vector < std::vector < double > >& matrix, const std::string& filename){
		int matrix_size = matrix.size();
		std::ofstream fout;
		fout.open(filename, std::ios::out|std::ios::binary|std::ios::trunc);
		fout.write((char * ) &matrix_size,sizeof(int));
		fout.write((char * ) &dim,sizeof(int));
		for(int i=0; i<matrix.size(); i++){
			for(int j=0; j<dim; j++){
				fout.write(( char * ) &matrix[i][j], sizeof( double ) );
				//std::cout << matrix[i][j] << ' ';
			}
			//std::cout << std::endl;
		}
		fout.close();
	}


	void train(const int& iter){
		int random_num;
		randominitialize(subject, data.entity_num);
		std::cout << "finish randominitialize subject" << std::endl;
		randominitialize(object, data.entity_num);
		std::cout << "finish randominitialize object" << std::endl;
		std::cout << "iyahhhhhhoooo" << data.relation_num << std::endl;
		randominitialize(relation, data.relation_num);
		std::cout << "finish randominitialize relation" << std::endl;
		double score;
		double sigmoidscore;
		int random_sample = 2;
		std::vector< double > a, b, c;
		std::random_device rnd;     
    		std::mt19937 mt(rnd());     
    		std::uniform_int_distribution<> rand(0, data.entity_num-1);
		std::vector<int> v = std::vector<int>(data.tripleID.size());
		for(int i=0; i<data.tripleID.size(); i++){
			v[i] = i;
		}

		for(int i=0; i<iter; i++){
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());
			std::shuffle(v.begin(), v.end(), engine);

			
			int cnt = 1;
			for (auto j: v){
				//std::cout << j << std::endl;
				int subject_elem = data.tripleID[j][0];
				int object_elem = data.tripleID[j][2];
				int relation_elem = data.tripleID[j][1];
				if(cnt % 1000 == 0){
					std::cout << cnt << ' ' << std::flush;
				}
				cnt++;
				a = updater(subject[subject_elem], object[object_elem], relation[relation_elem]);
				b = updater(object[object_elem], subject[subject_elem], relation[relation_elem]);
				c = updater(relation[relation_elem], subject[subject_elem], object[object_elem]);
				subject[subject_elem] = a;
				object[object_elem] = b;
				relation[relation_elem] = c;

				for(int k=0;k<random_sample;k++){
					random_num = rand(mt);
					a = negative_updater(subject[random_num], object[object_elem], relation[relation_elem]);
					b = negative_updater(object[object_elem], subject[random_num], relation[relation_elem]);
					c = negative_updater(relation[relation_elem], subject[random_num], object[object_elem]);
					subject[random_num] = a;
					object[object_elem] = b;
					relation[relation_elem] = c;

					random_num = rand(mt);
					a = negative_updater(subject[subject_elem], object[random_num], relation[relation_elem]);
					b = negative_updater(object[random_num], subject[subject_elem], relation[relation_elem]);
					c = negative_updater(relation[relation_elem], subject[subject_elem], object[random_num]);
					subject[subject_elem] = a;
					object[random_num] = b;
					relation[relation_elem] = c;
					//score = scorefuntion(subject[subject_elem], object[random_num], relation[relation_elem]);
				}
			}
			std::cout << std::endl;
			std::cout << "---------------" << std::endl;;
			std::cout << "---------------" << std::endl;;
			std::cout<< "---------" << i << "----------" << std::endl;
			std::cout << "scorreeeee:" << scorefuntion(subject[data.tripleID[0][0]], object[data.tripleID[0][2]], relation[data.tripleID[0][1]]) << std::endl;
			std::cout << "---------------" << std::endl;;
			std::cout << "---------------" << std::endl;;
			if(i % 30 ==0){
				std::string subject_name = "./model/" + std::to_string(i) + "_subject.txt";
				std::string object_name = "./model/" + std::to_string(i) + "_object.txt";
				std::string relation_name = "./model/" + std::to_string(i) + "_relation.txt";
				write_model(subject, subject_name);
				write_model(object, object_name);
				write_model(relation, relation_name);
				for(int x=0; x<data.tripleID.size(); x++){
					std::cout << scorefuntion(subject[data.tripleID[x][0]], object[data.tripleID[x][2]], relation[data.tripleID[x][1]]) << std::endl;
				}
			}

		}

	}
};

class Test{
public:
	std::vector< std::vector < int > > triple;
	Test(const std::string& testid, const std::string& subject){
		load(testid);

	}

	std::vector< std::vector <int> > load(const std::string& file_name){
        	std::fstream fs;
		std::vector<int> tmp(3);
		int l,m,n;
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
	
};



int main(int argc, char *argv[]) {

	std::string filename = argv[1];
	std::string testname = argv[2];
	int dimension = atoi(argv[3]);
	int iteration = atoi(argv[4]);
	std::string testidname= argv[5];
	double rate = std::stof(argv[6]);
	double lambda = std::stof(argv[7]);

	CP cp(filename, testname, dimension, testidname, rate, lambda);
	cp.train(iteration);

	return 0;
}

