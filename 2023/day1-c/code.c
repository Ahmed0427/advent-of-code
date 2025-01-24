#include <stdio.h>
#include <ctype.h>  

#define MAX_LINE_LENGTH 128

const char* digits[] = {
    "one", "two", "three",
    "four", "five", "six",
    "seven", "eight", "nine"
};

int part2(char* line) {
    int firstDigit = -1, lastDigit = -1;

    for (int i = 0; line[i] != '\0'; i++) {
        if (isdigit(line[i])) {
            if (firstDigit == -1) {
                firstDigit = line[i] - '0';  
            }
            lastDigit = line[i] - '0';  
        }

        for(int j = 0; j < 9; j++) {

            int k = 0;
            while(line[i + k] != '\0' && digits[j][k] != '\0' && line[i + k] == digits[j][k]) {
                k++;
            }

            if(digits[j][k] == '\0') {
                if (firstDigit == -1) {
                    firstDigit = j + 1;
                }
                lastDigit = j + 1;
            }
        }
    }

    if (firstDigit != -1 && lastDigit != -1) {
        return firstDigit * 10 + lastDigit;
    }
    
    return 0;  
}

int part1(char* line) {
    int firstDigit = -1, lastDigit = -1;

    for (int i = 0; line[i] != '\0'; i++) {
        if (isdigit(line[i])) {
            if (firstDigit == -1) {
                firstDigit = line[i] - '0';  
            }
            lastDigit = line[i] - '0';  
        }
    }

    if (firstDigit != -1 && lastDigit != -1) {
        return firstDigit * 10 + lastDigit;
    }
    
    return 0;  
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

    char line[MAX_LINE_LENGTH];
    int ans1 = 0, ans2 = 0;

    while (fgets(line, sizeof(line), file)) {
        int res1 = part1(line); 
        int res2 = part2(line); 
        ans1 += res1, ans2 += res2;
    }

    printf("part1: %d\npart2: %d\n", ans1, ans2);

    fclose(file);
    return 0;
}
