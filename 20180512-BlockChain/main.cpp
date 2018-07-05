#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MAXSERVER    5
#define IMAGESIZE    400000

extern void syncBlockChain(int S, unsigned char blockchainimage[MAXSERVER][IMAGESIZE]);

extern int calcAmount(int hash, int exchangeid);

static unsigned char blockchainimage[MAXSERVER][IMAGESIZE];

static int S, Q;

int calcHash(unsigned char buf[], int pos, int len) {
    int hash = 0;

    while (len-- > 0) {
        hash = buf[pos++] + (hash << 6) + (hash << 16) - hash; // hash = buf[pos++] + 65599 * hash;
        hash &= 0x7fffffff;
    }
    return hash;
}

static inline int get16(char c) {
    return c >= 'a' ? 10 + c - 'a' : c - '0';
}


static bool run() {
    int corrected = 0;

    scanf("%d %d", &S, &Q);
    for (int server = 0; server < S; ++server) {
        int len;
        scanf("%d", &len);
        for (int p = 0; p < len;) {
            char buf[80];
            scanf("%s", buf);
            for (int i = 0; i < 64 && p < len; i += 2, ++p)
                blockchainimage[server][p] = (get16(buf[i]) << 4) | get16(buf[i + 1]);
        }
    }

    syncBlockChain(S, blockchainimage);

    for (int q_count = 0; q_count < Q; ++q_count) {
        int hash, exchangeid, answer;
        scanf("%d %d %d", &hash, &exchangeid, &answer);
        int result = calcAmount(hash, exchangeid);
        if (result == answer)
            ++corrected;
    }

    return corrected == Q;
}

int main() {
    int TC, totalscore;

    freopen("sample_input.txt", "r", stdin);
//    freopen("my_input.txt", "r", stdin);

    setbuf(stdout, NULL);
    scanf("%d", &TC);

    totalscore = 0;
    for (int testcase = 1; testcase <= TC; ++testcase) {
        int score = 0;
        if (run())
            score = 100;
        totalscore += score;
        printf("#%d %d\n", testcase, score);
    }
    printf("total score = %d\n", totalscore / TC);
    return 0;
}