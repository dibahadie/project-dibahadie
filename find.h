#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

char* path_validation(char initial[]);

int find_first_index(char *filepath, char *str){
    filepath = path_validation(filepath);
    FILE *file = fopen(filepath, "r");
    if(file == NULL){
        printf("The given file doesn't exist\n");
        return -1;
    }
    char c = fgetc(file);
    char *content;
    content = (char*) malloc(sizeof(char) * MAX_FILE);
    while ((c != EOF)){
        content[strlen(content)] = c;
        c = fgetc(file);
    }
    char *loc_ptr = strstr(content, str);
    if(loc_ptr == NULL){
        printf("Expression not found\n");
        return -1;
    }
    int index = strlen(content) - strlen(loc_ptr);
    return index;
}

int find_count(char *filepath, char *str){
    filepath = path_validation(filepath);
    int counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL){
        printf("The given file doesn't exist\n");
        return -1;
    }
    char c = fgetc(file);
    char *content, *buff;
    buff = (char*) malloc(sizeof(str));
    content = (char*) malloc(sizeof(char) * MAX_FILE);
    while ((c != EOF)){
        content[strlen(content)] = c;
        c = fgetc(file);
    }
    char *loc_ptr = strstr(content, str);
    if(loc_ptr == NULL){
        printf("Expression not found\n");
        return -1;
    }
    while(loc_ptr != NULL){
        counter++;
        loc_ptr = strstr(loc_ptr + 1, str);
    }
    return counter;
}

int find_at(char *filepath, char *str, int n){
    filepath = path_validation(filepath);
    int counter = 1;
    FILE *file = fopen(filepath, "r");
    if(file == NULL){
        return -2;
    }
    char c = fgetc(file);
    char *content, *buff;
    buff = (char*) malloc(sizeof(str));
    content = (char*) malloc(sizeof(char) * MAX_FILE);
    while ((c != EOF)){
        content[strlen(content)] = c;
        c = fgetc(file);
    }
    char *loc_ptr = strstr(content, str);
    if(loc_ptr == NULL) return -1;
    while(loc_ptr != NULL && counter != n){
        counter++;
        loc_ptr = strstr(loc_ptr + 1, str);
    }
    if(counter != n) return -1;
    else{
        int index = strlen(content) - strlen(loc_ptr);
        return index;
    }
}

