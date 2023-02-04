#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

char* diff(char *filepath1, char *filepath2){
    int line_counter = 1;
    int rest_counter = 0;
    filepath1 = path_validation(filepath1);
    int r1 = existance_validation(filepath1);
    if(r1 != 1) return itoa(r1);
    filepath2 = path_validation(filepath2);
    int r2 = existance_validation(filepath2);
    if(r2 != 1) return itoa(r2);
    FILE *file1 = fopen(filepath1, "r");
    FILE *file2 = fopen(filepath2, "r");
    
    char *output = (char*) malloc(sizeof(char) * MAX_FILE);
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
            strcat(output, ">>>>>>>>>>>> #");
            strcat(output, itoa(line_counter));
            strcat(output, " - #");
            strcat(output, itoa(line_counter + rest_counter - 1));
            strcat(output, " >>>>>>>>>>>>\n");
            for(int i=0; i<rest_counter; i++){
                strcat(output, rest[i]);
                if(rest[i][strlen(rest[i]) - 1] != '\n') strcat(output, "\n");
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
            strcat(output, "<<<<<<<<<<<< #");
            strcat(output, itoa(line_counter));
            strcat(output, " - #");
            strcat(output, itoa(line_counter + rest_counter - 1));
            strcat(output, " <<<<<<<<<<<<\n");
            for(int i=0; i<rest_counter; i++){
                strcat(output, rest[i]);
                if(rest[i][strlen(rest[i]) - 1] != '\n') strcat(output, "\n");
            }
            break;
        }
        else if(strcmp(line1, line2)) {
            strcat(output, "============ #");
            strcat(output, itoa(line_counter));
            strcat(output, " ============\n");
            strcat(output, line1);
            if(line1[strlen(line1) - 1] != '\n') strcat(output, "\n");
            strcat(output, line2);
            if(line2[strlen(line2) - 1] != '\n') strcat(output, "\n");
        }
        line1 = fgets(line1, MAX_FILE, file1);
        line2 = fgets(line2, MAX_FILE, file2);
        line_counter++;
    }
    fclose(file1);
    fclose(file2);
    return output;
}

char *run_diff(char *input){
    char *filepath1, *filepath2;
    if(!get_input(input, &filepath1, "\n", "compare ")) return itoa(-105);
    filepath2 = strchr(filepath1 + 1, ' ');
    filepath2 ++;
    filepath1[strlen(filepath1) - 1 - strlen(filepath2)] = '\0';
    return diff(filepath1, filepath2);
    // return "NOTHING\n";
}