package main;

// import java.util.Random;
import java.util.Random;
import java.util.Set;

public class RandomSampler {
	Random nextRandom;
	Tuple tuple;
	TupleKeyGen key_gen;
	int b,c;
	
	public RandomSampler(TupleKeyGen key_gen, int sizeEnt, int sizeRelation, int randomSeed) {
		nextRandom = new Random(/*1*/randomSeed);
		tuple = new Tuple(-1,-1,-1);
		this.key_gen = key_gen;
		// a = 0;
		b = sizeEnt;
		c = sizeRelation;
	}
	
	final public int getNextEnt() {
		return nextRandom.nextInt(b); //(int)(nextRandom.nextInt()*b);// (b - a) * nextRandom.nextInt() + a;
	}
	
	final public int getNextRel() {
		return nextRandom.nextInt(c); //(int)(nextRandom.nextInt()*b);// (b - a) * nextRandom.nextInt() + a;
	}
	
	final public Tuple negativeSamplingSbj(Tuple gold, Set<Long> tuple_key_set) {
		for (;;) {
			// rand = nextRandom.nextLong() * 25214903917l + 11l;
			// if (rand < 0) rand = -rand;
			int entity1 = getNextEnt();// table[(int) ((rand >> 16) % tableSize)];
			
			/* rand = nextRandom.nextLong() * 25214903917l + 11l;
			if (rand < 0) rand = -rand;
			int entity2 = table[(int) ((rand >> 16) % tableSize)]; */
			// if (flag) {
			if (!tuple_key_set.contains(key_gen.calcKey(entity1, gold.relation, gold.obj))) 
			{
				tuple.set(entity1, gold.relation, gold.obj);
				return tuple;
			}
			//} else {
			//	if (!tuple_key_set.contains(key_gen.calcKey(gold.subj, gold.relation, entity1))) 
			//	{
			//		tuple.set(gold.subj, gold.relation, entity1);
			//		return tuple;
			//	}
			//}
		}
	}
	
	final public Tuple negativeSamplingObj(Tuple gold, Set<Long> tuple_key_set) {
		for (;;) {
			// rand = nextRandom.nextLong() * 25214903917l + 11l;
			// if (rand < 0) rand = -rand;
			int entity1 = getNextEnt();// table[(int) ((rand >> 16) % tableSize)];
			
			/* rand = nextRandom.nextLong() * 25214903917l + 11l;
			if (rand < 0) rand = -rand;
			int entity2 = table[(int) ((rand >> 16) % tableSize)]; */
			// if (flag) {
			if (!tuple_key_set.contains(key_gen.calcKey(gold.subj, gold.relation, entity1))) 
			{
				tuple.set(gold.subj, gold.relation, entity1);
				return tuple;
			}
			//} else {
			//	if (!tuple_key_set.contains(key_gen.calcKey(gold.subj, gold.relation, entity1))) 
			//	{
			//		tuple.set(gold.subj, gold.relation, entity1);
			//		return tuple;
			//	}
			//}
		}
	}
	
	final public Tuple negativeSamplingRel(Tuple gold, Set<Long> tuple_key_set) {
		for (;;) {
			int rel = getNextRel();// table[(int) ((rand >> 16) % tableSize)];
			if (!tuple_key_set.contains(key_gen.calcKey(gold.subj, rel, gold.obj))) 
			{
				tuple.set(gold.subj, rel, gold.obj);
				return tuple;
			}
		}
	}
}
