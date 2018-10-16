package main;

import java.io.Serializable;

public class TupleKeyGen implements Serializable {
	private static final long serialVersionUID = -4976246110288129906L;
	public int shift1, shift2;
	
	public TupleKeyGen(int e_size, int r_size) {
		shift1 = e_size * r_size;
		shift2 = r_size;
	}
	
	final public long calcKey(int s, int r, int o) { 
		long x = s * shift1 + o * shift2 + r;
		return x;
	}

}
