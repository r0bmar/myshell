/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include "myshell.h" 

/*
    This function executes a given command as a subshell.
    It returns the status of the process (0 if successful).
*/

int exec_subshell(SHELLCMD *t) {
	int status = 0; 
	pid_t pid; 
	switch(pid=fork()) { // creating new process
      case -1:
        perror("fork()");
        exit(1);
        break;
      case 0:
        if(ioRedirect(t)!=0) { // if we have to redirect, redirect. If the redirection fails, exit.
            exit(1);
        } 
      	status = execute_shellcmd(t->left); // execute the next command
        _exit(status); // exit child process
        break;
      default:
        while (wait(&status) != pid) { // complete all processes 
        	status = WEXITSTATUS(status); // set the status to the return value of the child process
        }
        break;
    }
	return status;
}
