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

#include "serverModel.h"


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


struct Server *newServerList(int numServers, double avgServiceTime) {

	struct Server *head = (struct Server *) malloc(sizeof(struct Server));
	if (head == NULL)
		exit(-9);

	head->serviceTime = 0.0;
	head->serverID = 0;
	head->next = NULL;

	// Push 'numServers' more servers to the stack
	for (int i = 0; i < numServers; i++) {
		pushServer(head, avgServiceTime);
	}

	return head;
}


/**
 * Checks if the list is empty. Returns 1 (empty) if head is NULL or head is the only node
 * @param head head node of a Server list
 * @return boolean 1 if empty, 0 if not
 */
int serversEmpty(struct Server *head) {

	if (head == NULL)
		return 1;
	else if (head->next == NULL)
		return 1;
	else
		return 0;
}


/**
 * Inserts a node at the tail of the list
 * @param head head node of a Server list
 * @param avgServiceTime mean time of all servers to service one customer
 * @return serverID returns the serverID number assigned during initialization
 */
int pushServer(struct Server *head, double avgServiceTime) {
	// create new node temp and allocate memory
	struct Server *temp;
	temp = (struct Server *) malloc(sizeof(struct Server));
	temp->busyFor = 0.0;
	// Calculated this server's serviceTime based on mean
	temp->serviceTime = expdist(avgServiceTime);
	temp->numServed = 0;
	temp->next = NULL;

	// Empty list, push node as head
	if (head == NULL) {
		temp->serverID = 0;
		temp->next = NULL;
		temp->serviceTime = 0;
		temp->numServed = 0;
		head = temp;
	}

	struct Server *current = head;
	// Iterate list until reaching the last node
	while (current != NULL && current->next != NULL)
		current = current->next;

	temp->serverID = current->serverID + 1;
	current->next = temp;

	return temp->serverID;
}


/** Removes the node at the head of the queue */
int popServer(struct Server *head) {

	struct Server *temp;
	int serverID;

	if (serversEmpty(head)) {
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


/**
 * Deletes all nodes in the list
 * @param head node of the server list
 * @return numDeleted number of nodes deleted
 */
int deleteServerList(struct Server *head) {

	int numDeleted = 0;

	struct Server *current = head;

	while (current->next != NULL) {
		popServer(current);
		numDeleted++;
	}

	free(head);

	return numDeleted;
}


/**
 * Checks for the first available server in the list. If found, the next customer is removed
 * from the queue, assigned to the server, and the ID of that server is returned. If all servers
 * are busy 0 is returned and nothing is modified.
 * @param head head node of a Server list
 * @return serverID ID of the assigned server, or 0 if no servers are available.
 */
int findAndAssignServer(struct Server *head) {

	struct Server *current = head;

	while (current != NULL && current->next != NULL) {
		current = current->next;
		// Check if the sever is busy
		if (current->busyFor == 0.0) {
			// Not busy; assign customer to this server and return its ID
			// busyFor is set to this server's time to service one customer
			current->busyFor = current->serviceTime;
			// numServed is incremented (assumes service will be completed)
			current->numServed++;

			return current->serverID;
		}
	}
	// No servers available
	return 0;
}


/**
 * Handles the iteration of time intervals for all servers.
 * (i.e, if interval == 0.1, busyFor decrements by 0.1)
 * @param head head node of a Server list
 * @param interval interval of time that has passed in the simulation
 * @return numAvailable the number of servers that are now available
 */
int iterateServersByInterval(struct Server *head, double interval) {

	int numAvailable = 0;

	if (serversEmpty(head))
		return numAvailable;

	struct Server *current = head;

	while (current != NULL && current->next != NULL) {
		current = current->next;
		// Update each server's data values based on interval of change
		if (current->busyFor - interval <= 0.0) {
			current->busyFor = 0.0;
			numAvailable++;
		}
		else
			current->busyFor -= interval;
	}

	return numAvailable;
}

/**
 * Returns the number of Server nodes in a list that are not busy
 * @param head Head node of a Server list
 * @return count Number of nodes in the list that are not busy
 */
int getAvailableServers (struct Server *head) {
	struct Server *current = head;
	int count = 0;

	if (current == NULL || current->next == NULL)
		return 0;

	while (current != NULL) {
		if (current->busyFor == 0.0)
			count++;
		current = current->next;
	}

	return count;
}


/**
 * Print formatted server information to specified output
 * @param head head node of a Server list
 * @param fout pointer to an output stream
 */
void printServers(struct Server *head, FILE *fout) {

	if (serversEmpty(head))
		return;

	struct Server *current = head;

	while (current != NULL && current->next != NULL) {
		current = current->next;
		fprintf(fout,
				" -> Teller %d: helped %d customers with an average service time of %f minutes.\n",
				current->serverID, current->numServed, current->serviceTime);
	}
}
