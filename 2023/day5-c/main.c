#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_SEEDS 32 
#define MAX_LINE_LENGTH 256
#define MAX_MAP_ENTRIES 64 

#define max(a, b) (((a) > (b)) ? a : b)
#define min(a, b) (((a) < (b)) ? a : b)

typedef struct {
    int64_t dest_start;
    int64_t src_start;
    int64_t range_length;
} MapEntry;

typedef struct {
    MapEntry entries[MAX_MAP_ENTRIES];
    int size;
} Map;

typedef struct {
    int64_t start;
    int64_t end;
} Interval;

typedef struct {
    Interval* list;
    int capacity;
    int size;
} IntervalList;

void get_seeds(char* line, int64_t* seeds, int64_t* seeds_count) {
    line = strchr(line, ':') + 2;  

    int64_t num = -1;
    while (*line != '\0') {
        if (isdigit(*line)) {
            if (num == -1) {
                num = (*line) - '0';
            } else {
                num = num * 10 + (*line) - '0';
            }
        } else {
            if (num != -1) {
                if (*seeds_count < MAX_SEEDS) {  
                    seeds[(*seeds_count)++] = num;
                }
                num = -1;
            }
        }
        line++;
    }
    if (num != -1) {
        if (*seeds_count < MAX_SEEDS) {  
            seeds[(*seeds_count)++] = num;
        }
    }
}

void parse_map(FILE* file, Map* map) {
    map->size = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) && line[0] != '\n') {
        MapEntry* entry = &map->entries[map->size];
        if (sscanf(line, "%ld %ld %ld", &entry->dest_start,
        &entry->src_start, &entry->range_length) == 3) {
            map->size++;
        }
    }
}

int64_t map_value(int64_t src, Map* map) {
    for (int i = 0; i < map->size; i++) {
        MapEntry* entry = &map->entries[i];
        if (src >= entry->src_start && src < entry->src_start + entry->range_length) {
            return entry->dest_start + (src - entry->src_start);
        }
    }
    return src;
}

int64_t solve_part1(int64_t seeds[MAX_SEEDS], int seeds_count, Map maps[7]) {
    int64_t result = INT64_MAX;
    for (int i = 0; i < seeds_count; i++) {
        int64_t location = seeds[i]; 
        for (int j = 0; j < 7; j++) {
            location = map_value(location, &maps[j]);
        }
        if (result > location) result = location;
    }
    return result;
}

IntervalList make_intervals(int64_t seeds[MAX_SEEDS], int seeds_count) {
    IntervalList result = {NULL, 0, 0};
    for (int i = 0; i < seeds_count; i += 2) {
        Interval interval;
        interval.start = seeds[i];
        interval.end = seeds[i] + seeds[i + 1] - 1;
        if (result.size >= result.capacity) {
            result.capacity = (result.capacity) ? result.capacity * 2 : 4; 
            result.list = realloc(result.list, result.capacity * sizeof(Interval));
        }  
        result.list[result.size++] = interval;
    }
    return result;
}

IntervalList update_intervals(IntervalList* input, Map map) {
    IntervalList current = *input;
    IntervalList result = {NULL, 0, 0};

    for (int j = 0; j < map.size; j++) {
        MapEntry* entry = &map.entries[j];
        int64_t src_start = entry->src_start;
        int64_t src_end = src_start + entry->range_length - 1;
        int64_t dest_start = entry->dest_start;

        IntervalList new_current = {NULL, 0, 0};
        for (int i = 0; i < current.size; i++) {
            Interval curr = current.list[i];

            if (curr.end < src_start || curr.start > src_end) {
                if (new_current.size >= new_current.capacity) {
                    new_current.capacity = new_current.capacity ? new_current.capacity * 2 : 4;
                    new_current.list = realloc(new_current.list, new_current.capacity * sizeof(Interval));
                }
                new_current.list[new_current.size++] = curr;
                continue;
            }

            int64_t overlap_start = max(curr.start, src_start);
            int64_t overlap_end = min(curr.end, src_end);

            Interval mapped = {
                dest_start + (overlap_start - src_start),
                dest_start + (overlap_end - src_start)
            };
            if (result.size >= result.capacity) {
                result.capacity = result.capacity ? result.capacity * 2 : 4;
                result.list = realloc(result.list, result.capacity * sizeof(Interval));
            }
            result.list[result.size++] = mapped;

            if (curr.start < overlap_start) {
                Interval pre = {curr.start, overlap_start - 1};
                if (new_current.size >= new_current.capacity) {
                    new_current.capacity = new_current.capacity ? new_current.capacity * 2 : 4;
                    new_current.list = realloc(new_current.list, new_current.capacity * sizeof(Interval));
                }
                new_current.list[new_current.size++] = pre;
            }

            if (curr.end > overlap_end) {
                Interval post = {overlap_end + 1, curr.end};
                if (new_current.size >= new_current.capacity) {
                    new_current.capacity = new_current.capacity ? new_current.capacity * 2 : 4;
                    new_current.list = realloc(new_current.list, new_current.capacity * sizeof(Interval));
                }
                new_current.list[new_current.size++] = post;
            }
        }

        free(current.list);
        current = new_current;
    }

    for (int i = 0; i < current.size; i++) {
        if (result.size >= result.capacity) {
            result.capacity = result.capacity ? result.capacity * 2 : 4;
            result.list = realloc(result.list, result.capacity * sizeof(Interval));
        }
        result.list[result.size++] = current.list[i];
    }

    free(current.list);
    return result;
}

int64_t solve_part2(int64_t seeds[MAX_SEEDS], int seeds_count, Map maps[7]) {
    IntervalList intervals = make_intervals(seeds, seeds_count);

    for (int i = 0; i < 7; i++) {
        IntervalList updated = update_intervals(&intervals, maps[i]);
        intervals = updated;
    }

    int64_t result = INT64_MAX;
    for (int i = 0; i < intervals.size; i++) {
        if (intervals.list[i].start < result) {
            result = intervals.list[i].start;
        }
    }

    free(intervals.list);
    return result;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    FILE* file = fopen(argv[1], "r");

    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int64_t seeds[MAX_SEEDS];
    int64_t seeds_count = 0;

    fgets(line, sizeof(line), file);
    get_seeds(line, seeds, &seeds_count);
    fgets(line, sizeof(line), file);

    Map maps[7] = {0};
    const char* map_headers[] = {
        "seed-to-soil", 
        "soil-to-fertilizer", 
        "fertilizer-to-water", 
        "water-to-light", 
        "light-to-temperature", 
        "temperature-to-humidity", 
        "humidity-to-location"
    };

    for (int i = 0; i < 7; i++) {
        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, map_headers[i])) {
                parse_map(file, &maps[i]);
                break;
            }
        }
    }

    int64_t ans1 = solve_part1(seeds, seeds_count, maps);
    int64_t ans2 = solve_part2(seeds, seeds_count, maps);

    printf("part 1: %ld\n", ans1);
    printf("part 2: %ld\n", ans2);

    fclose(file);
    return 0;
}
