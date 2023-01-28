#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "iolib.h"
#define MAX_SIZE 500
#define MAX_FILE 10000

char* path_validation(char initial[]);

int create_dir(char *path){
    char *remained_path;
    char *last_dir = NULL;

    last_dir = (char*) malloc(sizeof(path));
    remained_path = (char*) malloc(sizeof(path));
    last_dir = strrchr(path, '/');
    if(last_dir == NULL) {
        mkdir(path ,0777);
        return 1;
    }
    strncpy(remained_path, path, strlen(path) - strlen(last_dir));
    int result = create_dir(remained_path);
    if(*(path) == '/') mkdir(path + 1 ,0777);
    else mkdir(path, 0777);
    free(remained_path);
    return result;
}

int create_file(char *path){
    path = path_validation(path);
    if(strchr(path, '/') == NULL){
        FILE *file = fopen(path, "w");
        if(file != NULL){
            fclose(file);
            return 1;
        }else{
            return 0;
        }
    }else{
        char *filename = strrchr(path, '/');
        char *dirpath;
        struct stat buff;
        dirpath = (char*) malloc(sizeof(path));
        strncpy(dirpath, path, strlen(path) - strlen(filename));
        create_dir(dirpath);
        FILE *file = fopen(path, "r");
        if(file == NULL){
            file = fopen(path, "w");
            if(file == NULL){
                return file_creation_failed;
            }
            fclose(file);
            return 1;
        }
        else{
            return file_already_exists;
        }
    }
}

int run_create_file(char *input){
    char* filepath;
    char* input_format;
    if(!get_input(input, &filepath, "\n", "--file ")) return invalid_command;
    int result = create_file(filepath);
    return result;
}