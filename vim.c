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
        int r = run_find(input);
        if(r >= 0) printf("%d\n", r);
        return r;
    }

    else if(!strcmp(initial_command, "auto-indent")){
        char *filepath;
        if(!get_input(input, &filepath, "\n", "--file ")) return -105;
        auto_indent(filepath);
        return 1;
    }

    else if(!strcmp(initial_command, "compare")){
        char *filepath1, *filepath2;
        if(!get_input(input, &filepath1, "\n", "compare ")) return -105;
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
        if(!get_input(input, &depth_str, "\n", "tree ")) return -105;
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

    return -105;
}

