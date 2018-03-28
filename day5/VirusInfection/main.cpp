//
// Created by zhangji on 3/16/18.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

using namespace std;
typedef enum {
    CMD_ADD = 1,
    CMD_MOVE,
    CMD_INFECT,
    CMD_RECOVER,
    CMD_REMOVE
} COMMAND;

typedef enum {
    TITLE,
    LOCATION,
    START_TIME,
    END_TIME,
} FIELD;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

extern void init(void);

extern int add(int id, int pid, int fileSize);

extern int move(int id, int pid);

extern int infect(int id);

extern int recover(int id);

extern int remove(int id);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int run() {
    int N, score = 0;
    scanf("%d", &N);
    for (register int i = 0; i < N; ++i) {
        int cmd, ret, id, pid, size;
        scanf("%d", &cmd);
        switch (cmd) {
            case CMD_ADD:
                scanf("%d%d%d", &id, &pid, &size);
                ret = add(id, pid, size);
                break;
            case CMD_MOVE:
                scanf("%d%d", &id, &pid);
                ret = move(id, pid);
                break;
            case CMD_INFECT:
                scanf("%d", &id);
                ret = infect(id);
                break;
            case CMD_RECOVER:
                scanf("%d", &id);
                ret = recover(id);
                break;
            case CMD_REMOVE:
                scanf("%d", &id);
                ret = remove(id);
                break;
        }
        int checksum;
        scanf("%d", &checksum);
        if (ret == checksum) {
            ++score;
        } else {
            printf("r");
        }
    }
    return score;
}

int main() {
    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int T;
    scanf("%d", &T);
    int TotalScore = 0;
    for (int tc = 1; tc <= T; tc++) {
        init();
        int score = run();
        printf("#%d %d\n", tc, score);
        TotalScore += score;
    }
    printf("TotalScore = %d\n", TotalScore);
    return 0;
}