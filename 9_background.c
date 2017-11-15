/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include "myshell.h"

pid_t finished_cpid;
int processes[BUFSIZ];


/*
    This function handles the signal that is sent in the child process.
    It waits until all background processes are done and then jumps to the parent.
*/


void signal_handler(int sig) {
    bool statement = false; // checks if background processes are running in a subchild
    if(wait(NULL)==-1){
        fprintf(stderr,"%i process is finished\n",finished_cpid); // reports if process is finished
        statement = true;
    }
    while(wait(NULL)!=-1)
        ;
    if(!statement) fprintf(stderr,"%i process is finished\n",finished_cpid); // reports if process is finished
    kill(getpid(),SIGTERM);

}

/*
    This function attempts to execute a background process.
    It registers a signal before forking, then places one process in the background
    before executing another.
    It returns the success value of the execution.
*/

int exec_background(SHELLCMD *node) {	
    int status; // reports status of funciton
    pid_t pid; // tracks child pid in fork
    signal(SIGUSR1,signal_handler); // register signal so that it can be used to tell the program the background process is doen
	 // signal is registered
	switch(pid = fork()) { // fork begins
		case -1: // if fork is unsuccessful
			perror("fork()");
			exit(1);
		case 0: // child process
			fprintf(stderr,"%i's child has started execution\n",getpid()); // notify background process has started
            if(execute_shellcmd(node->left)==1) status=1; // run background process and if it fails, set status =-1. 
			kill(getpid(),SIGUSR1);
            _exit(1);
			break;

		default:
            status=execute_shellcmd(node->right); // execute foreground command
            break;
    }
	return status;
} 

