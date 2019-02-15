#ifndef MODEL_FILE_WRITER_HPP
#define MODEL_FILE_WRITER_HPP

#include "data.hpp"
#include "signal_operation.hpp"

#include <iostream>
#include <string>
#include <vector>

class ModelFileLoad{
public:
	int Size, vecDim;
	std::vector< std::vector<double> > Vec;
	std::string modelFile;
	ModelFileLoad() {}
	void model_load(std::string modelfile){
		std::ifstream fin;
		
		fin.open(modelfile);
		fin >> Size >> vecDim;
		Vec.resize(Size);
		for (int i = 0; i < Size; i++) {
			std::string tmp;
			fin >> tmp;
			Vec[i].resize(vecDim);
			for (int j = 0; j < vecDim; j++) {
				fin >> Vec[i][j];
			}
		}
		fin.close();
	}
};
#endif 
