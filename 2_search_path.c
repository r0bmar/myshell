/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include "myshell.h"

#define COLON ':' // the separator

/*
    This function takes two arguments, a character pointer and a boolean variable
    In the case where iscd is false, it updates the path for a given Unix function to its full path.
    In the case where iscd is true, it finds the path for a given input
    If the given Unix function does not exist, it new_pathurns without changing the path, and prints an error message.
*/

bool SearchPath(char *t) 
{
  if(t[0]=='/') return true; // if there is a slash in the input, nothing is new_pathurned
  char* path= NULL; 
  path = strdup(DEFAULT_PATH); // sets the current path to the default path
  char *start=path;
  char *new_path; // current path to check
  bool NoColons=false;
  bool added = false;
  while(!NoColons) // while there are colons in the expression
  {
      new_path= strrchr(path,COLON); // change the current path to test
      if(new_path!=NULL){ 
        new_path++; // change what new_path points to by one 
      }
      else
      {
        new_path=start;
        NoColons=!NoColons;
      }
      strcat(new_path,"/"); // add a slash to the path to test
      strcat(new_path,t); // complete the potential command by adding the input value
      if(access(new_path,F_OK|X_OK)!=-1) // test if the current command to be checked exists in Unix
      {
          strcpy(t,new_path); // copy new_path into t
          added = true;
      }
      else if(!NoColons) { // if there are colons, continue the process
          new_path--;
          *new_path='\0';
          new_path=start;
      }
  }
    if(!added){ // if the command was not found
        fprintf(stderr,"Directory does not exist, or"); // error message
        return false;
    }
    return true;
}

