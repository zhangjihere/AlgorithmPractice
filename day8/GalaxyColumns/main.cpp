#define _CRT_SECURE_NO_WARNINGS    // Ignore build errors when using unsafe functions in Visual Studio.

#include <stdio.h>

#define BOARD_MAX_WIDTH         10
#define BLOCK_SIZE              3
#define SPECIAL_BLOCK           9
#define CMD_CIRCULATE           100
#define CMD_MOVE                200

extern void init(int width);

extern void newBlock(int block[BLOCK_SIZE]);

extern void specialBlock(int type);

extern void circulate(int count);

extern void move(int distance);

extern int land();

int main(void) {
    setbuf(stdout, NULL);

    freopen("sample_input.txt", "r", stdin);
    //freopen("my_input.txt", "r", stdin);

    int testCnt;
    int totalScore = 0;
    int block[BLOCK_SIZE];

    scanf("%d", &testCnt);
    for (int tc = 1; tc <= testCnt; ++tc) {
        int boardWidth, blockCnt;
        scanf("%d %d", &boardWidth, &blockCnt);

        int score = 100;

        init(boardWidth);

        for (register int i = 0; i < blockCnt; ++i) {
            scanf("%d %d %d", &block[0], &block[1], &block[2]);

            if (block[0] == SPECIAL_BLOCK)
                specialBlock(block[2]);
            else
                newBlock(block);

            int cmdCnt, cmd, option;
            scanf("%d", &cmdCnt);
            for (register int j = 0; j < cmdCnt; ++j) {
                scanf("%d %d", &cmd, &option);

                switch (cmd) {
                    case CMD_CIRCULATE:
                        circulate(option);
                        break;
                    case CMD_MOVE:
                        move(option);
                        break;
                    default:
                        printf("cmd error\n");
                        break;
                }
            }

            int answer, result;
            scanf("%d", &answer);
            result = land();
            if (answer != result) {
                score = 0; // wrong answer
            }
        }

        printf("#%d %d\n", tc, score);
        totalScore += score;
    }

    printf("Total Score : %d\n", totalScore / testCnt);

    return 0;
}