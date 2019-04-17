#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // OS-defined data types
#include <sys/wait.h> // wait() and waitpid() function
#include <unistd.h> // operating system API
// define constant numbers
#define FALSE 0
#define SIZE_OF_BUFFER 256 // we want a long but not freaking long buffer, so we pick 256, a nice-looking number
#define SIZE_OF_CHAR 1 // sizeof(char) is always 1 in Clang. Why bother calling a function?
#define SYSCALL_ERR -1
#define TRUE 1
// define string messages
#define ERRMSG_CHDIR "%s: No such file or directory\n"
#define ERRMSG_GETCWD "Cannot get the current working directory\n"
#define ERRMSG_FORK "Cannot create child process\n"
#define GOOD_BYE "Bye...\n"
#define SHELL_PROMPT "sshell$ "

int runcd(){
	printf("IJN Yuudachi: poi, poi, poi\n");
}

int runpwd(){
	printf("IJN Yuudachi: poi, poi, poi\n");
}

int runexit(){
	printf("IJN Yuudachi: poi, poi, poi\n");
}

void parseInput(char *input){
	printf("IJN Yuudachi: poi, poi, poi\n");
}

void chopNewline(char *input){
	strtok(input, "\n"); // remove the trailing newline character
}

// helper function to run builtin commands. check if a string begins with "cd ". 
int inputIsCd(char *input){
	if(input[0] == 'c' && input[1] == 'd' && input[2] == ' '){
		return(TRUE);
	}else{
		return(FALSE);
	}
}
// helper function to run builtin commands. check if a string begins with "pwd".  
int inputIsPwd(char *input){
	if(input[0] == 'p' && input[1] == 'w' && input[2] == 'd'){
		return(TRUE);
	}else{
		return(FALSE);
	}
}
// helper function to run builtin commands. check if a string begins with "exit".  
int inputIsExit(char *input){
	if(input[0] == 'e' && input[1] == 'x' && input[2] == 'i' && input[3] == 't'){
		return(TRUE);
	}else{
		return(FALSE);
	}
}


// over here! This is the entrance
int main(int argc, char *argv[]){

	// get ready for taking user input
	size_t bufferSize = SIZE_OF_BUFFER;
	char *buffer = NULL;
	buffer = (char *) malloc(bufferSize);
	//int fd;

	while(1){
		// print the shell prompt
		printf(SHELL_PROMPT);
        fflush(stdout);
		
		// read commands from the input
		getline(&buffer,&bufferSize,stdin); // read raw input from stdin
		char *input = NULL;
		input = (char *) malloc(bufferSize);
		memcpy(input, buffer, bufferSize); // monking "passing by value". We decide to leave buffer intact and parsing on a copy. (for easier debugging?)
		
		
		// builtin command: cd
		if (inputIsCd(buffer)){
			chopNewline(input); // only modify input when necessary
			int chdirStatus;
			chdirStatus = chdir(input + 3 * SIZE_OF_CHAR); // execute chdir 
			if (chdirStatus == SYSCALL_ERR){ // handle error with schdir
				fprintf(stderr, ERRMSG_CHDIR, input + 3 * SIZE_OF_CHAR);
			}
			continue; // wait for new command
		}
		// builtin command: pwd
		if (inputIsPwd(buffer)){
			chopNewline(input); // only modify input when necessary
			char *workDir = NULL;
			workDir = (char *) malloc(bufferSize);
			if (getcwd(workDir, bufferSize) == NULL){ // execute getcwd + handle error
				fprintf(stderr, ERRMSG_GETCWD);
			}else{
				printf("%s\n", workDir);
			}
			continue; // wait for new command
		}
		// builtin command: exit
		if (inputIsExit(buffer)){
			break; // stop taking new command
		}


		char *cmd = "date";
		char *args[] = { cmd, "-u", NULL };
		//running commands by fork+exec+wait
		pid_t pid;
		pid = fork();
		// handle error with fork
		if (pid == SYSCALL_ERR){
			fprintf(stderr, ERRMSG_FORK);
			exit(EXIT_FAILURE);
		}
		// if fork success, execute the command in a child process
		int execStatus;
		chopNewline(input); // only modify input when necessary
		if (pid == 0) { //child
			execStatus = execvp(input, args); // execvp searches the program in the $PATH environment variable
			perror("execvp");
			exit(EXIT_FAILURE);
		} else {  //parent
			waitpid(-1, &execStatus, 0);
			fprintf(stderr, "+ Completed '%s' [%d]\n", input, execStatus);
		}
	}
	// exit the program. Bye!
	fprintf(stderr, GOOD_BYE);
	return EXIT_SUCCESS;
}
