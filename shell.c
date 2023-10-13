#include  <stdio.h>
#include  <sys/types.h>

//User included libraries
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> //File Control
#include <dirent.h>
#include <errno.h>
#include "linkedList.h"

#define TOKEN_SIZE 64
int background = 0;

/*
struct node* insert(struct node* current, char* value)
{
	//This function will insert the string value at a given point in the node
	//If there exists a node afterwards, it will relink it.
	//If the inserted node is at the end. the next character will not be set
	

	if (value == NULL) {
		return current;
	}

	//Allocate some memory for the new node
	struct node* newNode = (struct node*)malloc(sizeof(struct node));

	//Allocate some memory for the new node's data
	newNode->data = (char*)malloc(sizeof(value));

	//Set the new node to the previous node's next
	newNode->next = current->next;

	//Add the string data into newNode
	strcpy(newNode->data, value);

	//move the current->next to point to the new node we just created
	current->next = newNode;

	return newNode;

}
*/

void  parse(char *line, char **argv)
{
    while (*line != '\0') {       /* if not the end of line ....... */ 
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';     /* replace white spaces with 0    */
        *argv++ = line;          /* save the argument position     */

		//This will loop while loop will check character by character until it reaches a [\0, ' ', \t, \n]
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') {
			line++;             /* skip the argument until ...    */
		}

     	}
    *argv = '\0';                 /* mark the end of argument list  */
}

void inputRedirection(char *file) {
	int fileIn=open(file,O_RDONLY);
    	if(fileIn<0)
        	perror("File could not be opened");
    	dup2(fileIn,0);//connect file to stdin
    	if (close(fileIn) == -1) {
			perror("File could not be closed");
		}
}

void outputRedirection(char *file) {
	int fileOut=open(file,O_WRONLY | O_TRUNC | O_CREAT,0600); //0600 is a mode
    	if(fileOut<0)
        	perror("File could not be opened");
    	dup2(fileOut,1);//connect file to stdout
    	if (close(fileOut) == -1) {
			perror("File could not be closed");
		}
}

char** parseSpecialChars(char **argv) {
	//Declares a buffer for commands that execvp should execute
	char** args = malloc(TOKEN_SIZE*sizeof(char*));
	int bufferPosition = 0;

	if(!args) {
		printf("Allocation Error");
		exit(EXIT_FAILURE);
	}

	//Loop through the entire null seperated array and search for the special characters we use [>, <, |, &]
	//If on the current iteration we didn't encounter any special characters, add the argument to a buffer that will be returned for execute() to run in the main loop
	for (int i = 0; argv[i] != '\0'; i++) {
		//If it encounters ><, it will create the file, then connect the files to the stdout or stdin as required
		//Check for output redirection
		if(*argv[i] == '>') {
			i++;
			outputRedirection(argv[i]);

		//Check for input redirection
		} else if(*argv[i] == '<') {
			i++;
			inputRedirection(*argv[i]);
			
		
		//Check for pipe. If pipe, add a null to the end of the current argument list, then create the pipe.
		//Once the pipe is created, run the command.
		//Then, reset the buffer and continue reading what the user inputted
		} else if(*argv[i] == '|') {
			//Terminate the arguement
			args[bufferPosition]='\0';

			printf("Creating a pipe");

			//Create the pipe
			int fd[2];
			pipe(fd);
			dup2(fd[1], 1); //Connect write fd to stdout
			close(fd[1]);
			execute(args);
			dup2(fd[0], 0); //Connect read fd to stdin
			close(fd[0]);

			//Reset the buffer position for the next arguement
			bufferPosition = 0;
		} else if (*argv[i] == '&') {
			background = 1;
			args[bufferPosition] = NULL;
		} else {
			//The current word is nothing special
			//Add to the buffer
			args[bufferPosition]=argv[i];
			bufferPosition++;
		}
	}
	return args;
}

void printCommands(char **argv) {
     //Part 1 of this project indicates that we should parse the command into its paticular arguments
     //These arguements should then be placed into a linked list
     //Linked list was already defined in the file "linkedList.h"
     	//As we loop through the command we will insert into the linked list


	
	//Allocate head and current's memory address
	struct node* head;
	struct node* current;
	head = (struct node*)malloc(sizeof(struct node));
	current = (struct node*)malloc(sizeof(struct node)); 

	//Set up the first data point to have the first value
	head->data = (char*)malloc(sizeof(argv[0]));

	//Insert the data
	head->data = argv[0];
	head->next = NULL;
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
			if (current->next != NULL) {
				current = current->next;
				fputs(current->data, stdout);
				fputs(" ", stdout);
			}
		}
	}
	fputs("\n", stdout);


	current = head;
	//Print out all the commands, with the arguements
	fputs(current->data, stdout);
	fputs(": ", stdout);
	if (current->next != NULL) {
		current = current->next;
	}
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
	fputs("\n\n", stdout);


}
void  execute(char **argv) //write your code here
{

//HINT you need to introduce the following functions: fork, execvp, waitpid
//Advanced HINT: pipe, dup2, close, open

//Thus begins part 2: Executing commands


	int    status;
	char cwd[256];


//cd and ls are special: they operate on the parent function, not the child
	//If the command is change directory
	if (!strcmp(argv[0], "cd")) {

		//The third arguement should be empty - there should only be 2 args
		//if (argv[2] != "\0") {
		//	fputs("Too many args", stderr);
		//} else {
			if(chdir(argv[1]) != 0) {
				perror("cd operation failed");
			}
		fputs("cwd: ", stdout);
		fputs(getcwd(cwd, sizeof(cwd)), stdout);
		fputs("\n", stdout);
		//}
	//If the command is ls
	} else if (!strcmp(argv[0], "ls")) {
		struct dirent *d;
		DIR *dh = opendir(".");
		if (!dh) {
			perror ("Unable to read directory");
		}
		while ((d = readdir(dh)) != NULL) {
			if(d->d_name[0] == '.') {
				continue;
			}
			fputs(d->d_name, stdout);
			fputs("\n", stdout);
		}

	} else {
		//In the parent process, PID is the child PID
		//In the child process, PID is 0
		pid_t  pid = fork();

		
		//If we are the child process
		if (!pid) {
			
			/*
			fputs("Hello from child. PID: ", stdout);
			printf("%d", getpid());
			printf("%s", argv[0]);
			printf("\n args: %s", argv);
			*/

			//Execute execvp on the argument. If it fails, then report the failure
			//Then, exit with a failure
			
			int status = execvp(argv[0], argv);
			if (status < 0) {
				perror("Execvp Error");
				_exit(EXIT_FAILURE);
			}
			fputs("After running", stdout);
			
			//If we succeed, exit normally
			_exit(0);
		} else if (pid < 0) {	//Fork has failed. Report a failure.
			printf("Fork has failed");
			fputs("\n", stdout);
		} else { //This is the parent process
			
			//fputs("Hello from the parent. PID: ", stdout);
			//printf("%d", getpid());
			//fputs("\n", stdout);
			
			//If the background flag is set (That is, if & was run), then simply move on
			//Otherwise, wait for the child to finish
			if (background) {
				background = 1;
			} else {
				waitpid(pid, NULL, 0);
				//fputs("Parent is now moving on\n", stdout);
			}

			//If its a background process or we're done waiting, return the input and output to the console

			int fdOut = open("/dev/tty", O_WRONLY | O_TRUNC | O_CREAT,0600);
			int fdIn=open("/dev/tty",O_RDONLY);

			if (fdOut == -1 || fdIn == -1) {
				fprintf(stderr, "Failed to open /dev/tty\n");
				perror("open");
				// Handle the error as needed
			} else {
				dup2(fdOut, 1);    // Connect /dev/tty to stdin
				close(fdOut);

				dup2(fdIn, 0);    // Connect /dev/tty to stdin
				close(fdIn);
				// Now you can use stdin and stdout as usual
			}

			
		}
	}


}

void  main(void)
{
    char  line[1024];             /* the input line                 */
    char  *argv[64];              /* the command line argument      */
    char** args;	//Null terminated array of executable commands


    while (1) {                   /* repeat until done ....         */
        fputs("Shell -> ",stdout);     /*   display a prompt             */
        fgets(line, 1024, stdin);              /*   read in the command line     */
        fputs("\n", stdout);
        parse(line, argv);       /*   parse the line               */
        if (strcmp(argv[0], "exit") == 0)  /* is it an "exit"?     */
            exit(0);            /*   exit if it is                */

	//It should do nothing if there was no command entered
	if (line[0] == '\0') {
		//printf("There was nothing entered");
	} else {
		//printf("%d",*argv[0]);
		printCommands(argv);
		args = parseSpecialChars(argv);

		execute(args);            //otherwise, execute the command 
		background = 0;
		//free(args);
		//free(argv);
	}
    }
}
