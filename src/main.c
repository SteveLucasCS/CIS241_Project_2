//
// Created by Steve on 11/1/2019.
//

#include <stdlib.h>
#include <math.h>

#define AVG_SERVICE 2.0

/**
 * Calculates a randomized time it takes a specific teller to service one customer
 * based on the mean service time provided as the argument. Allows a the model to
 * simulate multiple tellers with different individual working speeds.
 * @param mean - mean service time per customer
 * @return time to service one customer
 */
double expdist (double mean) {
	double r = rand();
	r /= RAND_MAX;
	return -mean * log(r);
}

int main () {
	double t;
	t = expdist (AVG_SERVICE);
	
}
