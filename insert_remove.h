#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

void string_validation(char initial[]);
char* path_validation(char initial[]);


int insert(char *filepath, char *str, int line_no, int start_pos){
    string_validation(str);
    char* new_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter=0, index_counter = 0;
    filepath = path_validation(filepath);

    FILE *file = fopen(filepath, "r");
    if(file == NULL) {
        printf("The given file doesn't exist\n");
        return 0;
    }

    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no - 1)){
        new_content[strlen(new_content)] = c;
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    if(c == EOF && line_counter != (line_no - 1)){
        while(line_counter != (line_no)){
            new_content[strlen(new_content)] = '\n';
            line_counter++;
        }
    }
    while(c != EOF && index_counter != (start_pos)){
        new_content[strlen(new_content)] = c;
        index_counter++;
        c = fgetc(file);
    }
    if(c == EOF && index_counter != start_pos){
        while(index_counter != (start_pos)){
            new_content[strlen(new_content)] = ' ';
            index_counter++;
        }
    }
    for(int i=0; i<strlen(str); i++){
        new_content[strlen(new_content)] = str[i];
    }
    while(c != EOF){
        new_content[strlen(new_content)] = c;
        c = fgetc(file);
    }
    fclose(file);
    FILE *file1 = fopen(filepath, "w");
    fprintf(file1, "%s",new_content);
    fclose(file1);
    return 1;
}

int removef(char *filepath, int line_no, int start_pos, int size){
    filepath = path_validation(filepath);
    char* new_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = 0, del_counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL) {
        printf("The given file doesn't exist\n");
        return 0;
    }
    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no - 1)){
        new_content[strlen(new_content)] = c;
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    while(c != EOF && index_counter != (start_pos + 1)){
        new_content[strlen(new_content)] = c;
        index_counter++;
        c = fgetc(file);
    }
    while (del_counter < size){
        c = fgetc(file);
        del_counter++;
    }
    while(c != EOF){
        new_content[strlen(new_content)] = c;
        c = fgetc(file);
    }
    fclose(file);
    FILE *file1 = fopen(filepath, "w");
    fprintf(file1, "%s",new_content);
    fclose(file1);
    return 1;
}

int removeb(char *filepath, int line_no, int start_pos, int size){
    filepath = path_validation(filepath);
    char* new_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = 0, del_counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL) {
        printf("The given file doesn't exist\n");
        return 0;
    }
    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no - 1)){
        new_content[strlen(new_content)] = c;
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    while(c != EOF && index_counter != (start_pos)){
        new_content[strlen(new_content)] = c;
        index_counter++;
        c = fgetc(file);
    }
    while(del_counter < size){
        new_content[strlen(new_content) - 1] = '\0';
        del_counter ++;
    }
    printf("%s\n", new_content);
    while(c != EOF){
        new_content[strlen(new_content)] = c;
        c = fgetc(file);
    }
    fclose(file);
    FILE *file1 = fopen(filepath, "w");
    fprintf(file1, "%s",new_content);
    fclose(file1);
    return 1;
}

int cat(char* filepath){
    filepath = path_validation(filepath);
    FILE* file = fopen(filepath, "r");
    if(file == NULL) {
        printf("File can't be opened\n");
        return 0;
    }
    char c = fgetc(file);
    while(c != EOF){
        printf("%c", c);
        c = fgetc(file);
    }
    fclose(file);
    return 1;
}


