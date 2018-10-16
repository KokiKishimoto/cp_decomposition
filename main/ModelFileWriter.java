package main;

import java.io.BufferedWriter;
import java.io.IOException;
import util.FileUtility;

public class ModelFileWriter {
	String filename;
	String encoding;

	ModelFileWriter(String filename, String encoding) {
		this.filename = filename;
		this.encoding = encoding;
	}

	////////////////

	public void save(ParamVector[] subjVec, ParamVector[] objVec, ParamVector[] relationVec, SignalOperation op,
			TupleData data, int iter) {
		try {
			BufferedWriter bw1 = FileUtility.getWriteFileStream(filename + "-entity-subj-" + iter, encoding);
			bw1.append(data.entity_counter + " ");
			bw1.append(subjVec[0].values.length + "\n");
			int i, j;
			for (i = 0; i < data.entity_counter; ++i) {
				final double[] vec = subjVec[i].values;
				final double norm = op.norm(vec);
				bw1.append(data.entity_dict_rev.get(i));
				bw1.append(' ');
				for (j = 0; j < vec.length; ++j) {
					if (j > 0)
						bw1.append(' ');
					bw1.append(String.valueOf(vec[j] / norm));
				}
				bw1.append('\n');
			}
			FileUtility.closeWriteFile(bw1);

			bw1 = FileUtility.getWriteFileStream(filename + "-entity-obj-" + iter, encoding);
			bw1.append(data.entity_counter + " ");
			bw1.append(objVec[0].values.length + "\n");
			for (i = 0; i < data.entity_counter; ++i) {
				final double[] vec = objVec[i].values;
				final double norm = op.norm(vec);
				bw1.append(data.entity_dict_rev.get(i));
				bw1.append(' ');
				for (j = 0; j < vec.length; ++j) {
					if (j > 0)
						bw1.append(' ');
					bw1.append(String.valueOf(vec[j] / norm));
				}
				bw1.append('\n');
			}
			FileUtility.closeWriteFile(bw1);

			BufferedWriter bw2 = FileUtility.getWriteFileStream(filename + "-relation-" + iter, encoding);
			bw2.append(data.relation_counter + " ");
			bw2.append(relationVec[0].values.length + "\n");
			for (i = 0; i < data.relation_counter; ++i) {
				final double[] vec = relationVec[i].values;
				final double norm = op.norm(vec);
				bw2.append(data.relation_dict_rev.get(i));
				bw2.append(' ');
				for (j = 0; j < vec.length; ++j) {
					if (j > 0)
						bw2.append(' ');
					bw2.append(String.valueOf(vec[j] / norm));
				}
				bw2.append('\n');
			}
			FileUtility.closeWriteFile(bw2);
		} catch (IOException e) {
			System.err.println("raised IOException.");
		}
	}

	////////////////

	public void saveQ(ParamVector[] subjVec, ParamVector[] objVec, ParamVector[] relationVec, SignalOperation op, TupleData data, int iter) {
		try {
			BufferedWriter bw1 = FileUtility.getWriteFileStream(filename + "-entity-subj-" + iter, encoding);
			bw1.append(data.entity_counter + " ");
			bw1.append(subjVec[0].values.length + "\n");
			int i, j;
			for (i = 0; i < data.entity_counter; ++i) {
				final double[] vec = subjVec[i].values;
				// final double norm = op.norm(vec);
				bw1.append(data.entity_dict_rev.get(i));
				bw1.append(' ');
				for (j = 0; j < vec.length; ++j) {
					if (j > 0)
						bw1.append(' ');
					bw1.append(String.valueOf(op.quantize(vec[j])));
				}
				bw1.append('\n');
			}
			FileUtility.closeWriteFile(bw1);

			bw1 = FileUtility.getWriteFileStream(filename + "-entity-obj-" + iter, encoding);
			bw1.append(data.entity_counter + " ");
			bw1.append(objVec[0].values.length + "\n");
			for (i = 0; i < data.entity_counter; ++i) {
				final double[] vec = objVec[i].values;
				// final double norm = op.norm(vec);
				bw1.append(data.entity_dict_rev.get(i));
				bw1.append(' ');
				for (j = 0; j < vec.length; ++j) {
					if (j > 0)
						bw1.append(' ');
					bw1.append(String.valueOf(op.quantize(vec[j])));
				}
				bw1.append('\n');
			}
			FileUtility.closeWriteFile(bw1);

			BufferedWriter bw2 = FileUtility.getWriteFileStream(filename + "-relation-" + iter, encoding);
			bw2.append(data.relation_counter + " ");
			bw2.append(relationVec[0].values.length + "\n");
			for (i = 0; i < data.relation_counter; ++i) {
				final double[] vec = relationVec[i].values;
				// final double norm = op.norm(vec);
				bw2.append(data.relation_dict_rev.get(i));
				bw2.append(' ');
				for (j = 0; j < vec.length; ++j) {
					if (j > 0)
						bw2.append(' ');
					bw2.append(String.valueOf(op.quantize(vec[j])));
				}
				bw2.append('\n');
			}
			FileUtility.closeWriteFile(bw2);
		} catch (IOException e) {
			System.err.println("raised IOException.");
		}
	}

	public static ModelFileWriter getInstance(String filename, String encoding) {
		if (filename == null)
			return null;
		ModelFileWriter mfw = new ModelFileWriter(filename, encoding);
		return mfw;
	}

}
