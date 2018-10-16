package main;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;

import util.FileUtility;

public class Concat {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		BufferedWriter bw = FileUtility.getWriteFileStream(args[1], "UTF-8");
		// List<String> filenames = new ArrayList<String>();
		File dir = new File(args[0]);
		File[] files = dir.listFiles();
		int fileNum = files.length;
		BufferedReader[] br = new BufferedReader[fileNum]; // FileUtility.getReadFileStream(args[0], "UTF-8");
		for (int i=0; i<fileNum; ++i) {
			try {
				br[i] = new BufferedReader(
						new InputStreamReader(
								new FileInputStream(files[i])
								)
						);
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		try {
			final String[] conts = br[0].readLine().trim().split("\\s+");
			final int vecSize = Integer.parseInt(conts[1]);
			// StringBuilder sb = new StringBuilder();
			bw.append(conts[0]);
			bw.append(' '); 
			bw.append(/*conts[1]*/String.valueOf(vecSize*fileNum));
			bw.append("\n");
			for(int i=1; i<fileNum; ++i) 
				br[i].readLine();
			
			while(br[0].ready()) {
				for (int file_i=0; file_i<fileNum; ++file_i) {
					final String[] values = br[file_i].readLine().trim().split("\\s+");
					if (file_i == 0) {
						bw.append(values[0]);
						bw.append(' ');
					}
					if (file_i>0) bw.append(' ');
					for (int i=0; i<vecSize; ++i) {
						if (i>0) bw.append(' ');
						bw.append(/*complex[2*i]*/values[i+1]);
					}
				}
				bw.append('\n');
				// bw.append(sb.toString());
				// sb.delete(0, sb.length());
			}
			for (int i=0; i<fileNum; ++i)
				FileUtility.closeReadFile(br[i]);
			FileUtility.closeWriteFile(bw);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
