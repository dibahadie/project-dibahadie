#define MAX_SIZE 500
#define MAX_FILE 10000
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

int get_input(char* command, char** text, char* next_identifier, char*pre_identifier){
    int len;
    char *pre, *next;
    pre = (char*) malloc(sizeof(command));
    next = (char*) malloc(sizeof(command));
    pre = strstr(command, pre_identifier);
    if(pre == NULL) return 0;
    pre = pre + strlen(pre_identifier);
    next = strstr(command, next_identifier);
    if(next == NULL) return 0;
    if(next < pre) return 0;
    *text = (char*) malloc((strlen(pre) - strlen(next) - 1) * sizeof(char));
    strncpy(*text, pre, strlen(pre) - strlen(next));
    return 1;
}

void string_validation(char initial[]){
    int size = strlen(initial);
    if(initial[0] == '\"'){
        for(int i=0; i<size; i++) initial[i] = initial[i+1];
        initial[size - 2] = '\0';
        size -= 2;
    }
    for(int i=0; i<size - 1; i++){
        if(initial[i] == '\\' && initial[i+1] == 'n'){
            if(i == 0 || initial[i-1] != '\\'){
                for(int j=i; j<size; j++){
                    initial[j] = initial[j+1];
                }
                size--;
                initial[i] = '\n';
            }
        }
        else if(initial[i] == '\\' && initial[i+1] == '\"'){
            if(i == 0 || initial[i-1] != '\\'){
                for(int j=i; j<size; j++){
                    initial[j] = initial[j+1];
                }
                size--;
                initial[i] = '\"';
            }
        }
        else if(initial[i] == initial[i+1] && initial[i] == '\\'){
            for(int j=i; j<size; j++){
                initial[j] = initial[j+1];
            }
            size--;
        }
    }
}

char* path_validation(char initial[]){
    char *path;
    path = (char*) malloc(strlen(initial) * sizeof(char));
    strcpy(path, initial);
    int size = strlen(path);
    if(path[0] == '/'){
        path = path + 1;
        size--;
    }
    else if(path[0] == '\"' && path[1] == '/'){
        path = path + 2;
        path[size - 3] = '\0';
        size -= 2;
    }
    else if(path[0] == '\"'){
        path = path + 1;
        path[size - 2] = '\0';
        size--;
    }
    if(path[size - 1] == '\n') path[size - 1] = '\0';
    return path;
}

int gnc(char** command, char** next_command, char** remaining){
    *remaining = (char*) malloc(sizeof(*command));
    *next_command = (char*) malloc(sizeof(*command));
    char* a = strchr(*command, ' ');
    if(a == NULL) return 0;
    *remaining = a;
    strncpy(*next_command, *command, strlen(*command) - strlen(*remaining));
    (*remaining) ++;
    return 1;
}

