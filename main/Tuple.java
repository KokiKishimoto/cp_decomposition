package main;

import java.io.Serializable;

public class Tuple implements Serializable {
	private static final long serialVersionUID = -4403712347949568394L;
	int subj, obj, relation, relation_rev;
	boolean flag;
	
	public Tuple(int subj, int relation, int obj) {
		this.subj = subj;
		this.obj = obj;
		this.relation = relation;
	}
	
	public Tuple(int subj, int relation, int relation_rev, int obj) {
		this.subj = subj;
		this.obj = obj;
		this.relation = relation;
		this.relation_rev = relation_rev;
	}
	
	final public void set(int subj, int relation, int obj) {
		this.subj = subj;
		this.obj = obj;
		this.relation = relation;
	}
	
	final public void set(int subj, int relation, int relation_rev, int obj) {
		this.subj = subj;
		this.obj = obj;
		this.relation = relation;
		this.relation_rev = relation_rev;
	}
	
}
