package main;

import gnu.trove.THashMap;
import gnu.trove.THashSet;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import util.FileUtility;

public class TupleData implements Serializable {
	private static final long serialVersionUID = -2557625708120816686L;
	int entity_counter, relation_counter;
	Map<String, Integer> entity_dict, relation_dict;
	Map<Integer, String> entity_dict_rev, relation_dict_rev;
	Set<Long> tuple_key_set; 
	List<Tuple> tuple_list;
	TupleKeyGen key_gen;
	
	public TupleData() {
		entity_counter = relation_counter = 0;
		entity_dict = new THashMap<String, Integer>();
		relation_dict = new THashMap<String, Integer>();
		entity_dict_rev = new THashMap<Integer, String>();
		relation_dict_rev = new THashMap<Integer, String>();
		tuple_key_set = new THashSet<Long>();
		tuple_list = new ArrayList<Tuple>();
	}
	
	private int addEntity(String entity) {
		if (!entity_dict.containsKey(entity)) {
			int id = entity_counter;
			entity_dict.put(entity, entity_counter);
			entity_dict_rev.put(entity_counter, entity);
			++entity_counter;
			return id;
		} else {
			return entity_dict.get(entity);
		}
	}

	private int addRelation(String relation) {
		if (!relation_dict.containsKey(relation)) {
			int id = relation_counter;
			relation_dict.put(relation, relation_counter);
			relation_dict_rev.put(relation_counter, relation);
			++relation_counter;
			return id;
		} else {
			return relation_dict.get(relation);
		}
	}
	
	public void readFromRawFile(String filename, String encoding) {
		BufferedReader br = FileUtility.getReadFileStream(filename, encoding);
		try {
			String line = null;
			while(br.ready()) {
				line = br.readLine().trim();
				String[] tuples = line.split("(\t| )");
				if (tuples.length == 3) {
					int subj = addEntity(tuples[0]);
					int obj = addEntity(tuples[2]);
					int relation = addRelation(tuples[1]);
					Tuple tuple = new Tuple(subj, relation, obj);
					tuple_list.add(tuple);
				} else {
					System.err.println("file format error");
					System.exit(0);
				}
			}
			key_gen = new TupleKeyGen(entity_counter, relation_counter);
			for (Tuple t : tuple_list) {
				long key = key_gen.calcKey(t.subj, t.relation, t.obj);
				// if (tuple_key_set.contains(key)) 
				// System.exit(0);
				tuple_key_set.add(key);
			}
			FileUtility.closeReadFile(br);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void registerTuples (String filename, String encoding) {
		BufferedReader br = FileUtility.getReadFileStream(filename, encoding);
		try {
			while(br.ready()) {
				String line = br.readLine().trim();
				String[] conts = line.split("\\s+");
				final int subj = addEntity(conts[0]);
				final int obj = addEntity(conts[2]);
				final int relation = addRelation(conts[1]);
				tuple_key_set.add(key_gen.calcKey(subj, relation, obj));
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public static TupleData openObjectFile(String filename) {
		try {
			ObjectInputStream ois = new ObjectInputStream(new FileInputStream(filename));
			TupleData data = (TupleData)ois.readObject();
			ois.close();
			return data;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}
	
	public void writeObjectFile(String  filename) {
		try {
			ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(filename));
			oos.writeObject(this);
			oos.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
