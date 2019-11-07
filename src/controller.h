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

#ifndef CIS241_PROJECT_2_SRC_SIMULATION_H_
#define CIS241_PROJECT_2_SRC_SIMULATION_H_


void simulation(int, double);
void moveToServer(struct Customer *, struct Server *);
struct Customer * newQueue();
struct Server * newServers(int, double);
int getCustomersPerMinute();

/**
 * Runs one simulation of the model with the number of servers/tellers
 * determined by the function parameter. Individual server speed, average
 * server speed, and frequency of customer arrival are determined by pseudo-
 * randomly generated numbers and may vary each time this function is called.
 */
void simulation(int numServers, double avgServiceTime) {
	int queueSize = 0;
	// Initializes a head node for the queue of customers (head is NOT a customer)
	struct Customer *queue = newQueue();
	// Initialize a head node for the list of servers
	struct Server *serverList = newServers(numServers, avgServiceTime);

	//findOpenServer(serverList);
	moveToServer(queue, serverList);
}

/**
 * Handles the process of the next customer in queue moving from the queue to a server.
 */
void moveToServer(struct Customer *queueHead, struct Server *server) {

}

/** Initialize a new empty queue
 * @return head for a list of Customer nodes
 */
struct Customer * newQueue() {
	struct Customer *head = (struct Customer*)malloc(sizeof(struct Customer));
	if (head == NULL)
		exit(-99);

	head->timeInQueue = 0.0;
	head->customerID = 0;
	head->next = NULL;

	return head;
}

struct Server * newServers(int numServers, double avgServiceTime) {
	struct Server *head = (struct Server*)malloc(sizeof(struct Server));
	if (head == NULL)
		exit(-9);

	head->serviceTime = 0.0;
	head->serverID = 0;
	head->next = NULL;


	// Push 'numServers' more servers to the stack
	for (int i = 0; i < numServers; i++) {
		pushServer(head, avgServiceTime);
	}

}


/**
 * Generates the number of customers arriving by randomly generating a number between 0-100.
 * The number of customers per minute is determined by the range the randomly generated
 * number falls into. The range and probability of outcomes can be modified to suit
 * different models in the future.
 * @return customersPerMin - number of customers arriving per minute
 */
int getCustomersPerMinute() {
	// Generates a random number between a 0-100
	int r = rand() % 101;

	/* Determnines the number of customers per minute based on the range the random number
	 * falls into. These intervals and number of customers can be modified to alter the
	 * simulation. Current values based on project instructions.
	 */
	if (r >= 0 && r <= 15)  // 0-15 (15% chance)
		return 0;
	else if (r <= 35)  // 16-35 (20% chance)
		return 1;
	else if (r <= 60)  // 36-60 (25% chance)
		return 2;
	else if (r <= 70) // 61-70 (10% chance)
		return 3;
	else if (r <= 100)  // 71-100 (30% chance)
		return 4;
	// Random number is outside expected range (0-100); return -1
	printf(ANSI_RED "Error" ANSI_DEFAULT "in getCustomersPerMinute(): random number is "
	                                     "outside expected range (0-100)");
	return -1;
}

#endif
