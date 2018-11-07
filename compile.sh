clang++ -std=c++11 add_rev.cpp
./a.out -file_name ./WN18RR/train.txt -rev_filename ./WN18RR/a.txt
clang++ -std=c++11 -O2 cp.cpp
./a.out -train ./WN18RR/a.txt\
	-test ./WN18RR/test.txt\
	-valid ./WN18RR/valid.txt\
	-dimension 200\
	-iteration 10\
	-testid testid\
	-rate 0.05\
	-lambda 0.0001
clang++ -std=c++11 -O2 testdata.cpp
./a.out -testfile testid\
	-subject_model ./model/30_subject.txt\
	-object_model ./model/30_object.txt\
	-relation_model ./model/30_relation.txt
