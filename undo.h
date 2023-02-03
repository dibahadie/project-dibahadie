#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

char* path_validation(char initial[]);
int existance_validation(char path[]);
int get_input(char* command, char** text, char* next_identifier, char*pre_identifier);
void backup_filepath(char *path, char newpath[MAX_FILE]);

void backup(char *filepath){
    char b_path[MAX_FILE];
    backup_filepath(filepath, b_path);
    FILE *main_file = fopen(filepath, "r");
    FILE *back_file = fopen(b_path, "w");    
    char c = fgetc(main_file);
    while(c != EOF){
        fputc(c, back_file);
        c = fgetc(main_file);
    }
    fclose(main_file);
    fclose(back_file);
}

int undo(char *filepath){
    char b_path[MAX_FILE];
    backup_filepath(filepath, b_path);
    FILE *main_file = fopen(filepath, "w");
    FILE *back_file = fopen(b_path, "r");    
    char c = fgetc(back_file);
    while(c != EOF){
        fputc(c, main_file);
        c = fgetc(back_file);
    }
    fclose(main_file);
    fclose(back_file);
    return 1;
}

int run_undo(char *input){
    char* filepath = (char*) malloc(sizeof(char) * MAX_FILE);
    if(!get_input(input, &filepath, "\n", "--file ")) return -105;
    return undo(filepath);
}