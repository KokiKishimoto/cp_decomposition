#clang++ -std=c++11 -O2 cp.cpp
#./a.out -train ./FB15k/freebase_mtr100_mte100-train.txt\
#	-test ./FB15k/freebase_mtr100_mte100-test.txt\
#	-valid ./FB15k/freebase_mtr100_mte100-valid.txt\
#	-dimension 200\
#	-iteration 300\
#	-testid testid\
#	-rate 0.05\
#	-lambda 0.0001
#clang++ -std=c++11 -O2 cp.cpp
#./a.out -train ./wordnet-mlj12/wordnet-mlj12-train.txt\
#	-test ./wordnet-mlj12/wordnet-mlj12-test.txt\
#	-valid ./wordnet-mlj12/wordnet-mlj12-valid.txt\
#	-dimension 200\
#	-iteration 300\
#	-testid testid\
#	-rate 0.05\
#	-lambda 0.0001
clang++ -std=c++11 -O2 cp.cpp
./a.out -train ./WN18RR/train.txt\
	-test ./WN18RR/test.txt\
	-valid ./WN18RR/valid.txt\
	-dimension 200\
	-iteration 300\
	-testid testid\
	-rate 0.05\
	-lambda 0.0001
