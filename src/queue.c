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


/**
 * Initialize a new empty queue
 * @return head for a list of Customer nodes
 */
struct Customer *newQueue() {

	struct Customer *head = (struct Customer *) malloc(sizeof(struct Customer));
	if (head == NULL)
		exit(-99);

	head->arrivalTime = 0.0;
	head->customerID = 0;
	head->next = NULL;

	return head;
}


/**
 * Checks if the queue is empty. Returns 1 (empty) if head is NULL or head is the only node
 * @param head head node of a Customer list
 * @return boolean 1 if empty, 0 if not
 */
int queueEmpty(struct Customer *head) {

	if (head == NULL)
		return 1;
	else if (head->next == NULL)
		return 1;
	else
		return 0;
}


/**
 * Inserts a node at the tail of the queue
 * @param head head node of a Customer list
 * @param timeCreated time in the simulation that this node was created
 * @return customerID returns the customerID of the new node
 */
int pushCustomer(struct Customer *head, int customerID, double timeCreated) {
	// create new node temp and allocate memory
	struct Customer *temp = (struct Customer *) malloc(sizeof(struct Customer));

	if (temp == NULL) {
		printf("Error: failed to allocate memory for new Customer node.\n");
		exit(-88);
	}

	temp->arrivalTime = timeCreated;
	temp->customerID = customerID;
	temp->next = NULL;

	struct Customer *current = head;

	if (current == NULL)
		return -1;

	// Iterate list until reaching the last node
	while (current != NULL && current->next != NULL)
		current = current->next;

	current->next = temp;

	return temp->customerID;
}


/**
 * Removes the node at the beginning of the queue (first node after head)
 * @param head node of a Customer List
 * @return arrivedAt the time the customer entered the queue.
 */
double popCustomer(struct Customer *head) {

	if (head == NULL || head->next == NULL)
		return -1;

	struct Customer *temp = head->next->next;
	double arrivedAt = head->next->arrivalTime;

	// Delete first node
	free(head->next);
	head->next = temp;
	return arrivedAt;
}


/**
 * Returns the number of Customer nodes in a queue
 * @param head Head node of a Customer queue
 * @return count Number of nodes in the queue
 */
int getQueueSize(struct Customer *head) {

	struct Customer *current = head;
	int count = 0;

	while (current != NULL) {
		count++;
		current = current->next;
	}

	return count;
}


/**
 * Deletes all nodes in the queue
 * @param head head node of a Customer list
 * @return numDeleted number of nodes deleted
 */
int deleteQueue(struct Customer *head) {

	int numDeleted = 0;

	while (head != NULL && head->next != NULL) {
		popCustomer(head->next);
		numDeleted++;
	}

	free(head);

	return numDeleted;
}


/**
 * Print formatted queue information to specified output
 * @param head head node of a Customer list
 * @param fout pointer to an output stream
 */
void printQueue(struct Customer *head, FILE *fout) {

	if (queueEmpty(head))
		return;

	struct Customer *current = head;

	if (fout == NULL) {
		printf("queueLog file pointer is NULL\n");
		return;
	}

	fprintf(fout, "[ ");
	while (current != NULL && current->next != NULL) {
		current = current->next;
		fprintf(fout, "%d:%0.0f, ", current->customerID, current->arrivalTime);
	}
	fprintf(fout, " ]\n");
}


