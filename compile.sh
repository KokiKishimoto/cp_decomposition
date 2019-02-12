#clang++ -std=c++11 -O2 add_rev.cpp
#./a.out -file_name ./wordnet-mlj12/wordnet-mlj12-train.txt -rev_filename ./wordnet-mlj12/a.txt
#clang++ -std=c++11 -O2 ./cp.cpp
#./a.out -train ./wordnet-mlj12/a.txt \
#	-dimension 200 \
#	-iteration 100 \
#	-rate 0.025 \
#	-lambda 0.0001
clang++ -std=c++11 -O2 testcp.cpp
./a.out -train ./wordnet-mlj12/a.txt \
	-test ./wordnet-mlj12/wordnet-mlj12-test.txt \
	-valid ./wordnet-mlj12/wordnet-mlj12-valid.txt \
	-subject_model ./model/100_subject.txt \
	-object_model ./model/100_object.txt \
	-relation_model ./model/100_relation.txt
