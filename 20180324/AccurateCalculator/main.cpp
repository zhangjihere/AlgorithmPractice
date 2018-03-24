#include <iostream>
#include <cstring>

using namespace std;

#define MAX_N 500

static int T;
static char str[MAX_N + 1];
static int dummy0[11];
static char cmp[MAX_N + 21 + 1];
static int dummy1[22];
static char rst[MAX_N + 21 + 1];

extern void run(char* rst, const char* str);

int my_strlen(const char* a)
{
    return strlen(a);
}

void my_strcpy(char* a, const char* b)
{
    strcpy(a, b);
}

int my_strcmp(const char* a, const char* b)
{
    return strcmp(a, b);
}

char* my_strcat(char* a, const char* b)
{
    return strcat(a, b);
}

int main(void)
{
     freopen("sample_input.txt", "r", stdin);
    cin >> T;
    int total = 0;
    for (int t = 1; t <= T; ++t)
    {
        cin >> str >> cmp;
        run(rst, str);
        int dummy2[33];
        int score = (strcmp(rst, cmp) == 0) ? 100 : 0;
        cout << "#" << t << " " << score << endl;
        total += score;
    }
    cout << "total score = " << (total / T) << endl;
    return 0;
}