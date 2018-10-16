package main;

import java.util.Random;

public class Uniform {
	private static Random rand;
	private double a;
	private double b;
	public Uniform() { 
		a = 0.d; 
		b = 1.d; 
		rand = new Random();
	}
	public Uniform(double a, double b) {
		this.a = a;
		this.b = b;
		rand = new Random();
	}
	
	public void set(double a, double b) {
		this.a = a;
		this.b = b;
	}
	
	public void setSeed(int seed) { rand.setSeed(seed); }
	
	public double getNext() {
		return (b - a) * rand.nextDouble() + a;
	}
}
