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
			tmp[1] = "**" + triple[i][1];
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
			if (entity_map.find(triple[i][0]) == entity_map.end() || relation_map.find(triple[i][1]) == relation_map.end() || entity_map.find(triple[i][2]) == entity_map.end()){
				std::cout << "idfadfajsdf;akldf;a" << std::endl;
				continue;
			}else{
    
				tmp[0] = entity_map[triple[i][0]];
				tmp[1] = relation_map[triple[i][1]];
				tmp[2] = entity_map[triple[i][2]];

				tripleID.push_back(tmp);
			}


		}
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
	double learning_rate;
	double lam;
	Data data;
	Data testdata;
	CP(const std::string& filename, const std::string& testname, const int& dimension, const std::string& testidname, const double& rate, const double& lambda){
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
		std::cout << "entity_num:" << data.entity_num << std::endl;
		std::cout << "relation_num:" << data.relation_num<< std::endl;
		testdata.load(testname); 
		std::cout << "finish load" << std::endl;
		testdata.makeid(data.entity_map, data.relation_map, testdata.triple);
		std::cout << "finish makeid" << std::endl;
		testdata.write_file(testdata.tripleID, testidname);
		std::cout << "finish write_file" << std::endl;

		subject = std::vector<std::vector<double>> (data.entity_num, std::vector<double>(dim));
		object = std::vector<std::vector<double>> (data.entity_num, std::vector<double>(dim));
		relation = std::vector<std::vector<double>> (data.relation_num, std::vector<double>(dim));
		std::cout << "sub:" << subject.size() << std::endl;
		std::cout << "obj:" << object.size() << std::endl;
		std::cout << "rel:" << relation.size() << std::endl;
		std::cout << "entity_num:" << data.entity_num << std::endl;
	}
		
	void randominitialize(const int&entity_size, const int&relation_size){
		std::random_device rnd;     
    		std::mt19937 mt(rnd());     
    		std::uniform_real_distribution<> rand100(-sqrt(6)/sqrt(2*dim), sqrt(6)/sqrt(2*dim));
		for(int i=0;i<entity_size;i++){
			for(int j=0;j<dim;j++){
				subject[i][j] = rand100(mt);
				object[i][j] = rand100(mt);
			}
		}

		for(int i=0;i<relation_size;i++){
			for(int j=0;j<dim;j++){
				relation[i][j] = rand100(mt);
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

	std::vector< double > computgradient(std::vector< double >& vector1, std::vector< double >& vector2, std::vector< double >& vector3, int y){
		double score = scorefuntion(vector1, vector2, vector3);
		double yscore = y * score;
		std::vector< double > vecotor_product(dim);
		std::vector< double > gradient(dim);
		for(int i=0;i<dim;i++){
			gradient[i] = - y * sigmoid(-yscore) * vector2[i] * vector3[i];// + lam * vector1[i];
		}
		return gradient;
	}

	std::vector< double > updater(std::vector< double >& vector1, std::vector< double >& vector2, std::vector< double >& vector3, int y){
		std::vector< double > gradient(dim);
		std::vector< double > vector(dim);
		gradient = computgradient(vector1, vector2, vector3, y);
		double norm = normfunction(gradient);
		double threshold = 5;
		for(int i=0; i<dim; i++){
			if(norm >= threshold){
				double x = threshold / norm;
				vector[i] = vector1[i] - x * learning_rate * gradient[i];
			}else{
				vector[i] = vector1[i] - learning_rate * gradient[i];
				
			}

		}
		return vector;
	}
	double normfunction(std::vector< double >& vector){
		double norm = 0.0;
		for(int i=0; i<vector.size(); i++){
			norm = norm + std::pow(vector[i], 2.0);
		}
		norm = sqrt(norm);
		return norm;
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
			}
		}
		fout.close();
	}


	void train(const int& iter){
		int random_num;
		randominitialize(data.entity_num, data.relation_num);
		std::cout << "finish randominitialize relation" << std::endl;
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
				int subject_elem = data.tripleID[j][0];
				int object_elem = data.tripleID[j][2];
				int relation_elem = data.tripleID[j][1];
				if(cnt % 1000 == 0){
					std::cout << cnt << ' ' << std::flush;
				}
				cnt++;
				a = updater(subject[subject_elem], object[object_elem], relation[relation_elem], 1);
				b = updater(object[object_elem], subject[subject_elem], relation[relation_elem], 1);
				c = updater(relation[relation_elem], subject[subject_elem], object[object_elem], 1);
				subject[subject_elem] = a;
				object[object_elem] = b;
				relation[relation_elem] = c;

				for(int k=0;k<random_sample;k++){
					random_num = rand(mt);
					a = updater(subject[random_num], object[object_elem], relation[relation_elem], -1);
					b = updater(object[object_elem], subject[random_num], relation[relation_elem], -1);
					c = updater(relation[relation_elem], subject[random_num], object[object_elem], -1);
					subject[random_num] = a;
					object[object_elem] = b;
					relation[relation_elem] = c;

					random_num = rand(mt);
					a = updater(subject[subject_elem], object[random_num], relation[relation_elem], -1);
					b = updater(object[random_num], subject[subject_elem], relation[relation_elem], -1);
					c = updater(relation[relation_elem], subject[subject_elem], object[random_num], -1);
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
			if(i % 10 ==0){
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

