#include <malloc.h>

#define MAXSERVER           5
#define MAXTRANSACTION      16
#define IMAGESIZE           400000

extern int calcHash(unsigned char buf[], int pos, int len);

struct Block {
    int hash;
    int txN;
    int tx[MAXTRANSACTION][2];
    int vn;
    struct Block *nbro;
    struct Block *fcld;
};

Block blockchains;
int vs;

Block *createNewBlock(int hash, int len, int pos, struct Block *nbro, struct Block *fcld) {
    auto *new_block = (Block *) malloc(sizeof(Block));
    if (new_block != nullptr) {
        new_block->hash = hash;
        new_block->len = len;
        new_block->pos = pos;
        new_block->nbro = nbro;
        new_block->fcld = fcld;
    }
    return new_block;
}

int getNum(char ch) {
    int num = 0;
    if (ch >= '0' && ch <= '9') {
        num = ch - 0x30;
    } else {
        switch (ch) {
            case 'A':
            case 'a':
                num = 10;
                break;
            case 'B':
            case 'b':
                num = 11;
                break;
            case 'C':
            case 'c':
                num = 12;
                break;
            case 'D':
            case 'd':
                num = 13;
                break;
            case 'E':
            case 'e':
                num = 14;
                break;
            case 'F':
            case 'f':
                num = 15;
                break;
            default:
                num = 0;
        }
    }
    return num;
}

unsigned int hex2int(unsigned char c0, unsigned char c1) {
    //function : hex2int, this function will return integer value against
    //hexValue - which is in string format
    unsigned int x = 0;
    return x = static_cast<unsigned int>((getNum(c0)) * 16 + (getNum(c1)));
}

int num_match[4] = {16777216, 65536, 256, 1};

int parse16Array2int(unsigned char *arr, int pos, int len) {
    int n = 0;
    for (int i = pos + (len - 1); i > pos; i = i - 2) {
        n += hex2int(arr[i - 1], arr[i]) * num_match[i / 2];
    }
    return n;
}

void syncBlockChain(int S, unsigned char blockchainimage[MAXSERVER][IMAGESIZE]) {
//    0x4df5fb01
//    unsigned char temp[8] = {'4', 'd', 'f', '5', 'f', 'b', '0', '1'};
//    int ret = parse16Array2int(temp, 0, 8);
    vs = S / 2 + 1;
    for (int server = 0; server < S; server++) {
        
    }


}

int calcAmount(int hash, int exchangeid) {
    return 0;
}