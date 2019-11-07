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

/* This header includes models and functions to create, update, and delete the queue. */

#ifndef CIS241_PROJECT_2_SRC_QUEUEMODEL_H_
#define CIS241_PROJECT_2_SRC_QUEUEMODEL_H_


/** A linked list node representing a customer in queue */
struct Customer {
	/** Amount of time spent waiting in queue */
	double timeInQueue;
	/** Number ID of customer (for now it is what number customer they are that day)
	 * i.e. the 43th customer to enter the bank that day would have the ID of 43 */
	int customerID;

	struct Customer *next;
};


/** Inserts a node at the tail of the queue
 * @param head node at the start of the queue
 */
void pushCustomer(struct Customer *head) {
	// create new node temp and allocate memory
	struct Customer *temp;
	temp = (struct Customer*)malloc(sizeof(struct Customer));

	temp->timeInQueue = 0.0;

	// Empty list, push node as head
	if (head == NULL) {
		temp->customerID = 0;
		temp->next = NULL;
		head = temp;
	}

	struct Customer *current = head;
	// Iterate list until reaching the last node
	while (current->next != NULL) {
		current = current->next;
	}
	temp->customerID = current->customerID + 1;
	current->next = temp;
}

/** Removes the node at the head of the queue */
double popCustomer(struct Customer *head) {
	struct Customer *temp;
	// Amount of time the deleted node was in queue
	double timeServed = -1.0;

	if (head == NULL || head->next == NULL) {
		printf(ANSI_YELLOW "Warning:" ANSI_DEFAULT " popCustomer failed - stack is already empty.\n");
		return timeServed;
	}

	temp = head->next;
	head->next = temp->next;
	timeServed = temp->timeInQueue;
	// Delete first node
	free(temp);

	return timeServed;
}


#endif //CIS241_PROJECT_2_SRC_QUEUEMODEL_H_
