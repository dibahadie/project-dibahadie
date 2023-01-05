#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500

int create_dir(char *path);
int create_file(char *path);
int get_command();
int gnc(char** command, char** next_command, char** remaining);



int main(){
    mkdir("root", 0777);
    while(1){
        if(!get_command()) printf("invalid command\n");
    }
}

int create_dir(char *path){
    char *remained_path = path;
    char *last_dir = NULL;

    remained_path = (char*) malloc(sizeof(path));
    last_dir = strrchr(path, '/');
    if(last_dir == NULL) {
        mkdir(path ,0777);
        return 1;
    }
    strncpy(remained_path, path, strlen(path) - strlen(last_dir));
    int result = create_dir(remained_path);
    if(*(remained_path) == '/') mkdir(path + 1 ,0777);
    else mkdir(path, 0777);
    return result;
}
int create_file(char *path){
    path = path + 1;
    
    if(strchr(path, '/') == NULL){
        struct stat buff;
        if(stat(path, &buff) == -1){
            FILE *file = fopen(path, "w");
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
        if(stat(path, &buff) == -1){
            FILE *file = fopen(path, "w");
            if(file == NULL){
                printf("File creation failed.\n");
                return 1;
            }
            fclose(file);
            return 1;
        }
        else{
            printf("The given file already exists\n");
        }
        return 0;
    }
}
int get_command(){
    char *input;
    input = (char*) malloc(sizeof(char) * MAX_SIZE);
    fgets(input, MAX_SIZE, stdin);
    char *initial_command, *remaining;
    if(gnc(&input, &initial_command, &remaining) == 0) return 0;

    if(!strcmp(initial_command, "createfile")){
        char* filepath;
        char* input_format;
        if(gnc(&remaining, &input_format, &filepath) == 0) return 0;
        if(strcmp(input_format, "--file")) return 0;
        else{
            create_file(filepath);
            return 1;
        }
    }
    return 0;
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