#ifndef __HEADER_H__
#define __HEADER_H__

// libraries used
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// function that is defined in every part
pid_t execute_program(char *program, char **args);

#endif