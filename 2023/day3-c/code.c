#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN_MAX 256
#define LINES_CNT_MAX 256

char grid[LINES_CNT_MAX][LINE_LEN_MAX];
long long gearsValue[LINES_CNT_MAX * LINE_LEN_MAX][2];

int isDigit(char c) {
    return (c >= '0' && c <= '9');
}

int isValidPosition(int x, int y) {
    int directions[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},    
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}  
    };

    int valid = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];

        if (nx < 0 || ny < 0 || nx >= LINES_CNT_MAX || ny >= LINE_LEN_MAX) {
            continue;
        }

        if (!isDigit(grid[nx][ny]) && grid[nx][ny] != '.' && grid[nx][ny] != '\n') {
            valid = 1;
        }
    }

    return valid;
}

int isNearStar(int x, int y) {
    int directions[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},    
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}  
    };

    int valid = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];

        if (nx < 0 || ny < 0 || nx >= LINES_CNT_MAX || ny >= LINE_LEN_MAX) {
            continue;
        }

        if (grid[nx][ny] == '*') {
            return (nx * LINE_LEN_MAX) + ny;
        }
    }

    return -1;
}

long long sumValidNumbers() {
    int rowCount = LINES_CNT_MAX;
    int colCount = LINE_LEN_MAX;

    long long sum = 0;
    for (int i = 0; i < rowCount; i++) {
        int num = 0, valid = 0, gearPos = -1;
        for (int j = 0; j < colCount; j++) {
            if (isDigit(grid[i][j])) {
                num = num * 10 + (grid[i][j] - '0');
                if (isValidPosition(i, j)) valid = 1;
                gearPos = gearPos == -1 ? isNearStar(i, j) : gearPos;
            }
            else {
                if (valid) {
                    sum += num;  
                }
                if (gearPos != -1) {
                    if (gearsValue[gearPos][0] == 0) gearsValue[gearPos][0] = 1;
                    gearsValue[gearPos][0] *= num;
                    gearsValue[gearPos][1]++;
                }
                valid = 0;  
                gearPos = -1;
                num = 0;   
            }
        }
    }

    return sum;
}

long long part1() {
    return sumValidNumbers();
}

long long part2() {
    long long sum = 0;
    int n = LINES_CNT_MAX * LINE_LEN_MAX;
    for (int i = 0; i < n; i++) {
        if (gearsValue[i][1] == 2) {
            sum += gearsValue[i][0];
        }
    }
    return sum;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: program <filepath>\n");
        exit(1);
    }

     FILE *file = fopen(argv[1], "r");

     if (file == NULL) {
         perror("Couldn't open the file");
         exit(1);
     }

     int lineCnt = 0;
     char line[LINE_LEN_MAX];

     memset(grid, '.', sizeof(grid));
     memset(gearsValue, 0, sizeof(gearsValue));

     while (fgets(line, sizeof(line), file)) {
        strcpy(grid[lineCnt++], line);
     }

     long long ans1 = part1();
     long long ans2 = part2();
     printf("Part 1: %lld\n", ans1);
     printf("Part 2: %lld\n", ans2);

     fclose(file);
     return 0;
}
