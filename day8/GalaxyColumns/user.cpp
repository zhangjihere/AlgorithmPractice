//
// Created by zhangji on 3/21/18.
//

#include <malloc.h>

#define BLOCK_SIZE          3
#define MAX_AREA_W          10
#define MAX_AREA_H          900

void update_valid_col_span();

void clear_lattices();

void shiftdown_area();

void search_clear_lattice(int search_left, int search_right);

void add_clear_table(int ht, int cl);

int max(int a, int b, int c);


struct Lattice {
    int x;
    int y;
    int c;
};

typedef struct {
    Lattice lattices[MAX_AREA_H + 1][MAX_AREA_W + 1];
    int col_h[MAX_AREA_W + 1];
    int valid_left;
    int valid_right;
    int num_stack;
} Area;

int case_width;
int n_block[BLOCK_SIZE];
int s_block;

Area area;
int block_col;
int block_type;

int land_time;
int clear_idx;
Lattice *clear_table[9000];
int hash_clear_table[10900];


void init(int width) {
    case_width = width;
    for (int i = 1; i <= MAX_AREA_H; i++) {
        for (int j = 1; j <= MAX_AREA_W; j++) {
            area.lattices[i][j].c = 0;
        }
    }
    for (int j = 1; j < MAX_AREA_W; j++) {
        area.col_h[j] = 0;
    }
    area.valid_left = width;
    area.valid_right = 1;
    area.num_stack = 0;

    land_time++;
    clear_idx = 0;
}

void newBlock(int block[BLOCK_SIZE]) {
    block_col = 1;
    block_type = 1;
    n_block[0] = block[0];
    n_block[1] = block[1];
    n_block[2] = block[2];
    update_valid_col_span();
}

// 9
void specialBlock(int type) {
    block_col = 1;
    s_block = type;
    block_type = 2;
    update_valid_col_span();
}

// 100
void circulate(int count) {
    int temp = 0;
    if (count % BLOCK_SIZE == 1) {
        temp = n_block[2];
        n_block[2] = n_block[1];
        n_block[1] = n_block[0];
        n_block[0] = temp;
    } else if (count % BLOCK_SIZE == 2) {
        temp = n_block[0];
        n_block[0] = n_block[1];
        n_block[1] = n_block[2];
        n_block[2] = temp;
    }
}

void update_valid_col_span() {
    if (area.valid_right < block_col) {
        area.valid_right = block_col;
    }
    if (block_col < area.valid_left) {
        area.valid_left = block_col;
    }
}

// 200
void move(int distance) {
    block_col += distance;
    if (block_col > case_width) {
        block_col = case_width;
    }
    if (block_col < 1) {
        block_col = 1;
    }
    update_valid_col_span();

}

int land() {
    land_time++;
    clear_idx = 0;
    // lay block and add col height, when block is normal(3 lattice) block
    if (block_type == 1) {
        int h = area.col_h[block_col];
        area.lattices[h + 1][block_col].c = n_block[2];
        area.lattices[h + 2][block_col].c = n_block[1];
        area.lattices[h + 3][block_col].c = n_block[0];
        area.col_h[block_col] += BLOCK_SIZE;

    } else if (block_type == 2) {

    }

    // compute, clear, and rearrange area until stable
    if (block_type == 1) {// normal compute
        int search_left = 0;
        int search_right = 0;
        if (area.valid_right - area.valid_left == 0) {
            if (area.valid_left == 1) {
                search_left = search_right = 2;
            } else if (area.valid_left == case_width) {
                search_left = search_right = case_width - 1;
            } else {
                search_left = search_right = area.valid_left;
            }
        } else if (area.valid_right - area.valid_left == 1) {
            if (area.valid_left == 1) {
                search_left = search_right = 2;
            } else if (area.valid_right == case_width) {
                search_left = search_right = case_width - 1;
            } else {
                search_left = search_right = area.valid_left;
            }
        } else if (area.valid_right - area.valid_left > 1) {
            search_left = area.valid_left + 1;
            search_right = area.valid_right - 1;
        }
        search_clear_lattice(search_left, search_right);
    } else if (block_type == 2) {

    }
    clear_lattices();
    shiftdown_area();
    clear_idx = 0;
    return -1;
}

void shiftdown_area() {
    for (int col = 1; col <= area.valid_right; col++) {
        int delta_col_h = 0;
        for (int h = 1; h <= area.col_h[col]; h++) {
            if (area.lattices[h][col].c == 0) {
                if (h != area.col_h[col]) {
                    for (int i = h + 1; i <= area.col_h[col]; i++) {
                        if (area.lattices[i][col].c != 0) {
                            area.lattices[h][col].c = area.lattices[i][col].c;
                            area.lattices[i][col].c = 0;
                            delta_col_h++;
                            break;
                        }
                    }
                } else {
                    delta_col_h++;
                }
            }
        }
        area.col_h[col] -= delta_col_h;
    }
    for (int i = 1; i <= case_width; i++) {
        if (area.col_h[i] != 0) {
            area.valid_left = i;
            break;
        }
    }
    for (int i = case_width; i >= 1; i--) {
        if (area.col_h[i] != 0) {
            area.valid_right = i;
            break;
        }
    }
}

// need optimize
void clear_lattices() {
    for (int i = 0; i < clear_idx; i++) {
        int ht = clear_table[i]->x;
        int cl = clear_table[i]->y;
        int hash = cl * 1000 + ht;
        if (hash_clear_table[hash] == land_time) {
            clear_table[i]->c = 0;
        }
    }
}

void search_clear_lattice(int search_left, int search_right) {

    for (int k = search_left; k <= search_right; k++) {
        int ah = area.col_h[search_left - 1];
        int bh = area.col_h[search_left];
        int ch = area.col_h[search_left + 1];
        int max_h = max(ah, bh, ch);
        for (int h = 2; h <= max_h - 1; h++) {
            int c1 = area.lattices[h + 1][k - 1].c;
            int c2 = area.lattices[h + 1][k].c;
            int c3 = area.lattices[h + 1][k + 1].c;
            int c4 = area.lattices[h][k - 1].c;
            int c5 = area.lattices[h][k].c;
            int c6 = area.lattices[h][k + 1].c;
            int c7 = area.lattices[h - 1][k - 1].c;
            int c8 = area.lattices[h - 1][k].c;
            int c9 = area.lattices[h - 1][k + 1].c;
            if (c1 == c2 && c2 == c3) {
                add_clear_table(h + 1, k - 1);
                add_clear_table(h + 1, k);
                add_clear_table(h + 1, k + 1);
            }
            if (c3 == c6 && c6 == c9) {
                add_clear_table(h + 1, k + 1);
                add_clear_table(h, k + 1);
                add_clear_table(h - 1, k + 1);
            }
            if (c9 == c8 && c8 == c7) {
                add_clear_table(h - 1, k + 1);
                add_clear_table(h - 1, k);
                add_clear_table(h - 1, k - 1);
            }
            if (c7 == c4 && c4 == c1) {
                add_clear_table(h - 1, k - 1);
                add_clear_table(h, k - 1);
                add_clear_table(h + 1, k - 1);
            }
            if (c1 == c5 && c5 == c9) {
                add_clear_table(h + 1, k - 1);
                add_clear_table(h, k);
                add_clear_table(h - 1, k + 1);
            }
            if (c7 == c5 && c5 == c3) {
                add_clear_table(h - 1, k - 1);
                add_clear_table(h, k);
                add_clear_table(h + 1, k + 1);
            }
            if (c2 == c5 && c5 == c8) {
                add_clear_table(h + 1, k);
                add_clear_table(h, k);
                add_clear_table(h - 1, k);
            }
            if (c4 == c5 && c5 == c6) {
                add_clear_table(h, k - 1);
                add_clear_table(h, k);
                add_clear_table(h, k + 1);
            }
        }
    }
}

void add_clear_table(int ht, int cl) {
    int hash = cl * 1000 + ht;
    if (hash_clear_table[hash] != land_time) {
        hash_clear_table[hash] = land_time;
        clear_table[clear_idx] = &area.lattices[ht][cl];
        clear_idx++;
    }
}

int max(int a, int b, int c) {
    if (a > b) {
        if (a > c) {
            return a;
        }
        return c;
    } else {
        if (b > c) {
            return b;
        }
        return c;
    }
}