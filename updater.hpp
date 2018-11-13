#ifndef UPDATER_HPP
#define UPDATER_HPP

#include "triple.hpp"

#include <cmath>
#include <vector>

class Updater {
public:
	double initialRate, learningRate, lambda;
	double threshold;
	double eta;

	std::vector< std::vector<double> > subjVec, objVec, relationVec;

	Updater() {}

	Updater(const std::vector< std::vector<double> >& subjVec_, const std::vector< std::vector<double> >& objVec_, const std::vector< std::vector<double> >& relationVec_, double learningRate_, double threshold_) 
	: subjVec(subjVec_), objVec(objVec_), relationVec(relationVec_), initialRate(learningRate_), learningRate(learningRate_), threshold(threshold_) {
		lambda = 1.0 / std::pow(10.0, 8.0);
	}

	void decay(double time) {
		learningRate = initialRate / (1.0 + initialRate * lambda * time);
	}

	void SGD(Triple& triple, std::vector<double>& gradient_r, std::vector<double>& gradient_s, std::vector<double>& gradient_o, std::vector<double>& norms) {
		std::vector<double> r_v = relationVec[triple.relation];
		if (norms[0] >= threshold) {
			double x = (threshold / norms[0]);
			for (int i = 0; i < gradient_r.size(); i++) {
				r_v[i] -= learningRate * x * gradient_r[i];
			}
		} else {
			for (int i = 0; i < gradient_r.size(); i++) {
				r_v[i] -= learningRate * gradient_r[i];
			}			
		}

		std::vector<double> s_e_v = subjVec[triple.subj];
		if (norms[1] >= threshold) {
			double x = (threshold / norms[1]);
			for (int i = 0; i < gradient_s.size(); i++) {
				s_e_v[i] -= learningRate * x * gradient_s[i];
			}
		} else {
			for (int i = 0; i < gradient_s.size(); i++) {
				s_e_v[i] -= learningRate * gradient_s[i];
			}			
		}

		std::vector<double> o_e_v = objVec[triple.obj];
		if (norms[2] >= threshold) {
			double x = (threshold / norms[2]);
			for (int i = 0; i < gradient_o.size(); i++) {
				o_e_v[i] -= learningRate * x * gradient_o[i];
			}
		} else {
			for (int i = 0; i < gradient_o.size(); i++) {
				o_e_v[i] -= learningRate * gradient_o[i];
			}			
		}		
	}
};

#endif // UPDATER_HPP