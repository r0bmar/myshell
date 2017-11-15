/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include "myshell.h"
int lastexecstatus;
pid_t finished_cpid;

/*
    This function executes a simple command that is input into the shell.
    It uses a combination of fork and exec, along with other functions created
    as part of this project.
*/
int simplecommand(SHELLCMD *t) {
    pid_t pid;
    int status;
        if(!isInternal(t)) { // if this is an external Unix command
        switch(pid=fork()) // create new process
        {
            case -1:
                perror("fork()");
                exit(1);
            case 0:
                if(ioRedirect(t)!=0) { // if the output/input is redirected unsuccessfully
                    exit(1); // exit with failure
                }
                /*
                    The following line attempts to execute the input. If it does
                    not execute, it will assume that the input is a shellscript
                    and execute each line of the shellscript. See 8_shellscripts.c 
                    for more on this.
                */
                status = exec_shellscript(t);
                _exit(1); // exit the child process
                break;

            default:
                while(wait(&status)!=pid);// wait for the child process to finish executing
                break;
        }
        finished_cpid = pid;
    }
    else { // if this is an internal command from myshell
        lastexecstatus = execute_internal(t);
        if(WIFEXITED(lastexecstatus)) {
            return WEXITSTATUS(lastexecstatus);
        } // execute the command
    }
    return lastexecstatus=WEXITSTATUS(status); // returns the value corresponding to the success or failure of this function
}
