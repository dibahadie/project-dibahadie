#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 500
#define MAX_FILE 10000

int create_dir(char *path);
int create_file(char *path);
int get_command();
int gnc(char** command, char** next_command, char** remaining);
int get_input(char* command, char** filepath, char* next_identifier, char*pre_identifier);
int cat(char* filepath);
int insert(char *filepath, char *str, int line_no, int start_pos);
char* path_validation(char initial[]);
void string_validation(char initial[]);
int removef(char *filepath, int line_no, int start_pos, int size);
int removeb(char *filepath, int line_no, int start_pos, int size);
int find_first_index(char *filepath, char *str);
int find_count(char *filepath, char *str);
int find_at(char *filepath, char *str, int n);
int auto_indent(char *filepath);
void diff(char *filepath1, char *filepath2);
char* tree(char* initialpath, int depth, int given_depth);
int copyb(char *filepath, int line_no, int start_pos, int size);
int copyf(char *filepath, int line_no, int start_pos, int size);
int cutf(char *filepath, int line_no, int start_pos, int size);
int cutb(char *filepath, int line_no, int start_pos, int size);


int main(){
    mkdir("root", 0777);
    while(1){
        if(!get_command()) printf("invalid command\n");
    }
}

char* path_validation(char initial[]){
    char *path;
    path = (char*) malloc(strlen(initial) * sizeof(char));
    strcpy(path, initial);
    int size = strlen(path);
    if(path[0] == '/'){
        path = path + 1;
        size--;
    }
    else if(path[0] == '\"' && path[1] == '/'){
        path = path + 2;
        path[size - 3] = '\0';
        size -= 2;
    }
    else if(path[0] == '\"'){
        path = path + 1;
        path[size - 2] = '\0';
        size--;
    }
    if(path[size - 1] == '\n') path[size - 1] = '\0';
    return path;
}

int create_dir(char *path){
    char *remained_path;
    char *last_dir = NULL;

    last_dir = (char*) malloc(sizeof(path));
    remained_path = (char*) malloc(sizeof(path));
    last_dir = strrchr(path, '/');
    if(last_dir == NULL) {
        mkdir(path ,0777);
        return 1;
    }
    strncpy(remained_path, path, strlen(path) - strlen(last_dir));
    int result = create_dir(remained_path);
    if(*(path) == '/') mkdir(path + 1 ,0777);
    else mkdir(path, 0777);
    free(remained_path);
    return result;
}

int create_file(char *path){
    path = path_validation(path);
    if(strchr(path, '/') == NULL){
        FILE *file = fopen(path, "w");
        if(file != NULL){
            fclose(file);
            return 1;
        }else{
            return 0;
        }
    }else{
        char *filename = strrchr(path, '/');
        char *dirpath;
        struct stat buff;
        dirpath = (char*) malloc(sizeof(path));
        strncpy(dirpath, path, strlen(path) - strlen(filename));
        create_dir(dirpath);
        FILE *file = fopen(path, "r");
        if(file == NULL){
            file = fopen(path, "w");
            if(file == NULL){
                printf("File creation failed.\n");
                return 1;
            }
            fclose(file);
            return 1;
        }
        else{
            printf("The given file already exists\n");
        }
        return 0;
    }
}

int get_command(){
    char *input;
    input = (char*) malloc(sizeof(char) * MAX_SIZE);
    fgets(input, MAX_SIZE, stdin);
    char *initial_command, *remaining;
    if(gnc(&input, &initial_command, &remaining) == 0) return 0;

    if(!strcmp(initial_command, "createfile")){
        char* filepath;
        char* input_format;
        if(!get_input(input, &filepath, "\n", "--file ")) return 0;
        create_file(filepath);
        return 1;
    }
    else if(!strcmp(initial_command, "cat")){
        char *filepath;
        if(!get_input(input, &filepath, "\n", "--file ")) return 0;
        cat(filepath);
        return 1;
    }
    else if(!strcmp(initial_command, "insertstr")){
        char *filepath, *str, *str_line, *str_start;
        int line_no;
        int start_pos;
        if(!get_input(input, &filepath, " --str", "--file ")) return 0;
        if(!get_input(input, &str, " --pos", "--str ")) return 0;
        if(!get_input(input, &str_line, ":", "--pos ")) return 0;
        if(!get_input(input, &str_start, "\n", ":")) return 0;
        line_no = atoi(str_line);
        start_pos = atoi(str_start);
        insert(filepath, str, line_no, start_pos);
        return 1;
    }
    else if(!strcmp(initial_command, "removestr")){
        char *filepath, *line_str, *start_str, *size_str;
        int line_no, start_pos, size;
        if(!get_input(input, &filepath, " --pos", "--file ")) return 0;
        if(!get_input(input, &line_str, ":", "--pos ")) return 0;
        if(!get_input(input, &start_str, " -size", ":")) return 0;
        if(!get_input(input, &size_str, " -f\n", "-size ") && !get_input(input, &size_str, " -b\n", "-size ")) return 0;
        char mode = input[strlen(input) - 2];
        if(mode != 'f' && mode != 'b') return 0;
        line_no = atoi(line_str);
        start_pos = atoi(start_str);
        size = atoi(size_str);
        if(mode == 'f') removef(filepath, line_no, start_pos, size);
        if(mode == 'b') removeb(filepath, line_no, start_pos, size);
        return 1;
    }else if(!strcmp(initial_command, "find")){
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

    else if(!strcmp(initial_command, "copy")){
        char *filepath, *line_str, *start_str, *size_str;
        int line_no, start_pos, size;
        if(!get_input(input, &filepath, " --pos", "--file ")) return 0;
        if(!get_input(input, &line_str, ":", "--pos ")) return 0;
        if(!get_input(input, &start_str, " -size", ":")) return 0;
        if(!get_input(input, &size_str, " -f\n", "-size ") && !get_input(input, &size_str, " -b\n", "-size ")) return 0;
        char mode = input[strlen(input) - 2];
        if(mode != 'f' && mode != 'b') return 0;
        line_no = atoi(line_str);
        start_pos = atoi(start_str);
        size = atoi(size_str);
        if(mode == 'f') copyf(filepath, line_no, start_pos, size);
        if(mode == 'b') copyb(filepath, line_no, start_pos, size);
        return 1;
    }

    else if(!strcmp(initial_command, "cut")){
        char *filepath, *line_str, *start_str, *size_str;
        int line_no, start_pos, size;
        if(!get_input(input, &filepath, " --pos", "--file ")) return 0;
        if(!get_input(input, &line_str, ":", "--pos ")) return 0;
        if(!get_input(input, &start_str, " -size", ":")) return 0;
        if(!get_input(input, &size_str, " -f\n", "-size ") && !get_input(input, &size_str, " -b\n", "-size ")) return 0;
        char mode = input[strlen(input) - 2];
        if(mode != 'f' && mode != 'b') return 0;
        line_no = atoi(line_str);
        start_pos = atoi(start_str);
        size = atoi(size_str);
        if(mode == 'f') cutf(filepath, line_no, start_pos, size);
        if(mode == 'b') cutb(filepath, line_no, start_pos, size);
        return 1;
    }
    return 0;
}

int gnc(char** command, char** next_command, char** remaining){
    *remaining = (char*) malloc(sizeof(*command));
    *next_command = (char*) malloc(sizeof(*command));
    char* a = strchr(*command, ' ');
    if(a == NULL) return 0;
    *remaining = a;
    strncpy(*next_command, *command, strlen(*command) - strlen(*remaining));
    (*remaining) ++;
    return 1;
}

int get_input(char* command, char** text, char* next_identifier, char*pre_identifier){
    int len;
    char *pre, *next;
    pre = (char*) malloc(sizeof(command));
    next = (char*) malloc(sizeof(command));
    pre = strstr(command, pre_identifier);
    if(pre == NULL) return 0;
    pre = pre + strlen(pre_identifier);
    next = strstr(command, next_identifier);
    if(next == NULL) return 0;
    if(next < pre) return 0;
    *text = (char*) malloc((strlen(pre) - strlen(next) - 1) * sizeof(char));
    strncpy(*text, pre, strlen(pre) - strlen(next));
    return 1;
}

int cat(char* filepath){
    filepath = path_validation(filepath);
    FILE* file = fopen(filepath, "r");
    if(file == NULL) {
        printf("File can't be opened\n");
        return 0;
    }
    char c = fgetc(file);
    while(c != EOF){
        printf("%c", c);
        c = fgetc(file);
    }
    fclose(file);
    return 1;
}

int insert(char *filepath, char *str, int line_no, int start_pos){
    string_validation(str);
    char* new_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter=0, index_counter = 0;
    filepath = path_validation(filepath);

    FILE *file = fopen(filepath, "r");
    if(file == NULL) {
        printf("The given file doesn't exist\n");
        return 0;
    }

    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no - 1)){
        new_content[strlen(new_content)] = c;
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    if(c == EOF && line_counter != (line_no - 1)){
        while(line_counter != (line_no)){
            new_content[strlen(new_content)] = '\n';
            line_counter++;
        }
    }
    while(c != EOF && index_counter != (start_pos)){
        new_content[strlen(new_content)] = c;
        index_counter++;
        c = fgetc(file);
    }
    if(c == EOF && index_counter != start_pos){
        while(index_counter != (start_pos)){
            new_content[strlen(new_content)] = ' ';
            index_counter++;
        }
    }
    for(int i=0; i<strlen(str); i++){
        new_content[strlen(new_content)] = str[i];
    }
    while(c != EOF){
        new_content[strlen(new_content)] = c;
        c = fgetc(file);
    }
    fclose(file);
    FILE *file1 = fopen(filepath, "w");
    fprintf(file1, "%s",new_content);
    fclose(file1);
    return 1;
}

void string_validation(char initial[]){
    int size = strlen(initial);
    if(initial[0] == '\"'){
        for(int i=0; i<size; i++) initial[i] = initial[i+1];
        initial[size - 2] = '\0';
        size -= 2;
    }
    for(int i=0; i<size - 1; i++){
        if(initial[i] == '\\' && initial[i+1] == 'n'){
            if(i == 0 || initial[i-1] != '\\'){
                for(int j=i; j<size; j++){
                    initial[j] = initial[j+1];
                }
                size--;
                initial[i] = '\n';
            }
        }
        else if(initial[i] == '\\' && initial[i+1] == '\"'){
            if(i == 0 || initial[i-1] != '\\'){
                for(int j=i; j<size; j++){
                    initial[j] = initial[j+1];
                }
                size--;
                initial[i] = '\"';
            }
        }
        else if(initial[i] == initial[i+1] && initial[i] == '\\'){
            for(int j=i; j<size; j++){
                initial[j] = initial[j+1];
            }
            size--;
        }
    }
}

int removef(char *filepath, int line_no, int start_pos, int size){
    filepath = path_validation(filepath);
    char* new_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = 0, del_counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL) {
        printf("The given file doesn't exist\n");
        return 0;
    }
    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no - 1)){
        new_content[strlen(new_content)] = c;
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    while(c != EOF && index_counter != (start_pos + 1)){
        new_content[strlen(new_content)] = c;
        index_counter++;
        c = fgetc(file);
    }
    while (del_counter < size){
        c = fgetc(file);
        del_counter++;
    }
    while(c != EOF){
        new_content[strlen(new_content)] = c;
        c = fgetc(file);
    }
    fclose(file);
    FILE *file1 = fopen(filepath, "w");
    fprintf(file1, "%s",new_content);
    fclose(file1);
    return 1;
}

int removeb(char *filepath, int line_no, int start_pos, int size){
    filepath = path_validation(filepath);
    char* new_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = 0, del_counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL) {
        printf("The given file doesn't exist\n");
        return 0;
    }
    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no - 1)){
        new_content[strlen(new_content)] = c;
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    while(c != EOF && index_counter != (start_pos)){
        new_content[strlen(new_content)] = c;
        index_counter++;
        c = fgetc(file);
    }
    while(del_counter < size){
        new_content[strlen(new_content) - 1] = '\0';
        del_counter ++;
    }
    printf("%s\n", new_content);
    while(c != EOF){
        new_content[strlen(new_content)] = c;
        c = fgetc(file);
    }
    fclose(file);
    FILE *file1 = fopen(filepath, "w");
    fprintf(file1, "%s",new_content);
    fclose(file1);
    return 1;
}

int find_first_index(char *filepath, char *str){
    filepath = path_validation(filepath);
    FILE *file = fopen(filepath, "r");
    if(file == NULL){
        printf("The given file doesn't exist\n");
        return -1;
    }
    char c = fgetc(file);
    char *content;
    content = (char*) malloc(sizeof(char) * MAX_FILE);
    while ((c != EOF)){
        content[strlen(content)] = c;
        c = fgetc(file);
    }
    char *loc_ptr = strstr(content, str);
    if(loc_ptr == NULL){
        printf("Expression not found\n");
        return -1;
    }
    int index = strlen(content) - strlen(loc_ptr);
    return index;
}

int find_count(char *filepath, char *str){
    filepath = path_validation(filepath);
    int counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL){
        printf("The given file doesn't exist\n");
        return -1;
    }
    char c = fgetc(file);
    char *content, *buff;
    buff = (char*) malloc(sizeof(str));
    content = (char*) malloc(sizeof(char) * MAX_FILE);
    while ((c != EOF)){
        content[strlen(content)] = c;
        c = fgetc(file);
    }
    char *loc_ptr = strstr(content, str);
    if(loc_ptr == NULL){
        printf("Expression not found\n");
        return -1;
    }
    while(loc_ptr != NULL){
        counter++;
        loc_ptr = strstr(loc_ptr + 1, str);
    }
    return counter;
}

int find_at(char *filepath, char *str, int n){
    filepath = path_validation(filepath);
    int counter = 1;
    FILE *file = fopen(filepath, "r");
    if(file == NULL){
        return -2;
    }
    char c = fgetc(file);
    char *content, *buff;
    buff = (char*) malloc(sizeof(str));
    content = (char*) malloc(sizeof(char) * MAX_FILE);
    while ((c != EOF)){
        content[strlen(content)] = c;
        c = fgetc(file);
    }
    char *loc_ptr = strstr(content, str);
    if(loc_ptr == NULL) return -1;
    while(loc_ptr != NULL && counter != n){
        counter++;
        loc_ptr = strstr(loc_ptr + 1, str);
    }
    if(counter != n) return -1;
    else{
        int index = strlen(content) - strlen(loc_ptr);
        return index;
    }
}

int auto_indent(char *filepath){
    filepath = path_validation(filepath);
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

char* tree(char* initialpath, int depth, int given_depth){
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

int copyb(char *filepath, int line_no, int start_pos, int size){
    char *new_content;
    char *copied_content;
    new_content = (char*) malloc(sizeof(char) * MAX_FILE);
    copied_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL) return 0;
    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no - 1)){
        new_content[strlen(new_content)] = c;
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    while(c != EOF && index_counter != (start_pos)){
        new_content[strlen(new_content)] = c;
        index_counter++;
        c = fgetc(file);
    }
    for(int i=0; i<size; i++){
        copied_content[size - 1 - i] = new_content[strlen(new_content) - 1 - i];
    }
    fclose(file);
    copied_content[strlen(copied_content)] = '\0';
    char command[MAX_FILE] = "echo ";
    strcat(command, copied_content);
    strcat(command, " | pbcopy");
    system(command);
    return 1;
}

int copyf(char *filepath, int line_no, int start_pos, int size){
    char *copied_content;
    copied_content = (char*) malloc(sizeof(char) * MAX_FILE);
    int line_counter = 0, index_counter = 0, copy_counter = 0;
    FILE *file = fopen(filepath, "r");
    if(file == NULL) return 0;
    char c = fgetc(file);
    while(c != EOF && line_counter != (line_no - 1)){
        if(c == '\n') line_counter ++;
        c = fgetc(file);
    }
    while(c != EOF && index_counter != (start_pos)){
        index_counter++;
        c = fgetc(file);
    }
    while (copy_counter < size){
        c = fgetc(file);
        copied_content[strlen(copied_content)] = c;
        copy_counter++;
    }
    fclose(file);
    copied_content[strlen(copied_content)] = '\0';
    char command[MAX_FILE] = "echo ";
    strcat(command, copied_content);
    strcat(command, " | pbcopy");
    system(command);
    return 1;
}

int cutf(char *filepath, int line_no, int start_pos, int size){
    copyf(filepath, line_no, start_pos, size);
    removef(filepath, line_no, start_pos, size);
    return 1;
}

int cutb(char *filepath, int line_no, int start_pos, int size){
    copyb(filepath, line_no, start_pos, size);
    removeb(filepath, line_no, start_pos, size);
    return 1;
}

