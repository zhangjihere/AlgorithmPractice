//
// Created by zhangji on 3/20/18.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define PARSE              100
#define SET                200
#define ADD                300
#define ERASE              400
#define GENERATE           500

#define THRESHOLD          0x07FFFFFF

#define MAX_KEY_LENGTH     15
#define MAX_VALUE_LENGTH   127
#define MAX_MESSAGE_LENGTH 81920

extern void parse(char in[MAX_MESSAGE_LENGTH], int size);

extern void set(char targetkey[MAX_KEY_LENGTH + 1], char newvalue[MAX_VALUE_LENGTH + 1]);

extern void
add(char parentkey[MAX_KEY_LENGTH + 1], char childkey[MAX_KEY_LENGTH + 1], char childvalue[MAX_VALUE_LENGTH + 1]);

extern void erase(char targetkey[MAX_KEY_LENGTH + 1]);

extern int generate(char targekey[MAX_KEY_LENGTH + 1], char out[MAX_MESSAGE_LENGTH]);

static char in[MAX_MESSAGE_LENGTH];
static char out[MAX_MESSAGE_LENGTH];

static int expected;
static int point;

static int calcHash(char *message, int len) {
    int ret = 0, count = 0;

    for (register int i = 1; i < len; ++i) {
        ret += message[i] * count++;
        ret &= THRESHOLD;
    }

    return ret;
}

static inline int get16(char c) {
    return c >= 'a' ? 10 + c - 'a' : c - '0';
}

static void run() {
    int lineN;

    point = 0;
    scanf("%d %d", &lineN, &expected);
    for (int line = 0; line < lineN; ++line) {
        int command, len;
        char buf[80];
        char targetkey[MAX_KEY_LENGTH + 1], newvalue[MAX_VALUE_LENGTH + 1];
        char parentkey[MAX_KEY_LENGTH + 1], childkey[MAX_KEY_LENGTH + 1], childvalue[MAX_VALUE_LENGTH + 1];
        int hash, length, resultlength, resulthash;

        scanf("%d", &command);
        switch (command) {
            case PARSE:
                scanf("%d", &len);
                for (register int p = 0; p < len;) {
                    scanf("%s", buf);
                    for (register int i = 0; i < 64 && p < len; i += 2, ++p)
                        in[p] = (get16(buf[i]) << 4) | get16(buf[i + 1]);
                }
                parse(in, len);
                break;
            case SET:
                scanf("%s %s", targetkey, newvalue);
                set(targetkey, newvalue);
                break;
            case ADD:
                scanf("%s %s %s", parentkey, childkey, childvalue);
                add(parentkey, childkey, childvalue);
                break;
            case ERASE:
                scanf("%s", targetkey);
                erase(targetkey);
                break;
            case GENERATE:
                scanf("%s %d %d", targetkey, &length, &hash);
                resultlength = generate(targetkey, out);
                if (length == resultlength) {
                    resulthash = calcHash(out, length);
                    if (resulthash == hash) point++;
                }
                break;
        }
    }
}

int main() {
    int T, total_score;

    freopen("sample_input.txt", "r", stdin);

    scanf("%d", &T);

    total_score = 0;
    for (int testcase = 1; testcase <= T; ++testcase) {
        run();
        printf("#%d %d\n", testcase, point);
        if (point == expected) total_score++;
    }

    printf("total score = %d\n", total_score * 100 / T);
    return 0;
}