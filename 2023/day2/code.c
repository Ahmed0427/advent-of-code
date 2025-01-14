#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LEN 200
#define COLOR_LEN 10

const int maxRedCnt = 12;
const int maxGreenCnt = 13;
const int maxBlueCnt = 14;

int redCnt = 0, greenCnt = 0, blueCnt = 0;

int parseLine(const char *gameStr) {
    char str[LINE_LEN];
    strcpy(str, gameStr);

    char *start = strchr(str, ':');
    if (start != NULL) {
        start++; 
    }

    while (*start == ' ') {
        start++; 
    }

    char *savePtr1 = NULL;
    char *savePtr2 = NULL;

    char *section = strtok_r(start, ";", &savePtr1);
    while (section != NULL) {
        while (*section == ' ') {
            section++; 
        }

        char *pair = strtok_r(section, ",", &savePtr2);
        while (pair != NULL) {
            while (*pair == ' ') {
                pair++; 
            }

            int cnt;
            char color[COLOR_LEN]; 

            sscanf(pair, "%d %s", &cnt, color);

            if (strncmp(color, "red", 3) == 0) {
                redCnt = (redCnt < cnt) ? cnt : redCnt;            
            }
            if (strncmp(color, "green", 5) == 0) {
                greenCnt = (greenCnt < cnt) ? cnt : greenCnt;            
            }
            if (strncmp(color, "blue", 4) == 0) {
                blueCnt = (blueCnt < cnt) ? cnt : blueCnt;            
            }

            pair = strtok_r(NULL, ",", &savePtr2);
        }


        section = strtok_r(NULL, ";", &savePtr1);
    }

    if (redCnt > maxRedCnt) return 0;
    if (greenCnt > maxGreenCnt) return 0;
    if (blueCnt > maxBlueCnt) return 0;

    return 1; 
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("Unable to open the file\n");
        return 1;
    }

    char line[LINE_LEN];
    int ans1 = 0, ans2 = 0, game = 1;
    while (fgets(line, sizeof(line), file)) {
        redCnt = 0, greenCnt = 0, blueCnt = 0;
        int valid = parseLine(line);

        if (valid == 1) ans1 += game;
        ans2 += redCnt * greenCnt * blueCnt;

        game++;
    }

    printf("part1: %d\n", ans1);
    printf("part2: %d\n", ans2);

    fclose(file); 
    return 0;
}
