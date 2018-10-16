package main;

import java.util.Collections;
import java.util.List;
import java.util.Random;
import java.util.Set;

public class CP {
	SignalOperation op;
	Updater updater;
	ParamVector[] subjVec, objVec, relationVec;
	TupleData traindata;
	double[] outVec1, outVec2, outVec3;
	double[] norms;
	
	double fs;
	double lambda;
	int vecDim; // vecDim_r;
	int randomSeed;
	
	public CP(
			TupleData traindata, 
			int vecDim, 
			int randomSeed,
			double learningRate, 
			double threshold,
			double lambda) 
	{
		this.traindata = traindata;
		op = new SignalOperation(/* vecDim */);
		subjVec = new ParamVector[traindata.entity_counter];
		objVec = new ParamVector[traindata.entity_counter];
		int i;
		for (i=0; i<traindata.entity_counter; ++i) {
			subjVec[i] = new ParamVector(vecDim);
			subjVec[i].initByUniformNormDist(i+randomSeed, op, false);
		}
		
		for (i=0; i<traindata.entity_counter; ++i) {
			objVec[i] = new ParamVector(vecDim);
			objVec[i].initByUniformNormDist(traindata.entity_counter+i+randomSeed, op, false);
		}
		relationVec = new ParamVector[traindata.relation_counter];
		for (i=0; i<traindata.relation_counter; ++i) {
			relationVec[i] = new ParamVector(vecDim);
			relationVec[i].initByUniformNormDist(traindata.entity_counter*2+i+randomSeed, op, false);
		}
		updater = new Updater(subjVec, objVec, relationVec, learningRate, threshold);
		outVec1 = new double[vecDim];
		outVec2 = new double[vecDim];
		outVec3 = new double[vecDim];
		this.vecDim = vecDim;
		this.lambda = lambda;
		norms = new double[3];
		this.randomSeed = randomSeed;
	}
	
	final public double sigmoid(double x) { return 1.d / (1.d + Math.exp(-x)); }
	
	public double computeGradients(Tuple tuple, double y) 
	{
		final double[] subj_v = subjVec[tuple.subj].values;
		final double[] obj_v = objVec[tuple.obj].values;
		final double[] relation_v = relationVec[tuple.relation].values;
		
		op.product(subj_v, obj_v, relation_v, outVec1, outVec2, outVec3);
		
		double score = op.innerProduct(relation_v, outVec1);
		
		double yscore = y * score;
		double loss = Math.log(1.d+Math.exp(-yscore));
		fs = -(y * sigmoid(-yscore));
		
		for (int i=0; i<vecDim; ++i) {
			outVec1[i] = fs * outVec1[i];
			outVec2[i] = fs * outVec2[i];
			outVec3[i] = fs * outVec3[i];
 		}
		
		norms[0] = op.norm(outVec1);
		norms[1] = op.norm(outVec2);
		norms[2] = op.norm(outVec3);
		
		if (loss == Double.POSITIVE_INFINITY || 
				loss == Double.NEGATIVE_INFINITY || 
				loss == Double.NaN) 
			System.err.println(yscore + " " + loss + " " + fs);
		return loss;
	}
	
	public void train(TupleData data, 
			RandomSampler sampler,
			int maxEpoch,
			int negativeSize,
			ModelFileWriter mfw) 
	{
		int sampleC = 1;
		double all = 0.d;
		double loss = 0.d;
		int i;
		List<Tuple> train_tuples = data.tuple_list;
		Set<Long> tuple_key_set = data.tuple_key_set;
		for (int iter = 1; iter <= maxEpoch; ++iter) {
			Collections.shuffle(train_tuples, new Random(iter+randomSeed));
			System.err.format("Epoch %d:\n", iter);
			for (Tuple tuple : train_tuples) {
				if ((sampleC % 1000) == 0) {
					System.err.format("%d ", sampleC);
					++all;
					updater.decay(all);
				}
				++sampleC;
				for (i=0; i<=negativeSize; ++i) {
					if (i == 0) {
						loss += computeGradients(tuple, 1.d);
						updater.SGD(tuple, outVec1, outVec2, outVec3, norms, lambda);
					} else {
						Tuple neg_tuple = sampler.negativeSamplingSbj(tuple, tuple_key_set);
						loss += computeGradients(neg_tuple, -1.d);
						updater.SGD(neg_tuple, outVec1, outVec2, outVec3, norms, lambda);
						
						neg_tuple = sampler.negativeSamplingObj(tuple, tuple_key_set);
						loss += computeGradients(neg_tuple, -1.d);
						updater.SGD(neg_tuple, outVec1, outVec2, outVec3, norms, lambda);
					}
				}
				if (updater.learningRate <= 0.d) {
					System.err.format("\n");
					System.err.format("Learning rate was zero \n");
					System.err.format("Total loss: %f\n", loss);
					if (mfw != null) {
						mfw.save(subjVec, objVec, relationVec, op, data, iter);
					}
					return ;
				}
			}
			System.err.format("\n");
			System.err.format("Total loss: %f\n", loss);
			loss = 0.d;
			sampleC = 1;
	
			if (mfw != null && iter % 10 == 0) {
				mfw.save(subjVec, objVec, relationVec, op, data, iter);
			}
		}
	}
}
