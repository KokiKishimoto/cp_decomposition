#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> // EXIT_FAILURE のため

//using namespace std;
//std::vector< std::vector <int> > a;
std::vector< std::vector <int> > a;
std::vector<int> b(3);

int ReadFile(std::string read_triple_file){
    std::fstream fs;
    int l,m,n;
    fs.open(read_triple_file, std::ios::in);
    //fs.open("test.dat", std::ios::in);
    if(! fs.is_open()) {
        return EXIT_FAILURE;
    }
    while (fs >> l >> m >> n){
    //do {
    	// cin のように一行読み込む
	//int l,m,n;
    	//fs >> l >> m >> n;
	b[0] = l;
	b[1] = m;
	b[2] = n;

	//std::cout << b[0] << b[1] << b[2] << std::endl;
	a.push_back(b);
    }

    fs.close(); // デストラクタでも閉じてくれますが、明示的に閉じる習慣を。
    return 0;

}

int main(int argc, char *argv[]) {
//int main(){
    //std::vector< std::vector <std::string> > a;
    //std::vector<std::string> b(3);
    //std::vector<std::vector <std::string> > a(10, std::vector<std::string>(13));
    
    std::string read_triple_file = argv[1];

    
    //std::cout << a[0][0] + a[0][1] << std::endl;


    /*
    for(int i=0;i<2;i++){
	for(int j=0;j<3;j++){
	    std::cout<<a[i][j] << ' ';
	}
	std::cout<<std::endl;
    }
    */
    ReadFile(read_triple_file);
    return 0;
}
