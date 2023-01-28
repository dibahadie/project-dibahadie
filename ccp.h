#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

int insert(char *filepath, char *str, int line_no, int start_pos);
int removef(char *filepath, int line_no, int start_pos, int size);
int removeb(char *filepath, int line_no, int start_pos, int size);

int copyb(char *filepath, int line_no, int start_pos, int size){
    char *new_content;
    char *copied_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    copied_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL) return 0;
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
    for(int i=0; i<size; i++){
        copied_content[size - 1 - i] = new_content[strlen(new_content) - 1 - i];
    }
    fclose(file);
    copied_content[strlen(copied_content)] = '\0';
    char command[MAX_FILE] = "echo ";
    strcat(command, copied_content);
    strcat(command, " | pbcopy");
    system(command);
    return 1;
}

int copyf(char *filepath, int line_no, int start_pos, int size){
    char *copied_content;
    copied_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = 0, copy_counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL) return 0;
    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no - 1)){
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    while(c != EOF && index_counter != (start_pos)){
        index_counter++;
        c = fgetc(file);
    }
    while (copy_counter < size){
        c = fgetc(file);
        copied_content[strlen(copied_content)] = c;
        copy_counter++;
    }
    fclose(file);
    copied_content[strlen(copied_content)] = '\0';
    char command[MAX_FILE] = "echo ";
    strcat(command, copied_content);
    strcat(command, " | pbcopy");
    system(command);
    return 1;
}

int cutf(char *filepath, int line_no, int start_pos, int size){
    copyf(filepath, line_no, start_pos, size);
    removef(filepath, line_no, start_pos, size);
    return 1;
}

int cutb(char *filepath, int line_no, int start_pos, int size){
    copyb(filepath, line_no, start_pos, size);
    removeb(filepath, line_no, start_pos, size);
    return 1;
}

int paste(char *filepath, int line_no, int start_pos){
    char *str;
    str = (char*) malloc(sizeof(char) * MAX_FILE);
    FILE *tmp = fopen("root/tmp.txt", "r");
    system("pbpaste > root/tmp.txt");
    str = fgets(str, MAX_FILE, tmp);
    printf("%s\n", str);
    insert(filepath, str, line_no, start_pos);
    return 1;
}