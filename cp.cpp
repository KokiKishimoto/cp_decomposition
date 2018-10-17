#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> 
#include <set>
#include <map>
#include <stdlib.h>


class ReadFile{
public:
    std::fstream fs;
    std::vector< std::vector <std::string> > triple;
    std::string l,m,n;
    std::map<std::string, int> entity_map;
    std::map<std::string, int> relation_map;
    std::vector< std::vector <int> > tripleID;
    //ReadFile(){}
    ReadFile(const std::string& file_name){
	load(file_name); 
    }
    int load(const std::string& file_name){
	std::vector<std::string> tmp(3);
        fs.open(file_name, std::ios::in);
        if(! fs.is_open()) {
            return EXIT_FAILURE;
        }
        while (fs >> l >> m >> n){
    	tmp[0] = l;
    	tmp[1] = m;
    	tmp[2] = n;
    
    	//std::cout << tmp[0] << tmp[1] << tmp[2] << std::endl;
    	triple.push_back(tmp);
        }
    
        fs.close(); 
	return 0;
    }

    void makedict(void){
	int entity_cnt = 0;
    	int relation_cnt = 0;
    	//std::cout << triple.size() << std::endl;
    	for(int i=0; i<triple.size(); i++){
	   if(entity_map.find(triple[i][0]) == entity_map.end()){
    	   	    entity_map[triple[i][0]] = entity_cnt;
    	   	    entity_cnt++;
    	   }
    	   if(relation_map.find(triple[i][1]) == relation_map.end()){
    	   	    relation_map[triple[i][1]] = relation_cnt;
    	   	    relation_cnt++;
    	   }
    	   if(entity_map.find(triple[i][2]) == entity_map.end()){
    	   	    entity_map[triple[i][2]] = entity_cnt;
    	   	    entity_cnt++;
	   std::cout << triple[i][0] << ':' << entity_map[triple[i][0]] << ' ';
	   std::cout << triple[i][1] << ':' << relation_map[triple[i][1]] << ' ';
	   std::cout << triple[i][2] << ':' << entity_map[triple[i][2]] << ' ';
    	   }
	   std::cout << std::endl;
	   //for(int i=0; i<triple.size(); i++){
    	   //    for(int j=0; j<triple.front().size(); j++){
    	   //        std::cout << triple[i][j] << ' ';
    	   //    }
    	   //    std::cout << std::endl;
    	   //}
     
        }
    }
    void makeid(void){
    	std::vector<int> tmp(3);
        for(int i=0; i<triple.size(); i++){
	   //std::cout << triple[i][0] << std::endl;
    	   //std::cout << "key:" << triple[i][0] << "value:" << entity_map[triple[i][0]] << std::endl;
    
    	   tmp[0] = entity_map[triple[i][0]];
    	   tmp[1] = relation_map[triple[i][1]];
    	   tmp[2] = entity_map[triple[i][2]];
    
    	   std::cout << tmp[0] << ' ' << tmp[1] << ' ' << tmp[2];
    	   tripleID.push_back(tmp);
    	   std::cout << std::endl;
    
    
    	   //std::cout << tripleID[i][0] << ' ';
    	   //std::cout << tripleID[i][1] << ' ';
    	   //std::cout << tripleID[i][2] << ' ';
	}
    }
};

class CP{
public:
    CP(const std::string& file_name, const int& dim){
	ReadFile readfile(file_name);
	int entity_num = readfile.entity_map.size();
	//std::cout << "entity_num:" << entity_num << std::endl;
    	std::vector< std::vector <float> > subject(entity_num, std::vector<float>(dim) );
    	std::vector< std::vector <float> > object(entity_num, std::vector<float>(dim) );
    	std::vector< std::vector <float> > relation(entity_num, std::vector<float>(dim) );
	readfile.makedict();
    	readfile.makeid();

    
    }
    //void initialive_vecotor(){
    //    std::random_device rnd;     // ?????????????
    //	std::mt19937 mt(rnd());     //  ???????????32??????????????
    //	std::uniform_int_distribution<> rand100(0, 99);        // [0, 99] ???????
    //	for (int i = 0; i < 20; ++i) {
    //	    std::cout << rand100(mt) << "\n";
    //}

    //}


};

int main(int argc, char *argv[]) {
    //std::string read_triple_file = argv[1]; 
    //std::string dim = int(argv[2]); 
    //int dim = argv[1]; 

    CP cp(argv[1], atoi(argv[2]));
    //CP cp("test.dat", 400);
    /*
    ReadFile readfile(argv[1]);
    //std::cout << readfile.triple[0][1] << std::endl;
    readfile.makedict();
    
    
    for(int i=0; i<readfile.triple.size(); i++){
	for(int j=0; j<readfile.triple.front().size(); j++){
	    std::cout << readfile.triple[i][j] << ' ';
	}
	std::cout << std::endl;
    }
    readfile.makeid();
    */

    return 0;
}

/*
class CP{
    CP(){}


    int ReadFile(const std::string& read_triple_file ,std::vector< std::vector <std::string> >& triple){
        std::vector<std::string> tmp(3);
        std::fstream fs;
        std::string l,m,n;
        fs.open(read_triple_file, std::ios::in);
        //fs.open("test.dat", std::ios::in);
        if(! fs.is_open()) {
            return EXIT_FAILURE;
        }
        while (fs >> l >> m >> n){
        	// cin のように一行読み込む
    	//int l,m,n;
        	//fs >> l >> m >> n;
    	tmp[0] = l;
    	tmp[1] = m;
    	tmp[2] = n;
    
    	//std::cout << tmp[0] << tmp[1] << tmp[2] << std::endl;
    	triple.push_back(tmp);
        }
    
        fs.close(); // デストラクタでも閉じてくれますが、明示的に閉じる習慣を。
        return 0;
    
    }
    void ID(const std::vector< std::vector <std::string> >& triple, std::map<std::string, int>& entity_map, std::map<std::string, int>& relation_map){
        int entity_cnt = 0;
        int relation_cnt = 0;
        //std::cout << triple.size() << std::endl;
        for(int i=0; i<triple.size(); i++){
    	//entity_set.insert(triple[i][0]);
    	//entity_set.insert(triple[i][2]);
    	//relation_set.insert(triple[i][1]);
    	if(entity_map.find(triple[i][0]) == entity_map.end()){
    	    entity_map[triple[i][0]] = entity_cnt;
    	    entity_cnt++;
    	}
    	if(relation_map.find(triple[i][1]) == relation_map.end()){
    	    relation_map[triple[i][1]] = relation_cnt;
    	    relation_cnt++;
    	}
    	if(entity_map.find(triple[i][2]) == entity_map.end()){
    	    entity_map[triple[i][2]] = entity_cnt;
    	    entity_cnt++;
    	}
    
    	//std::cout << triple[i][0] << ':' << entity_map[triple[i][0]] << std::endl;
    	//std::cout << triple[i][2] << ':' << entity_map[triple[i][2]] << std::endl;
    	//std::cout << triple[i][1] << ':' << relation_map[triple[i][1]] << std::endl;
        }
    }
    
    void ToID(std::vector< std::vector <std::string> >& triple, std::vector< std::vector <int> >& tripleID, std::map<std::string, int> entity_map, std::map<std::string, int>& relation_map){
        for(int i=0; i<triple.size(); i++){
    	//std::cout << triple[i][0] << std::endl;
    	//std::cout << "key:" << triple[i][0] << "value:" << entity_map[triple[i][0]] << std::endl;
    	std::vector<int> tmp(3);
    
    	tmp[0] = entity_map[triple[i][0]];
    	tmp[1] = relation_map[triple[i][1]];
    	tmp[2] = entity_map[triple[i][2]];
    
    	std::cout << tmp[0] << ' ' << tmp[1] << ' ' << tmp[2];
    	tripleID.push_back(tmp);
    	std::cout << std::endl;
    
    
    	//std::cout << tripleID[i][0] << ' ';
    	//std::cout << tripleID[i][1] << ' ';
    	//std::cout << tripleID[i][2] << ' ';
        }
    }
}


int main(int argc, char *argv[]) {
    std::string read_triple_file = argv[1]; 
    std::vector< std::vector <std::string> > triple;
    ReadFile(read_triple_file, triple);
    //std::vector< std::vector <float> > subject;
    //std::vector< std::vector <float> > object;
    //std::vector< std::vector <float> > relation;
    //std::set<std::string> entity_set; 
    //std::set<std::string> relation_set; 
    std::map<std::string, int> entity_map;
    std::map<std::string, int> relation_map;
    ID(triple, entity_map, relation_map); 

    std::vector< std::vector <int> > tripleID;
    ToID(triple, tripleID, entity_map, relation_map);

    //std::vector< std::vector <float> > subject;
    //std::vector< std::vector <float> > object;
    //std::vector< std::vector <float> > relation;


    return 0;
}
*/
