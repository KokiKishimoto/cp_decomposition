package main;


public class ParamVector {
	static Uniform distGen = new Uniform();
	static Gaussian gaussianGen = new Gaussian();
	public double[] values;
	
	public ParamVector(int size) {
		values = new double[size];
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append('[');
		if (values != null) {
			for (int i=0; i<values.length; ++i) {
				if (i>0) sb.append(',');
				sb.append(values[i]);
			}
		}
		sb.append(']');
		return sb.toString();
	}
	
	public void initByUniformDist(int seed) {
		double bnd = 1.d / Math.sqrt((double) values.length);
		distGen.set(-bnd, bnd);
		distGen.setSeed(seed);
		for (int i=0; i<values.length; ++i) {
			values[i] = distGen.getNext();
		}
	}
	
	public void initByUniformNormDist(int seed, SignalOperation op, boolean flag) {
		double bnd = Math.sqrt(6.d) / Math.sqrt((double) values.length*2);
		distGen.set(-bnd, bnd);
		distGen.setSeed(seed);
		for (int i=0; i<values.length; ++i) {
			values[i] = distGen.getNext();
		}
		if (flag) {
			double norm = op.norm(values);
			for (int i=0; i<values.length; ++i) {
				values[i] = values[i] / norm;
			}
		}
	}
}
