package main;

import java.util.Random;

public class Gaussian {
	Random rand;
	
	public Gaussian() {
		rand = new Random();
	}

	public void setSeed(int seed) {
		rand.setSeed(seed);
	}
	
	public double getNext() {
		return rand.nextGaussian();
	}
	
	public void sampling(ParamVector vec) {
		// ComplexNumber[] nums = vec.values;
		double[] values = vec.values;
		for (int i=0; i<values.length; ++i) {
			// nums[i].real = getNext();
			// nums[i].imag = getNext();
			values[i] = getNext();
		}
	}
	
}
