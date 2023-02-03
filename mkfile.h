#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include "iolib.h"
#define MAX_SIZE 500
#define MAX_FILE 10000

char* path_validation(char initial[]);
int get_input(char* command, char** text, char* next_identifier, char*pre_identifier);

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

void backup_filepath(char *path, char newpath[MAX_FILE]){
    char *ptr = strrchr(path, '/');
    if(ptr == NULL){
        newpath[0] = '.';
        newpath[1] = '\0';
        strcat(newpath, path);
        return;
    }
    else{
        for(int i=0; i<MAX_FILE; i++) newpath[i] = '\0';
        ptr++;
        strncpy(newpath, path, strlen(path) - strlen(ptr));
        strcat(newpath, ".");
        strcat(newpath, ptr);
        return;
    }
}

int create_file(char *path){
    path = path_validation(path);
    char b_filepath[MAX_FILE];
    if(strchr(path, '/') == NULL){
        FILE *file = fopen(path, "r");
        
        if(file != NULL){
            return file_already_exists;
        }else{
            file = fopen(path, "w");
            backup_filepath(path, b_filepath);
            FILE *b_file = fopen(b_filepath, "w");
            
            if(file == NULL) return file_creation_failed;
            else{
                fclose(file);
                fclose(b_file);
                return 1;
            }
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
            backup_filepath(path, b_filepath);
            FILE *b_file = fopen(b_filepath, "w");
            if(file == NULL){
                return file_creation_failed;
            }
            fclose(file);
            fclose(b_file);
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

int existance_validation(char path[]){
    char *file_name = strrchr(path, '/');
    file_name++;
    int size = strlen(path);
    if(file_name == NULL){
        FILE *file = fopen(path, "r");
        if(file == NULL) return no_such_file;
        fclose(file);
    }
    char *dir_path = (char*) malloc(strlen(path) * sizeof(char));
    strncpy(dir_path, path, size - strlen(file_name));
    DIR *dir = opendir(dir_path);
    if(dir == NULL) return no_such_direcotry;
    closedir(dir);
    FILE *file = fopen(path, "r");
    if(file == NULL) return no_such_file;
    fclose(file);
    return 1;
}