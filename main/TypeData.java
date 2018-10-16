package main;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.Map;
import java.util.Set;

import gnu.trove.THashMap;
import gnu.trove.THashSet;
import util.FileUtility;

public class TypeData {
	Map<Integer, Set<Integer>> typeMap;
	
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
	
	public Set<Integer> getSet(int key) {
		return typeMap.get(key);
	}
	
	public void readFromRawFile(String filename, String encoding, TupleData trainData) {
		typeMap = new THashMap<Integer, Set<Integer>>();
		BufferedReader br = FileUtility.getReadFileStream(filename, encoding);
		try {
			String line = null;
			while(br.ready()) {
				line = br.readLine().trim();
				String[] conts = line.split("\t");
				int relation = addRelation(conts[0], trainData);
				Set<Integer> set = new THashSet<Integer>();
				String[] entities = conts[1].split(",");
				for (String entity : entities) {
					int entity_id = addEntity(entity, trainData);
					set.add(entity_id);
				}
				typeMap.put(relation, set);
			}
			FileUtility.closeReadFile(br);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
