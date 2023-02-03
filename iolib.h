#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

typedef enum{
    no_such_file = -100,
    no_such_direcotry = -101,
    file_already_exists = -102,
    expression_not_found = -103,
    invalid_depth = -104,
    invalid_command = -105,
    file_creation_failed = -106
}errors;

void print_output(char *output){
    int error = atoi(output);
    switch (error){
    case no_such_file:
        printf("File doesn't exist\n");
        break;

    case no_such_direcotry:
        printf("Directory doesn't exist\n");
        break;

    case file_already_exists:
        printf("The given file already exists\n");
        break;

    case expression_not_found:
        printf("-1\nExpression not found\n");
        break;

    case invalid_depth:
        printf("Invalid depth\n");
        break;

    case invalid_command:
        printf("Invalid command\n");
        break;
    
    case file_creation_failed:
        printf("File creation failed\n");
        break;
    }
}

void output(char *outstr){
    printf("");
}