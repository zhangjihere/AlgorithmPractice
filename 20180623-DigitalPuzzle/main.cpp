#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>

extern void init(int N, int M, int K);
extern void addPiece(int piece[]);
extern int findCenterPiece();

static int mSeed;
static int pseudo_rand(void)
{
    mSeed = mSeed * 431345 + 2531999;
    return mSeed & 0x7FFFFFFF;
}

static int run()
{
    int N, M, K;
    scanf("%d %d %d", &N, &M, &K);// 3 8 1

    init(N, M, K);

    int piece[60];
    for (int i = 0; i < N * N + K; ++i) {
        for (int k = 0; k < 4; ++k) {
            scanf("%d", &mSeed);
            int idx = k * M;
            piece[idx] = piece[idx + M - 1] = 0;
            if (mSeed > 0) {
                for (int m = 1; m < M - 1; ++m)
                    piece[idx + m] = pseudo_rand() % 3 - 1;
            }
            else {
                mSeed *= -1;
                for (int m = M - 2; m > 0; --m)
                    piece[idx + m] = 1 - pseudo_rand() % 3;
            }
        }

        addPiece(piece);
    }

    int result = findCenterPiece();
    return result;
}

int main()
{
    setbuf(stdout, NULL);
//    freopen("sample_input.txt", "r", stdin);
    freopen("my_input.txt", "r", stdin);

    int T;
    scanf("%d", &T);

    for (int tc = 1; tc <= T; tc++)
    {
        printf("#%d %d\n", tc, run());
    }

    return 0;
}