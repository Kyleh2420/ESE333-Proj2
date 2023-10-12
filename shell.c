#include  <stdio.h>
#include  <sys/types.h>

//User included libraries
#include <string.h>
#include <stdlib.h>
#include "linkedList.h"

void  parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}

void  execute(char **argv) //write your code here
{

//HINT you need to introduce the following functions: fork, execvp, waitpid
//Advanced HINT: pipe, dup2, close, open

     pid_t  pid;
     int    status;

     //Part 1 of this project indicates that we should parse the command into its paticular arguments
     //These arguements should then be placed into a linked list
     //Linked list was already defined in the file "linkedList.h"
     //The arguements are sepearted by a "" first - then by spaces
     	//That is, if there is a ", the program will search for the next " before then looking for spaces
     	//As we loop through the command, we will add each part into a buffer before inserting into the linked list

	
	//Allocate head and current's memory address
	struct node* head;
	struct node* current;
	head = (struct node*)malloc(sizeof(struct node));
	current = (struct node*)malloc(sizeof(struct node)); 

	//Set up the first data point to have the first value
	head->data = (char*)malloc(sizeof(argv[0]));

	//Insert the data
	head->data = argv[0];
	//Then, set current to head.
	current = head;

	//Loop through the whole parsed input, and add them to the linked list
	for (int i = 1; *argv[i] != '\0'; i++) {
		current = insert(current, argv[i]);
		//fputs(current->data, stdout);
		/*
		fputs(argv[i], stdout);
		fputs("\n", stdout);
		*/
	}

	//Reset current back to head
	current = head;

	/*
	//The following while loop will print out the entire linked list from the beginning
	//This is used as a test output
	fputs(current->data, stdout);
	fputs("\n", stdout); 
	while (current->next != NULL) {
		current = current->next;
		fputs(current->data, stdout);
		fputs("\n", stdout); 
	}

	*/

	current = head;
	//Print out all the commands, not the arguements
	fputs("Commands: ", stdout);
	fputs(current->data, stdout);
	fputs(" ", stdout);
	while (current->next != NULL) {
		
		current = current->next;
		if (strcmp(current->data, "|") == 0) {
			current = current->next;
			fputs(current->data, stdout);
			fputs(" ", stdout);
		}
	}
	fputs("\n", stdout);


	current = head;
	//Print out all the commands, with the arguements
	fputs(current->data, stdout);
	fputs(": ", stdout);
	current = current->next;
	while (current->next != NULL) {
		if (strcmp(current->data, "|") == 0) {
			fputs("\n", stdout);
			current = current->next;
			fputs(current->data, stdout);
			fputs(": ", stdout);
			current = current->next;
			continue;
		}
		fputs(current->data, stdout);
		fputs(" ", stdout);
		current = current->next;
	}
	fputs(current->data, stdout);
	fputs("\n", stdout);

}

void  main(void)
{
     char  line[1024];             /* the input line                 */
     char  *argv[64];              /* the command line argument      */

     while (1) {                   /* repeat until done ....         */
          fputs("Shell -> ",stdout);     /*   display a prompt             */
          fgets(line, 1024, stdin);              /*   read in the command line     */
          fputs("\n", stdout);
          parse(line, argv);       /*   parse the line               */
          if (strcmp(argv[0], "exit") == 0)  /* is it an "exit"?     */
               exit(0);            /*   exit if it is                */
          execute(argv);           /* otherwise, execute the command */
     }
}
