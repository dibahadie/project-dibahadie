#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>

int create_dir(char *path){
    char *remained_path = path;
    char *last_dir = NULL;

    remained_path = (char*) malloc(sizeof(path));
    last_dir = strrchr(path, '/');
    if(last_dir == NULL) {
        mkdir(path ,0777);
        return 1;
    }
    strncpy(remained_path, path, strlen(path) - strlen(last_dir));
    int result = create_dir(remained_path);
    if(*(remained_path) == '/') mkdir(path + 1 ,0777);
    else mkdir(path, 0777);
    return result;
}

int create_file(char *path){
    path = path + 1;
    if(strchr(path, '/') == NULL){
        struct stat buff;
        if(stat(path, &buff) == -1){
            FILE *file = fopen(path, "w");
            fclose(file);
            return 1;
        }else{
            return 0;
        }
    }else{
        char *filename = strrchr(path, '/');
        char *dirpath;
        dirpath = (char*) malloc(sizeof(path));
        strncpy(dirpath, path, strlen(path) - strlen(filename));
        create_dir(dirpath);
        FILE *file = fopen(path, "w");
        fclose(file);
        return 1;
    }
}

int main(){
    mkdir("root", 0777);
}