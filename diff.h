#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

void diff(char *filepath1, char *filepath2){
    int line_counter = 1;
    int rest_counter = 0;
    FILE *file1 = fopen(filepath1, "r");
    FILE *file2 = fopen(filepath2, "r");
    char *line1, *line2;
    line1 = (char*) malloc(sizeof(char) * MAX_FILE);
    line2 = (char*) malloc(sizeof(char) * MAX_FILE);
    line1 = fgets(line1, MAX_FILE, file1);
    line2 = fgets(line2, MAX_FILE, file2);
    while(line1 != NULL || line2 != NULL){
        if(line1 == NULL) {
            char **rest;
            rest = (char**) malloc(sizeof(char*) * MAX_FILE);
            for(int i=0; i<MAX_FILE; i++){
                rest[i] = (char*) malloc(sizeof(char) * MAX_FILE);
            }
            do{
                strcpy(rest[rest_counter], line2);
                rest_counter++;
            }while(fgets(line2, MAX_FILE, file2) != NULL);
            printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", line_counter, line_counter + rest_counter - 1);
            for(int i=0; i<rest_counter; i++){
                printf("%s", rest[i]);
                if(rest[i][strlen(rest[i]) - 1] != '\n') printf("\n");
            }
            break;
        }
        else if(line2 == NULL) {
            char **rest;
            rest = (char**) malloc(sizeof(char*) * MAX_FILE);
            for(int i=0; i<MAX_FILE; i++){
                rest[i] = (char*) malloc(sizeof(char) * MAX_FILE);
            }
            do{
                strcpy(rest[rest_counter], line1);
                rest_counter++;
            }while(fgets(line1, MAX_FILE, file1) != NULL);
            printf("<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n", line_counter, line_counter + rest_counter - 1);
            for(int i=0; i<rest_counter; i++){
                printf("%s", rest[i]);
                if(rest[i][strlen(rest[i]) - 1] != '\n') printf("\n");
            }
            break;
        }
        else if(strcmp(line1, line2)) {
            printf("============ #%d ============\n", line_counter);
            printf("%s", line1);
            if(line1[strlen(line1) - 1] != '\n') printf("\n");
            printf("%s", line2);
        }
        line1 = fgets(line1, MAX_FILE, file1);
        line2 = fgets(line2, MAX_FILE, file2);
        line_counter++;
    }
}