/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include "myshell.h"

/*
	This function defines execution if the input is two commands,
	where they are semicolon-separated.	
*/

int exec_semicolon(SHELLCMD *t) { 
	int status = 0;
	if(t->left != NULL) { // if the left subtree of the command tree is not empty
        if(ioRedirect(t)!=0){ exit(1);}
		status = execute_shellcmd(t->left); // execute left command and return status
	}
	if(t ->right != NULL) { // if the right subtree of the command tree is not empty
        if(ioRedirect(t)!=0){ exit(1);}
		status = execute_shellcmd(t->right); // execute right command and return status
	}
	return status;
}

/*
	This function defines execution if the input is two commands,
	where they are and-separated.	
*/

int exec_and(SHELLCMD *t) {
	int status = 0;
	if(t -> left != NULL) { // if the left subtree of the command tree is not empty
        if(ioRedirect(t)!=0){ exit(1);}
		status = execute_shellcmd(t->left); // execute left command and return status
		if(status == 0 && t-> right != NULL) { // if the left command succeeded and the right subtree is empty
			status = execute_shellcmd(t->right); // execute right command and return status
		}
	}
	return status;
}

/*
	This function defines execution if the input is two commands,
	where they are or-separated.	
*/

int exec_or(SHELLCMD *t) {
	int status = 0;
	if(t -> left != NULL) { // if the left subtree of the command tree is not empty
        if(ioRedirect(t)!=0){ exit(1);}
		status = execute_shellcmd(t->left); // execute left command and return status
		if(status != 0 && t-> right != NULL) { // if the left command failed and the right subtree is empty
			status = execute_shellcmd(t->right); // execute right command and return status
		}
	}
	return status;
}


