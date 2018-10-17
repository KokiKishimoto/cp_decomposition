#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> 
#include <set>
#include <map>


class ReadFile{
public:
    std::fstream fs;
    std::vector< std::vector <std::string> > triple;
    std::string l,m,n;
    std::map<std::string, int> entity_map;
    std::map<std::string, int> relation_map;
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
        	// cin のように一行読み込む
    	//int l,m,n;
        	//fs >> l >> m >> n;
    	tmp[0] = l;
    	tmp[1] = m;
    	tmp[2] = n;
    
    	//std::cout << tmp[0] << tmp[1] << tmp[2] << std::endl;
    	triple.push_back(tmp);
        }
    
        fs.close(); 
	return 0;
    }

    void makeid(void){
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
    	   }
        
        }
    }
};

int main(int argc, char *argv[]) {
    std::string read_triple_file = argv[1]; 

    ReadFile readfile(argv[1]);
    //std::cout << readfile.triple[0][1] << std::endl;
    readfile.makeid();
    
    
    for(int i=0; i<readfile.triple.size(); i++){
	for(int j=0; j<readfile.triple.front().size(); j++){
	    std::cout << readfile.triple[i][j] << std::endl;
	}
    }

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
