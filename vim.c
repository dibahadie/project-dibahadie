#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "string_manipulation.h"
#include "ccp.h"
#include "insert_remove.h"
#include "find.h"
#include "mkfile.h"
#include "auto_indent.h"
#include "tree.h"
#include "diff.h"
#define MAX_SIZE 500
#define MAX_FILE 10000


int get_command();

int main(){
    mkdir("root", 0777);
    while(1){
        int r = get_command();
        print_error(r);
    }
}

int get_command(){
    char *input;
    input = (char*) malloc(sizeof(char) * MAX_SIZE);
    fgets(input, MAX_SIZE, stdin);
    char *initial_command, *remaining;
    if(gnc(&input, &initial_command, &remaining) == 0) return 0;

    if(!strcmp(initial_command, "createfile")){
        return run_create_file(input);
    }

    else if(!strcmp(initial_command, "cat")){
        return run_cat(input);
    }

    else if(!strcmp(initial_command, "insertstr")){
        return run_insert(input);
    }

    else if(!strcmp(initial_command, "removestr")){
        return run_remove(input);
    }
    
    else if(!strcmp(initial_command, "find")){
        char *filepath, *str;
        if(!get_input(input, &str, " --file", "--str ")) return 0;
        string_validation(str);
        char *count_ptr = strstr(input, "-count");
        char *all_ptr = strstr(input, "-all");
        char *byword_ptr = strstr(input, "-byword");
        char *at_ptr = strstr(input, "-at");
        if(count_ptr == NULL && all_ptr == NULL && byword_ptr == NULL && at_ptr == NULL){
            if(!get_input(input, &filepath, "\n", "--file ")) return 0;
            printf("%d\n", find_first_index(filepath, str));
            return 1;
        }
        if(all_ptr == NULL && byword_ptr == NULL && at_ptr == NULL){
            if(!get_input(input, &filepath, " -count", "--file ")) return 0;
            printf("number of occurrence : %d\n", find_count(filepath, str));
            return 1;
        }
        if(count_ptr == NULL && all_ptr == NULL && byword_ptr == NULL){
            char *n_str;
            int n;
            if(!get_input(input, &filepath, " -at", "--file ")) return 0;
            if(!get_input(input, &n_str, "\n", "-at ")) return 0;
            n = atoi(n_str);
            printf("%d\n", find_at(filepath, str, n));
            return 1;
        }
    }
    else if(!strcmp(initial_command, "auto-indent")){
        char *filepath;
        if(!get_input(input, &filepath, "\n", "--file ")) return 0;
        auto_indent(filepath);
        return 1;
    }

    else if(!strcmp(initial_command, "compare")){
        char *filepath1, *filepath2;
        if(!get_input(input, &filepath1, "\n", "compare ")) return 0;
        filepath2 = strchr(filepath1 + 1, ' ');
        filepath2 ++;
        filepath1[strlen(filepath1) - 1 - strlen(filepath2)] = '\0';
        printf("%s\n", filepath1);
        diff(filepath1, filepath2);
        return 1;
    }

    else if(!strcmp(initial_command, "tree")){
        char *depth_str;
        int depth;
        if(!get_input(input, &depth_str, "\n", "tree ")) return 0;
        depth = atoi(depth_str);
        printf("%s", tree("root", 0, depth));
        return 1;
    }

    else if(!strcmp(initial_command, "copystr")){
        return run_copy(input);
    }

    else if(!strcmp(initial_command, "cutstr")){
        return run_cut(input);
    }

    else if(!strcmp(initial_command, "pastestr")){
        return run_paste(input);
    }

    
    return 0;
}

