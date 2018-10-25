clang++ -std=c++11 -O2 cp.cpp
#clang++ -std=c++11 cp.cpp
#./a.out train.dat test.dat 200 100 testid 0.05
./a.out ./WN18RR/train.txt ./WN18RR/test.txt 200 100 testid 0.025 0.0001
#./a.out ./FB15k/freebase_mtr100_mte100-train.txt ./FB15k/freebase_mtr100_mte100-test.txt 200 1 subject.txt object.txt relation.txt testid 0.05
