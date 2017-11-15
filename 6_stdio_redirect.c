/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include "myshell.h"
/*
    This function redirects the output/input stream to a given file.
    It returns -1 on fail and 0 on success.
*/

int ioRedirect(SHELLCMD* node)
{
    int ioRdirect=0; // if it successfully changes input stream
    int fd; // input stream of file
    if(node->infile!=NULL) // if input file is given
    {
        fd=open(node->infile,O_RDONLY,00400); // open the file and read it only
        if(fd==-1) // if opening failed
        {
            ioRdirect=1; // give fail value
            fprintf(stderr,"File %s couldnt be opened\n",node->infile);
        }
        else {
            /*
                Changes the input stream from stdin to file.
                If unsuccessful, print error message.
            */
            if(dup2(fd,0)==-1) { 
                ioRdirect=1;
                fprintf(stderr,"Failed to change streams\n");
            }
        }
        close(fd); // close the file
    }
    if(node->outfile!=NULL) { // if output file is given
        if(node->append == true) { // if it is required to append 
            fd= open(node->outfile,O_WRONLY|O_CREAT|O_APPEND,00200); // open the file in append mode
        }
        else {
            fd= open(node->outfile,O_TRUNC,00200); // clear the current outfile
            close(fd); // close the file
            fd= open(node->outfile,O_WRONLY|O_CREAT,00200); // open the file
        }
        if(fd==-1) // if opening failed
        {
            ioRdirect=1;
            fprintf(stderr,"File %s couldnt be opened\n",node->infile);
        }
        else
        {
            /*
                Changes the output stream from stdout to file.
                If unsuccessful, print error message.
            */
            if(dup2(fd,1)==-1) {
                ioRdirect=1;
                fprintf(stderr,"Failed to change streams\n");
            }
        }
        close(fd); // close file
    }
    return ioRdirect; // return function status (0 if successful)
}

