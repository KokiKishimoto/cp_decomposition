clang++ -std=c++11 -O2 testdata.cpp
./a.out -testfile testid\
	-subject_model ./model/60_subject.txt\
	-object_model ./model/60_object.txt\
	-relation_model ./model/60_relation.txt
