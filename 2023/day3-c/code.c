#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN_MAX 256
#define LINES_CNT_MAX 256

int isDigit(char c) {
    return (c >= '0' && c <= '9');
}

int isValidPosition(int x, int y, char grid[LINES_CNT_MAX][LINE_LEN_MAX]) {
    int directions[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},    
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}  
    };

    for (int i = 0; i < 8; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];

        if (nx < 0 || ny < 0 || nx >= LINES_CNT_MAX || ny >= LINE_LEN_MAX) {
            continue;
        }

        if (!isDigit(grid[nx][ny]) && grid[nx][ny] != '.' && grid[nx][ny] != '\n') {
            return 1;
        }
    }

    return 0;
}

int sumValidNumbers(char grid[LINES_CNT_MAX][LINE_LEN_MAX]) {
    int rowCount = LINES_CNT_MAX;
    int colCount = LINE_LEN_MAX;

    int sum = 0;
    for (int i = 0; i < rowCount; i++) {
        int num = 0, valid = 0;
        for (int j = 0; j < colCount; j++) {
            if (isDigit(grid[i][j])) {
                num = num * 10 + (grid[i][j] - '0');
                if (isValidPosition(i, j, grid)) {
                    valid = 1;
                }
            }
            else {
                if (valid) {
                    sum += num;  
                }
                valid = 0;  
                num = 0;   
            }
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
     char grid[LINES_CNT_MAX][LINE_LEN_MAX];

     memset(grid, '.', sizeof(grid));

     while (fgets(line, sizeof(line), file)) {
        strcpy(grid[lineCnt++], line);
     }

     long long result = sumValidNumbers(grid);
     printf("Part 1 result: %lld\n", result);

     fclose(file);
     return 0;
}
