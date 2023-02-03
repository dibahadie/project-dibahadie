#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

int find_at(char *filepath, char *str, int n);
char* path_validation(char initial[]);
int existance_validation(char path[]);
int get_input(char* command, char** text, char* next_identifier, char*pre_identifier);
void string_validation(char initial[]);
char *strstr_wild(char *str, char *to_be_found);
int strlen_wild(char *str, char *to_be_found);
int removef(char *filepath, int line_no, int start_pos, int size);
int insert(char *filepath, char *str, int line_no, int start_pos);
int find_count(char *filepath, char *str);

int replace_at(char *filepath, char *initial_str, char *final_str, int n){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;

    FILE *file = fopen(filepath, "r");
    int index = find_at(filepath, initial_str, n);
    char c;
    char *content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 1, start_pos = 0;
    for(int i=0; i<index; i++){
        c = fgetc(file);
        content[strlen(content)] = c;
        if(c == '\n') {
            line_counter++;
            start_pos = -1;
        }
        start_pos++;
    }
    while(c != ' ' && c != '\0' && c != EOF){
        c = fgetc(file);
        content[strlen(content)] = c;
    }
    content[strlen(content)] = '\0';
    
    removef(filepath, line_counter, start_pos, strlen_wild(content, initial_str));
    insert(filepath, final_str, line_counter, start_pos);
    return 1;
}

int replace_all(char *filepath, char *initial_str, char *final_str){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;
    int count = find_count(filepath, initial_str);
    for(int i=0; i<count; i++){
        replace_at(filepath, initial_str, final_str, 1);
    }
    return 1;
}

int strlen_wild(char *str, char *to_be_found){
    char *ptr = strchr(to_be_found, '*');
    if(ptr == NULL) {
        return strlen(to_be_found);
    }
    else{
        if(to_be_found[0] == '*'){
            to_be_found ++;
            ptr = strstr(str, to_be_found);
            if(ptr == NULL) return -1;
            int index = strlen(str) - strlen(ptr);
            int counter = 0;
            if(index == 0 || str[index - 1] == '\0' || str[index - 1] == EOF || str[index - 1] == '\n' || str[index - 1] == ' ') return strlen_wild(ptr + strlen(to_be_found), to_be_found - 1);
            while(str[index] != EOF && str[index] != '\0' && str[index] != ' '){
                counter++;
                index--;
            }
            return counter + strlen(to_be_found) - 1;
        }
        else if(to_be_found[strlen(to_be_found) - 1] == '*' && to_be_found[strlen(to_be_found) - 1] != '\\'){
            char *new_str;
            new_str = (char*) malloc(sizeof(to_be_found));
            strncpy(new_str, to_be_found, strlen(to_be_found) - 1);
            ptr = strstr(str, new_str);
            int counter = 0;
            int index = strlen(str) - strlen(ptr) + strlen(new_str);
            while(str[index] != '\0' && str[index] != EOF && str[index] != ' ') {
                index++;
                counter++;
                ptr++;
            }
            return strlen(new_str) + counter;
        }
        else if(to_be_found[strlen(to_be_found) - 1] == '*' && to_be_found[strlen(to_be_found) - 1] == '\\'){
            return strlen(to_be_found) - 1;
        }
        else{
            char *first_part = (char*) malloc(sizeof(to_be_found));
            char *second_part = (char*) malloc(sizeof(to_be_found));
            strncpy(first_part, to_be_found, strlen(to_be_found) - strlen(ptr));

            if(first_part[strlen(first_part) - 1] != ' '){
                strcpy(second_part, to_be_found + strlen(first_part) + 2);
                char *find = strstr_wild(str, first_part);
                if(find == NULL) return -1;
                char *ptr_tmp = find;
                ptr_tmp += strlen(first_part);
                int counter = 0;
                while(*ptr_tmp != ' ' && *ptr_tmp != '\0' && *ptr_tmp != EOF){
                    ptr_tmp++;
                    counter++;
                }
                ptr_tmp++;
                return strlen(to_be_found) + counter - 1;
            }
            else{
                strcpy(second_part, to_be_found + strlen(first_part));
                char *find = strstr_wild(str, second_part);
                if(find == NULL) return -1;
                char *ptr_tmp = find;
                ptr_tmp -= strlen(first_part);
                char *new_first = (char*) malloc(sizeof(to_be_found));
                strncpy(new_first, ptr_tmp, strlen(first_part));
                if(strcmp(new_first, first_part) == 0) return strlen_wild(str, second_part) + strlen(first_part);
                else return strlen_wild(str + 1 ,to_be_found);
            }
            return -1;
        }
    }
}


int run_replace(char *input){
    char *filepath, *initial_str, *final_str;
    char *at_ptr, *all_ptr;

    if(!get_input(input, &initial_str, " --str2", "--str1 ")) return -105;
    if(!get_input(input, &final_str, " --file", "--str2 ")) return -105;
    string_validation(initial_str);
    string_validation(final_str);
    at_ptr = strstr(input, "-at");
    all_ptr = strstr(input, "-all");

    if(at_ptr == NULL && all_ptr == NULL){
        if(!get_input(input, &filepath, "\n", "--file ")) return -105;
        return replace_at(filepath, initial_str, final_str, 1);
    }
    else if(at_ptr == NULL && all_ptr != NULL){
        if(!get_input(input, &filepath, " -all", "--file ")) return -105;
        return replace_all(filepath, initial_str, final_str);
    }
    else if(at_ptr != NULL && all_ptr == NULL){
        char *n_str = (char*) malloc(sizeof(char) * MAX_SIZE);
        if(!get_input(input, &filepath, " -at", "--file ")) return -105;
        if(!get_input(input, &n_str, "\n", "-at ")) return -105;
        int n = atoi(n_str);
        return replace_at(filepath, initial_str, final_str, n);
    }
    return -105;
}

