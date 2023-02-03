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
char* path_validation(char initial[]);
int existance_validation(char path[]);
int get_input(char* command, char** text, char* next_identifier, char*pre_identifier);
void backup(char *filepath);


int copyb(char *filepath, int line_no, int start_pos, int size){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;

    char *new_content;
    char *copied_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    copied_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 1, index_counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL) return 0;
    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no)){
        new_content[strlen(new_content)] = c;
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    while(c != EOF && index_counter != (start_pos)){
        new_content[strlen(new_content)] = c;
        index_counter++;
        c = fgetc(file);
    }
    new_content[strlen(new_content)] = c;
    for(int i=1; i<=size; i++){
        copied_content[size - i] = new_content[strlen(new_content) - i];
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
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;
    char *copied_content;
    copied_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = -1, copy_counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL) return 0;
    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no - 1)){
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    if(line_counter == 0) index_counter++;
    while(c != EOF && index_counter != (start_pos)){
        index_counter++;
        c = fgetc(file);
    }
    while (copy_counter < size){
        copied_content[strlen(copied_content)] = c;
        c = fgetc(file);
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
    backup(filepath);
    copyf(filepath, line_no, start_pos, size);
    removef(filepath, line_no, start_pos, size);
    return 1;
}

int cutb(char *filepath, int line_no, int start_pos, int size){
    backup(filepath);
    copyb(filepath, line_no, start_pos, size);
    removeb(filepath, line_no, start_pos, size);
    return 1;
}

int paste(char *filepath, int line_no, int start_pos){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;
    backup(filepath);

    char *str;
    str = (char*) malloc(sizeof(char) * MAX_FILE);
    FILE *tmp = fopen("root/tmp.txt", "r");
    system("pbpaste > root/tmp.txt");
    str = fgets(str, MAX_FILE, tmp);
    insert(filepath, str, line_no, start_pos);
    return 1;
}

int run_copy(char *input){
    char *filepath, *line_str, *start_str, *size_str;
    int line_no, start_pos, size;
    if(!get_input(input, &filepath, " --pos", "--file ")) return -105;
    if(!get_input(input, &line_str, ":", "--pos ")) return -105;
    if(!get_input(input, &start_str, " -size", ":")) return -1055;
    if(!get_input(input, &size_str, " -f\n", "-size ") && !get_input(input, &size_str, " -b\n", "-size ")) return -105;
    char mode = input[strlen(input) - 2];
    if(mode != 'f' && mode != 'b') return -105;
    line_no = atoi(line_str);
    start_pos = atoi(start_str);
    size = atoi(size_str);
    int r;
    if(mode == 'f') r =copyf(filepath, line_no, start_pos, size);
    if(mode == 'b') r= copyb(filepath, line_no, start_pos, size);
    return r;
}

int run_cut(char *input){
    char *filepath, *line_str, *start_str, *size_str;
    int line_no, start_pos, size;
    if(!get_input(input, &filepath, " --pos", "--file ")) return -105;
    if(!get_input(input, &line_str, ":", "--pos ")) return -105;
    if(!get_input(input, &start_str, " -size", ":")) return -105;
    if(!get_input(input, &size_str, " -f\n", "-size ") && !get_input(input, &size_str, " -b\n", "-size ")) return -105;
    char mode = input[strlen(input) - 2];
    if(mode != 'f' && mode != 'b') return -105;
    line_no = atoi(line_str);
    start_pos = atoi(start_str);
    size = atoi(size_str);
    if(mode == 'f') cutf(filepath, line_no, start_pos, size);
    if(mode == 'b') cutb(filepath, line_no, start_pos, size);
    return 1;
}

int run_paste(char *input){
    char *filepath, *line_str, *start_str;
    int line_no, start_pos, size;
    if(!get_input(input, &filepath, " --pos", "--file ")) return 0;
    if(!get_input(input, &line_str, ":", "--pos ")) return 0;
    if(!get_input(input, &start_str, "\n", ":")) return 0;
    line_no = atoi(line_str);
    start_pos = atoi(start_str);
    paste(filepath, line_no, start_pos);
    return 1;
}