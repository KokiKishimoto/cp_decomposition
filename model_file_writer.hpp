#ifndef MODEL_FILE_WRITER_HPP
#define MODEL_FILE_WRITER_HPP

#include "data.hpp"
#include "signal_operation.hpp"

#include <iostream>
#include <string>
#include <vector>

class ModelFileWriter {
public:
	ModelFileWriter() {}

	void save(std::vector< std::vector<double> >& subjVec, std::vector< std::vector<double> >& objVec, std::vector< std::vector<double> >& relationVec, SignalOperation& op, Data& data, int iter) {
		std::ofstream fout;
		fout.open("./model/" + std::to_string(iter) + "_subject.txt");

		fout << data.entity_counter << " " << subjVec[0].size() << "\n";
		for (int i = 0; i < data.entity_counter; i++) {
			const std::vector<double>& vec = subjVec[i];
			const double norm = op.norm(vec);
			fout << data.entity_dict_rev[i] << " ";

			for (int j = 0; j < vec.size(); j++) {
				if (j > 0) fout << " ";
				fout << vec[j] / norm;
			}
			fout << "\n";
		}

		fout.close();


		fout.open("./model/" + std::to_string(iter) + "_object.txt");

		fout << data.entity_counter << " " << objVec[0].size() << "\n";
		for (int i = 0; i < data.entity_counter; i++) {
			const std::vector<double>& vec = objVec[i];
			const double norm = op.norm(vec);
			fout << data.entity_dict_rev[i] << " ";

			for (int j = 0; j < vec.size(); j++) {
				if (j > 0) fout << " ";
				fout << vec[j] / norm;
			}
			fout << "\n";
		}

		fout.close();


		fout.open("./model/" + std::to_string(iter) + "_relation.txt");

		fout << data.relation_counter << " " << relationVec[0].size() << "\n";
		for (int i = 0; i < data.relation_counter; i++) {
			const std::vector<double>& vec = relationVec[i];
			const double norm = op.norm(vec);
			fout << data.relation_dict_rev[i] << " ";

			for (int j = 0; j < vec.size(); j++) {
				if (j > 0) fout << " ";
				fout << vec[j] / norm;
			}
			fout << "\n";
		}

		fout.close();
	}
};
#endif 