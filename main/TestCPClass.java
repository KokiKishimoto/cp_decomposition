package main;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Set;

import gnu.trove.THashMap;
import gnu.trove.THashSet;
import util.FileUtility;
import util.OptionParser;


public class TestCPClass {
	public class Key {
		int r, e;
		public Key(int r, int e) {
			this.r = r;
			this.e = e;
		}
		@Override
		public int hashCode() {
			return e << 12 | r;
		}
		@Override
		public boolean equals(Object o) {
			if (o instanceof Key) {
				Key k = (Key)o;
				if (k.r == r && k.e == e) {
					return true;
				}
			}
			return false;
		}
	}
	
	TupleData trainData;
	TestTupleData testData;
	ParamVector[] subjVec, objVec, relationVec;
	double[] outVec1, outVec2, outVec3;
	double[] outVec1_inv, outVec2_inv, outVec3_inv;
	SignalOperation op;
	int vecDim;
	
	public TestCPClass(TestOptions options) throws IOException {
		trainData = TupleData.openObjectFile(options.dataFile); // new TupleData();
		testData = new TestTupleData();
		testData.readFromRawFileRev(options.testFile, options.encoding, trainData);
		
		// isInverse = options.isInverse;
		
		BufferedReader br = FileUtility.getReadFileStream(options.modelSubjFile, options.encoding);
		String line = br.readLine();
		String[] conts = line.trim().split("\\s+");
		int entitySize = Integer.parseInt(conts[0]);
		vecDim = Integer.parseInt(conts[1]);
		// import subj entity vectors
		subjVec = new ParamVector[entitySize];
		int i,j;
		for (i=0; i<entitySize; ++i) {
			line = br.readLine();
			conts = line.trim().split("\\s+");
			subjVec[i] = new ParamVector(vecDim);
			for (j=0; j<vecDim; ++j) {
				subjVec[i].values[j] = Double.parseDouble(conts[j+1]);
			}
		}
		FileUtility.closeReadFile(br);
		
		br = FileUtility.getReadFileStream(options.modelObjFile, options.encoding);
		line = br.readLine();
		// import obj entity vectors
		objVec = new ParamVector[entitySize];
		for (i=0; i<entitySize; ++i) {
			line = br.readLine();
			conts = line.trim().split("\\s+");
			objVec[i] = new ParamVector(vecDim);
			for (j=0; j<vecDim; ++j) {
				objVec[i].values[j] = Double.parseDouble(conts[j+1]);
			}
		}
		FileUtility.closeReadFile(br);
		
		// import relation vectors
		br = FileUtility.getReadFileStream(options.modelRFile, options.encoding);
		line = br.readLine();
		conts = line.trim().split("\\s+");
		int relationSize = Integer.parseInt(conts[0]);
		relationVec = new ParamVector[relationSize];
		for (i=0; i<relationSize; ++i) {
			line = br.readLine();
			conts = line.trim().split("\\s+");
			relationVec[i] = new ParamVector(vecDim);
			for (j=0; j<vecDim; ++j) {
				relationVec[i].values[j] = Double.parseDouble(conts[j+1]);
			}
		}
		FileUtility.closeReadFile(br);
		
		// init score vectors
		op = new SignalOperation();
		outVec1 = new double[vecDim];
		outVec2 = new double[vecDim];
		outVec3 = new double[vecDim];
		
		outVec1_inv = new double[vecDim];
		outVec2_inv = new double[vecDim];
		outVec3_inv = new double[vecDim];
		
		// subj__ = new double[vecDim];
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		OptionParser optionParser = new OptionParser(TestOptions.class);
		TestOptions options = (TestOptions) optionParser.parse(args);
		TestCPClass tester = null;
		try {
			tester = new TestCPClass(options);
		} catch (IOException e) {
			e.printStackTrace();
		}
		tester.test_inv();
	}
	
	final private void pre_score_inv(int subj, int relation, int relation_inv, int obj) {
		final double[] obj_v = objVec[obj].values;
		final double[] obj_v_inv = subjVec[obj].values;
		final double[] relation_v = relationVec[relation].values;
		final double[] relation_v_inv = relationVec[relation_inv].values;
		final double[] subj_v = subjVec[subj].values; //weightVec_[subj].values;
		final double[] subj_v_inv = objVec[subj].values;
		op.product(subj_v, obj_v, relation_v, outVec1, outVec2, outVec3);
		op.product(obj_v_inv, subj_v_inv, relation_v_inv, outVec1_inv, outVec2_inv, outVec3_inv);
		// op.ccorr(obj_v, relation_v, outVec1__);
		// op.ccorr(subj_v, relation_v, outVec2__);
	}
	
	final private double innerProductSubj(int entity, double[] vec) {
		return op.innerProduct(subjVec[entity].values, vec);
	}
	
	final private double innerProductObj(int entity, double[] vec) {
		return op.innerProduct(objVec[entity].values, vec);
	}
	
	private void test_inv() {
		double total = (double)testData.tuples_list.size();
		double correct = 0;
		int j=0;
		for (Tuple test_tuple : testData.tuples_list) {
			if (test_tuple.flag) {
				total = total - 1;
				continue;
			}
			pre_score_inv(test_tuple.subj, test_tuple.relation, test_tuple.relation_rev, test_tuple.obj);
			double score = innerProductSubj(test_tuple.subj, outVec2) + innerProductObj(test_tuple.subj, outVec3_inv);
			if (score > 0.d) {
				correct++;
			}
		}
		System.out.println("CA: " + (correct / total));
	}
}
