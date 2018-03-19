# include <stdio.h>

int array_length(char *string) {
    int i = 0;
    while (string[i++]) {
    }
    return i;
}

int d[1001 + 1][1001 + 1];

int main() {

    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int T;
    scanf("%d", &T);
    char a[1000], b[1000];
    int a_len = 0, b_len = 0;
    for (int t = 0; t < T; t++) {
        scanf("%s", &a);
        scanf("%s", &b);
        a_len = array_length(a);
        b_len = array_length(b);

        for (int i = 1; i <= a_len; i++) {
            for (int j = 1; j <= b_len; j++) {
                d[i][j] = 0;
            }
        }
        for (int i = 1; i <= a_len; i++) {
            for (int j = 1; j < b_len; j++) {
                if (a[i - 1] == b[j - 1]) {
                    d[i][j] = d[i - 1][j - 1] + 1;
                } else {
                    d[i][j] = (d[i - 1][j] > d[i][j - 1]) ? d[i - 1][j] : d[i][j - 1];
                }
            }
        }
        printf("#%d %d\n", t + 1, d[a_len - 1][b_len - 1]);
    }
}
