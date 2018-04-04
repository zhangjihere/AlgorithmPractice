//
// Created by zhangji on 4/4/18.
//

//#include <stdio.h>
#include <malloc.h>

#define MAX_MAP_SIZE        64
#define ZONE_SIZE           4

extern void randomscan(char zone[ZONE_SIZE][ZONE_SIZE]);

/////////
/////////
void print_map(char map[MAX_MAP_SIZE][MAX_MAP_SIZE]);

void print_rand_map(char map[ZONE_SIZE][ZONE_SIZE]);

int test_case;
int map_n;

struct ScanArea {
    int y_lm;
    int x_lm;
    int is_filled;
    unsigned long long hash;
};

ScanArea *createNewStat(int y_lm, int x_lm) {
    auto *new_area = (ScanArea *) malloc(sizeof(ScanArea));
    if (new_area != nullptr) {
        new_area->y_lm = y_lm;
        new_area->x_lm = x_lm;
        new_area->is_filled = 0;
        new_area->hash = 0;
    }
    return new_area;
}

unsigned long long hash_area(int y_a, int x_a, char map[MAX_MAP_SIZE][MAX_MAP_SIZE]) {
    unsigned long long rst = 1;
    for (int y = y_a; y < y_a + 4; y++) {
        for (int x = x_a; x < x_a + 4; x++) {
            rst = rst * map[y][x];
            if (rst == 0) {
                return rst;
            }
        }
    }
    return rst;
}

unsigned long long hash_rand_map(char map[ZONE_SIZE][ZONE_SIZE]) {
    unsigned long long rst = 1;
    for (int y = 0; y < 0 + 4; y++) {
        for (int x = 0; x < 0 + 4; x++) {
            rst = rst * map[y][x];
        }
    }
    return rst;
}

int match_area(ScanArea *scan_area, char map[MAX_MAP_SIZE][MAX_MAP_SIZE], char rand_map[ZONE_SIZE][ZONE_SIZE]) {
    int y = scan_area->y_lm;
    int x = scan_area->x_lm;
    int equal_num = 0;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            char map_char = map[y + j][x + i];
            char rand_m_char = rand_map[j][i];
            if (map_char == 0) {
                continue;
            } else {
                if (map_char == rand_m_char) {
                    equal_num++;
                } else {
                    return 0;
                }
            }
        }
    }
    if (equal_num >= 8) {
        return 1;
    } else {
        return 0;
    }
}

void fill_back_area(ScanArea *scan_area, char map[MAX_MAP_SIZE][MAX_MAP_SIZE], char rand_map[ZONE_SIZE][ZONE_SIZE]) {
    int y = scan_area->y_lm;
    int x = scan_area->x_lm;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            map[y + j][x + i] = rand_map[j][i];
        }
    }
}

void reconstruct(int N, char map[MAX_MAP_SIZE][MAX_MAP_SIZE]) {
    test_case++;
    map_n = N;
//    print_map(map);
    char rand_map[ZONE_SIZE][ZONE_SIZE];
    int chk_point_side = N / 2 - 1;
    int chk_point_num = chk_point_side * chk_point_side;
    struct ScanArea *scanArea[chk_point_num];
    int k = 0;
    for (int i = 0; i < chk_point_side; i++) {
        for (int j = 0; j < chk_point_side; j++) {
            int y_delta = i * 2;
            int x_delta = j * 2;
            scanArea[k] = createNewStat(y_delta, x_delta);
            scanArea[k]->hash = hash_area(y_delta, x_delta, map);
            k++;
        }
    }
    // init end
    // process start

    int is_completed = 0;

    while (is_completed == 0) {
        // check completed
        for (int i = 0; i < chk_point_num; i++) {
            if (scanArea[i]->is_filled == 0) {
                is_completed = 0;
                break;
            } else {
                is_completed = 1;
            }
        }
        if (is_completed == 0) {
            // scan
            randomscan(rand_map);
//            print_rand_map(rand_map);
            unsigned long long rand_map_hash = hash_rand_map(rand_map);
            // check if scaned or not
            int is_scaned = 0;
            for (int i = 0; i < chk_point_num; i++) {
                if (scanArea[i]->hash == rand_map_hash) {
                    is_scaned = 1;
                    break;
                }
            }
            if (is_scaned == 1) {
                continue;
            } else {
                // match scan area
                for (int i = 0; i < chk_point_num; i++) {
                    if (scanArea[i]->is_filled == 0) {
                        int is_match = match_area(scanArea[i], map, rand_map);
                        if (is_match == 1) {
                            scanArea[i]->hash = rand_map_hash;
                            scanArea[i]->is_filled = 1;
                            fill_back_area(scanArea[i], map, rand_map);
                            break;
                        } else {
                            continue;
                        }
                    }
                }
            }
        }

    }


}


void print_map(char map[MAX_MAP_SIZE][MAX_MAP_SIZE]) {
    for (int y = 0; y < map_n; ++y) {
        for (int x = 0; x < map_n; ++x) {
            printf("%d\t", map[y][x]);
        }
        printf("\n");
    }
    printf("== tc: %d == map_n: %d =====================================\n", test_case, map_n);
}

void print_rand_map(char map[ZONE_SIZE][ZONE_SIZE]) {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            printf("%d\t", map[y][x]);
        }
        printf("\n");
    }
    printf("---------------------------------\n");
}