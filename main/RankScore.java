package main;

public class RankScore implements Comparable<RankScore> {
	double score;
	int id;
	
	public void set(int id, double score) {
		this.id = id;
		this.score = score;
	}

	@Override
	public int compareTo(RankScore o) {
		if (score > o.score) {
			return -1;
		} else if (score < o.score) {
			return 1;
		}
		return 0;
	}
}
