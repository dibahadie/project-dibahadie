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
            if(index == 0 || str[index - 1] == '\0' || str[index - 1] == EOF || str[index - 1] == ' ') return strstr_wild(ptr + strlen(to_be_found), to_be_found - 1);
            while(str[index] != EOF && str[index] != '\0' && str[index] != ' '){
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
            if(str[index] == '\0' || str[index] == EOF  || str[index] == ' ') return strstr_wild(str + strlen(new) + 1, to_be_found);
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

            if(first_part[strlen(first_part) - 1] != ' '){
                strcpy(second_part, to_be_found + strlen(first_part) + 2);
                char *find = strstr_wild(str, first_part);
                if(find == NULL) return NULL;
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
            else{
                strcpy(second_part, to_be_found + strlen(first_part));
                char *find = strstr_wild(str, second_part);
                if(find == NULL) return NULL;
                char *ptr_tmp = find;
                ptr_tmp -= strlen(first_part);
                char *new_first = (char*) malloc(sizeof(to_be_found));
                strncpy(new_first, ptr_tmp, strlen(first_part));
                if(strcmp(new_first, first_part) == 0) return ptr_tmp;
                else return strstr_wild(str + 1 ,to_be_found);
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
        return -103;
    }
    int index = strlen(content) - strlen(loc_ptr);
    return index;
}

int find_count(char *filepath, char *str){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;

    int counter = 0;
    FILE *file = fopen(filepath, "r");

    char c = fgetc(file);
    char *content, *buff;
    buff = (char*) malloc(sizeof(str));
    content = (char*) malloc(sizeof(char) * MAX_FILE);
    while ((c != EOF)){
        content[strlen(content)] = c;
        c = fgetc(file);
    }
    char *loc_ptr = strstr_wild(content, str);
    if(loc_ptr == NULL){
        return 0;
    }
    while(loc_ptr != NULL){
        counter++;
        loc_ptr = strstr_wild(loc_ptr + 1, str);
    }
    return counter;
}

int find_at(char *filepath, char *str, int n){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;

    int counter = 1;
    FILE *file = fopen(filepath, "r");

    char c = fgetc(file);
    char *content, *buff;
    buff = (char*) malloc(sizeof(str));
    content = (char*) malloc(sizeof(char) * MAX_FILE);
    while ((c != EOF)){
        content[strlen(content)] = c;
        c = fgetc(file);
    }
    char *loc_ptr = strstr_wild(content, str);
    if(loc_ptr == NULL) return -1;
    while(loc_ptr != NULL && counter != n){
        counter++;
        loc_ptr = strstr_wild(loc_ptr + 1, str);
    }
    if(counter < n) return -103;
    else{
        int index = strlen(content) - strlen(loc_ptr);
        return index;
    }
}

int find_byword(char *filepath, char *str, int n){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;
    FILE *file = fopen(filepath, "r");
    char c = fgetc(file);
    int word_counter = 1;
    int index = find_at(filepath, str, n);
    for(int i=0; i<index - 1; i++) {
        c = fgetc(file);
        if(c == ' ' || c == '\n') word_counter++;
    }
    return word_counter;
    }

int find_all(char *filepath, char *str){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;
    
    int count = find_count(filepath, str);    
    if(count == 0) return -103;
    printf("%d", find_at(filepath, str, 1));
    for(int i=1; i<count; i++){
        printf(", %d", find_at(filepath, str, i+1));
    }
    return 1;
}

int find_all_byword(char *filepath, char *str){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;

    int count = find_count(filepath, str);
    if(count == 0) return -103;
    int word = find_byword(filepath, str, 1);
    printf("%d", word);
    for(int i=1; i<count; i++){
        word = find_byword(filepath, str, i+1);
        printf(", %d", word);
    }
    printf("\n");
    return -1;
}

int run_find(char *input){
    char *filepath, *str;
    if(!get_input(input, &str, " --file", "--str ")) return -105;
    string_validation(str);

    char *count_ptr = strstr(input, "-count");
    char *all_ptr = strstr(input, "-all");
    char *byword_ptr = strstr(input, "-byword");
    char *at_ptr = strstr(input, "-at");

    // no option
    if(count_ptr == NULL && all_ptr == NULL && byword_ptr == NULL && at_ptr == NULL){
        if(!get_input(input, &filepath, "\n", "--file ")) return -105;
        return find_first_index(filepath, str);
    }

    // count option
    else if(all_ptr == NULL && byword_ptr == NULL && at_ptr == NULL){
    if(!get_input(input, &filepath, " -count", "--file ")) return -105;
        return find_count(filepath, str);
    }

    // at option
    else if(count_ptr == NULL && all_ptr == NULL && byword_ptr == NULL){
        char *n_str;
        int n;
        if(!get_input(input, &filepath, " -at", "--file ")) return -105;
        if(!get_input(input, &n_str, "\n", "-at ")) return -105;
        n = atoi(n_str);
        return find_at(filepath, str, n);
    }

    // byword option
    else if(count_ptr == NULL && all_ptr == NULL && at_ptr == NULL){
        if(!get_input(input, &filepath, " -byword", "--file ")) return -105;
        return find_byword(filepath, str, 1);
    }

    // all option
    else if(count_ptr == NULL && at_ptr == NULL && byword_ptr == NULL){
        if(!get_input(input, &filepath, " -all", "--file ")) return -105;
        return find_all(filepath, str);
    }

    // all byword options
    else if(count_ptr == NULL && at_ptr == NULL){
        if(all_ptr > byword_ptr){
            if(!get_input(input, &filepath, " -byword", "--file ")) return -105;
        }
        else{
            if(!get_input(input, &filepath, " -all", "--file ")) return -105;
        }
        return find_all_byword(filepath, str);
    }

    // at byword options
    else if(count_ptr == NULL && all_ptr == NULL){
        char *n_str;
        int n;
        if(at_ptr > byword_ptr){
            if(!get_input(input, &filepath, " -byword", "--file ")) return -105;
            if(!get_input(input, &n_str, "\n", "-at ")) return -105;
        }
        else{
            if(!get_input(input, &filepath, " -at", "--file ")) return -105;
            if(!get_input(input, &n_str, " -byword", "-at ")) return -105;
        }
        n = atoi(n_str);
        return find_byword(filepath, str, n);
    }
    
    return -105;
    }

    