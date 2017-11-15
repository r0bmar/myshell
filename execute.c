/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include "myshell.h"

// -------------------------------------------------------------------

//  THIS FUNCTION SHOULD TRAVERSE THE COMMAND-TREE and EXECUTE THE COMMANDS
//  THAT IT HOLDS, RETURNING THE APPROPRIATE EXIT-STATUS.
//  READ print_shellcmd0() IN globals.c TO SEE HOW TO TRAVERSE THE COMMAND-TREE

int execute_shellcmd(SHELLCMD *t)
{
    int  exitstatus = 0;
    if(t == NULL) {			// hmmmm, that's a problem
        exitstatus	= EXIT_FAILURE;
    }
    
    else if(t->type == CMD_COMMAND) { // normal, exit commands
        exitstatus = simplecommand(t);
    }
    
    else if(t->type == CMD_SEMICOLON) { // if two commands are given, separated by a semicolon
      exitstatus = exec_semicolon(t);

    }
    else if(t->type == CMD_AND) { // if two commands are given, separated by an and
      exitstatus = exec_and(t);
    }
    else if(t->type == CMD_OR) { // if two commands are given, separated by an or
      exitstatus = exec_or(t);
    }
    else if(t->type == CMD_SUBSHELL) { // if a command is given to be executed in a subshell
      exitstatus = exec_subshell(t);
    }
    else if(t->type == CMD_PIPE) { // if two commands are given, separated by a |
      exitstatus = exec_pipe(t);
    }
    else if(t-> type == CMD_BACKGROUND) // if the given command shall be executed in the background
      exitstatus = exec_background(t);
    return exitstatus;
}



