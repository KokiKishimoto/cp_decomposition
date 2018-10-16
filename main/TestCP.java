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


public class TestCP {
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
	
	List<RankScore> scores_subj, scores_obj;
	TupleData trainData;
	TestTupleData testData, validData;
	ParamVector[] subjVec, objVec, relationVec;
	double[] outVec1, outVec2, outVec3;
	double[] outVec1_inv, outVec2_inv, outVec3_inv;
	SignalOperation op;
	Map<Key, Set<Integer>> subjSet, objSet;
	int vecDim;
	boolean isInverse;
	
	public TestCP(TestOptions options) throws IOException {
		trainData = TupleData.openObjectFile(options.dataFile); // new TupleData();
		testData = new TestTupleData();
		testData.readFromRawFileRev(options.testFile, options.encoding, trainData);
		scores_subj = new ArrayList<RankScore>(trainData.entity_counter);
		scores_obj = new ArrayList<RankScore>(trainData.entity_counter);
		
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
			scores_subj.add(new RankScore());
			scores_obj.add(new RankScore());
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
		
		subjSet = new THashMap<Key, Set<Integer>>();
		objSet = new THashMap<Key, Set<Integer>>();
		for (Tuple t : trainData.tuple_list) {
			Key k1 = new Key(t.relation, t.obj);
			if (!subjSet.containsKey(k1)) {
				Set<Integer> s = new THashSet<Integer>();
				s.add(t.subj);
				subjSet.put(k1, s);
			} else {
				Set<Integer> s = subjSet.get(k1);
				s.add(t.subj);
				subjSet.put(k1, s);
			}
			Key k2 = new Key(t.relation, t.subj);
			if (!objSet.containsKey(k2)) {
				Set<Integer> s = new THashSet<Integer>();
				s.add(t.obj);
				objSet.put(k2, s);
			} else {
				Set<Integer> s = objSet.get(k2);
				s.add(t.obj);
				objSet.put(k2, s);
			}
		}
		for (Tuple t : testData.tuples_list) {
			Key k1 = new Key(t.relation, t.obj);
			if (!subjSet.containsKey(k1)) {
				Set<Integer> s = new THashSet<Integer>();
				s.add(t.subj);
				subjSet.put(k1, s);
			} else {
				Set<Integer> s = subjSet.get(k1);
				s.add(t.subj);
				subjSet.put(k1, s);
			}
			Key k2 = new Key(t.relation, t.subj);
			if (!objSet.containsKey(k2)) {
				Set<Integer> s = new THashSet<Integer>();
				s.add(t.obj);
				objSet.put(k2, s);
			} else {
				Set<Integer> s = objSet.get(k2);
				s.add(t.obj);
				objSet.put(k2, s);
			}
		}
		validData = new TestTupleData();
		validData.readFromRawFile(options.validFile, options.encoding, trainData);
		for (Tuple t : validData.tuples_list) {
			Key k1 = new Key(t.relation, t.obj);
			if (!subjSet.containsKey(k1)) {
				Set<Integer> s = new THashSet<Integer>();
				s.add(t.subj);
				subjSet.put(k1, s);
			} else {
				Set<Integer> s = subjSet.get(k1);
				s.add(t.subj);
				subjSet.put(k1, s);
			}
			Key k2 = new Key(t.relation, t.subj);
			if (!objSet.containsKey(k2)) {
				Set<Integer> s = new THashSet<Integer>();
				s.add(t.obj);
				objSet.put(k2, s);
			} else {
				Set<Integer> s = objSet.get(k2);
				s.add(t.obj);
				objSet.put(k2, s);
			}
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		OptionParser optionParser = new OptionParser(TestOptions.class);
		TestOptions options = (TestOptions) optionParser.parse(args);
		TestCP tester = null;
		try {
			tester = new TestCP(options);
		} catch (IOException e) {
			e.printStackTrace();
		}
		if (options.isInverse)
			tester.test_inv();
		else
			tester.test_();
	}
	
	final private void pre_score_(int subj, int relation, int obj) {
		final double[] obj_v = objVec[obj].values;
		final double[] relation_v = relationVec[relation].values;
		final double[] subj_v = subjVec[subj].values; //weightVec_[subj].values;
		op.product(subj_v, obj_v, relation_v, outVec1, outVec2, outVec3);
		// op.ccorr(obj_v, relation_v, outVec1__);
		// op.ccorr(subj_v, relation_v, outVec2__);
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
	
	private void test_() {
		double total = (double)testData.tuples_list.size() * 2;
		double mrr = 0.d, mrr_filter = 0.d;
		double rank_1 = 0.d, rank_1_filter = 0.d;
		double rank_3 = 0.d, rank_3_filter = 0.d;
		double rank_5 = 0.d, rank_5_filter = 0.d;
		double rank_10 = 0.d, rank_10_filter = 0.d;
		int j=0;
		for (Tuple test_tuple : testData.tuples_list) {
			if (test_tuple.flag) {
				total = total - 2;
				continue;
			}
			pre_score_(test_tuple.subj, test_tuple.relation, test_tuple.obj);
			for (int entity = 0; entity<trainData.entity_counter; ++entity) {
				RankScore r1 = scores_subj.get(entity); 
				r1.set(entity, innerProductSubj(entity, outVec2));
				RankScore r2 = scores_obj.get(entity);
				r2.set(entity, innerProductObj(entity, outVec3));
			}
			Collections.sort(scores_subj);
			Collections.sort(scores_obj);
			Key k_s = new Key(test_tuple.relation, test_tuple.obj);
			Key k_o = new Key(test_tuple.relation, test_tuple.subj);
			Set<Integer> hypo_s;
			if (subjSet.containsKey(k_s)) {
				hypo_s = subjSet.get(k_s);
			} else {
				hypo_s = new THashSet<Integer>();
			}
			Set<Integer> hypo_o;
			if (objSet.containsKey(k_o)) {
				hypo_o = objSet.get(k_o);
			} else {
				hypo_o = new THashSet<Integer>();
			}
			int rank1 = -1, rank2 = -1;
			int rank_filter_1 = -1, rank_filter_2 = -1;
			int k = 0;
			for (int i=0; i<scores_subj.size(); ++i) {
				RankScore r1 = scores_subj.get(i);
				if (r1.id == test_tuple.subj) {
					rank1 = i;
					rank_filter_1 = k;
					break;
				}
				if (!hypo_s.contains(r1.id)) {
					++k;
				}
			}
			k=0;
			for (int i=0; i<scores_obj.size(); ++i) {
				RankScore r1 = scores_obj.get(i);
				if (r1.id == test_tuple.obj) {
					rank2 = i;
					rank_filter_2 = k;
					break;
				}
				if (!hypo_o.contains(r1.id)) {
					++k;
				}
			}
			System.out.println(++j + " " + rank1 + " " + rank2 + "\n " + rank_filter_1 + " " + rank_filter_2);
			if (rank1 == 0) {
				++rank_1;
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank1 <= 2) {
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank1 <= 4) {
				++rank_5;
				++rank_10;
			} else if (rank1 <= 9) {
				++rank_10;
			}
			
			if (rank2 == 0) {
				++rank_1;
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank2 <= 2) {
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank2 <= 4) {
				++rank_5;
				++rank_10;
			} else if (rank2 <= 9) {
				++rank_10;
			}
			
			mrr += 1.d/(rank1+1.d);
			mrr += 1.d/(rank2+1.d);
			
			if (rank_filter_1 == 0) {
				++rank_1_filter;
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 2) {
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 4) {
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 9) {
				++rank_10_filter;
			}
			
			if (rank_filter_2 == 0) {
				++rank_1_filter;
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 2) {
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 4) {
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 9) {
				++rank_10_filter;
			}
			mrr_filter += 1.d/(rank_filter_1+1.d);
			mrr_filter += 1.d/(rank_filter_2+1.d);
		}
		System.out.println("Raw Rank1: " + (rank_1 / total));
		System.out.println("Raw Rank3: " + (rank_3 / total));
		System.out.println("Raw Rank5: " + (rank_5 / total));
		System.out.println("Raw Rank10: " + (rank_10 / total));
		System.out.println("Raw MRR: " + (mrr / total));
		
		System.out.println("Filtered Rank1: " + (rank_1_filter / total));
		System.out.println("Filtered Rank3: " + (rank_3_filter / total));
		System.out.println("Filtered Rank5: " + (rank_5_filter / total));
		System.out.println("Filtered Rank10: " + (rank_10_filter / total));
		System.out.println("Filtered MRR: " + (mrr_filter / total));
	}
	
	private void test_inv() {
		double total = (double)testData.tuples_list.size() * 2;
		double mrr = 0.d, mrr_filter = 0.d;
		double rank_1 = 0.d, rank_1_filter = 0.d;
		double rank_3 = 0.d, rank_3_filter = 0.d;
		double rank_5 = 0.d, rank_5_filter = 0.d;
		double rank_10 = 0.d, rank_10_filter = 0.d;
		int j=0;
		for (Tuple test_tuple : testData.tuples_list) {
			if (test_tuple.flag) {
				total = total - 2;
				continue;
			}
			pre_score_inv(test_tuple.subj, test_tuple.relation, test_tuple.relation_rev, test_tuple.obj);
			for (int entity = 0; entity<trainData.entity_counter; ++entity) {
				RankScore r1 = scores_subj.get(entity); 
				r1.set(entity, innerProductSubj(entity, outVec2) + innerProductObj(entity, outVec3_inv));
				RankScore r2 = scores_obj.get(entity);
				r2.set(entity, innerProductObj(entity, outVec3) + innerProductSubj(entity, outVec2_inv));
			}
			Collections.sort(scores_subj);
			Collections.sort(scores_obj);
			Key k_s = new Key(test_tuple.relation, test_tuple.obj);
			Key k_o = new Key(test_tuple.relation, test_tuple.subj);
			Set<Integer> hypo_s;
			if (subjSet.containsKey(k_s)) {
				hypo_s = subjSet.get(k_s);
			} else {
				hypo_s = new THashSet<Integer>();
			}
			Set<Integer> hypo_o;
			if (objSet.containsKey(k_o)) {
				hypo_o = objSet.get(k_o);
			} else {
				hypo_o = new THashSet<Integer>();
			}
			int rank1 = -1, rank2 = -1;
			int rank_filter_1 = -1, rank_filter_2 = -1;
			int k = 0;
			for (int i=0; i<scores_subj.size(); ++i) {
				RankScore r1 = scores_subj.get(i);
				if (r1.id == test_tuple.subj) {
					rank1 = i;
					rank_filter_1 = k;
					break;
				}
				if (!hypo_s.contains(r1.id)) {
					++k;
				}
			}
			k=0;
			for (int i=0; i<scores_obj.size(); ++i) {
				RankScore r1 = scores_obj.get(i);
				if (r1.id == test_tuple.obj) {
					rank2 = i;
					rank_filter_2 = k;
					break;
				}
				if (!hypo_o.contains(r1.id)) {
					++k;
				}
			}
			System.out.println(++j + " " + rank1 + " " + rank2 + "\n " + rank_filter_1 + " " + rank_filter_2);
			if (rank1 == 0) {
				++rank_1;
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank1 <= 2) {
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank1 <= 4) {
				++rank_5;
				++rank_10;
			} else if (rank1 <= 9) {
				++rank_10;
			}
			
			if (rank2 == 0) {
				++rank_1;
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank2 <= 2) {
				++rank_3;
				++rank_5;
				++rank_10;
			} else if (rank2 <= 4) {
				++rank_5;
				++rank_10;
			} else if (rank2 <= 9) {
				++rank_10;
			}
			
			mrr += 1.d/(rank1+1.d);
			mrr += 1.d/(rank2+1.d);
			
			if (rank_filter_1 == 0) {
				++rank_1_filter;
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 2) {
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 4) {
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_1 <= 9) {
				++rank_10_filter;
			}
			
			if (rank_filter_2 == 0) {
				++rank_1_filter;
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 2) {
				++rank_3_filter;
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 4) {
				++rank_5_filter;
				++rank_10_filter;
			} else if (rank_filter_2 <= 9) {
				++rank_10_filter;
			}
			mrr_filter += 1.d/(rank_filter_1+1.d);
			mrr_filter += 1.d/(rank_filter_2+1.d);
		}
		System.out.println("Raw Rank1: " + (rank_1 / total));
		System.out.println("Raw Rank3: " + (rank_3 / total));
		System.out.println("Raw Rank5: " + (rank_5 / total));
		System.out.println("Raw Rank10: " + (rank_10 / total));
		System.out.println("Raw MRR: " + (mrr / total));
		
		System.out.println("Filtered Rank1: " + (rank_1_filter / total));
		System.out.println("Filtered Rank3: " + (rank_3_filter / total));
		System.out.println("Filtered Rank5: " + (rank_5_filter / total));
		System.out.println("Filtered Rank10: " + (rank_10_filter / total));
		System.out.println("Filtered MRR: " + (mrr_filter / total));
	}
}
