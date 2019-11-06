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

//#include "simulationModel.h"
//#include "simulationProcesses.h"

#define ANSI_RED "\x1b[31m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_DEFAULT "\x1b[0m"

#define DEFAULT_AVG_SERVICE_TIME 2.0
#define DEFAULT_MAX_SERVERS 4
#define DEFAULT_MIN_SERVERS 7
#define DEFAULT_CURRENT_SERVERS 7


int overrideParameters(int, const char **, double *, int *, int *, int *);


int main(int argc, char *argv[]) {
	// Initialize and seed pseudo-random number generator
	srand(time(0));

	/** An array of servers (tellers in this case is initialized with float values.
	 * Each value of the array corresponds to a servers's average time to service one
	 * customer. (Server refers to whatever is serving a customer in the queue, whether
	 * it's a bank teller or network server responding to a ping).
	 */
	double *serverArray;

	/** Minimum number of servers (i.e. cannot have less than 1) */
	int minServers = DEFAULT_MIN_SERVERS;
	/** Maximum number of servers (i.e. cannot have more than 20) */
	int maxServers = DEFAULT_MAX_SERVERS;
	/** Current number of servers in the simulation */
	int currentServers = DEFAULT_CURRENT_SERVERS;

	/** The average time it takes 1 server to completely service 1 customer (in MINUTES)*/
	double avgServiceTime = DEFAULT_AVG_SERVICE_TIME;

	/* Check for command line arguments, override simulation parameters if arguments are valid */
	switch (overrideParameters(argc,
			(const char **) argv, &avgServiceTime, &minServers, &currentServers, &maxServers)) {
		case 1:
			// Parameters overridden successfully
			printf("Command line arguments found - simulation parameters modified\n");
			break;
		case -1: return -1;
		case -2: return -1;
		default: break; // Continues with default parameters
	}

	/* Run the simulation with the number of servers/tellers passed as the argument */
//	simulation (currentServers);

}

/**
 * Command line arguments can be used to set the simulation parameters regarding the
 * number of servers and average service time per customer.
 * Accepted argument formats:
 * $ (target) avgServiceTime
 * $ (target) currentServers maxServers
 * $ (target) minServers currentServers maxServers avgServiceTime
 * @return status
 */
int overrideParameters(const int argc,
		const char *argv[],
		double *avgServiceTime,
		int *minServers,
		int *currentServers,
		int *maxServers
) {
	if (argc == 1) // No command line arguments
		return 0;

	else if (argc == 2) {  // Average service time only
		*avgServiceTime = atof(argv[1]);
		if (*avgServiceTime <= 0.0) {
			// Validate avgServiceTime
			printf(ANSI_RED "ERROR: " ANSI_DEFAULT "Invalid command line argument: avgServicTime.\n"
			       "Accepted argument formats:\n"
			       "$ (target) avgServiceTime\n"
			       "$ (target) currentServers maxServers\n"
			       "$ (target) minServers currentServers maxServers avgServiceTime\n");
			return -1;
		}
		else {
			return 1;
		}
	}
	else if (argc >= 4) {
		int tempMin = (int) argv[1];
		int tempMax = (int) argv[3];
		int tempCurrent = (int) argv[2];

		// If there are problems with arguments, warn user and continue with default values
		if (tempCurrent > tempMax || tempMin > tempCurrent || tempMin < 1) {
			printf(ANSI_RED "ERROR: " ANSI_DEFAULT "Invalid command line argument: server parameters.\n"
			       "Accepted argument formats:\n"
			       "$ (target) avgServiceTime\n"
			       "$ (target) currentServers maxServers\n"
			       "$ (target) minServers currentServers maxServers avgServiceTime\n");
			return -2;
		}
		else { // Valid arguments
			*minServers = tempMin;
			*currentServers = tempCurrent;
			*maxServers = tempMax;
		}
		// If there is a 5th argument, it is assigned to avgServiceTime
		if (argc >= 5) {
			*avgServiceTime = atof(argv[1]);
			// Validate avgServiceTime
			if (*avgServiceTime <= 0.0) {
				printf(ANSI_RED "ERROR: " ANSI_DEFAULT "Invalid command line argument: avgServicTime.\n"
				       "Accepted argument formats:\n"
				       "$ (target) avgServiceTime\n"
				       "$ (target) currentServers maxServers\n"
				       "$ (target) minServers currentServers maxServers avgServiceTime\n");
				return -1;
			}
			else
				return 1;
		}
		return 1;
	}
}
