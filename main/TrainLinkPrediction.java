package main;

import util.OptionParser;	

public class TrainLinkPrediction {
	public static void main(String[] args) {
		OptionParser optionParser = new OptionParser(TrainOptions.class);
		TrainOptions options = (TrainOptions) optionParser.parse(args);
		TupleData trainData = new TupleData();
		trainData.readFromRawFile(options.trainFile, options.encoding);
		
		if (options.testFile != null) {
			trainData.registerTuples(options.testFile, options.encoding);
		}
		if (options.validFile != null) {
			trainData.registerTuples(options.validFile, options.encoding);
		}
		
		RandomSampler sampler = new RandomSampler(trainData.key_gen, trainData.entity_counter, trainData.relation_counter, options.randomSeed);
		
		if (options.dataFile != null) {
			trainData.writeObjectFile(options.dataFile);
		}
		ModelFileWriter mfw = ModelFileWriter.getInstance(options.outputFile, options.encoding);
		if (options.isNormal) {
			CP model = new CP(trainData, options.vecDim, options.randomSeed, options.learningRate, options.normClip, options.lambda);
			model.train(trainData, sampler, options.maxEpoch, options.negativeSize, mfw);
		} else {
			QuantizeCP model = new QuantizeCP(trainData, options.vecDim, options.randomSeed, options.learningRate, options.normClip, options.lambda);
			model.train(trainData, sampler, options.maxEpoch, options.negativeSize, mfw);
		}
	}
}
