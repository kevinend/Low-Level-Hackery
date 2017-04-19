#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sys/wait.h"
#include "sys/types.h"
#include "sys/ptrace.h" // ptrace system call
#include "sys/user.h"   // user_regs_struct

#define NUM_OF_CHILDREN 5

int 
main( void ) 
{
	pid_t process_ids[NUM_OF_CHILDREN];	
	
	printf( "Starting the parent off\n" );
	for ( int i = 0; i < NUM_OF_CHILDREN; ++i ) {
		process_ids[i] = fork();
		if ( process_ids[i] < 0 ) {
			perror( "fork" );
			exit( EXIT_FAILURE );
		}
		else if ( process_ids[i] == 0 ) {
			printf( "Within a child process\n" );
			exit( EXIT_SUCCESS );                    
		}
	}

	int children;
	int child_status;
	int child_pid;

	children = NUM_OF_CHILDREN;
	while ( children != 0 ) {

		child_pid = wait( &child_status );
		printf( "The child process %d terminated with status 0x%x\n", child_pid, child_status );
		--children;
	}

	printf( "Ending the parent\n" );
}


