#ifndef __clang__
//#pragma GCC optimize("-Ofast")

#include <malloc.h>

#else
# include <cstdlib>
#endif

#define MAXSERVER           5
#define MAXTRANSACTION      17
#define IMAGESIZE           400000
#define BLOCK_HASH_SIZE     20000

extern int calcHash(unsigned char buf[], int pos, int len);

typedef struct Block {
    int p_hash;             // parent hash
    int p_index;            // parent blk index
    int c_hash;             // current hash
    int tx[MAXTRANSACTION]; //index:Exch_Id, value:Amt
    int c_score;            // child score
    int s_num;              // server num
} Block;

typedef struct Leaf {
    int lfs[2][BLOCK_HASH_SIZE]; // switch two leafs queue
    int lfs_len[2];
} Leaf;

Block blks[BLOCK_HASH_SIZE];
int umark[BLOCK_HASH_SIZE];
Leaf leafs;
int mark;
int half_S;

int cpt_hash_absorb(int h) {
    int index = h % BLOCK_HASH_SIZE;
    index = index < 0 ? -index : index;
    while (umark[index] == mark) {
        if (blks[index].c_hash == h) {
            return index;
        }
        index++;
        index = index % BLOCK_HASH_SIZE;
        index = index < 0 ? -index : index;
    }
    blks[index].s_num = 0;
    umark[index] = mark;
    return index;
}

int cpt_hash_match(int h) {
    int index = h % BLOCK_HASH_SIZE;
    index = index < 0 ? -index : index;
    while (blks[index].c_hash != h && umark[index] == mark) {
        index++;
        index = index % BLOCK_HASH_SIZE;
        index = index < 0 ? -index : index;
    }
    return index;
}

int cpt_b2i(unsigned char *bs, int pos, int len) {
    if (len == 4)
        return bs[pos + 0] * 16777216 + bs[pos + 1] * 65536 + bs[pos + 2] * 256 + bs[pos + 3];
    if (len == 2)
        return bs[pos + 0] * 256 + bs[pos + 1];
    return bs[pos];
}

int clearTxAmt(int *txAmt) {
    for (int i = 1; i < MAXTRANSACTION; i++) {
        txAmt[i] = 0;
    }
}

int _anti(int sw) {
    if (sw == 0)
        return 1;
    if (sw == 1)
        return 0;
}

// 4 + 2 + 1 + 1 + 2 + 1 +2 = 7+1+2+1+2
//      |tran|                ^

int blks_num[5];

void syncBlockChain(int S, unsigned char blockchainimage[MAXSERVER][IMAGESIZE]) {
    mark++;
    half_S = S / 2 + 1;
    for (int t = 0; t < BLOCK_HASH_SIZE; t++) {
        blks[t].s_num = 0;
    }
    // absord block
    int bci_len = 0;
    for (int s = 0; s < S; s++) {
        bci_len = cpt_b2i(blockchainimage[s], 0, 4);
        for (int pos = 3 + 1; pos < bci_len + 4;) {
            int p_hash = cpt_b2i(blockchainimage[s], pos, 4);
            int t_num = blockchainimage[s][pos + 4 + 2];
            int c_hash = calcHash(blockchainimage[s], pos, 4 + 2 + 1 + t_num * 3);

            int c_index = cpt_hash_absorb(c_hash);
            if (blks[c_index].s_num == 0) {
                clearTxAmt(blks[c_index].tx);
                blks[c_index].p_hash = p_hash;
                blks[c_index].c_hash = c_hash;
                for (int k = 0; k < t_num; k++) {
                    int t_idx = pos + 4 + 2 + 1 + k * 3;
                    int n = blockchainimage[s][t_idx];
                    blks[c_index].tx[n] += cpt_b2i(blockchainimage[s], t_idx + 1, 2);
                }
            }
            blks[c_index].s_num += 1;
            blks_num[s]++;
            pos += (4 + 2 + 1 + t_num * 3);
        }
        int t = 0;
    }

    // match block to index
    for (int b = 0; b < BLOCK_HASH_SIZE; b++) {
        if (umark[b] == mark && blks[b].p_hash != 0) {
            int p_hash = blks[b].p_hash;
            int p_index = cpt_hash_match(p_hash);
            blks[b].p_index = p_index;

            Block *blk_par = &blks[p_index];
            blk_par->c_score += 1;
        }
    }

    // find 0 c_score fill to 0 leaf queue
    leafs.lfs_len[0] = 0;
    leafs.lfs_len[1] = 0;
    int *lfslen = &leafs.lfs_len[0];
    for (int l = 0; l < BLOCK_HASH_SIZE; l++) {
        if (umark[l] == mark && blks[l].c_score == 0) {
            leafs.lfs[0][(*lfslen)++] = l;
        }
    }
    // step to reduce leaf and add parent amount
    int sw = 0;
    while (blks[leafs.lfs[sw][0]].p_hash != 0) {
        for (int i = 0; i < leafs.lfs_len[sw]; i++) {
            int index = leafs.lfs[sw][i];
            int p_index = blks[index].p_index;
            if (blks[index].s_num >= half_S) {
                for (int j = 1; j < MAXTRANSACTION; j++) {
                    if (blks[index].tx[j] > 0) {
                        blks[p_index].tx[j] += blks[index].tx[j];
                    }
                }
            }
            blks[p_index].c_score--;
            if (blks[p_index].c_score == 0) {
                int anti_sw = _anti(sw);
                int *lidx = &leafs.lfs_len[anti_sw];
                leafs.lfs[anti_sw][(*lidx)++] = p_index;
            }
        }
        leafs.lfs_len[sw] = 0;
        sw = _anti(sw);
    }
}

int calcAmount(int hash, int exchangeid) {
    int index = cpt_hash_match(hash);
    if (blks[index].s_num >= half_S) {
        return blks[index].tx[exchangeid];
    } else {
        return 0;
    }
}