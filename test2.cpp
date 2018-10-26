#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main() {
    fstream fs;

    fs.open("output.bin", ios::in | ios::binary);
    if(! fs.is_open()) {
        return EXIT_FAILURE;
    }

    int n;
    fs.read((char*)&n, sizeof n); // 0x41424344
    cout << n << endl; //=> 1094861636 (sizeof n == 4 ??????????????)

    fs.close();
    return 0;
}
