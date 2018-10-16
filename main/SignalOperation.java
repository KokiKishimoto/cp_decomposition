package main;

// import org.jtransforms.fft.DoubleFFT_1D;

public class SignalOperation {
	// private DoubleFFT_1D fft2 = null;

	public SignalOperation() {}

	// public SignalOperation(int size) {
		// fft2 = new DoubleFFT_1D(size);
	//}
	
	////////////

	public double norm(double[] a) {
		return Math.sqrt(innerProduct(a, a));
	}

	public void normalize(double[] a) {
		double norm = norm(a);
		for (int i=0; i<a.length; ++i)
			a[i] /= norm;
	}
	
	
	
	public double innerProduct(double[] a, double[] b) {
		double score = 0.d;
		for (int i = 0; i < a.length; ++i)
			score += a[i] * b[i];
		return score;
	}

	//////////////////////////

	public ParamVector product(double[] a, double[] b) {
		ParamVector out = new ParamVector(a.length);
		for (int i = 0; i < a.length; ++i) {
			out.values[i] = a[i] * b[i];
		}
		return out;
	}

	public double[] product(double[] a, double[] b, double[] c, double[] out1, double[] out2, double[] out3) {
		// int k=0;
		for (int i = 0; i < a.length; ++i) {
			out1[i] = a[i] * b[i];
			out2[i] = b[i] * c[i];
			out3[i] = a[i] * c[i];
		}
		return out1;
	}
	
	/////////////////////////
	public double quantize(double x) {
		double sign = (x < 0 ? -1.d : 1.d);
		return sign / 3.d;
	}
	
	public double qproduct(double[] a, double[] b, double[] c, double[] out1, double[] out2, double[] out3) {
		double score = 0.d;
		for (int i = 0; i < a.length; ++i) {
			double a_ = quantize(a[i]);
			double b_ = quantize(b[i]);
			double c_ = quantize(c[i]);
			out1[i] = a_ * b_;
			out2[i] = b_ * c_;
			out3[i] = a_ * c_;
			score += c_ * out1[i];
		}
		return score;
	}
	
}
