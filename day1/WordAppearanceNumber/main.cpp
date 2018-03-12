//
// Created by zhangji on 3/12/18.
//

#include <iostream>
#include <stdio.h>

using namespace std;

const int MAXN = 500000;
const int MAXM = 100000;

extern int FindString(int N, char *A, int M, char *B);

char A[MAXN + 5];
char B[MAXM + 5];

int main(void) {

    setbuf(stdout, NULL);

    int TestCase;

//    freopen("sample_input.txt", "r", stdin);
    freopen("sample_input.txt", "r", stdin);
    
    for (scanf("%d", &TestCase); TestCase--;) {

        scanf("%s", A);
        scanf("%s", B);

        //N = Length of A, M = Length of B
        int N = 0, M = 0;
        for (; A[N]; N++);
        for (; B[M]; M++);

        int answer = FindString(N, A, M, B);

        static int tc = 0;
        printf("#%d %d\n", ++tc, answer);
    }
    return 0;
}