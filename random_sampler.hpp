#ifndef RANDOM_SAMPLER_HPP
#define RANDOM_SAMPLER_HPP

#include "triple.hpp"
#include "triple_key_gen.hpp"

#include <random>
#include <set>

class RandomSampler {
public:
	std::mt19937 rnd;
	Triple triple;
	TripleKeyGen key_gen;
	std::uniform_int_distribution<> dist_ent;
	std::uniform_int_distribution<> dist_rel;

	RandomSampler(TripleKeyGen& key_gen_, int sizeEnt, int sizeRelation, std::mt19937& rnd_) : key_gen(key_gen_), rnd(rnd_) {
		dist_ent = std::uniform_int_distribution<>(0, sizeEnt - 1);
		dist_rel = std::uniform_int_distribution<>(0, sizeRelation - 1);
	}

	int getNextEnt() {
		return dist_ent(rnd);
	}

	int getNextRel() {
		return dist_rel(rnd);
	}

	Triple negativeSamplingSbj(Triple& gold, std::set<long long>& triple_key_set) {
		for (;;) {
			int entity1 = getNextEnt();
			if (triple_key_set.find(key_gen.calcKey(entity1, gold.relation, gold.obj)) == triple_key_set.end()) 
			{
				triple.set(entity1, gold.relation, gold.obj);
				return triple;
			}
		}
	}
	
	Triple negativeSamplingObj(Triple& gold, std::set<long long>& triple_key_set) {
		for (;;) {
			int entity1 = getNextEnt();
			if (triple_key_set.find(key_gen.calcKey(gold.subj, gold.relation, entity1)) == triple_key_set.end()) 
			{
				triple.set(gold.subj, gold.relation, entity1);
				return triple;
			}
		}
	}
	
	Triple negativeSamplingRel(Triple& gold, std::set<long long>& triple_key_set) {
		for (;;) {
			int rel = getNextRel();
			if (triple_key_set.find(key_gen.calcKey(gold.subj, rel, gold.obj)) == triple_key_set.end()) 
			{
				triple.set(gold.subj, rel, gold.obj);
				return triple;
			}
		}
	}
};

#endif