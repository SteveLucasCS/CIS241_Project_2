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

/* This header includes models and functions prototypes related to the queue. */

#ifndef CIS241_PROJECT_2_SRC_QUEUE_H_
#define CIS241_PROJECT_2_SRC_QUEUE_H_

/** A linked list node representing a customer in queue */
struct Customer {
	/** The minute of the day that the customer arrived (an 8 hour day has 480 minutes */
	double arrivalTime;
	/** Number ID of customer (for now it is what number customer they are that day)
	 * i.e. the 43th customer to enter the bank that day would have the ID of 43 */
	int customerID;

	struct Customer *next;
};

struct Customer *newQueue();
int pushCustomer(struct Customer *, int, double);
double popCustomer(struct Customer *);
int getQueueSize(struct Customer *);
int deleteQueue(struct Customer *);
void printQueue(struct Customer *, FILE *);

#endif //CIS241_PROJECT_2_SRC_QUEUE_H_
