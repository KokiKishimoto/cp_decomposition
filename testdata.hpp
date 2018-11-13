#ifndef TESTDATA_HPP
#define TESTDATA_HPP

#include "triple.hpp"
#include "data.hpp"

#include <vector>

class TestData {
public:
	std::vector<Triple> triples;

	TestData() {}

	void readFromRawFile(std::string filename, Data& traindata) {
		std::ifstream fin;
		std::string word;
		std::vector<std::string> tmp;

		fin.open(filename);
		while (getline(fin, word, '\t')) {
			tmp.push_back(word);
			if ((int)tmp.size() == 3) {
				int subj = addEntity(tmp[0], traindata);
				int obj = addEntity(tmp[2], traindata);
				int relation = addRelation(tmp[1], traindata);
				triples.emplace_back(subj, relation, obj);
				if (subj == -1 || obj == -1 || relation == -1) triples.back().flag = true;
				tmp.clear();
			}
		}
		fin.close();
	}

	void readFromRawFileRev(std::string filename, Data& traindata) {
		std::ifstream fin;
		std::string word;
		std::vector<std::string> tmp;

		fin.open(filename);
		while (getline(fin, word, '\t')) {
			tmp.push_back(word);
			if ((int)tmp.size() == 3) {
				int subj = addEntity(tmp[0], traindata);
				int obj = addEntity(tmp[2], traindata);
				int relation = addRelation(tmp[1], traindata);
				int relation_rev = addRelation("**" + tmp[1], traindata);
				triples.emplace_back(subj, relation, relation_rev, obj);
				if (subj == -1 || obj == -1 || relation == -1 || relation_rev == -1) triples.back().flag = true;
				tmp.clear();
			}
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