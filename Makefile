C99 = cc -std=c99
CFLAGS = -Wall -Werror -pedantic 

PROJ = myshell
OBJ = myshell.o execute.o globals.o parser.o 1_simple_external_cmds.o 2_search_path.o 3_internal_cmds.o 4_sequential.o 5_subshell.o 6_stdio_redirect.o 7_pipelines.o 8_shellscripts.o 9_background.o

$(PROJ) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJ) $(OBJ) 

%.o : %.c $(PROJ).h
	$(C99) $(CFLAGS) -c $<

clean: 
	rm -f $(PROJ) *.o
