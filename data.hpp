#ifndef DATA_HPP
#define DATA_HPP

#include "triple.hpp"
#include "triple_key_gen.hpp"

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

class Data {
public:
	int entity_counter, relation_counter;
	std::map<std::string, int>  entity_dict, relation_dict;
	std::map<int, std::string> entity_dict_rev, relation_dict_rev;
	std::set<long long> triple_key_set;
	std::vector<Triple> triples;
	TripleKeyGen key_gen;

	Data() : entity_counter(0), relation_counter(0) {}

	void readFromRawFile(std::string filename) {
		std::ifstream fin;
		std::string word;
		std::vector<std::string> tmp;

		fin.open(filename);
		while (getline(fin, word, '\t')) {
			tmp.push_back(word);
			if ((int)tmp.size() == 3) {
				int subj = addEntity(tmp[0]);
				int obj = addEntity(tmp[2]);
				int relation = addRelation(tmp[1]);
				triples.emplace_back(subj, relation, obj);
				tmp.clear();
			}
		}

		key_gen = TripleKeyGen(entity_counter, relation_counter);
		for (const Triple& t : triples) {
			long long key = key_gen.calcKey(t.subj, t.relation, t.obj);
			triple_key_set.insert(key);
		}
		fin.close();
	}

private:
	int addEntity(std::string entity) {
		if (entity_dict.find(entity) == entity_dict.end()) {
			int id = entity_counter;
			entity_dict[entity] = entity_counter;
			entity_dict_rev[entity_counter] = entity;
			++entity_counter;
			return id;
		} else {
			return entity_dict[entity];
		}
	}

	int addRelation(std::string relation) {
		if (relation_dict.find(relation) == relation_dict.end()) {
			int id = relation_counter;
			relation_dict[relation] = relation_counter;
			relation_dict_rev[relation_counter] = relation;
			++relation_counter;
			return id;
		} else {
			return relation_dict[relation];
		}
	}
};

#endif
