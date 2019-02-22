#ifndef TRIPLE_HPP
#define TRIPLE_HPP

class Triple {
public:
	int subj, obj, relation, relation_rev;
	bool flag;

	Triple() : flag(false) {}
	Triple(int subj_, int relation_, int obj_) {
		set(subj_, relation_, obj_);
		flag = false;
	}

	Triple(int subj_, int relation_, int relation_rev_, int obj_) {
		set(subj_, relation_, relation_rev_, obj_);
		flag = false;
	}

	void set(int subj_, int relation_, int obj_) {
		subj = subj_;
		obj = obj_;
		relation = relation_;
	}

	void set(int subj_, int relation_, int relation_rev_, int obj_) {
		subj = subj_;
		obj = obj_;
		relation = relation_;
		relation_rev = relation_rev_;
	}
};

#endif // TRIPLE_HPP
