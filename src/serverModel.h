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


#ifndef CIS241_PROJECT_2_SERVERMODEL_H
#define CIS241_PROJECT_2_SERVERMODEL_H

#include <stdio.h>

/**
 * Calculates a randomized time it takes a specific teller to service one customer
 * based on the mean service time provided as the argument. Allows a the model to
 * simulate multiple tellers with different individual working speeds.
 * (Method provided by CIS241 Project 2 Instructions)
 * @param mean - mean service time per customer
 * @return time to service one customer
 */
double expdist(double mean) {
	double r = rand();
	r /= RAND_MAX;
	return -mean * log(r);
}

/** A linked list node representing a server/teller */
struct Server {
	/** Boolean (0 or 1) representing if the server is busy. If false (0), the server is
	 * available to service a new customer */
	int busy;
	/** Time it takes this server to completely service one customer. */
	double serviceTime;
	/** ID number corresponding to a server/teller */
	int serverID;

	struct Server *next;
};

/** Inserts a node at the tail of the list
 * @param head first node of the server list
 * @param avgServiceTime mean time of all servers to service one customer
 * @return serverID returns the serverID number assigned during initialization
 */
int pushServer(struct Server *head, double avgServiceTime) {
	// create new node temp and allocate memory
	struct Server *temp;
	temp = (struct Server*)malloc(sizeof(struct Server));

	// Calculated this server's serviceTime based on mean
	temp->serviceTime = expdist(avgServiceTime);

	// Empty list, push node as head
	if (head == NULL) {
		temp->serverID = 0;
		temp->next = NULL;
		head = temp;
	}

	struct Server *current = head;
	// Iterate list until reaching the last node
	while (current->next != NULL) {
		current = current->next;
	}

	temp->serverID = current->serverID + 1;
	current->next = temp;
}

/** Removes the node at the head of the queue */
int popServer(struct Server *head) {
	struct Server *temp;
	int serverID;

	if (head == NULL || head->next == NULL) {
		printf("Cannot pop - stack is already empty.\n");
		return -1;
	}
	temp = head->next;
	head->next = temp->next;
	serverID = temp->serverID;
	// Delete first node
	free(temp);

	return serverID;
}

#endif //CIS241_PROJECT_2_SERVERMODEL_H
