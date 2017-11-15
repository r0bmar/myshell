#include "myshell.h"

/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */
int lastexecstatus = EXIT_SUCCESS; // the fact that we are starting our program it is trivial that if the first command called is just exit (noargs) then the status of the previous command is successful i.e =0.
pid_t kill_BackRunning_Processes;

int main(int argc, char *argv[])
{
//  REMEMBER THE PROGRAM'S NAME (TO REPORT ANY LATER ERROR MESSAGES)
    argv0	= (argv0 = strrchr(argv[0],'/')) ? argv0+1 : argv[0];
    argc--;				// skip 1st command-line argument
    argv++;

//  INITIALIZE THE THREE INTERNAL VARIABLES
    HOME	= getenv("HOME");
    if(HOME == NULL) {
	HOME	= DEFAULT_HOME;
    }

    PATH	= getenv("PATH");
    if(PATH == NULL) {
	PATH	= DEFAULT_PATH;
    }

    
    CDPATH	= getenv("CDPATH");
    if(CDPATH == NULL) {
	CDPATH	= DEFAULT_CDPATH;
    }

//  DETERMINE IF THIS SHELL IS INTERACTIVE
    interactive		= (isatty(fileno(stdin)) && isatty(fileno(stdout)));

    int exitstatus	= EXIT_SUCCESS;

    // OUR LASTEXITSTATUS

//  READ AND EXECUTE COMMANDS FROM stdin UNTIL IT IS CLOSED (with control-D)
    while(!feof(stdin)) {
	SHELLCMD	*t = parse_shellcmd(stdin);

	if(t != NULL) {

//  WE COULD DISPLAY THE PARSED COMMAND-TREE, HERE, BY CALLING:
//	    print_shellcmd(t);

	    exitstatus = execute_shellcmd(t);
        
	    free_shellcmd(t);
	}
    }
  
    if(interactive) {
	fputc('\n', stdout);
    }
    return exitstatus;
}
