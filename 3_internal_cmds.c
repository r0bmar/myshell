/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include "myshell.h"

/*
    This function checks if the given input command is an internal command.
    If it is, it returns true, if not, it returns false.
*/


bool isInternal(SHELLCMD *node) {
    return( (strcmp(node->argv[0],"exit") == 0) || (strcmp(node->argv[0],"cd") == 0) || (strcmp(node->argv[0],"time") == 0));
}

/*
    This function will attempt to execute an internal command. There are 3 cases.
    These cases are outlined below in the documentation.

    If command passed is either exit ,cd or time (regarded as internal commands in myshell), the run the following 
    function which returns the status of either of the called commands. 

*/

bool isStringNumber(char *string)
{
    char * p =string;
    while (*p!='\0')
    {
        if(!isdigit(*p)) return false;
        p++;
    }
    return true;
}

int execute_internal(SHELLCMD *node) {
    int status=1;
    if(strcmp(node->argv[0],"exit") == 0) { // if the input is exit
        if(node->argc==1)
        {
            kill(0, SIGKILL);
                
            exit(lastexecstatus); // exit the shell
        }
        else if(node->argc==2 && isStringNumber(node->argv[1]))
        {
            int exitstatus= atoi(node->argv[1]);
            kill(0, SIGKILL);
            exit(exitstatus);
        }
        else
        {
            fprintf(stderr,"\nincorrect call of exit. Call as exit or exit (numeric argument)\n");
            status=1;
        }
    }
    else if(strcmp(node->argv[0],"time") == 0) { // if the input is time
        if(node->argc == 1) { // if there are no arguments
            status = 1;
             fprintf(stderr,"No command given \nTime is : %d\n",0); // print out a time of 0
        }
        else { // if there is 1 or more arguments
            struct timeval start, end; // start and end time 
            gettimeofday(&start, NULL); // get the current time
            char ** current = node->argv; // make a new variable to point to the current input argument
            node->argv++; // move the pointer across 1 to the next argument (the first argument of the time function)
            node->argc--; // as we are performing simple commands over the next string in argv, we must let the program know that since we dont care about the first string the arguments which are called over the new command are 1 less than before.
            status = simplecommand(node); // execute the first argument
            node->argv--; // change the pointer back to the current input argument
            node->argv = current; 
            gettimeofday(&end, NULL); // get the current time 
            double delta = ((end.tv_sec  - start.tv_sec)*1000.0 + (end.tv_usec - start.tv_usec)/1000.0); // get the time difference 
            fprintf(stderr,"Time is : %f msec\n", delta); // print the difference in time
        }
        return status; // return exit status
    }
    else // if input is "cd"
    {
        // start with assumption program has failed 
        status=1;
        // if only one argument is provided i.e cd , make change from current working directory to Home
        if(node->argc==1)
        {
            status= chdir(HOME);
            //if unsuccessful report error 
            if(status!=0)
            {
              perror(HOME);
            }
        }
        /*
            For commands where cd is provided arguments 
        */
        else if(node->argc==2)
        {
                // checks if argument provided is a pathway to new directory or not.
                if(strrchr(node->argv[1],'/')==NULL)
                {
                    const char seperator= ':'; // seperator in CDPATH
                    char newPath[PATH_MAX]=""; // The string of characters which will test if a valid directory can be made with the given arguments 
                    char *p=newPath; // constructs newPath
                    char *cdpath= CDPATH; //traverses CDPATH
                    // loop through CDPATH till the end to test all cases
                    while(*cdpath!='\0')
                    {
                        /*  
                            If separator is found, then previous characters found before the : in CDPATH
                            may be a possible valid directory to cd into .
                        */
                        if(*cdpath==seperator)
                        {
                            strcat(newPath,"/");
                            strcat(newPath,node->argv[1]);
                            status=chdir(newPath);
                            if(status==0)
                            {
                                // if change of directory is successful and is possible, exit while loop
                                break;
                            }
                            // if previous versions of constructed newPath fails, change to empty string to build a new one.
                            strcpy(newPath,"");
                            p=newPath;
                            cdpath++; // proceed to next char after : in CDPATH
                            continue; 
                        } 
                        *p=*cdpath; // create possible cdpath's for arguments provided for cd command in newPath
                        p++; 
                        cdpath++;
                    } 
                }
                else
                {
                    // if argument provided starts with '/'
                    status=chdir(node->argv[1]);
                }
        }
        // if all the above cases fail invalid argument is provided for cd. 
        if(status!=0)
        {
            perror(node->argv[1]);
        }
    }
    return status;
}
