#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> // EXIT_FAILURE のため

//using namespace std;
//void ReadFile(std::string read_triple_file, 

int main(int argc, char *argv[]) {
    //std::vector< std::vector <std::string> > a;
    std::vector< std::vector <int> > a;
    //std::vector<std::string> b(3);
    std::vector<int> b(3);
    //std::vector<std::vector <std::string> > a(10, std::vector<std::string>(13));

    std::fstream fs;
    fs.open(argv[1], std::ios::in);
    if(! fs.is_open()) {
        // exit だけでなく return の戻り値としても使用できる。
        return EXIT_FAILURE;
    }
    do {

    	// cin のように一行読み込む
	//std::string l,m,n;
	int l,m,n;
    	fs >> l >> m >> n;
	b[0] = l;
	b[1] = m;
	b[2] = n;

	std::cout << b[0] << b[1] << b[2] << std::endl;
	a.push_back(b);

    }while (! fs.eof()); // eof: ファイル終端にファイルポインタがあってもfalseです。
    // 終端を越えようとしていればtrueです。そのため、input.bin が空であっても最初はfalseです
    // よって、do-while であっても while であっても表示結果は同じで最低1ループは実行されます。
    // ここでは現実をそのまま直感的に表現したdo-whileを採用しました。
    
    std::cout << a[0][0] + a[0][1] << std::endl;

    fs.close(); // デストラクタでも閉じてくれますが、明示的に閉じる習慣を。

    /*
    for(int i=0;i<2;i++){
	for(int j=0;j<3;j++){
	    std::cout<<a[i][j] << ' ';
	}
	std::cout<<std::endl;
    }
    */
    return 0;
}
