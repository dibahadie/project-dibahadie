#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

char* path_validation(char initial[]);

char *tree(char* initialpath, int depth, int given_depth){
    static char tree_str[MAX_FILE];
    if(depth >= 2 * given_depth) return tree_str;
    char* path;
    path = (char*) malloc(sizeof(char) * MAX_FILE);
    struct dirent *dirpath;
    DIR *dir = opendir(initialpath);
    if(!dir) return 0;
    while((dirpath = readdir(dir)) != NULL){
        if(strcmp(dirpath->d_name, ".") != 0 && strcmp(dirpath->d_name, "..") != 0){
            for(int i=0; i<depth; i++){
                if(i % 2 == 0 || i == 0) tree_str[strlen(tree_str)] = '|';
                else tree_str[strlen(tree_str)] = ' ';
            }
            char add[MAX_FILE] = "├─";
        strcat(add, dirpath->d_name);
        strcat(add, "\n");
        strcat(tree_str, add);
        strcpy(path, initialpath);
        strcat(path, "/");
        strcat(path, dirpath->d_name);
        tree(path, depth + 2 , given_depth);
        }
    }
    closedir(dir);
    return tree_str;
}
