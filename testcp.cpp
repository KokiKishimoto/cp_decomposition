#include "rank_score.hpp"
#include "data.hpp"
#include "testdata.hpp"
#include "signal_operation.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <utility>

using Key = std::pair<int, int>;

class TestCP {
public:
	std::vector<RankScore> scores_subj, scores_obj;
	Data trainData;
	TestData testData, validData;
	std::vector< std::vector<double> > subjVec, objVec, relationVec;
	std::vector<double> outVec1, outVec2, outVec3;
	std::vector<double> outVec1_inv, outVec2_inv, outVec3_inv;
	SignalOperation op;
	std::map<Key, std::set<int> > subjSet, objSet;
	int vecDim;
	//bool isInverse;

	TestCP(std::string dataFile, std::string testFile, std::string validFile, std::string modelSubjFile, std::string modelObjFile, std::string modelRelFile) {
		trainData = Data();
		trainData.readFromRawFile(dataFile);

		testData = TestData();
		testData.readFromRawFileRev(testFile, trainData);

		scores_subj.resize(trainData.entity_counter, RankScore());
		scores_obj.resize(trainData.entity_counter, RankScore());

		std::ifstream fin;

		fin.open(modelSubjFile);
		int entitySize;
		fin >> entitySize >> vecDim;
		subjVec.resize(entitySize);
		for (int i = 0; i < entitySize; i++) {
			std::string tmp;
			fin >> tmp;
			subjVec[i].resize(vecDim);
			for (int j = 0; j < vecDim; j++) {
				fin >> subjVec[i][j];
			}
		}
		fin.close();

		fin.open(modelObjFile);
		fin >> entitySize >> vecDim;
		objVec.resize(entitySize);
		for (int i = 0; i < entitySize; i++) {
			std::string tmp;
			fin >> tmp;
			objVec[i].resize(vecDim);
			for (int j = 0; j < vecDim; j++) {
				fin >> objVec[i][j];
			}
		}
		fin.close();

		fin.open(modelRelFile);
		int relationSize;
		fin >> relationSize >> vecDim;
		relationVec.resize(relationSize);
		for (int i = 0; i < relationSize; i++) {
			std::string tmp;
			fin >> tmp;
			relationVec[i].resize(vecDim);
			for (int j = 0; j < vecDim; j++) {
				fin >> relationVec[i][j];
			}
		}
		fin.close();

		op = SignalOperation();
		outVec1.resize(vecDim);
		outVec2.resize(vecDim);
		outVec3.resize(vecDim);

		outVec1_inv.resize(vecDim);
		outVec2_inv.resize(vecDim);
		outVec3_inv.resize(vecDim);

		for (Triple& t : trainData.triples) {
			Key k1(t.relation, t.obj);
			subjSet[k1].insert(t.subj);

			Key k2(t.relation, t.subj);
			objSet[k2].insert(t.obj);
		}

		for (Triple& t : testData.triples) {
			Key k1(t.relation, t.obj);
			subjSet[k1].insert(t.subj);

			Key k2(t.relation, t.subj);
			objSet[k2].insert(t.obj);
		}

		validData = TestData();
		validData.readFromRawFile(validFile, trainData);
		for (Triple& t : validData.triples) {
			Key k1(t.relation, t.obj);
			subjSet[k1].insert(t.subj);

			Key k2(t.relation, t.subj);
			objSet[k2].insert(t.obj);
		}
	}

	void test_() {
		double total = (double)testData.triples.size() * 2;
		double mrr = 0.0, mrr_filter = 0.0;
		double rank_1 = 0.0, rank_1_filter = 0.0;
		double rank_3 = 0.0, rank_3_filter = 0.0;
		double rank_5 = 0.0, rank_5_filter = 0.0;
		double rank_10 = 0.0, rank_10_filter = 0.0;
		int j=0;
		for (Triple& test_tuple : testData.triples) {
			if (test_tuple.flag) {
				total = total - 2;
				continue;
			}
			pre_score_(test_tuple.subj, test_tuple.relation, test_tuple.obj);
			for (int entity = 0; entity<trainData.entity_counter; ++entity) {
				RankScore& r1 = scores_subj[entity]; 
				r1.set(entity, innerProductSubj(entity, outVec2));
				RankScore& r2 = scores_obj[entity];
				r2.set(entity, innerProductObj(entity, outVec3));
			}
			std::sort(scores_subj.begin(), scores_subj.end());
			std::sort(scores_obj.begin(), scores_obj.end());

			Key k_s(test_tuple.relation, test_tuple.obj);
			Key k_o(test_tuple.relation, test_tuple.subj);
			const std::set<int>& hypo_s = subjSet[k_s];
			const std::set<int>& hypo_o = subjSet[k_o];

			int rank1 = -1, rank2 = -1;
			int rank_filter_1 = -1, rank_filter_2 = -1;
			int k = 0;
			for (int i=0; i<scores_subj.size(); ++i) {
				RankScore& r1 = scores_subj[i];
				if (r1.id == test_tuple.subj) {
					rank1 = i;
					rank_filter_1 = k;
					break;
				}
				if (hypo_s.find(r1.id) == hypo_s.end()) {
					++k;
				}
			}
			k=0;
			for (int i=0; i<scores_obj.size(); ++i) {
				RankScore& r1 = scores_obj[i];
				if (r1.id == test_tuple.obj) {
					rank2 = i;
					rank_filter_2 = k;
					break;
				}
				if (hypo_o.find(r1.id) == hypo_o.end()) {
					++k;
				}
			}
			std::cout << ++j << " " << rank1 << " " << rank2 << "\n " << rank_filter_1 << " " << rank_filter_2 << std::endl;
			if (rank1 == 0) {
				++rank_1;
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank1 <= 2) {
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank1 <= 4) {
				++rank_5;
				++rank_10;
			} else if (rank1 <= 9) {
				++rank_10;
			}
			
			if (rank2 == 0) {
				++rank_1;
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank2 <= 2) {
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank2 <= 4) {
				++rank_5;
				++rank_10;
			} else if (rank2 <= 9) {
				++rank_10;
			}
			
			mrr += 1.0/(rank1+1.0);
			mrr += 1.0/(rank2+1.0);
			
			if (rank_filter_1 == 0) {
				++rank_1_filter;
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 2) {
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 4) {
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 9) {
				++rank_10_filter;
			}
			
			if (rank_filter_2 == 0) {
				++rank_1_filter;
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 2) {
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 4) {
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 9) {
				++rank_10_filter;
			}
			mrr_filter += 1.0/(rank_filter_1+1.0);
			mrr_filter += 1.0/(rank_filter_2+1.0);
		}
		std::cout << "Raw Rank1: " << (rank_1 / total) << "\n";
		std::cout << "Raw Rank3: " << (rank_3 / total) << "\n";
		std::cout << "Raw Rank5: " << (rank_5 / total) << "\n";
		std::cout << "Raw Rank10: " << (rank_10 / total) << "\n";
		std::cout << "Raw MRR: " << (mrr / total) << "\n";
		
		std::cout << "Filtered Rank1: " << (rank_1_filter / total) << "\n";
		std::cout << "Filtered Rank3: " << (rank_3_filter / total) << "\n";
		std::cout << "Filtered Rank5: " << (rank_5_filter / total) << "\n";
		std::cout << "Filtered Rank10: " << (rank_10_filter / total) << "\n";
		std::cout << "Filtered MRR: " << (mrr_filter / total) << "\n";
	}
	
	void test_inv() {
		double total = (double)testData.triples.size() * 2;
		double mrr = 0.0, mrr_filter = 0.0;
		double rank_1 = 0.0, rank_1_filter = 0.0;
		double rank_3 = 0.0, rank_3_filter = 0.0;
		double rank_5 = 0.0, rank_5_filter = 0.0;
		double rank_10 = 0.0, rank_10_filter = 0.0;
		int j=0;
		for (Triple& test_tuple : testData.triples) {
			if (test_tuple.flag) {
				total = total - 2;
				continue;
			}
			pre_score_inv(test_tuple.subj, test_tuple.relation, test_tuple.relation_rev, test_tuple.obj);
			for (int entity = 0; entity<trainData.entity_counter; ++entity) {
				RankScore& r1 = scores_subj[entity]; 
				r1.set(entity, innerProductSubj(entity, outVec2) + innerProductObj(entity, outVec3_inv));
				RankScore& r2 = scores_obj[entity];
				r2.set(entity, innerProductObj(entity, outVec3) + innerProductSubj(entity, outVec2_inv));
			}
			std::sort(scores_subj.begin(), scores_subj.end());
			std::sort(scores_obj.begin(), scores_obj.end());
			Key k_s(test_tuple.relation, test_tuple.obj);
			Key k_o(test_tuple.relation, test_tuple.subj);
			const std::set<int>& hypo_s = subjSet[k_s];
			const std::set<int>& hypo_o = subjSet[k_o];

			int rank1 = -1, rank2 = -1;
			int rank_filter_1 = -1, rank_filter_2 = -1;
			int k = 0;
			for (int i=0; i<scores_subj.size(); ++i) {
				RankScore& r1 = scores_subj[i];
				if (r1.id == test_tuple.subj) {
					rank1 = i;
					rank_filter_1 = k;
					break;
				}
				if (hypo_s.find(r1.id) == hypo_s.end()) {
					++k;
				}
			}
			k=0;
			for (int i=0; i<scores_obj.size(); ++i) {
				RankScore& r1 = scores_obj[i];
				if (r1.id == test_tuple.obj) {
					rank2 = i;
					rank_filter_2 = k;
					break;
				}
				if (hypo_o.find(r1.id) == hypo_o.end()) {
					++k;
				}
			}
			std::cout << ++j << " " << rank1 << " " << rank2 << "\n " << rank_filter_1 << " " << rank_filter_2 << std::endl;
			if (rank1 == 0) {
				++rank_1;
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank1 <= 2) {
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank1 <= 4) {
				++rank_5;
				++rank_10;
			} else if (rank1 <= 9) {
				++rank_10;
			}
			
			if (rank2 == 0) {
				++rank_1;
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank2 <= 2) {
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank2 <= 4) {
				++rank_5;
				++rank_10;
			} else if (rank2 <= 9) {
				++rank_10;
			}
			
			mrr += 1.0/(rank1+1.0);
			mrr += 1.0/(rank2+1.0);
			
			if (rank_filter_1 == 0) {
				++rank_1_filter;
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 2) {
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 4) {
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 9) {
				++rank_10_filter;
			}
			
			if (rank_filter_2 == 0) {
				++rank_1_filter;
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 2) {
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 4) {
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 9) {
				++rank_10_filter;
			}
			mrr_filter += 1.0/(rank_filter_1+1.0);
			mrr_filter += 1.0/(rank_filter_2+1.0);
		}
		std::cout << "Raw Rank1: " << (rank_1 / total) << "\n";
		std::cout << "Raw Rank3: " << (rank_3 / total) << "\n";
		std::cout << "Raw Rank5: " << (rank_5 / total) << "\n";
		std::cout << "Raw Rank10: " << (rank_10 / total) << "\n";
		std::cout << "Raw MRR: " << (mrr / total) << "\n";
		
		std::cout << "Filtered Rank1: " << (rank_1_filter / total) << "\n";
		std::cout << "Filtered Rank3: " << (rank_3_filter / total) << "\n";
		std::cout << "Filtered Rank5: " << (rank_5_filter / total) << "\n";
		std::cout << "Filtered Rank10: " << (rank_10_filter / total) << "\n";
		std::cout << "Filtered MRR: " << (mrr_filter / total) << "\n";
	}

private:
	void pre_score_(int subj, int relation, int obj) {
		const std::vector<double>& obj_v = objVec[obj];
		const std::vector<double>& relation_v = relationVec[relation];
		const std::vector<double>& subj_v = subjVec[subj];
		op.product(subj_v, obj_v, relation_v, outVec1, outVec2, outVec3);
	}

	void pre_score_inv(int subj, int relation, int relation_inv, int obj) {
		const std::vector<double>& obj_v = objVec[obj];
		const std::vector<double>& obj_v_inv = subjVec[obj];
		const std::vector<double>& relation_v = relationVec[relation];
		const std::vector<double>& relation_v_inv = relationVec[relation_inv];
		const std::vector<double>& subj_v = subjVec[subj];
		const std::vector<double>& subj_v_inv = objVec[subj];

		op.product(subj_v, obj_v, relation_v, outVec1, outVec2, outVec3);
		op.product(obj_v_inv, subj_v_inv, relation_v_inv, outVec1_inv, outVec2_inv, outVec3_inv);
	}

	double innerProductSubj(int entity, std::vector<double>& vec) {
		return op.innerProduct(subjVec[entity], vec);
	}
	
	double innerProductObj(int entity, std::vector<double>& vec) {
		return op.innerProduct(objVec[entity], vec);
	}
};

int ArgPos(std::string str, int argc, char **argv) {
	int a;
	for (a = 1; a < argc; a++){
		if (str == argv[a]) {
			if (a == argc - 1) {
				std::cout << "Argument missing for" << str << std::endl;
				exit(1);
			}
			return a;
		}
	}
	return -1;
}

int main(int argc, char **argv){
	std::string trainname;
	std::string testname;
	std::string validname;
	std::string subjectname;
	std::string objectname;
	std::string relationname;
	int i;
	if ((i = ArgPos((char *)"-train", argc, argv)) > 0) trainname = argv[i + 1];
	if ((i = ArgPos((char *)"-test", argc, argv)) > 0) testname = argv[i + 1];
	if ((i = ArgPos((char *)"-valid", argc, argv)) > 0) validname = argv[i + 1];
	if ((i = ArgPos((char *)"-subject_model", argc, argv)) > 0) subjectname = argv[i + 1];
	if ((i = ArgPos((char *)"-object_model", argc, argv)) > 0) objectname = argv[i + 1];
	if ((i = ArgPos((char *)"-relation_model", argc, argv)) > 0) relationname = argv[i + 1];
	TestCP test(trainname, testname, validname, subjectname, objectname, relationname);

	bool isInverse = true;
	if (isInverse) {
		test.test_inv();
	} else {
		test.test_();
	}
	return 0;
}
