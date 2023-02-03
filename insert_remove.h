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


int insert(char *filepath, char *str, int line_no, int start_pos){
    string_validation(str);
    char* new_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter=0, index_counter = 0;
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;
    FILE *file = fopen(filepath, "r");
    backup(filepath);

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
    return -107;
}

int removef(char *filepath, int line_no, int start_pos, int size){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;
    backup(filepath);

    char* new_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = 0, del_counter = 0;
    FILE *file = fopen(filepath, "r");

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
    return -107;
}

int removeb(char *filepath, int line_no, int start_pos, int size){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;
    backup(filepath);

    char* new_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = 0, del_counter = 0;
    FILE *file = fopen(filepath, "r");
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
    while(c != EOF){
        new_content[strlen(new_content)] = c;
        c = fgetc(file);
    }
    fclose(file);
    FILE *file1 = fopen(filepath, "w");
    fprintf(file1, "%s",new_content);
    fclose(file1);
    return -107;
}

char* cat(char* filepath){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    char *r_str = (char*) malloc(sizeof(char) * MAX_FILE);
    if(r != 1) return itoa(r);

    FILE * file = fopen(filepath, "r");
    char c = fgetc(file);
    char *output = (char*) malloc(sizeof(char) * MAX_FILE);
    while(c != EOF){
        output[strlen(output)] = c;
        c = fgetc(file);
    }
    output[strlen(output)] = '\0';
    fclose(file);
    return output;
}

char* run_cat(char *input){
    char *filepath;
    if(!get_input(input, &filepath, "\n", "--file ")) return itoa(-105);
    return cat(filepath);
}

char* run_insert(char *input){
    char *filepath, *str, *str_line, *str_start;
    int line_no;
    int start_pos;
    if(!get_input(input, &filepath, " --str", "--file ")) return itoa(-105);
    if(!get_input(input, &str, " --pos", "--str ")) return itoa(-105);
    if(!get_input(input, &str_line, ":", "--pos ")) return itoa(-105);
    if(!get_input(input, &str_start, "\n", ":")) return itoa(-105);
    line_no = atoi(str_line);
    start_pos = atoi(str_start);
    int r = insert(filepath, str, line_no, start_pos);
    return itoa(r);
}

char* run_remove(char *input){
    char *filepath, *line_str, *start_str, *size_str;
    int line_no, start_pos, size;
    if(!get_input(input, &filepath, " --pos", "--file ")) return itoa(-105);
    if(!get_input(input, &line_str, ":", "--pos ")) return itoa(-105);
    if(!get_input(input, &start_str, " -size", ":")) return itoa(-105);
    if(!get_input(input, &size_str, " -f\n", "-size ") && !get_input(input, &size_str, " -b\n", "-size ")) return itoa(-105);
    char mode = input[strlen(input) - 2];
    if(mode != 'f' && mode != 'b') return itoa(-105);
    line_no = atoi(line_str);
    start_pos = atoi(start_str);
    size = atoi(size_str);
    int r;
    if(mode == 'f') r = removef(filepath, line_no, start_pos, size);
    else if(mode == 'b') r = removeb(filepath, line_no, start_pos, size);
    return itoa(r);
}