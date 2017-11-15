/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include "myshell.h"

/*
	This function executes a command of type CMD_PIPE.
	It pipes the input stream to the read end and the output stream to the write end.
*/

int exec_pipe(SHELLCMD* node) {
	int status;
	int currentSTD; // current standard I/O
	if((currentSTD = dup(0)) == -1) { // assigns dup to currentSTD, returns if unsuccessful
		perror("dup()");
		return 1;
	}
	int fd[2]; // creates pipe
	if(pipe(fd) == -1) { // try to pipe. If pipe is unsuccesful, return.
		perror("pipe()");
		return 1;
	}
	pid_t pid;
	switch(pid = fork()) { // create new process
		case -1:
			perror("fork()");
			return 1;
		case 0:
			close(fd[0]); // close the read end
			if(dup2(fd[1],1) == -1) { // assign the standard output to the write end of the pipe
				perror("dup2()");
				_exit(EXIT_FAILURE);
			}
			execute_shellcmd(node->left); // execute the left command  
			_exit(1); // exit child process
		default:
			while(wait(&status)!=pid); // wait until child process is complete
			close(fd[1]); // close write end
			if(dup2(fd[0],0) == -1) { // assign the standard input to the read end of the pipe
				perror("dup2()");
				exit(EXIT_FAILURE);
			}
			execute_shellcmd(node->right); // execute right command
	}
	if(dup2(currentSTD, 0) == -1) { // reassign the current stream to stdin
		perror("dup2()");
		return 1;
	}
	return 0; // if it reaches this line, it has to have succeeded
}
