#include "signal_operation.hpp"
#include "updater.hpp"
#include "random_sampler.hpp"
#include "data.hpp"
#include "triple.hpp"
#include "model_file_writer.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <random>
#include <set>
#include <string>
#include <vector>

class CP {
public:
	SignalOperation op;
	Updater updater;
	std::vector< std::vector<double> > subjVec, objVec, relationVec;
	Data traindata;
	std::vector<double> outVec1, outVec2, outVec3;
	std::vector<double> norms;

	double fs;
	double lambda;
	int vecDim;
	std::mt19937 rnd; //int randomSeed;

	CP(Data& traindata, int vecDim_, int randomSeed, double learningRate, double threshold, double lambda_) : vecDim(vecDim_), lambda(lambda_) {
		op = SignalOperation();

		rnd = std::mt19937(randomSeed);
		double bnd = sqrt(6.0) / sqrt((double)vecDim * 2);
		std::uniform_real_distribution<> dist(-bnd, bnd);

		subjVec.resize(traindata.entity_counter);
		initializeModel(subjVec, traindata.entity_counter, dist);

		objVec.resize(traindata.entity_counter);
		initializeModel(objVec, traindata.entity_counter, dist);

		relationVec.resize(traindata.relation_counter);
		initializeModel(relationVec, traindata.relation_counter, dist);

		updater = Updater(learningRate, threshold);

		outVec1.resize(vecDim);
		outVec2.resize(vecDim);
		outVec3.resize(vecDim);

		norms.resize(3);
	}
	void initializeModel(std::vector< std::vector<double> >& Vec, const int counter, std::uniform_real_distribution<>& dist){
		for (int i = 0; i < counter; i++) {
			Vec[i].resize(vecDim);
			for (int j = 0; j < vecDim; j++) {
				Vec[i][j] = dist(rnd);
			}
		}
	}

	double sigmoid(double x) { return 1.0 / (1.0 + std::exp(-x));}

	double computeGradients(Triple& triple, double y) {
		const std::vector<double>& subj_v = subjVec[triple.subj];
		const std::vector<double>& obj_v = objVec[triple.obj];
		const std::vector<double>& relation_v = relationVec[triple.relation];

		op.product(subj_v, obj_v, relation_v, outVec1, outVec2, outVec3);

		double score = op.innerProduct(relation_v, outVec1);

		double yscore = y * score;
		double loss = std::log(1.0 + std::exp(-yscore));
		fs = -(y * sigmoid(-yscore));

		for (int i = 0; i < vecDim; i++) {
			outVec1[i] = fs * outVec1[i];
			outVec2[i] = fs * outVec2[i];
			outVec3[i] = fs * outVec3[i];
		}

		norms[0] = op.norm(outVec1);
		norms[1] = op.norm(outVec2);
		norms[2] = op.norm(outVec3);

		return loss;
	}

	void train(Data& data, int maxEpoch, int negativeSize, ModelFileWriter& mfw) {
		int sampleC = 1;
		double all = 0.0;
		double loss = 0.0;
		std::vector<Triple> train_triples = data.triples;
		std::set<long long> triple_key_set = data.triple_key_set;
		RandomSampler sampler(data.key_gen, data.entity_counter, data.relation_counter, rnd);

		for (int iter = 1; iter <= maxEpoch; ++iter) {
			std::shuffle(train_triples.begin(), train_triples.end(), rnd);
			std::cerr << "Epoch " << iter << ":" << std::endl;
			for (Triple& triple : train_triples) {
				if ((sampleC % 1000) == 0) {
					std::cerr << sampleC << " ";
					++all;
					updater.decay(all);
				}
				++sampleC;
				for (int i = 0; i <= negativeSize; i++) {
					if (i == 0) {
						loss += computeGradients(triple, 1.0);
						updater.SGD(triple, outVec1, outVec2, outVec3, norms, subjVec, objVec, relationVec);
					} else {
						Triple neg_triple = sampler.negativeSamplingSbj(triple, triple_key_set);
						loss += computeGradients(neg_triple, -1.0);
						updater.SGD(neg_triple, outVec1, outVec2, outVec3, norms, subjVec, objVec, relationVec);

						neg_triple = sampler.negativeSamplingObj(triple, triple_key_set);
						loss += computeGradients(neg_triple, -1.0);
						updater.SGD(neg_triple, outVec1, outVec2, outVec3, norms, subjVec, objVec, relationVec);
					}
				}
				if (updater.learningRate <= 0.0) {
					std::cerr << std::endl;
					std::cerr << "Learning rate was zero " << std::endl;
					std::cerr << "Total loss: " << loss << std::endl;
					mfw.save(subjVec, objVec, relationVec, op, data, iter);
				}
			}
			std::cerr << std::endl;
			std::cerr << std::fixed << std::setprecision(15) << "Total loss: " << loss << std::endl;

			loss = 0.0;
			sampleC = 1;

			if (iter % 10 == 0) {
				mfw.save(subjVec, objVec, relationVec, op, data, iter);
			}
		}
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
	int dimension;
	int iteration;
	double rate;
	double lambda;
	int i;
	if ((i = ArgPos((char *)"-train", argc, argv)) > 0) trainname = argv[i + 1];
	if ((i = ArgPos((char *)"-dimension", argc, argv)) > 0) dimension = atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-iteration", argc, argv)) > 0) iteration = atoi(argv[i + 1]);
	if ((i = ArgPos((char *)"-rate", argc, argv)) > 0) rate = std::stof(argv[i + 1]);
	if ((i = ArgPos((char *)"-lambda", argc, argv)) > 0) lambda = std::stof(argv[i + 1]);

	Data data;
	data.readFromRawFile(trainname);

	int randomSeed = 0;
	double threshold = 5.0;
	CP cp(data, dimension, randomSeed, rate, threshold, lambda);

	ModelFileWriter mfw;
	int negativeSize = 5;
	cp.train(data, iteration, negativeSize, mfw);

	return 0;
}
