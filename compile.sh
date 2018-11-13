clang++ -std=c++11 -O2 add_rev.cpp
./a.out -file_name ./WN18RR/train.txt -rev_filename ./WN18RR/a.txt
clang++ -std=c++11 -O2 cp.cpp
./a.out -train ./WN18RR/a.txt\
	-dimension 200\
	-iteration 10\
	-rate 0.05\
	-lambda 0.0001
clang++ -std=c++11 -O2 testcp.cpp
./a.out -train ./WN18RR/a.txt\
	-test ./WN18RR/test.txt\
	-valid ./WN18RR/valid.txt\
	-subject_model ./model/10_subject.txt\
	-object_model ./model/10_object.txt\
	-relation_model ./model/10_relation.txt
