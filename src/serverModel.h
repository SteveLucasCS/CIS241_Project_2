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


/** A linked list node representing a server/teller */
struct Server {
	/** Number of iterations the server is expected to be busy for.
	 * (If serviceTime == 2.2, busyFor will == 2.2 when the customer arrives.
	 * Each iteration with decrement this value. When busyFor == 0, the server
	 * is available for service. */
	double busyFor;

	/** Time it takes this server to completely service one customer. */
	double serviceTime;

	/** Number of customers served in this node's lifetime (usually 1 simulation) */
	int numServed;

	/** ID number corresponding to a server/teller */
	int serverID;

	struct Server *next;
};

struct Server *newServerList(int, double);
double expdist(double);
int serversEmpty(struct Server *);
int pushServer(struct Server *, double);
int popServer(struct Server *);
int deleteServerList(struct Server *);
int findAndAssignServer(struct Server *);
int iterateServersByInterval(struct Server *, double);
int getAvailableServers(struct Server *);
void printServerList(struct Server *, FILE *);

#endif //CIS241_PROJECT_2_SERVERMODEL_H
