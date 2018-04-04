//
// Created by zhangji on 4/4/18.
//

#pragma GCC optimize("-Ofast")

#include <malloc.h>

#define MAX_MAP_SIZE        64
#define ZONE_SIZE           4
#define MAX_SCAN_COUNT      100000

extern void randomscan(char zone[ZONE_SIZE][ZONE_SIZE]);

/////////
/////////
struct ScanArea {
    int y_lm;
    int x_lm;
    int is_filled;
    unsigned long long hash;
};

struct ReceiveArea {
    char area[ZONE_SIZE][ZONE_SIZE];
    unsigned long long hash;
    int is_used;
};

int process_scan_area(int chk_point_side, ScanArea **scanArea, char map[MAX_MAP_SIZE][MAX_MAP_SIZE],
                      ReceiveArea **revArea, int revPos);

ScanArea *createScanArea(int y_lm, int x_lm) {
    auto *new_area = (ScanArea *) malloc(sizeof(ScanArea));
    if (new_area != nullptr) {
        new_area->y_lm = y_lm;
        new_area->x_lm = x_lm;
        new_area->is_filled = 0;
        new_area->hash = 0;
    }
    return new_area;
}

ReceiveArea *createReveiveArea(char rand_map[ZONE_SIZE][ZONE_SIZE], unsigned long long hash) {
    auto *new_rev_area = (ReceiveArea *) malloc(sizeof(ReceiveArea));
    if (new_rev_area != nullptr) {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                new_rev_area->area[y][x] = rand_map[y][x];
            }
        }
        new_rev_area->is_used = 0;
        new_rev_area->hash = hash;
    }
    return new_rev_area;
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

void re_compute_scan_area(int chk_point_side, ScanArea *const *scanArea, char map[MAX_MAP_SIZE][MAX_MAP_SIZE]) {
    int kk = 0;
    for (int ii = 0; ii < chk_point_side; ii++) {
        for (int jj = 0; jj < chk_point_side; jj++) {
            int y_delta = ii * 2;
            int x_delta = jj * 2;
            scanArea[kk]->hash = hash_area(y_delta, x_delta, map);
            if (scanArea[kk]->hash != 0) {
                scanArea[kk]->is_filled = 1;
            }
            kk++;
        }
    }
}


void reconstruct(int N, char map[MAX_MAP_SIZE][MAX_MAP_SIZE]) {
    // init end
    char rand_map[ZONE_SIZE][ZONE_SIZE];
    int chk_point_side = N / 2 - 1;
    int chk_point_num = chk_point_side * chk_point_side;
    struct ScanArea *scanArea[chk_point_num];
    int k = 0;
    for (int i = 0; i < chk_point_side; i++) {
        for (int j = 0; j < chk_point_side; j++) {
            int y_delta = i * 2;
            int x_delta = j * 2;
            scanArea[k] = createScanArea(y_delta, x_delta);
            scanArea[k]->hash = hash_area(y_delta, x_delta, map);
            k++;
        }
    }
    ReceiveArea *revAreas[MAX_SCAN_COUNT];
    int revPos = 0;
    // process start
    int is_completed = 0;
    while (is_completed == 0) {
        if (is_completed == 0) {
            // scan ,add
            randomscan(rand_map);
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
                // add revieve area
                revAreas[revPos++] = createReveiveArea(rand_map, rand_map_hash);
                // match scan area
                int ret = 0;
                do {
                    ret = process_scan_area(chk_point_side, scanArea, map, revAreas, revPos);
                } while (ret == 1);
            }
        }
        // check completed
        for (int i = 0; i < chk_point_num; i++) {
            if (scanArea[i]->is_filled == 0) {
                is_completed = 0;
                break;
            } else {
                is_completed = 1;
            }
        }
    }

}

int process_scan_area(int chk_point_side, ScanArea **scanArea, char map[MAX_MAP_SIZE][MAX_MAP_SIZE],
                      ReceiveArea **revArea, int revPos) {

    for (int i = 0; i < chk_point_side * chk_point_side; i++) {
        if (scanArea[i]->is_filled == 0) {
            for (int pos = revPos - 1; pos >= 0; pos--) {
                if (revArea[pos]->is_used == 0) {
                    int is_match = match_area(scanArea[i], map, revArea[pos]->area);
                    if (is_match == 1) {
                        scanArea[i]->hash = revArea[pos]->hash;
                        scanArea[i]->is_filled = 1;
                        fill_back_area(scanArea[i], map, revArea[pos]->area);
                        re_compute_scan_area(chk_point_side, scanArea, map);
                        revArea[pos]->is_used = 1;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
