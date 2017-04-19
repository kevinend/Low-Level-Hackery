#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sys/types.h"
#include "sys/wait.h"
#include "sys/user.h"
#include "sys/ptrace.h"

int
main( void )
{
	pid_t child_id;

	printf( "In the parent process\n" );
	child_id = fork();
	if ( child_id < 0 ) {
		perror( "fork" );
		exit( EXIT_FAILURE );
	}
	else if ( child_id == 0 ) {
		printf( "In the child process..doing some work\n" );
		exit( EXIT_SUCCESS );
	}

	// need to wait for the child to terminate first
	int status;
	int i = 1;
	while ( i > 0 ) {
		
		wait( &status );
		--i;
	}
	
	printf( "End of the parent process\n" );
	return 0;
}
