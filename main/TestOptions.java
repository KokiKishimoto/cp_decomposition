package main;

import util.Option;

public class TestOptions {
	    @Option(name = "-model-subj-file", required = true, usage = "Entity Vector file\n")
	    public String modelSubjFile;
	    
	    @Option(name = "-model-obj-file", usage = "Entity Vector file\n")
	    public String modelObjFile;
	    
	    //@Option(name = "-model-w-file", usage = "Entity Vector file\n")
	    //public String modelWFile;
	    
	    @Option(name = "-model-r-file", required = true, usage = "Relation Vector file\n")
	    public String modelRFile;
	    
	    // @Option(name = "-model-p-file", usage = "Permutation Vector file\n")
	    // public String modelPFile;
	    
	    @Option(name = "-test-file", required = true, usage = "test file\n")
	    public String testFile;
	    
	    @Option(name = "-valid-file", required = true, usage = "valid file\n")
	    public String validFile;
	    
	    @Option(name = "-data-file", required = true, usage = "Output file\n")
	    public String dataFile;
	    
	    @Option(name = "-encoding", usage = "Input & output text encoding (Default UTF-8)\n")
	    public String encoding = "UTF-8";

	    // @Option(name = "-model-num", usage = "model num (Default = 1)\n")
	    // public int modelNum = 2;
	    
	    // @Option(name = "-complex", usage = "using Fourier HRR\n")
	    // public boolean isComplex = false;
	    
	    // @Option(name = "-angle", usage = "using Circulant HRR\n")
	    // public boolean isAngle = false;
	    
	    @Option(name = "-inverse", usage = "using shuffle\n")
	    public boolean isInverse = false;
	    
	    @Option(name = "-log-time", usage = "Enable logging parsing time (Default false)\n")
	    public boolean enableLogging = true;
}
