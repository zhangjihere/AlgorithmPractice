/**
 * zhangji 2018-06-23
 */

#define H_LEN   5000

int _N;
int _M;
int _K;

#include <stdio.h>
#include <malloc.h>

void printSide(int *pic) {
    for (int n = 0; n < 4; n++) {
        for (int i = n * _M; i < n * _M + _M; i++) {
            if (pic[i] != -1) {
                printf(" %d\t", pic[i]);
            } else {
                printf("%d\t", pic[i]);
            }
        }
        if (n != 3) {
            printf("+");
        }
    }
    printf("\n");
}

int tc;
int pic_idx;

typedef struct Side {
    int pic;
    int val[15];
    int m_side[5];
    int m_num;

    Side *next;
} Side;

typedef struct SideBook {
    int num;
    Side *side;
};

typedef struct Piece {
    int sides[4];
    int m_num;
} Piece;

SideBook sideList[H_LEN];

Piece ps[49];

void init(int N, int M, int K) {
    _N = N;
    _M = M;
    _K = K;
    tc++;
    pic_idx = -1;
    for (int i = 0; i < H_LEN; i++) {
        sideList[i].num = 0;
        sideList[i].side = nullptr;
    }
    for (int i = 0; i < 49; i++) {
        ps[i].m_num = 0;
        ps[i].sides[0] = 0;
        ps[i].sides[1] = 0;
        ps[i].sides[2] = 0;
        ps[i].sides[3] = 0;
    }
}

int computeHash(int sum) {
    int hi = sum % H_LEN;
    if (hi < 0)
        hi = -hi;
    return hi;
}

Side *createNewStat() {
    auto *new_side = (Side *) malloc(sizeof(Side));
    new_side->m_num = 0;
    new_side->pic = -1;
    new_side->next = nullptr;
    return new_side;
}

void addPiece(int piece[]) {
//    printSide(piece);
    pic_idx++;
    SideBook *sideBook = nullptr;
    Side *side = nullptr;
    for (int n = 0, sum = 0, hi = 0; n < 4; n++) {
        sum = 0;
        side = createNewStat();
        for (int start = n * _M, j = 0, end = n * _M + _M; start + j < end; j++) {
            sum += sum * 3 + (piece[start + j] + 1);
            side->val[j] = piece[start + j];
        }
        side->pic = pic_idx;
        hi = computeHash(sum);
        sideBook = &sideList[hi];
        if (sideBook->side == nullptr) {
            sideBook->num++;
            sideBook->side = side;
        } else {
            sideBook->num++;
            side->next = sideBook->side;
            sideBook->side = side;
        }

        ps[pic_idx].sides[n] = hi;
        ps[pic_idx].m_num++;
    }

}

int findCenterPiece() {


    return 0;
}