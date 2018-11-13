#ifndef TESTDATA_HPP
#define TESTDATA_HPP

#include "triple.hpp"
#include "data.hpp"

#include <iostream>
#include <vector>

class TestData {
public:
	std::vector<Triple> triples;

	TestData() {}

	void readFromRawFile(std::string filename, Data& traindata) {
		std::ifstream fin;

		fin.open(filename);
		std::string t0, t1, t2;
		while (fin >> t0 >> t1 >> t2) {
			int subj = addEntity(t0, traindata);
			int obj = addEntity(t2, traindata);
			int relation = addRelation(t1, traindata);
			triples.emplace_back(subj, relation, obj);
			if (subj == -1 || obj == -1 || relation == -1) triples.back().flag = true;
		}
		fin.close();
	}

	void readFromRawFileRev(std::string filename, Data& traindata) {
		std::ifstream fin;

		fin.open(filename);
		std::string t0, t1, t2;
		while (fin >> t0 >> t1 >> t2) {
			int subj = addEntity(t0, traindata);
			int obj = addEntity(t2, traindata);
			int relation = addRelation(t1, traindata);
			int relation_rev = addRelation("**" + t1, traindata);
			triples.emplace_back(subj, relation, relation_rev, obj);
			if (subj == -1 || obj == -1 || relation == -1 || relation_rev == -1) triples.back().flag = true;
		}
		fin.close();
	}

private:
	int addEntity(std::string entity, Data& data) {
		if (data.entity_dict.find(entity) == data.entity_dict.end()) {
			return -1;
		} else {
			return data.entity_dict[entity];
		}
	}

	int addRelation(std::string relation, Data& data) {
		if (data.relation_dict.find(relation) == data.relation_dict.end()) {
			return -1;
		} else {
			return data.relation_dict[relation];
		}
	}
};
#endif