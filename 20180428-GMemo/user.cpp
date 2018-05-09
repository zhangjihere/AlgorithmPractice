//
// Created by zhangji on 5/8/18.
//

#include <malloc.h>
#include <iostream>

using namespace std;

#define rint register int
#define MAX_SCREEN_SIZE        1000
#define MAX_MEMO        10000
#define MAX_MEMO_STR    2555

typedef struct {
    int y, x;
} AXIS;

typedef struct {
    int mId;
    int mY, mX;
    int mHeight, mWidth;
    char str[MAX_MEMO_STR];
    struct MemoPP *memoPP;
} Memo;
Memo memoBook[MAX_MEMO];

struct MemoPP {
    int cur_mID;
    MemoPP *prev;
    MemoPP *back;
};
MemoPP *memoBoard;

int screen_size;
char screen[MAX_SCREEN_SIZE][MAX_SCREEN_SIZE];

void print_res(char res[5][5], int mY, int mX);

void print_memo(Memo *memo);

void paste_memo2screen(Memo *memo);

void print_screen();


MemoPP *createNewStat(MemoPP *prev, int mId, MemoPP *back) {
    auto *new_memoPP = (MemoPP *) malloc(sizeof(MemoPP));
    if (new_memoPP != nullptr) {
        new_memoPP->prev = prev;
        new_memoPP->cur_mID = mId;
        new_memoPP->back = back;
    }
    return new_memoPP;
}

int _strcpy(char* src, char* dst)
{
    rint l = 0;
    while(*dst++ = *src++) ++l;
    return l;
}


void init(int mScreenSize) {
    screen_size = mScreenSize;
    memoBoard = nullptr;
    for (int i = 0; i < MAX_MEMO; i++) {
        for (int j = 0; j < MAX_MEMO_STR; j++) {
            memoBook[i].str[j] = 0;
        }
    }
}

// 100 to Front
void create_memo(int mId, int mY, int mX, int mHeight, int mWidth, char str[]) {
    //
    MemoPP *newMemoPP = createNewStat(nullptr, mId, memoBoard);
    if (memoBoard != nullptr) {
        memoBoard->prev = newMemoPP;
    }
    memoBoard = newMemoPP;
    //
    memoBook[mId].memoPP = newMemoPP;
    memoBook[mId].mId = mId;
    memoBook[mId].mY = mY;
    memoBook[mId].mX = mX;
    memoBook[mId].mHeight = mHeight;
    memoBook[mId].mWidth = mWidth;
    _strcpy(str, memoBook[mId].str);
    //
//    print_memo(&memoBook[mId]);
//    paste_memo2screen(&memoBook[mId]);
//    print_screen();
}

void toggleMemo2Top(Memo *memo) {
    MemoPP *memoPP = memo->memoPP;
    if (memoPP->prev == nullptr) { // memo is on the top of board
        // do nothing
    } else {
        MemoPP *memoBoard_prev = memoPP->prev;
        MemoPP *memoBoard_back = memoPP->back;
        // break the old link
        if (memoBoard_back == nullptr) { // denote the memo is on the bottom of board
            memoBoard_prev->back = memoBoard_back;
        } else {
            memoBoard_prev->back = memoBoard_back;
            memoBoard_back->prev = memoBoard_prev;
        }
        // connect the memo to the top of memoBoard
        memoPP->prev = nullptr;
        memoPP->back = memoBoard;
        memoBoard->prev = memoPP;
        memoBoard = memoPP;
    }
}

// 200 to Front
AXIS select_memo(int mId) {
    auto *axis = (AXIS *) malloc(sizeof(AXIS));
    Memo *memo = &memoBook[mId];
    toggleMemo2Top(memo);
    axis->x = memo->mX;
    axis->y = memo->mY;
    return *axis;
}

// 300 to Front
void move_memo(int mId, int mY, int mX) {
    Memo *memo = &memoBook[mId];
    memo->mY = mY;
    memo->mX = mX;

    toggleMemo2Top(memo);
}

// 400
void modify_memo(int mId, int mHeight, int mWidth, char str[]) {
    Memo *memo = &memoBook[mId];
    memo->mHeight = mHeight;
    memo->mWidth = mWidth;
    for (int j = 0; j < MAX_MEMO_STR && memo->str[j] != 0; j++) {
        memo->str[j] = 0;
    }
    _strcpy(str, memo->str);
    toggleMemo2Top(memo);
}

bool point_in_res_area(int y, int x, int up, int down, int left, int right) {
    return (up <= y && y <= down) && (left <= x && x <= right);
}

// 999
void get_screen_context(int mY, int mX, char res[5][5]) {
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            res[y][x] = -1;
        }
    }
    int residual_num = 25;
    int res_up = mY;
    int res_down = mY + 5 - 1;
    int res_left = mX;
    int res_right = mX + 5 - 1;
    Memo *memo;
    MemoPP *memoPP = memoBoard;
    while (memoPP != nullptr) {
        int mid = memoPP->cur_mID;
        memo = &memoBook[mid];
        int m_width = memo->mWidth;
        int m_height = memo->mHeight;
        if (residual_num != 0) {
            int memo_up = memo->mY;
            int memo_down = memo->mY + m_height - 1;
            int memo_left = memo->mX;
            int memo_right = memo->mX + m_width - 1;
//            int count=
            for (int y = memo_up; y <= memo_down; y++) {
                for (int x = memo_left; x <= memo_right; x++) {
                    if (point_in_res_area(y, x, res_up, res_down, res_left, res_right)) {
                        int res_y = y - res_up;
                        int res_x = x - res_left;
                        if (res[res_y][res_x] == -1) {
                            int str_char_index = (y - memo_up) * m_width + (x - memo_left);
                            res[res_y][res_x] = memo->str[str_char_index];
                            residual_num--;
                        }
                    }
                }
            }
        } else {
            return;
        }
        memoPP = memoPP->back;
    }
    if (residual_num != 0) {
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                if (res[y][x] == -1) {
                    res[y][x] = 0;
                }
            }
        }
    }
//    print_res(res, mY, mX);
}

void print_res(char res[5][5], int mY, int mX) {
    printf("print res(%d,%d):\n", mY, mX);
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            printf("%c ", res[y][x]);
        }
        printf("\n");
    }
}

void paste_memo2screen(Memo *memo) {
    int m_y = memo->mY;
    int m_x = memo->mX;
    int m_height = memo->mHeight;
    int m_width = memo->mWidth;
    for (int y = m_y; y <= m_y + m_height - 1; y++) {
        for (int x = m_x; x <= m_x + m_width - 1; x++) {
            screen[y][x] = memo->str[(y - m_y) * m_width + (x - m_x)];
        }
    }
}

void print_memo(Memo *memo) {
    printf("print screen mid: %d, (%d,%d), h:%d, w:%d:\n", memo->mId, memo->mY, memo->mX, memo->mHeight, memo->mWidth);
    int m_y = memo->mY;
    int m_x = memo->mX;
    int m_height = memo->mHeight;
    int m_width = memo->mWidth;
    for (int y = m_y; y <= m_y + m_height - 1; y++) {
        for (int x = m_x; x <= m_x + m_width - 1; x++) {
            printf("%c ", memo->str[(y - m_y) * m_width + (x - m_x)]);
        }
        printf("\n");
    }
}

void print_screen() {
    for (int y = 0; y < screen_size; y++) {
        for (int x = 0; x < screen_size; x++) {
            cout << screen[y][x] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
