#ifndef SIGNAL_OPERATION_HPP
#define SIGNAL_OPERATION_HPP

#include <cmath>
#include <vector>

class SignalOperation {
public:
	SignalOperation() {}

	double norm(const std::vector<double>& a) {
		return std::sqrt(innerProduct(a, a));
	}

	double innerProduct(const std::vector<double>& a, const std::vector<double>& b) {
		double score = 0.0;
		for (int i = 0; i < a.size(); i++) {
			score += a[i] * b[i];
		}
		return score;
	}

	std::vector<double> product(const std::vector<double>& a, const std::vector<double>& b) {
		std::vector<double> out(a.size());
		for (int i = 0; i < a.size(); i++) {
			out[i] = a[i] * b[i];
		}
		return out;
	}

	std::vector<double> product(const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c, std::vector<double>& out1, std::vector<double>& out2, std::vector<double>& out3) {
		for (int i = 0; i < a.size(); i++) {
			out1[i] = a[i] * b[i];
			out2[i] = b[i] * c[i];
			out3[i] = c[i] * a[i];
		}
		return out1;
	}
};

#endif // SIGNAL_OPERATION_HPP