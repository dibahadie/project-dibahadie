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
#include "replace.h"
#include "grep.h"
#include "undo.h"
#include "arman.h"
#define MAX_SIZE 500
#define MAX_FILE 10000


char* run_command(char *input);

int main(){
    mkdir("root", 0777);
    mkdir("backup", 0777);
    while(1){
        char *input = (char*) malloc(sizeof(char) * MAX_SIZE);
        fgets(input, MAX_SIZE, stdin);
        char* output = run_command(input);
        printf("%s", output);
        if(output[strlen(output) - 1] != '\n') printf("\n");
        free(input);
    }
}

char* run_command(char *input){
    char *initial_command, *remaining;
    if(gnc(&input, &initial_command, &remaining) == 0) return NULL;

    if(strstr(input, "=D") != NULL){
        return run_arman(input);
    }

    else if(!strcmp(initial_command, "createfile")){
        int r = run_create_file(input);
        return itoa(r);
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
        return run_find(input);
    }

    else if(!strcmp(initial_command, "grep")){
        return run_grep(input);
    }

    else if(!strcmp(initial_command, "auto-indent")){
        return run_auto_indent(input);
    }

    else if(!strcmp(initial_command, "compare")){
        return run_diff(input);
    }

    else if(!strcmp(initial_command, "tree")){
        return run_tree(input);
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

    else if(!strcmp(initial_command, "replace")){
        return run_replace(input);
    }

    else if(!strcmp(initial_command, "undo")){
        return run_undo(input);
    }

    return itoa(-105);
}

