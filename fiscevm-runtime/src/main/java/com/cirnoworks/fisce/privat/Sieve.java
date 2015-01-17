package com.cirnoworks.fisce.privat;

public class Sieve {

	void runSieve() {
		int SIZE = 8192;
		boolean flags[] = new boolean[SIZE + 1];
		int i, prime, k, iter, count;
		int iterations = 0;
		double seconds = 0.0;
		int score = 0;
		long startTime, elapsedTime;

		startTime = System.currentTimeMillis();
		while (true) {
			count = 0;
			for (i = 0; i <= SIZE; i++)
				flags[i] = true;
			for (i = 0; i <= SIZE; i++) {
				if (flags[i]) {
					prime = i + i + 3;
					for (k = i + prime; k <= SIZE; k += prime)
						flags[k] = false;
					count++;
				}
			}
			iterations++;
			elapsedTime = System.currentTimeMillis() - startTime;
			if (elapsedTime >= 2000)
				break;
		}
		seconds = elapsedTime / 1000.0;
		score = (int) Math.round(iterations / seconds);
		System.out.println(iterations + " iterations in " + seconds
				+ " seconds");
		System.out.println("Sieve score = " + score);
	}

	public static void main(String[] args) {
		Sieve sieve = new Sieve();
		for (int i = 0; i < 10; i++) {
			sieve.runSieve();
		}
	}
}
