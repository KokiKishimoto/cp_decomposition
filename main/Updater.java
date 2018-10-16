package main;

public class Updater {
	double initialRate, learningRate, lambda;
	double threshold;
	double eta;

	
	ParamVector[] subjVec_, objVec_, relationVec_;

	public Updater(ParamVector[] subjVec_, ParamVector[] objVec_, ParamVector[] relationVec_, double learningRate, double threshold) {
		this.subjVec_ = subjVec_;
		this.objVec_ = objVec_;
		this.relationVec_ = relationVec_;
		this.learningRate = learningRate;
		this.initialRate = learningRate;
		lambda = 1.0 / Math.pow(10.d, 8.d);
		this.threshold = threshold;
	}
	
	public void decay(double time) {
		learningRate = initialRate / (1.d + initialRate * lambda * time);
	}

	
	public void SGD(Tuple tuple, double[] gradient_r, double[] gradient_s, double[] gradient_o, double[] norms,
			// double fs,
			double lambda) {
		// update relation vector
		double[] r_v = relationVec_[tuple.relation].values; // paramVec[target.id].values;
		int i;
		if (norms[0] >= threshold) {
			double x = (threshold / norms[0]);
			for (i = 0; i < gradient_r.length; ++i) {
				r_v[i] -= learningRate * x * gradient_r[i];
			}
		} else {
			for (i = 0; i < gradient_r.length; ++i) {
				r_v[i] -= learningRate * gradient_r[i];
			}
		}
		// update subj entity vector
		double[] s_e_v = subjVec_[tuple.subj].values; // paramVec_dash[context.id].values;
		if (norms[1] >= threshold) {
			double x = (threshold / norms[1]);
			for (i = 0; i < gradient_s.length; ++i) {
				s_e_v[/* gradient_s.length-i-1 */i] -= learningRate * x * gradient_s[i]; // gradients[1][i];
			}
		} else {
			for (i = 0; i < gradient_s.length; ++i) {
				s_e_v[/* gradient_s.length-i-1 */i] -= learningRate * gradient_s[i];
			}
		}
		// update obj entity vector
		double[] o_e_v = objVec_[tuple.obj].values;
		if (norms[2] >= threshold) {
			double x = (threshold / norms[2]);
			for (i = 0; i < gradient_o.length; ++i) {
				o_e_v[i] -= learningRate * x * gradient_o[i]; // gradients[1][i];
			}
		} else {
			for (i = 0; i < gradient_o.length; ++i) {
				o_e_v[i] -= learningRate * gradient_o[i];
			}
		}
	}
}
