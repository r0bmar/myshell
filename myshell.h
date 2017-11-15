/*
   CITS2002 Project 2 2017
   Name(s):     Robin Markwitz (, Jainish Pithadiya)
   Student number(s):   21968594 (, 21962504)
   Date:        3 November 2017
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>

//  Written by Chris.McDonald@uwa.edu.au, October 2017

#if defined(__linux__)
    extern	char	*strdup(const char *str);
    extern	int	fileno(FILE *fp);
#endif

// ----------------------------------------------------------------------

#define	DEFAULT_HOME		"/tmp"
#define	DEFAULT_PATH		"/bin:/usr/bin:/usr/local/bin:."
#define	DEFAULT_CDPATH		".:.."

#define COMMENT_CHAR		'#'	// comment character
#define HOME_CHAR		'~'	// home directory character

//  ----------------------------------------------------------------------

//  AN enum IN C99 'GENERATES' A SEQUENCE OF UNIQUE, ASCENDING CONSTANTS
typedef enum {
	CMD_COMMAND = 0,	// an actual command node itself
	CMD_SEMICOLON,		// as in   cmd1 ;  cmd2	
	CMD_AND,		// as in   cmd1 && cmd2
	CMD_OR,			// as in   cmd1 || cmd2	
	CMD_SUBSHELL,		// as in   ( cmds )
	CMD_PIPE,		// as in   cmd1 |  cmd2	
	CMD_BACKGROUND		// as in   cmd1 &
} CMDTYPE;


typedef	struct sc {
    CMDTYPE	type;		// the type of the node, &&, ||, etc

    int		argc;		// the number of args iff type == CMD_COMMAND
    char	**argv;		// the NULL terminated argument vector

    char	*infile;	// as in    cmd <  infile
    char	*outfile;	// as in    cmd >  outfile
    bool	append;		// true iff cmd >> outfile

    struct sc	*left, *right;	// pointers to left and right sub-shellcmds
} SHELLCMD;


extern SHELLCMD	*parse_shellcmd(FILE *);	// in parser.c
extern void	free_shellcmd(SHELLCMD *);	// in parser.c
extern int	execute_shellcmd(SHELLCMD *);	// in execute.c

// Our functions

extern int simplecommand(SHELLCMD *); // in 1_simple_external_cmds.c
extern bool SearchPath(char *); //in 2_search_path.c
extern bool isInternal(SHELLCMD *); //in 3_internal_cmds.c
extern int execute_internal(SHELLCMD *); //in 3_internal_cmds.c
extern int exec_semicolon(SHELLCMD *); // in 4_sequential.c
extern int exec_and(SHELLCMD *); //in 4_sequential.c
extern int exec_or(SHELLCMD *); //in 4_sequential.c
extern int exec_subshell(SHELLCMD *); //in 5_subshell.c
extern int ioRedirect(SHELLCMD*); // in 6_stdio_redirect.c
extern int exec_pipe(SHELLCMD*); //in 7_pipelines.c
extern int exec_shellscript(SHELLCMD*); // in 8_shellscripts.c
extern int exec_background(SHELLCMD *); // in 9_background.c
extern void signal_handler(int); // in 9_background.c

// our global Variables

extern int lastexecstatus; // value of the status of the last called command
extern pid_t finished_cpid; // the process ID of the completed child (if running a background command)

/* The global variable HOME points to a directory name stored as a
   character string. This directory name is used to indicate two things:

    - the directory used when the  cd  command is requested without arguments.
    - replacing the leading '~' character in args,  e.g.  ~/filename

   The HOME variable is initialized with the value inherited from the
   invoking environment (or DEFAULT_HOME if undefined).
 */

extern	char	*HOME;

/* The global variables PATH and CDPATH point to character strings representing
   colon separated lists of directory names.

   The value of PATH is used to search for executable files when a command
   name does not contain a '/'

   Similarly, CDPATH provides a colon separated list of directory names
   that are used in an attempt to chage the current working directory.
 */

extern	char	*PATH;
extern	char	*CDPATH;

extern	char	*argv0;		// The name of the shell, typically myshell
extern	bool	interactive;	// true if myshell is connected to a 'terminal'


//  ----------------------------------------------------------------------

//  TWO PRE-PROCESSOR MACROS THAT MAY HELP WITH DEBUGGING YOUR CODE.
//      check_allocation(p) ENSURES THAT A POINTER IS NOT NULL, AND
//      print_shellcmd(t)  PRINTS THE REQUESTED COMMAND-TREE
//  THE TWO ACTUAL FUNCTIONS ARE DEFINED IN globals.c

#define	check_allocation(p)	\
	check_allocation0(p, __FILE__, __func__, __LINE__)
extern	void check_allocation0(void *p, char *file, const char *func, int line);

#define	print_shellcmd(t)	\
	printf("called from %s, %s() line %i:\n", __FILE__,__func__,__LINE__); \
	print_shellcmd0(t)
extern	void print_shellcmd0(SHELLCMD *t);

