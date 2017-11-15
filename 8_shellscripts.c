/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include "myshell.h"

/*
	This function executes a given shellscript.
	Note that if the input is a function that exists in Unix, then it will
	exit at the execv() line. So the rest of this function handles if it is
	not a Unix function.
	Returns 0 if successful, a non zero value if unsuccessful.
*/

int exec_shellscript(SHELLCMD* node) 
{
	int status = 0; 
	if(SearchPath(node->argv[0])) { // if the function is found
		execv(node->argv[0], node->argv); // run function.
		/*
			This will exit this function if it succeeds. If it fails the assumption
			is that a shellscript exists with the given name, and will thus execute the
			remainder of this function.
		*/
	}
	FILE * file = fopen(node->argv[0],"r"); // open the file, if it exists, with the name of the argument given to the shell
	if(file == NULL) {
		fprintf(stderr, "%s\n", " executable file does not exist."); // if the shellscript with this name does not exist
	}
	while(!feof(file)) { // while the end of the file has not been reached
		SHELLCMD * temp = parse_shellcmd(file); // create a new shellcommand tree and parse it 
		status = execute_shellcmd(temp); // execute the new tree
		free(temp); // free dynamically allocated memory for the shellcommand
	}
	fclose(file); // close file
	return status;
}
