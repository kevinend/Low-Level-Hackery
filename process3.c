#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sys/types.h"
#include "sys/wait.h"
#include "sys/user.h"
#include "sys/ptrace.h"

/* 
 * forking copies an entire process' address space, so variables declared 
 * before the fork would have independent copies of the data.
 * (basically the opposite of threads)
 *
 */

 #define NUM_OF_CHILDREN 5

 int 
 main( void )
 {
 	// each process gets its own copy of `indpendent_var`
 	int independent_var = 10;
 	pid_t child_processes[NUM_OF_CHILDREN];

	printf( "Parent Begin:\n" );
	printf( "Parent Process, independent_var == %d\n", independent_var );
	for ( int i = 0; i < NUM_OF_CHILDREN; ++i ) {

		child_processes[i] = fork();
		if ( child_processes[i] < 0 ) {
			perror( "fork" );
			exit( EXIT_FAILURE );
		}
		else if ( child_processes[i] == 0 ) {
			++independent_var;
			printf( "In a child process, independent_var == %d\n", independent_var );
			exit( EXIT_SUCCESS );
		}	
	}

	int child_count = NUM_OF_CHILDREN;
	int child_status;

	while ( child_count > 0 ) {

		wait( &child_status );
		--child_count;
	}

	printf( "Parent Process, independent_var == %d\n", independent_var );
	printf( "Parent End:\n" );	

	return 0;

 }
