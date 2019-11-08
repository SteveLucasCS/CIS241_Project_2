/* Authors: Steve Lucas and authors of CIS241 Project 2 Instructions
 * References and expdist() function provided by CIS241 Project 2 Instructions
 * Version 1.0 Date: 11/8/2019
 * GitHub Repository: https://github.com/SteveLucasCS/CIS241_Project_2
 * License MIT
 *
 * - Github: https://github.com/SteveLucasCS
 * - Website: https://www.stevelucas.tech
 * - email: stevelucascs@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "serverModel.h"
#include "server.c"
#include "queueModel.h"
#include "queue.c"

#define DATA_IN "proj2.dat"
#define DEFAULT_AVG_SERVICE_TIME 2.0
#define DEFAULT_MIN_SERVERS 4
#define DEFAULT_STARTING_SERVERS 4
#define DEFAULT_MAX_SERVERS 4
#define MINUTES_PER_SIM 48.0
#define SIM_TIME_INTERVAL 1.0

void simulation(int, double, FILE *, FILE *, FILE *, FILE *);
int getCustomersPerMin(int, int[], int[]);
int simCustomerArrival(struct Customer *, int, double);
double serveCustomer(struct Customer *, struct Server *);
double *saveWaitTime(double, int, double *, int *);
void printCustomerLog(FILE *, int, double, double *);
int overrideParameters(int, const char *[], double *, int *, int *, int *);


int main(int argc, char *argv[]) {

	FILE *simResults, *queueLog, *serverLog, *customerLog;
	simResults = fopen("simResults.txt", "w");
	queueLog = fopen("queueLog.txt", "w");
	serverLog = fopen("serverLog.txt", "w");
	customerLog = fopen("customerLog.txt", "w");

	if (simResults == NULL)
		printf("Error opening simulation results file.");

	fprintf(simResults, "Simulation Summaries\n\n");

	// Initialize and seed pseudo-random number generator
	srand(time(0));

	/** Minimum number of servers (i.e. cannot have less than 1) */
	int minServers = DEFAULT_MIN_SERVERS;
	/** Maximum number of servers (i.e. cannot have more than 20) */
	int maxServers = DEFAULT_MAX_SERVERS;
	/** Current number of servers in the simulation */
	int numServers = DEFAULT_STARTING_SERVERS;

	/** The average time it takes 1 server to completely service 1 customer (in MINUTES)*/
	double avgServiceTime = DEFAULT_AVG_SERVICE_TIME;

	/* Check for program arguments, override simulation parameters if arguments are valid */
	switch (overrideParameters(argc,
			(const char **) argv, &avgServiceTime, &minServers, &numServers, &maxServers)) {
		case 1:
			// Parameters overridden successfully
			printf("Command line arguments found - simulation parameters modified\n");
			break;
		case -1: return -1;
		case -2: return -1;
		default: break; // Continues with default parameters
	}

	int i = 0;
	numServers = maxServers;

	while (numServers - i >= minServers) {
		printf("\nSimulation %d -- Number of Tellers: %d", i + 1, numServers);
		fprintf(simResults, "\nSimulation %d -- Number of Tellers: %d", i + 1, numServers);
		/* Run the simulation with the number of servers/tellers passed as the argument */
		simulation(numServers, avgServiceTime, simResults, queueLog, customerLog, serverLog);
		i++;
	}


	fclose(simResults);
	fclose(serverLog);
	fclose(queueLog);
	fclose(customerLog);

	return 0;
}


/**
 * Runs one simulation of the model with the number of servers/tellers
 * determined by the function parameter. Individual server speed, average
 * server speed, and frequency of customer arrival are determined by pseudo-
 * randomly generated numbers and may vary each time this function is called.
 * @param numServers Number of servers that will be modeled in the simulation
 * @param avgServiceTime Mean of servers time to complete service for one customer
 */
void simulation(int numServers,
		double avgServiceTime,
		FILE *simResults,
		FILE *queueLog,
		FILE *customerLog,
		FILE *serverLog) {

	// Gets customer arrival frequency probabilities from data file
	FILE *fin;
	fin = fopen(DATA_IN, "r");

	if (fin == NULL) {
		printf("Error: failed to open data input file.\n");
		exit(-6);
	}

	int colOneBuff = -1, colTwoBuff = -1, *inputCustomerArrivals, *inputCustomerProbabilities,
			inputCapacity = 5, inputSize = 0;

	inputCustomerArrivals = (int *) malloc(sizeof(int) * inputCapacity);
	inputCustomerProbabilities = (int *) malloc(sizeof(int) * inputCapacity);

	while (fscanf(fin, "%d %d", &colOneBuff, &colTwoBuff) != EOF) {
		inputSize++;
		inputCustomerArrivals[inputSize - 1] = colOneBuff;
		inputCustomerProbabilities[inputSize - 1] = colTwoBuff;

		if (inputSize >= inputCapacity - 1) {
			inputCapacity *= 2;
			inputCustomerArrivals = (int *) realloc(inputCustomerArrivals, sizeof(int) * inputCapacity);
			inputCustomerProbabilities =
					(int *) realloc(inputCustomerProbabilities, sizeof(int) * inputCapacity);
		}
	}

	fclose(fin);

	// Initialize queue and server list
	int customersServiced = 0, totalCustomers = 0;
	double cumulativeWaitTime = 0.0, avgWaitTime = 0.0;
	// Array of all customers wait times during this simulation
	double *customerWaitTimes;
	int waitTimeCapacity = 20;

	customerWaitTimes = (double *) malloc(sizeof(double) * waitTimeCapacity);

	// Initializes a head node for the queue of customers (head is NOT a customer)
	struct Customer *queue = newQueue();
	// Initialize a head node for the list of servers
	struct Server *servers = newServerList(numServers, avgServiceTime);

	/* Iterate through each minute of an 8 hour work day (480 minutes).
	 * Check the queue and server/teller status at each minute.
	 * Process customers when servers are available.
	 * Each iteration simulates 0.1 minutes by default
	 */
	double currentTime = 0.0;

	while (currentTime <= MINUTES_PER_SIM) {
		// Customers enter at 1 minute intervals for simulation simplicity
		if (fmod(currentTime, 1.0) == 0.0) {
			int newArrivals =
					getCustomersPerMin(inputSize, inputCustomerArrivals, inputCustomerProbabilities);
			for (int i = 1; i <= newArrivals; i++)
				pushCustomer(queue, ++totalCustomers, currentTime);
		}

		iterateServersByInterval(servers, SIM_TIME_INTERVAL);
		printf("[%0.0f]: In Queue: %d\tServers Available: %d\n", currentTime, getQueueSize(queue), getAvailableServers(servers));

		// Move customers to available servers until no more are available or the queue reaches 0.
		while (getAvailableServers(servers) > 0 && getQueueSize(queue) > 0) {
			double arrivalTime;
			customersServiced++;

			// Removes customer from queue and gets their arrival time
			arrivalTime = serveCustomer(queue, servers);

			if (arrivalTime >= 0.0) {
				cumulativeWaitTime += currentTime - arrivalTime;
				printf("\tCustomer being helped - time waited: %0.0f\n", currentTime - arrivalTime);
				// Save wait time to array and resize array if necessary
				customerWaitTimes = saveWaitTime(currentTime - arrivalTime,
						customersServiced,
						customerWaitTimes,
						&waitTimeCapacity);
			}
		}


		currentTime += SIM_TIME_INTERVAL;
	} // End of time loop

	// Report simulation results
	avgWaitTime = cumulativeWaitTime / (double) customersServiced;

	printf("Results: Customers Helped: %d\tAverage Wait Time: %0.0f\n",
			customersServiced,
			avgWaitTime);

	printCustomerLog(customerLog, customersServiced, avgWaitTime, customerWaitTimes);

	fprintf(simResults, "Number of customers that arrived: %d\n", totalCustomers);
	fprintf(simResults, "Number of customers helped: %d\n", customersServiced);
	fprintf(simResults, "Average customer wait time: %0.0f\n", avgWaitTime);
	fprintf(simResults, "Number of customers helped by each teller:\n");
	printServers(servers, simResults);

	free(customerWaitTimes);
	deleteQueue(queue);
	deleteServerList(servers);
}


/**
 * Generates the number of customers arriving by randomly generating a number between 0-100.
 * The number of customers per minute is determined by the range the randomly generated
 * number falls into. The range and probability of outcomes can be modified to suit
 * different models in the future.
 * @return customersPerMin - number of customers arriving per minute
 */
int getCustomersPerMin(int inputSize, int numCustomers[], int probability[]) {

	// Generates a random number between a 0-100
	int r = rand() % 101;
	int n = 0;

	/* Determines the number of customers per minute based on the range the random number
	 * falls into. These intervals and number of customers can be modified to alter the
	 * simulation. Current values based on project instructions. */
	for (int i = 0; i < inputSize; i++) {
		n += probability[i];
		if (r <= n)
			return numCustomers[i];
	}

	return numCustomers[inputSize - 1];
}


/**
 * Handles the process of removing a customer from the queue and assigning them to
 * an available server/teller. If no server is avaialable the customer remains in the
 * queue and this function returns -1.0.
 * @param queue head node of the queue
 * @param servers head node of the server list
 * @return arrivalTime The time the customer entered the queue
 * @return -1 No server was available to serve the customer
 */
double serveCustomer(struct Customer *queue, struct Server *servers) {

	if (findAndAssignServer(servers) > 0) {
		double result = popCustomer(queue);
		return result;
	}

	return -1.0;
}


/**
 * Save a customer's wait time to an array. If the array size nears capacity, memory is
 * reallocated and array capacity is doubled.
 * @param newTime	Wait time of the customer being added to the array
 * @param customerNum Index of the new value corresponding to customerID
 * @param waitTimeArray Address of the array that the data is being added to
 * @param currentCapacity Currently capacity allocated for the array
 * @return waitTimeArray Address of the array (may change if reallocated)
 */
double *saveWaitTime(double newTime,
		int customerNum,
		double *waitTimeArray,
		int *capacity) {

	if (customerNum + 2 >= *capacity) {
		*capacity *= 2;
		waitTimeArray = (double *) realloc(waitTimeArray, *capacity);
	}
	waitTimeArray[customerNum - 1] = newTime;

	return waitTimeArray;
}


/**
 * Output customer data from the simulation (customers already serviced, not in queue)
 * @param fout Pointer to an output stream
 * @param customersServiced Number of customers that have been serviced
 * @param avgWaitTime Average wait time for customers
 * @param waitTimes Array of all customer wait times
 */
void printCustomerLog(FILE *fout, int customersServiced, double avgWaitTime, double *waitTimes) {

	fprintf(fout,
			"Customers Serviced: %d\nAverage Wait Time: %0.00f\nCustomer Wait Times: [ ",
			customersServiced, avgWaitTime);

	for (int i = 0; i < customersServiced; i++)
		fprintf(fout, "%d:%0.0f, ", i, waitTimes[i]);

	fprintf(fout, " ]\n");
}


/**
 * Command line arguments can be used to set the simulation parameters regarding the
 * number of servers and average service time per customer.
 * Accepted argument formats:
 * $ (exe) avgServiceTime
 * $ (exe) startingServers maxServers
 * $ (exe) minServers startingServers maxServers avgServiceTime
 *
 * @param argc Number of program arguments passed
 * @param argv[] Array of program arguments passed
 * @param avgServiceTime Pointer to variable for average service time for the simulation
 * @param minServers Pointer to variable for minimum servers tested in the simulation
 * @param numServers Pointer to variable for starting/current number of servers in the sim
 * @param maxServers Pointer to variable for maximum servers tested in the sim
 *
 * @return 0 No program arguments, continues with default values
 * @return 1 Simulation variables successfully overridden by program argument values
 * @return -1 Invalid parameters, see console output for error message
 */
int overrideParameters(const int argc,
		const char *argv[],
		double *avgServiceTime,
		int *minServers,
		int *numServers,
		int *maxServers
) {

	if (argc == 1) // No program arguments
		return 0;

	else if (argc == 2) {  // Average service time only
		*avgServiceTime = atof(argv[1]);
		if (*avgServiceTime <= 0.0) {
			// Validate avgServiceTime
			printf("ERROR: Invalid program argument: avgServicTime.\n"
			       "Accepted argument formats:\n"
			       "$ (target) avgServiceTime\n"
			       "$ (target) startingServers maxServers\n"
			       "$ (target) minServers startingServers maxServers avgServiceTime\n");
			return -1;
		}
		else
			return 1;
	}
	else if (argc >= 4) {
		int tempMin = (int) argv[1];
		int tempMax = (int) argv[3];
		int tempStarting = (int) argv[2];

		// If there are problems with arguments, warn user and continue with default values
		if (tempStarting > tempMax || tempMin > tempStarting || tempMin < 1) {
			printf("ERROR: Invalid program argument: server parameters.\n"
			       "Accepted argument formats:\n"
			       "$ (target) avgServiceTime\n"
			       "$ (target) startingServers maxServers\n"
			       "$ (target) minServers startingServers maxServers avgServiceTime\n");
			return -1;
		}
		else { // Valid arguments
			*minServers = tempMin;
			*numServers = tempStarting;
			*maxServers = tempMax;
		}
		// If there is a 5th argument, it is assigned to avgServiceTime
		if (argc >= 5) {
			*avgServiceTime = atof(argv[1]);
			// Validate avgServiceTime
			if (*avgServiceTime <= 0.0) {
				printf("ERROR: Invalid program argument: avgServicTime.\n"
				       "Accepted argument formats:\n"
				       "$ (target) avgServiceTime\n"
				       "$ (target) startingServers maxServers\n"
				       "$ (target) minServers startingServers maxServers avgServiceTime\n");
				return -1;
			}
			else
				return 1;
		}
		return 1;
	}
	else
		return 0;
}