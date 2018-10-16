package main;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import util.FileUtility;

public class TestTupleData {
	List<Tuple> tuples_list;
	
	private int addEntity(String entity, TupleData data) {
		if (!data.entity_dict.containsKey(entity)) {
			return -1;
		} else {
			return data.entity_dict.get(entity);
		}
	}

	private int addRelation(String relation, TupleData data) {
		if (!data.relation_dict.containsKey(relation)) {
			return -1;
		} else {
			return data.relation_dict.get(relation);
		}
	}
	
	public void readFromRawFile(String filename, String encoding, TupleData trainData) {
		tuples_list = new ArrayList<Tuple>();
		BufferedReader br = FileUtility.getReadFileStream(filename, encoding);
		try {
			String line = null;
			while(br.ready()) {
				line = br.readLine().trim();
				String[] tuples = line.split("(\t| )");
				if (tuples.length == 3 || tuples.length == 4) {
					int subj = addEntity(tuples[0], trainData);
					int obj = addEntity(tuples[2], trainData);
					int relation = addRelation(tuples[1], trainData);
					Tuple tuple = new Tuple(subj, relation, obj);
					if (subj == -1 || obj == -1 || relation == -1)
						tuple.flag = true;
					tuples_list.add(tuple);
					// tuple_key_set.add(Tuple.calcKey(subj, relation, obj));
				} else {
					System.err.println("file format error");
					System.exit(0);
				}
			}
			FileUtility.closeReadFile(br);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void readFromRawFileRev(String filename, String encoding, TupleData trainData) {
		tuples_list = new ArrayList<Tuple>();
		BufferedReader br = FileUtility.getReadFileStream(filename, encoding);
		try {
			String line = null;
			while(br.ready()) {
				line = br.readLine().trim();
				String[] tuples = line.split("(\t| )");
				if (tuples.length == 3 || tuples.length == 4) {
					int subj = addEntity(tuples[0], trainData);
					int obj = addEntity(tuples[2], trainData);
					int relation = addRelation(tuples[1], trainData);
					int relation_rev = addRelation("**"+tuples[1], trainData);
					Tuple tuple = new Tuple(subj, relation, relation_rev, obj);
					if (subj == -1 || obj == -1 || relation == -1 || relation_rev == -1)
						tuple.flag = true;
					tuples_list.add(tuple);
					// tuple_key_set.add(Tuple.calcKey(subj, relation, obj));
				} else {
					System.err.println("file format error");
					System.exit(0);
				}
			}
			FileUtility.closeReadFile(br);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
