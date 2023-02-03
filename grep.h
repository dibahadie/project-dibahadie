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
void string_validation(char initial[]);
int find_line_number(char *filepath, char *str, int greater);
int find_count(char *filepath, char *str);

void grep(char **files, char *str, int **lines, int n){
    for(int i=0; i<n; i++){
        int count = find_count(files[i], str);
        int line_counter = 0;
        for(int j=0; j<count; j++){
            int line = find_line_number_at(files[i], str, j+1);
            if(line != -1) lines[i][line - 1] = 1;
        }
    }
}

void print_line(char *filepath, int line_number){
    FILE *file = fopen(filepath, "r");
    char *line = (char*) malloc(sizeof(char) * MAX_FILE);
    for(int i=0; i<line_number; i++) fgets(line, MAX_FILE, file);
    printf("%s", line);
    fclose(file);
}

void print_grep(char **files, int **lines, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<MAX_FILE; j++){
            if(lines[i][j] == 1) {
                printf("%s : ", files[i]);
                print_line(files[i], j+1);
            }
        }
    }
}

void print_names(char **files, int **lines, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<MAX_FILE; j++){
            if(lines[i][j] == 1){
                printf("%s\n", files[i]);
                break;
            }
        }
    }
}

int files_validation(char *files, char **files_list){
    char *ptr = files;
    char *quote_ptr;
    int counter = 0;
    while(*ptr != '\0'){
        if(*ptr == '\"') {
            ptr++;
            quote_ptr = strchr(ptr, '\"');
            strncpy(files_list[counter++], ptr, strlen(ptr) - strlen(quote_ptr));
            ptr = quote_ptr + 2;
        }
        else{
            quote_ptr = strchr(ptr, ' ');
            if(quote_ptr == NULL){
                strncpy(files_list[counter++], ptr, strlen(ptr));
                return counter;
            }
            strncpy(files_list[counter++], ptr, strlen(ptr) - strlen(quote_ptr));
            ptr = quote_ptr + 1;
        }
    }
    return counter;
}

int run_grep(char *input){
    char *str = (char*) malloc(sizeof(char) * MAX_FILE);
    if(!get_input(input, &str, " --file", "--str ")) return -105;
    string_validation(str);
    char *files_str = (char*) malloc(sizeof(char) * MAX_FILE);
    char **files = (char**) malloc(sizeof(char*) * MAX_FILE);
    for(int i=0; i<MAX_FILE; i++) files[i] = (char*) malloc(sizeof(char) * MAX_FILE);
    if(!get_input(input, &files_str, "\n", "--files ")) return -105;
    int count = files_validation(files_str, files);
    int **lines = (int**) malloc(sizeof(int*) * count);
    for(int i=0; i<count; i++) lines[i] = malloc(sizeof(int) * MAX_FILE);
    for(int i=0; i<count; i++){
        for(int j=0; j<MAX_FILE; j++){
            lines[i][j] = 0;
        }
    }

    char *c_ptr = (char*) malloc(sizeof(char) * MAX_FILE);
    char *l_ptr = (char*) malloc(sizeof(char) * MAX_FILE);
    c_ptr = strstr(input, "-c");
    l_ptr = strstr(input, "-l");


    if(c_ptr == NULL && l_ptr == NULL){
        grep(files, str, lines, count);
        print_grep(files, lines, count);
        return 1;
    }
    
    else if(c_ptr != NULL && l_ptr == NULL){
        grep(files, str, lines, count);
        int c = 0;
        for(int i=0; i<count; i++){
            for(int j=0; j<MAX_FILE; j++){
                if(lines[i][j] == 1){
                    c++;
                    break;
                }
            }
        }
        printf("%d\n", c);
        return 1;
    }

    else if(c_ptr == NULL && l_ptr != NULL){
        grep(files, str, lines, count);
        print_names(files, lines, count);
        return 1;
    }

    return -105;
}



