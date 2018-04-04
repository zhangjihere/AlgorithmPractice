//
// Created by zhangji on 4/4/18.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MAX_MAP_SIZE        64
#define ZONE_SIZE           4

#define MAX_SCAN_COUNT      100000

extern void reconstruct(int N, char map[MAX_MAP_SIZE][MAX_MAP_SIZE]);

static char map[MAX_MAP_SIZE][MAX_MAP_SIZE];
static int N;
static int seed;
static int scancount;

static int pseudo_rand() {
    seed = seed * 214013 + 2531011;
    return (seed >> 16) & 0x7fff;
}

void randomscan(char zone[ZONE_SIZE][ZONE_SIZE]) {
    if (scancount >= MAX_SCAN_COUNT)
        return;

    int x0 = (pseudo_rand() % ((N - ZONE_SIZE) / 2 + 1)) * 2;  // x0 = 0, 2, 4, ..., N - 4
    int y0 = (pseudo_rand() % ((N - ZONE_SIZE) / 2 + 1)) * 2;  // y0 = 0, 2, 4, ..., N - 4

    for (int y = 0; y < ZONE_SIZE; ++y)
        for (int x = 0; x < ZONE_SIZE; ++x)
            zone[y][x] = map[y + y0][x + x0];

    ++scancount;
}

static void makemap() {
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x)
            map[y][x] = pseudo_rand() % 15 + 1;
}

static bool compare(char map1[MAX_MAP_SIZE][MAX_MAP_SIZE], char map2[MAX_MAP_SIZE][MAX_MAP_SIZE]) {
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x)
            if (map1[y][x] != map2[y][x])
                return false;

    return true;
}

int main() {
    int TC, totalscore, totalscan;

    freopen("sample_input.txt", "r", stdin);

    setbuf(stdout, NULL);
    scanf("%d", &TC);

    totalscore = totalscan = 0;
    for (int testcase = 1; testcase <= TC; ++testcase) {
        char guess[MAX_MAP_SIZE][MAX_MAP_SIZE];
        int min_scan_count;

        scanf("%d %d %d", &N, &seed, &min_scan_count);

        makemap();

        scancount = 0;

        for (int y = 0; y < N; ++y)
            for (int x = 0; x < N; ++x)
                guess[y][x] = 0;

        for (int k = 0; k < N; ++k) {
            guess[k][0] = map[k][0];
            guess[0][k] = map[0][k];
            guess[k][1] = map[k][1];
            guess[1][k] = map[1][k];
            guess[k][N - 1] = map[k][N - 1];
            guess[N - 1][k] = map[N - 1][k];
            guess[k][N - 2] = map[k][N - 2];
            guess[N - 2][k] = map[N - 2][k];
        }

        reconstruct(N, guess);

        if (!compare(map, guess))
            scancount = MAX_SCAN_COUNT;
        totalscan += scancount;

        int score;
        if (TC == 5) { // sample testcase
            score = scancount < MAX_SCAN_COUNT ? 100 : 0;
            totalscore += score;
        } else { // eval testcase
            score = scancount <= min_scan_count + (min_scan_count / 2) ? 100 : 0;
            totalscore += score;
        }

        printf("#%d %d\n", testcase, score);
    }

    if (totalscan > MAX_SCAN_COUNT) totalscan = MAX_SCAN_COUNT;
    printf("total score = %d, total scan = %d\n", totalscore / TC, totalscan);
    return 0;
}