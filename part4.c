/*
* Description: CIS 415 Project 2: Master Control Program - Part 4
*
* Author: Nicholas Bonat
*
* Date: 11/8
*
*/
#include "header.h"
#include "helper.c"
/*---------------------------------------------------------------------------*/

// define functions
pid_t execute_program(char *program, char **array);
void print_proc(pid_t _pid);

// global alarm
static int alarmed = 0;
void set_alarm();

int main(int argc, char *argv[]){
	// define variables
	FILE *input;
    char *line;
    size_t count;
    size_t totalLines;
    pid_t *pids;
    char **program;
    size_t i;
    int *state = NULL;

    // input.txt is necessary
    if (argc != 2){
        printf("input.txt needed\n");
        exit(-1);
    }
    // make sure the file can be opened
    input = fopen(argv[1], "r");
    if (input == NULL){
        perror("failed to open file");
        exit(-1);
    }

    count = 0;
    totalLines = 1000;

    // allocate memory
    line = malloc(sizeof(char) * totalLines);
    state = malloc(sizeof(int) * 1000);
    pids = malloc(sizeof(pid_t) * 1000);

    while(getline(&line, &totalLines, input) > 0){
        program = split_command(line);
        pids[count] = execute_program(program[0], program);

        // setting state for the program
        state[count] = 1;
        count++;

        free_array(program);
    }
    // close file
    fclose(input);
    signal(SIGALRM, set_alarm);

    // scheduler
    int status = -1;

    while(status){
        status = 0;

        for(i = 0; i < count; i++){
            // running process or not
            if (state[i] != 0){
                if (state[i] == 1){
                    // send signal
                    kill(pids[i], SIGUSR1);
                }
                else if (state[i] == 2){
                    // continue
                    kill(pids[i], SIGCONT);
                }
                // running
                state[count] = 3;
                printf("running pid #%d\n", pids[i]);

                alarmed = 0;

                alarm(1);

                while(!alarmed);

                // print information from /proc
                print_proc(pids[i]);

                printf("killing pid: %d\n", pids[i]);
                kill(pids[i], SIGSTOP);

                int code;
                waitpid(pids[i], &code, WUNTRACED);

                // process is done or not
                if(WIFEXITED(code)){
                	// process is done
                    printf("terminating pid: %d\n", pids[i]);
                    state[i] = 0;
                }
                else{
                	// process is not done
                    printf("enqueued back pid: %d\n", pids[i]);
                    state[i] = 2;
                }
            }
            // only change status if every state is 0
            status = status | state[i];
        }
    }
    free(line);
    free(pids);
    free(state);
	return 0;
}

// helper function to excecute the process
pid_t execute_program(char *program, char **array){
    pid_t pid;
    sigset_t sigset;

    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    pid = fork();

    // unsuccessful creation of child process
    if(pid < 0){
        perror("fork failed");
        exit(-1);
    }
    // successful creation of child process
    if(pid == 0){
    	int signal;
    	if(sigwait(&sigset, &signal) == 0){
    		printf("executing pid: %u\n", getpid());
        	execvp(program, array);
    	}
        exit(-1);
    }
    // pid ID
    return pid;
}

void set_alarm(){
    printf("alarm recieved\n");
    // set alarm to 1
    alarmed = 1;
}

//used to print out some status info of the requested process
void print_proc(pid_t _pid){

    FILE *file;
    int pid;
    int sTime;
    int eTime;
    char tmp[1000];
    char name[1000];
    char state;

    // use /proc to get info
    sprintf(tmp, "/proc/%u/stat", _pid);

    file = fopen(tmp, "r");
    if(file == NULL){
    	return;
    }

    // all possible things to scan
    fscanf(file, "%d %s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %d %d", &pid, name, &state, &sTime, &eTime);
    
    // tab format like top(1)
    printf("PID\tNAME\tSTATE\tTIME\n");
    // printing pid, pid name, state of the pid, and total time
    printf("[%d]\t%s\t%c\t%d\n", pid, name, state, sTime + eTime);

    fclose(file);
}
