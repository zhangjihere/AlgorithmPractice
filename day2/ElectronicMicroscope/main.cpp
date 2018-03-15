//
// Created by zhangji on 3/14/18.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS    // Ignore build errors when using unsafe functions in Visual Studio.
#endif

#include <stdio.h>

enum {
    MAX_N = 100,
    MAX_DATA = (MAX_N + 1) * (MAX_N + 1)
};

typedef struct {
    int x;
    int y;
} Cell;

typedef struct {
    int count;
    Cell cell[MAX_N];
} RESULT;

typedef enum {
    CMD_INFECT, // 0
    CMD_RECOVER // 1
} COMMAND;

extern void init(int cell[MAX_N][MAX_N], int N);

extern RESULT infect();

extern RESULT recover();

static int score, numObserve, N;
static int cell[MAX_N][MAX_N], key[MAX_N][MAX_N];
static int data[MAX_DATA];

static int isIn(int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; }

/*
* If 0 < size, this function returns the number of infected cells in square region from (top, left) to (top + size - 1, left + size - 1).
* Otherwise, it returns 0.
*/
int observe(int top, int left, int size) {
    int sum = 0;
    if (0 < size && top < N && left < N) {
        int i, y, x, sign;
        int right = left < N - size ? left + size - 1 : N - 1;
        int bottom = top < N - size ? top + size - 1 : N - 1;
        for (i = 0; i < 4; ++i) {
            sign = (i == 0 || i == 3) ? 1 : -1;
            y = (i & 1) ? bottom + 1 : top;
            while (y) {
                x = (i & 2) ? right + 1 : left;
                while (x) {
                    sum += sign * data[y * (N + 1) + x];
                    x -= x & -x;
                }
                y -= y & -y;
            }
        }
    }
    ++numObserve;
    return sum;
}

/* This function updates data for observe API function. */
static void updateData(int y, int x, int s) {
    ++y;
    while (y <= N) {
        int t = x + 1;
        while (t <= N) {
            data[y * (N + 1) + t] += s;
            t += t & -t;
        }
        y += y & -y;
    }
}

static void run() {
    int numCmd, answer = 0, i, j, cmd;

    /* Initialize */
    scanf("%d", &N);// 1 5 10 20 100
    for (i = 0; i < (N + 1) * (N + 1); ++i)
        data[i] = 0;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            scanf("%d", cell[i] + j);
            updateData(i, j, cell[i][j]);
        }
    }
    score = numObserve = 0;
    init(cell, N);

    scanf("%d", &numCmd);// 3 3 5 30 100
    for (cmd = 0; cmd < numCmd; ++cmd) {
        int numChanged, X[MAX_N], Y[MAX_N];
        COMMAND cmdType;
        RESULT result;

        /* Get Input */
        scanf("%d %d", &cmdType, &numChanged);
        for (i = 0; i < numChanged; ++i) {
            scanf("%d %d", X + i, Y + i);
            key[Y[i]][X[i]] = 1;
            updateData(Y[i], X[i], cmdType == CMD_INFECT ? 1 : -1);
        }

        /* Execute User Function */
        if (cmdType == CMD_INFECT)
            result = infect();
        else if (cmdType == CMD_RECOVER)
            result = recover();

        /* Check the Answer */
        if (result.count == numChanged) {
            for (i = 0; i < numChanged; ++i) {
                if (isIn(result.cell[i].x, result.cell[i].y) && key[result.cell[i].y][result.cell[i].x])
                    key[result.cell[i].y][result.cell[i].x] = 0;
                else
                    break;
            }
            if (i == numChanged)
                ++answer;
        }

        /* Clear */
        for (i = 0; i < numChanged; ++i)
            key[Y[i]][X[i]] = 0;
    }

    int cutline;
    scanf("%d", &cutline);

    if (answer == numCmd) {
        if (numObserve <= cutline)
            score = 100;
        else
            score = 100 * cutline / numObserve;
    }

    printf("my: %d target: %d\n", numObserve, cutline);
}

int main() {
    int T, testCase, totalScore = 0;

    freopen("sample_input.txt", "r", stdin);
//    freopen("my_input.txt", "r", stdin);
    setbuf(stdout, NULL);
    scanf("%d", &T);
    for (testCase = 1; testCase <= T; ++testCase) {
        run();
        totalScore += score;
        printf("#%d %d\n", testCase, score);
    }
    printf("Total Score = %d\n", totalScore / T);
    return 0;
}