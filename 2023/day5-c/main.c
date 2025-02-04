#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

void get_seeds(char* line, int64_t* seeds, int64_t* seeds_count) {
    line = strchr(line, ':') + 2;  

    int64_t num = -1;
    while (*line != '\0') {
        if (isdigit(*line)) {
            if (num == -1) {
                num = (*line) - '0';
            }
            else {
                num = num * 10 + (*line) - '0';
            }
        } else {
            if (num != -1) {
                if (*seeds_count < 254) {  
                    seeds[(*seeds_count)++] = num;
                }
                num = -1;
            }
        }
        line++;
    }
}

void get_section(int64_t sec[50][3], FILE* file_d) {
    int64_t i = 0;
    char line[100];
    while (fgets(line, sizeof(line), file_d) && line[0] != '\n') {
        if (sscanf(line, "%ld %ld %ld", &sec[i][0], &sec[i][1], &sec[i][2]) == 3) {
            i++;
        }
    }
}

int64_t getDest(int64_t src, int64_t map[50][3]) {
    for (int64_t i = 0; i < 50; i++) {
        if (map[i][2] == 0) continue;
        if (map[i][1] > src) continue;
        if (map[i][1] + map[i][2] <= src) continue;
        return map[i][0] + (src - map[i][1]);
    }
    return src;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    FILE* file_d = fopen(argv[1], "r");

    if (!file_d) {
        perror("Couldn't open file");
        exit(1);
    }

    char line[256];
    fgets(line, sizeof(line), file_d);  

    int64_t seeds[20];
    int64_t seeds_count = 0;
    get_seeds(line, seeds, &seeds_count);

    fgets(line, sizeof(line), file_d);  

    int64_t seed_map[50][3];
    int64_t soil_map[50][3];
    int64_t fert_map[50][3];
    int64_t water_map[50][3];
    int64_t light_map[50][3];
    int64_t temp_map[50][3];
    int64_t humid_map[50][3];

    
    memset(seed_map, 0, sizeof(seed_map));
    memset(soil_map, 0, sizeof(soil_map));
    memset(fert_map, 0, sizeof(fert_map));
    memset(water_map, 0, sizeof(water_map));
    memset(light_map, 0, sizeof(light_map));
    memset(temp_map, 0, sizeof(temp_map));
    memset(humid_map, 0, sizeof(humid_map));

    
    while (fgets(line, sizeof(line), file_d)) {
        switch (line[0]) {
            case 's':
                switch (line[1]) {
                    case 'e': get_section(seed_map, file_d); break;
                    case 'o': get_section(soil_map, file_d); break;
                }
                break;

            case 'f': get_section(fert_map, file_d); break;
            case 'w': get_section(water_map, file_d); break;
            case 'l': get_section(light_map, file_d); break;
            case 't': get_section(temp_map, file_d); break;
            case 'h': get_section(humid_map, file_d); break;
        }
    }

    int64_t ans1 = 2e9;
    for (int64_t i = 0; i < seeds_count; i++) {
        int64_t seed_dist = getDest(seeds[i], seed_map);
        int64_t soil_dist = getDest(seed_dist, soil_map);
        int64_t fert_dist = getDest(soil_dist, fert_map);
        int64_t water_dist = getDest(fert_dist, water_map);
        int64_t light_dist = getDest(water_dist, light_map);
        int64_t temp_dist = getDest(light_dist, temp_map);
        int64_t humid_dist = getDest(temp_dist, humid_map);
        ans1 = (ans1 < humid_dist) ? ans1 : humid_dist;
    }

    printf("ans1: %ld\n", ans1);

    fclose(file_d);
    return 0;
}
