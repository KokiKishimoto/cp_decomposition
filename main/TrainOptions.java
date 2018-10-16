package main;

import util.Option;

public class TrainOptions {
    @Option(name = "-train-file", required = true, usage = "Input train file\n")
    public String trainFile;
    
    @Option(name = "-test-file", usage = "test file\n")
    public String testFile;
    
    @Option(name = "-valid-file", usage = "vaild file\n")
    public String validFile;
    
    @Option(name = "-output-data-file", usage = "Valid file\n")
    public String dataFile;
    
    @Option(name = "-output-file", usage = "Output file\n")
    public String outputFile;
    
    @Option(name = "-data-file", usage = "Valid file\n")
    public String tDataFile;
    
    @Option(name = "-model-e-file", usage = "Model file\n")
    public String eModelFile;
    
    @Option(name = "-model-r-file", usage = "Model file\n")
    public String rModelFile;
    
    @Option(name = "-vec-dim", usage = "Vector dimension (Default = 50)\n")
    public int vecDim = 50;
    
    @Option(name = "-block-dim", usage = "Vector dimension (Default = 50)\n")
    public int blockVecDim = 50;
    
    @Option(name = "-block-size", usage = "Vector dimension (Default = 50)\n")
    public int blockSize = 2;
    
    @Option(name = "-minibatch", usage = "Vector dimension (Default = 50)\n")
    public int minibatch = 100;
    
    // @Option(name = "-bucket", usage = "Vector dimension (Default = 50)\n")
    // public int bucket = 50;
    
    // @Option(name = "-sketch-dim", usage = "Sketch Vector dimension (Default = 100)")
    // public int sketchVecDim = 400;
    
    @Option(name = "-max-epoch", usage = "Max Epoch size (Default = 50)\n")
    public int maxEpoch = 100;
    
    @Option(name = "-complex", usage = "using Fourier HRR\n")
    public boolean isComplex = false;
    
    @Option(name = "-block", usage = "using Fourier HRR\n")
    public boolean isBlock = false;
    
    @Option(name = "-full", usage = "using Fourier HRR\n")
    public boolean isFull = false;
    
    @Option(name = "-wo-imag", usage = "using Fourier HRR\n")
    public boolean isImag = false;
    
    @Option(name = "-normal", usage = "using Fourier HRR\n")
    public boolean isNormal = false;
     
    @Option(name = "-num-negative", usage = "Negative sampling size (Default = 5)\n")
    public int negativeSize = 5;
    
    @Option(name = "-rand-seed", usage = "Random Seed (Default = 0)\n")
    public int randomSeed = 0;
    
    @Option(name = "-k", usage = "Householder num (Default = 1)\n")
    public int k = 48;
    
    //@Option(name = "-house-num", usage = "Householder num (Default = 1)\n")
    //public int orthoNum = 2;
    
    @Option(name = "-learning-rate", usage = "Learning rate (Default = 0.025)\n")
    public double learningRate = 0.025; // 0.025;
    
    @Option(name = "-lambda", usage = "regularizer (Default = 0.025)\n")
    public double lambda = 0.025;
    
    @Option(name = "-norm-clip", usage = "Threshold (Default = 5.0)\n")
    public double normClip = 5.d;
    
    @Option(name = "-mode-ccorr", usage = "using circular correlation (Default = false)")
    public boolean modeFlag = true;
    
    @Option(name = "-encoding", usage = "Input & output text encoding (Default UTF-8)\n")
    public String encoding = "UTF-8";

    @Option(name = "-log-time", usage = "Enable logging parsing time (Default false)\n")
    public boolean enableLogging = true;
}
