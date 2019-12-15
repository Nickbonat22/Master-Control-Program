/*
* Description: CIS 415 Project 2: Master Control Program - Helper functions
*
* Author: Nicholas Bonat
*
* Date: 11/5
*
*/
#include "helper.h"
/*---------------------------------------------------------------------------*/
// helper function to split the command and arguments and put into an array
char **split_command(char *line){
    int holder = 0;
    int current = 0;
    int count = 0;
    char *temp;
    char **array;
    temp = line;

    while(*temp){
        // isspace helps to see if there are spaces
        if (isspace(*temp)){
            if (current - holder > 0){
                count++;
            }
            holder = current + 1;
        }
        current++;
        temp++;
    }
    count++;

    // allocate memory
    array = (char**)malloc(sizeof(char*) * count);
    count = 0;
    holder = 0;
    current = 0;
    temp = line;

    while(*temp){
        if (isspace(*temp)){
            if (current - holder > 0){
                array[count] = malloc(sizeof(char) * (current - holder + 1));
                strncpy(array[count], line + holder, current - holder);
                array[count][current - holder] = 0;
                count++;
            }
            holder = current + 1;
        }
        current++;
        temp++;
    }
    array[count] = NULL;
    return array;
}

// helper function to completely free the items in the array and the array
void free_array(char **array){
    char **tempArr = array;
    while(*tempArr){
        free(*tempArr);
        tempArr++;
    }
    free(array);
}