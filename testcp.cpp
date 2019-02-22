#include "rank_score.hpp"
#include "data.hpp"
#include "testdata.hpp"
#include "signal_operation.hpp"
#include "model_file_load.hpp"

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
	ModelFileLoad subjData, objData, relationData;
	std::vector< std::vector<double> > subjVec, objVec, relationVec;
	std::vector<double> outVec1, outVec2, outVec3;
	std::vector<double> outVec1_inv, outVec2_inv, outVec3_inv;
	SignalOperation op;
	std::map<Key, std::set<int> > subjSet, objSet;
	int vecDim;
	int k;
	std::vector<double> rank1, rank2;
	std::vector<double> rank_filter_1, rank_filter_2;

	TestCP(std::string dataFile, std::string testFile, std::string validFile, std::string modelSubjFile, std::string modelObjFile, std::string modelRelFile) {
		trainData = Data();
		trainData.readFromRawFile(dataFile);

		testData = TestData();
		testData.readFromRawFileRev(testFile, trainData);

		scores_subj.resize(trainData.entity_counter, RankScore());
		scores_obj.resize(trainData.entity_counter, RankScore());

		subjData = ModelFileLoad();
		subjData.model_load(modelSubjFile);
		subjVec = subjData.Vec;

		objData = ModelFileLoad();
		objData.model_load(modelObjFile);
		objVec = objData.Vec;

		relationData = ModelFileLoad();
		relationData.model_load(modelRelFile);
		relationVec = relationData.Vec;

		vecDim = subjData.vecDim;

		op = SignalOperation();
		outVec1.resize(vecDim);
		outVec2.resize(vecDim);
		outVec3.resize(vecDim);

		outVec1_inv.resize(vecDim);
		outVec2_inv.resize(vecDim);
		outVec3_inv.resize(vecDim);

		generate_key(trainData.triples);
		generate_key(testData.triples);

		validData = TestData();
		validData.readFromRawFile(validFile, trainData);
		generate_key(validData.triples);
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
			generate_rank(scores_subj, test_tuple.relation, test_tuple.subj, test_tuple.obj, subjSet, rank1, rank_filter_1);
			generate_rank(scores_obj, test_tuple.relation, test_tuple.obj, test_tuple.subj, objSet, rank2, rank_filter_2);

		}
		rank_1 = calculate_ranking(rank1, rank2, 1);
		rank_3 = calculate_ranking(rank1, rank2, 3);
		rank_5 = calculate_ranking(rank1, rank2, 5);
		rank_10 = calculate_ranking(rank1, rank2, 10);
		mrr = calate_mrr(rank1, rank2);

		rank_1_filter = calculate_ranking(rank_filter_1, rank_filter_2, 1);
		rank_3_filter = calculate_ranking(rank_filter_1, rank_filter_2, 3);
		rank_5_filter = calculate_ranking(rank_filter_1, rank_filter_2, 5);
		rank_10_filter = calculate_ranking(rank_filter_1, rank_filter_2, 10);
		mrr_filter = calate_mrr(rank_filter_1, rank_filter_2);

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
	double calate_mrr(const std::vector<double>& a, const std::vector<double>& b){
		double sum = 0.0;
		double mrr_value = 0.0;
		for(int i=0; i<a.size(); i++){
			sum = sum + 1/(a[i]+1) + 1/(b[i]+1);
		}
		mrr_value = sum;
		return mrr_value;
	}

	double calculate_ranking(std::vector<double>& a, std::vector<double>& b, int rank){
		double rank_num = 0.0;
		for(int i=0; i<a.size(); i++){
			if(a[i]<rank){
				rank_num++;       
			}
			if(b[i]<rank){
				rank_num++;       
			}
		}
		return rank_num;
	}
	void generate_key(std::vector<Triple>& triples){
		for (Triple& t : triples) {
			Key k1(t.relation, t.obj);
			subjSet[k1].insert(t.subj);

			Key k2(t.relation, t.subj);
			objSet[k2].insert(t.obj);
		}
	}
	void generate_rank(std::vector<RankScore>& scores_ent, int relation, int ent1, int ent2, std::map<Key, std::set<int> >& entSet, std::vector<double>& rank, std::vector<double>& rank_filter){
		std::sort(scores_ent.begin(), scores_ent.end());
		Key k_e(relation, ent2);
		const std::set<int>& hypo_e = entSet[k_e];
		int k = 0;
		for (int i=0; i<scores_ent.size(); ++i) {
			RankScore& re = scores_ent[i];
			if (re.id == ent1) {
				rank.push_back(i);
				rank_filter.push_back(k);
				std::cout << i << ' ' << k << std::endl;
				break;
			}
			if (hypo_e.find(re.id) == hypo_e.end()) {
				++k;
			}
		}
	}

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

	test.test_inv();
	return 0;
}
