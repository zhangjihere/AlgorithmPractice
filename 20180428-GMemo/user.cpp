//#define int register int

#define MAX_SCREEN_SIZE         1000
#define MAX_MEMO                10000
#define MAX_MEMO_STR            2555

typedef struct {
    int y, x;
} AXIS;

struct Memo {
    int left, up, right, down, w, h, z, l;
    char str[MAX_MEMO_STR];
};

Memo memoBook[MAX_MEMO];

int z, memos;

int _min(int x, int y) { return x < y ? x : y; }

int _max(int x, int y) { return x > y ? x : y; }

int _strcpy(char *src, char *dst) {
    int l = 0;
    while (*dst++ = *src++) ++l;
    return l;
}

void init(int mScreenSize) {
    z = 0;
    memos = 0;
}

void create_memo(int mId, int mY, int mX, int mHeight, int mWidth, char str[]) {
    memoBook[mId].left = mX;
    memoBook[mId].up = mY;
    memoBook[mId].w = mWidth;
    memoBook[mId].h = mHeight;
    memoBook[mId].right = memoBook[mId].left + memoBook[mId].w - 1;
    memoBook[mId].down = memoBook[mId].up + memoBook[mId].h - 1;
    memoBook[mId].l = _strcpy(str, memoBook[mId].str);
    memoBook[mId].z = ++z;
    ++memos;
}

AXIS select_memo(int mId) {
    memoBook[mId].z = ++z;
    return AXIS{memoBook[mId].up, memoBook[mId].left};
}

void move_memo(int mId, int mY, int mX) {
    memoBook[mId].left = mX;
    memoBook[mId].up = mY;
    memoBook[mId].right = memoBook[mId].left + memoBook[mId].w - 1;
    memoBook[mId].down = memoBook[mId].up + memoBook[mId].h - 1;
    memoBook[mId].z = ++z;
}

void modify_memo(int mId, int mHeight, int mWidth, char str[]) {
    memoBook[mId].w = mWidth;
    memoBook[mId].h = mHeight;
    memoBook[mId].right = memoBook[mId].left + memoBook[mId].w - 1;
    memoBook[mId].down = memoBook[mId].up + memoBook[mId].h - 1;
    memoBook[mId].l = _strcpy(str, memoBook[mId].str);
    memoBook[mId].z = ++z;
}

void get_screen_context(int mY, int mX, char res[5][5]) {
    int zTable[5][5] = {0};

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            res[i][j] = 0;
        }
    }

    for (int i = 0; i < memos; ++i) {
        if (memoBook[i].right < mX || memoBook[i].left > mX + 4
            || memoBook[i].down < mY || memoBook[i].up > mY + 4)
            continue;
        // check the intersection area
        int startX = _max(memoBook[i].left, mX), startY = _max(memoBook[i].up, mY);
        int endX = _min(memoBook[i].right, mX + 4), endY = _min(memoBook[i].down, mY + 4);
        // check first char in the memo matching the res area
        int idx = (startY - memoBook[i].up) * memoBook[i].w + (startX - memoBook[i].left);
        for (int y = startY; y <= endY; ++y) {
            for (int x = startX; x <= endX; ++x) {
                if (memoBook[i].z > zTable[y - mY][x - mX]) {
                    res[y - mY][x - mX] = idx < memoBook[i].l ? memoBook[i].str[idx] : 0;
                    zTable[y - mY][x - mX] = memoBook[i].z;
                }
                idx++;
            }
            idx += memoBook[i].w - (endX - startX + 1);
        }
    }
}
