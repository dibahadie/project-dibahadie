#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

char* path_validation(char initial[]);

int auto_indent(char *filepath){
    filepath = path_validation(filepath);
    int r = existance_validation(filepath);
    if(r != 1) return r;
    
    int tab_counter = 0;
    FILE *file = fopen(filepath, "r");
    char *content;
    content = (char*) malloc(sizeof(char) * MAX_FILE);
    char c = fgetc(file);
    while(c != EOF){
        content[strlen(content)] = c;
        c = fgetc(file);
    }
    int size = strlen(content);
    for(int i=0; i<size; i++){
        if(content[i] == '{'){
            if(i == 0) continue;
            int j = i-1;
            while(j >= 0 && content[j] == ' '){
                for(int k=j; k<size; k++) content[k] = content[k+1];
                size--;
            }
        }
        if(content[i] == '}'){
            int j = i + 1;
            while(j<size && content[j] == ' '){
                for(int k=j; k<size; k++) content[k] = content[k+1];
                size--;
            }
        }
    }

    for(int i=0; i<size-1; i++){
        if(content[i] == '{' && content[i+1] != '\n'){
            for(int j=size; j>i+1; j--) content[j] = content[j-1];
            content[i+1] = '\n';
            size++;
        }
        if(content[i] == '}' && content[i+1] != '\n'){
            for(int j=size; j>i+1; j--) content[j] = content[j-1];
            content[i+1] = '\n';
            size++;
        }
        if(content[i] == '{' && content[i+1] == '}'){
            for(int j=size; j>i+1; j--) content[j] = content[j-1];
            content[i+1] = '\n';
            size++;
        }
    }

    for(int i=1; i<size; i++){
        if(content[i] == '}' && content[i-1] != '\n'){
            for(int j=size; j>i-1; j--) content[j] = content[j-1];
            content[i] = '\n';
            size++;
        }
        if(content[i] == '{' && content[i-1] != ' ' && content[i-1] != '\n'){
            for(int j=size; j>i-1; j--) content[j] = content[j-1];
            content[i] = ' ';
            size++;
        }
    }

    for(int i=0; i<size; i++){
        if(content[i] == '{') tab_counter += 4;
        else if(i != size - 1 && content[i] == '\n' && content[i+1] == '}') tab_counter -=4;
        if(tab_counter < 0) break;
        if(content[i] == '\n'){
            int j = i+1;
            while(j < size && content[j] == ' '){
                for(int k=j; k<size; k++) content[k] = content[k+1];
                size--;
            }
            size += tab_counter;
            for(int j=size; j>i+tab_counter; j--) content[j] = content[j-tab_counter];
            for(int j=i+1; j<i+tab_counter+1; j++) content[j] = ' ';
        }
    }
    fclose(file);
    FILE *file1 = fopen(filepath, "w");
    fprintf(file1, "%s" ,content);
    fclose(file1);
    return 1;
}