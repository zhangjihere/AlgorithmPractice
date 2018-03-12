//
// Created by zhangji on 3/12/18.
//


static long long hash = 1009;

long pow(long p_number, int time) {
    long result = 1;
    for (int i = 0; i < time; i++) {
        result *= p_number;
    }
    return result;
}

int FindString(int N, char *A, int M, char *B) {
    int match_num = 0;
    long long hs_B = 0;
    long long hs_A = 0;

    long long ss = 1;
    for (int i = 0; i < M; i++) {
        ss *= hash;
    }
    /*for (int i = 0; i < M; i++) {
        hs_B += B[i] * pow(HASH, M - i - 1);
        hs_A_init += A[i] * pow(HASH, M - i - 1);
    }*/
    for (int i = 0; i < M; i++) {
        hs_B = hs_B * hash + B[i];
        hs_A = hs_A * hash + A[i];
    }
    if (hs_B == hs_A) {
        match_num += 1;
    }

    for (int i = M; i < N; i++) {
        hs_A = hash * hs_A - A[i - M] * ss + A[i];
        if (hs_A == hs_B) {
            match_num += 1;
        }
    }
    return match_num;
    
    
    ///

   /* int count = 0;
    if (N < M) return 0;

    long t = 1;
    for (int i = 0; i < M; i++) {
        t *= HASH;
    }

    long wordHash = 0;
    long storyHash = 0;

    for (int i = 0; i < M; i++) {
        wordHash = wordHash * HASH + B[i];
        storyHash = storyHash * HASH + A[i];
    }

    for (int i = 0; i <= N - M; i++) {
        if (wordHash == storyHash) {
            count++;
        }
        if (i + M < N) {
            storyHash = storyHash * HASH + A[i + M] - A[i] * t;
        }
    }
    return count;*/
}

