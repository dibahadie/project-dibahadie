#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#define MAX_SIZE 500
#define MAX_FILE 10000

void string_validation(char initial[]);
char* path_validation(char initial[]);
int existance_validation(char path[]);
int get_input(char* command, char** text, char* next_identifier, char*pre_identifier);
void backup(char *filepath);

int run_arman(char *input){
    char *first_command = (char*) malloc(sizeof(char) * MAX_FILE);
    char *second_command = (char*) malloc(sizeof(char) * MAX_FILE);
    second_command = strstr(input, "=D ") + 3;
    strncpy(first_command, input, strlen(input) - strlen(second_command) - 4);
    first_command[strlen(first_command)] = '\n';
    return 1;
}