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

char *strstr_wild(char *str, char *to_be_found){
    char *ptr = strchr(to_be_found, '*');
    if(ptr == NULL) {
        return strstr(str, to_be_found);
    }
    else{
        if(to_be_found[0] == '*'){
            to_be_found ++;
            ptr = strstr(str, to_be_found);
            if(ptr == NULL) return NULL;
            int index = strlen(str) - strlen(ptr);
            while(str[index] != EOF && str[index] != '\0' && str[index] != ' ' && str[index] != '\n'){
                ptr--;
                index--;
            }
            return ++ptr;
        }
        else if(to_be_found[strlen(to_be_found) - 1] == '*' && to_be_found[strlen(to_be_found) - 1] != '\\'){
            char *new;
            new = (char*) malloc(sizeof(to_be_found));
            strncpy(new, to_be_found, strlen(to_be_found) - 1);
            ptr = strstr(str, new);
            if(ptr == NULL) return NULL;
            int index = strlen(str) - strlen(ptr) + strlen(new);
            if(str[index] == '\0' || str[index] == EOF || str[index] == '\n') return NULL;
            return ptr;
        }
        else if(to_be_found[strlen(to_be_found) - 1] == '*' && to_be_found[strlen(to_be_found) - 1] == '\\'){
            int size = strlen(to_be_found);
            char *new;
            new = (char*) malloc(sizeof(to_be_found));
            strcpy(new, to_be_found);
            for(int i=0; i<size - 1; i++){
                if(new[i] == '\\' && new[i+1] == '*'){
                    for(int j=i; j<size; j++) new[j] = new[j+1];
                    size --;
                    new[i] = '*';
                }
            }
            return strstr(str, new);
        }
        else{
            char *first_part = (char*) malloc(sizeof(to_be_found));
            char *second_part = (char*) malloc(sizeof(to_be_found));
            strncpy(first_part, to_be_found, strlen(to_be_found) - strlen(ptr));
            strcpy(second_part, to_be_found + strlen(first_part) + 2);

            if(first_part[strlen(first_part) - 1] != ' '){
                char *find = strstr(str, first_part);
                char *ptr_tmp = find;
                ptr_tmp += strlen(first_part);
                if(*(ptr_tmp) == ' ' || *(ptr_tmp) == EOF || *(ptr_tmp) == '\0') return NULL;
                else{
                    while(*ptr_tmp != ' ' && *ptr_tmp != '\0' && *ptr_tmp != EOF){
                        ptr_tmp++;
                    }
                    ptr_tmp++;
                    int second_size = strlen(to_be_found) - strlen(first_part) - 2;
                    for(int i=0; i<second_size; i++){
                        if(*ptr_tmp != second_part[i]) return strstr_wild(ptr_tmp, to_be_found);
                        ptr_tmp++;
                    }
                    return find;
                }
            }
            return NULL;
        }
    }
}

int find_first_index(char *filepath, char *str){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;
    FILE *file = fopen(filepath, "r");
    char c = fgetc(file);
    char *content;
    content = (char*) malloc(sizeof(char) * MAX_FILE);
    while ((c != EOF)){
        content[strlen(content)] = c;
        c = fgetc(file);
    }
    char *loc_ptr = strstr_wild(content, str);
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

