#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>

/*
 * Since fork gives us an exact image of our program we can use the parent program
 * as a debugger for the child process. The use of ptrace allows us to examine
 * the state of any given process, so we can use it within the "debugger" to
 * give us information about our child program.
 *
 * NOTE: This is the building block -- modern debuggers are way more complicated than this
 *
 * int execve( const char *filename, char * const argv[], char *const envp[] );
 * long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);
 */

/* IMPORTANT:
 *  	If the current program is being ptraced, a SIGTRAP signal is sent to
 *		it after a successful execve().
 *
 *		 In lamens terms -- the program will hang until we begin tracing -- SIGTRAPS are breakpoints in a typical debugger
 */


void initialize_target_program( char *program_name );
void run_debugger( pid_t program_id );

void 
initialize_target_program( char *program_name ) 
{
	// setup ptrace for this particular process, then exec to overwrite address space with program to debug
	pid_t current_pid;
	char *arg_list[] = { NULL };

	current_pid = getpid();
	if ( ptrace( PTRACE_TRACEME, current_pid, 0, 0 ) < 0 ) {
		perror( "ptrace" );
		return;
	}

	execve( program_name, arg_list, NULL );
	return;
}

void
run_debugger( pid_t program_id ) 
{
	// SIGTRAP sent to the parent before the process begins executing 
	int status;
	int instruction_count = 0;
	printf( "Debugging this current process...\n" );

	wait( &status );
	while ( WIFSTOPPED( status ) ) {
		instruction_count++;		
		ptrace( PTRACE_SINGLESTEP, program_id, 0, 0 );
		wait( &status );
	}
	
	printf( "End of this program\n" );
	printf( "Total number of instructions executed == %d\n", instruction_count );
	return;
}

int 
main( int argc, char **argv ) 
{
	if ( argc < 2 ) {
		printf( "No target program name supplied\n" );
		exit( EXIT_FAILURE );
	}

	pid_t child_process;

	child_process = fork();
	if ( child_process < 0 ) {
		perror( "fork" );
		exit( EXIT_FAILURE );
	}
	else if ( child_process == 0 ) {
		initialize_target_program( argv[1] );
	}
	else {
		run_debugger( child_process );
	}

	return 0;

}
