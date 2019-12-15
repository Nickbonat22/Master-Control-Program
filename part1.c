/*
* Description: CIS 415 Project 2: Master Control Program - Part 1
*
* Author: Nicholas Bonat
*
* Date: 10/29
*
*/
#include "header.h"
#include "helper.c"
/*---------------------------------------------------------------------------*/

// define functions
pid_t execute_program(char *program, char **array);

int main(int argc, char *argv[]){
	// define variables
	FILE *input;
    char *line;
    size_t count;
    size_t totalLines;
    pid_t *pids;
    char **program;
    size_t i;

    // input.txt is necessary
    if (argc != 2){
        printf("input.txt needed\n");
        exit(-1);
    }

    input = fopen(argv[1], "r");
    if (input == NULL){
        perror("failed to open file");
        exit(-1);
    }

    count = 0;
    totalLines = 1000;
    
    // allocate memory
    line = malloc(sizeof(char) * totalLines);
    pids = malloc(sizeof(pid_t) * 1000);

    while(getline(&line, &totalLines, input) > 0){
        program = split_command(line);
        pids[count] = execute_program(program[0], program);
        count++;

        free_array(program);
    }
    // close file
    fclose(input);
    // check on the child processess from above
    for(i = 0; i < count; i++){
        waitpid(pids[i], NULL, 0);
        printf("done with pid: %u\n", pids[i]);
    }

    free(line);
    free(pids);

    return 0;
}

// helper function to excecute the process
pid_t execute_program(char *program, char **array){
    pid_t pid;
    pid = fork();

    // unsuccessful creation of child process
    if(pid < 0){
        perror("fork failed");
        exit(-1);
    }
    // successful creation of child process
    if(pid == 0){
    	printf("executing pid: %u\n", getpid());
        execvp(program, array);
        exit(-1);
    }
    // pid ID
    return pid;
}