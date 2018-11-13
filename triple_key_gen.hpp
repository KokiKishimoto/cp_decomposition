#ifndef TRIPLE_KEY_GEN_HPP
#define TRIPLE_KEY_GEN_HPP

class TripleKeyGen {
public:
	int shift1, shift2;

	TripleKeyGen() {}

	TripleKeyGen(int e_size, int r_size) {
		shift1 = e_size * r_size;
		shift2 = r_size;
	}

	long long calcKey(int s, int r, int o) {
		long long x = s * shift1 + o * shift2 + r;
		return x;
	}
};

#endif // TUPLE_KEY_GEN_HPP