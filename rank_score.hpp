#ifndef RANK_SCORE_HPP
#define RANK_SCORE_HPP

class RankScore {
public:
	double score;
	int id;

	RankScore() {}

	void set(int id_, double score_) {
		id = id_;
		score = score_;
	}

	bool operator<(const RankScore& o) const {
		return score > o.score;
	}
};

#endif
