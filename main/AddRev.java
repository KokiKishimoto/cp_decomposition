package main;

import java.io.BufferedReader;
import java.io.IOException;

import util.FileUtility;

public class AddRev {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		BufferedReader br = FileUtility.getReadFileStream(args[0], "UTF-8");
		
		try {
			while(br.ready()) {
				String line = br.readLine();
				line = line.trim();
				String[] conts = line.split("(\t| )");
				System.out.println(line);
				System.out.print(conts[2]);
				System.out.print('\t');
				System.out.print("**" + conts[1]);
				System.out.print('\t');
				System.out.println(conts[0]);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		FileUtility.closeReadFile(br);
	}

}
