#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256

void printStrList(char** list, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s ", list[i]);
    }
    printf("\n");
}

void freeStrList(char** list, int size) {
    for (int i = 0; i < size; i++) {
        free(list[i]);
    }
    free(list);
}

char** split(char* src_str, char* del, int* size) {
    char* str = strdup(src_str); 
    int capacity = 2, count = 0;
    char** result = malloc(sizeof(char*) * capacity); 

    if (!result) {
        perror("Memory allocation failed");
        freeStrList(result, count);
        free(str);
        exit(1);
    }
    
    char* save_ptr = str;
    char* token = strtok_r(str, del, &save_ptr);

    while (token != NULL) {
        if (count >= capacity) {
            capacity *= 2;
            result = realloc(result, sizeof(char*) * capacity);

            if (!result) {
                perror("Memory reallocation failed");
                freeStrList(result, count);
                free(str);
                exit(1);
            }
        }

        result[count++] = strdup(token);

        if (!result[count - 1]) {
            perror("strdup failed");
            freeStrList(result, count);
            free(str);
            exit(1);
        }
            
        token = strtok_r(NULL, del, &save_ptr);
    }
    
    free(str);
    *size = count;
    return result;
}

int stoi(char* str) {
    int res = 0;
    int len = (int)strlen(str);
    for (int i = 0; i < len; i++) {
        res = res * 10 + (str[i] - '0');
    }
    return res;
}

int calc_line_worth(char* line) {
    char* dup_line = strdup(line);
    
    char* nums_str = dup_line + strcspn(dup_line, ":") + 2;

    int groups_size = 0;
    char** groups_list = split(nums_str, "|", &groups_size); 

    int my_nums_size = 0;
    char** my_nums_list = split(groups_list[0], " ", &my_nums_size);

    int win_nums_size = 0;
    char** win_nums_list = split(groups_list[1], " ", &win_nums_size);

    int result = 0;
    for (int i = 0; i < my_nums_size; i++) {
        for (int j = 0; j < win_nums_size; j++) {
            int my_num = stoi(my_nums_list[i]);
            int win_num = stoi(win_nums_list[j]);

            if (my_num != win_num) continue; 
            result = !result ? 1 : result * 2;
        }
    }

    freeStrList(win_nums_list, win_nums_size);
    freeStrList(my_nums_list, my_nums_size);
    freeStrList(groups_list, groups_size);
    free(dup_line);

    return result;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>", argv[0]);
        exit(1);
    }

    FILE* file_d = fopen(argv[1], "r");

    if (!file_d) {
        perror("couldn't open file");
        exit(1);
    }
    
    int ans = 0;
    char line[MAX_LENGTH];
    while (fgets(line, sizeof(line), file_d)) {
        line[strcspn(line, "\n")] = '\0';
        ans += calc_line_worth(line);
    }

    printf("part1: %d\n", ans);
}
