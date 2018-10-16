package main;

import java.io.BufferedReader;
import java.io.IOException;

import util.FileUtility;

public class NBitQuantize {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		BufferedReader br = FileUtility.getReadFileStream(args[0], "UTF-8");
		String line;
		double total = 0.d;
		double total_num = 0.d;
		try {
			line = br.readLine();
			String[] conts = line.trim().split("\\s");
			while(br.ready()) {
				line = br.readLine().trim();
				conts = line.split("\\s");
				for (int i=1; i<conts.length; ++i) {
					total += Math.abs(Double.parseDouble(conts[i]));
					total_num += 1.d;
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		FileUtility.closeReadFile(br);
		
		double alpha = total / total_num;
		br = FileUtility.getReadFileStream(args[0], "UTF-8");
		try {
			line = br.readLine();
			System.out.println(line);
			while(br.ready()) {
				line = br.readLine().trim();
				String[] conts = line.split("\\s");
				System.out.print(conts[0] + " ");
				for (int i=1; i<conts.length; ++i) {
					if (i>1) System.out.print(" ");
					double x = Double.parseDouble(conts[i]);
					if (x >= 0) {
						System.out.print(alpha);
					} else {
						System.out.print(-alpha);
					}
				}
				System.out.println();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		FileUtility.closeReadFile(br);
		
	}
}
