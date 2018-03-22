//
// Created by zhangji on 3/21/18.
//

#ifndef __clang__

// Maybe thie can speed up waste time more than several hundreds miliseconds
#pragma GCC optimize ("-Ofast")

# include <malloc.h>

#else

# include <cstdlib>

#endif

#define BLOCK_SIZE          3
#define MAX_AREA_W          10
#define MAX_AREA_H          900

#define N_BLOCK             1
#define S_BLOCK             2

struct Lattice {
//    int h;
//    int col;
    int s;
};

//   |_4,1_|_4,2_|_4,3_|__
//   |_3,1_|_3,2_|_3,3_|__
// h |_2,1_|_2,2_|_2,3_|__
//   |_1,1_|_1,2_|_1,3_|__
//      col
typedef struct {
    Lattice lattices[MAX_AREA_H + 1][MAX_AREA_W + 1];//lattices[h][col]
    int col_h[MAX_AREA_W + 1];
    int valid_left;
    int valid_right;
    int num_stack;
} Area;

struct Node {
    int h;
    int col;
    int s;
    Node *next;
};

int max(int a, int b, int c);

Node *creat_new_node(int h, int col, int s);

void update_valid_col_span();

void normal_search(int search_left, int search_right);

void search_clear_lattice(int search_left, int search_right);

void search_one_symbol_in_all(int s);

void search_chain_symbol_in_all(int search_left, int search_right, int top, int s);

void add_clear_table(int ht, int cl);

void clear_lattices();

void shiftdown_area();


void print_lattice();

int case_width;
int n_block[BLOCK_SIZE];
int s_block_type;

Area area;
int block_col;
int block_type;

int clear_time;
int clear_idx;
Lattice *clear_table[9000];
int hash_clear_table[10900];

Node queue;

void init(int width) {
    case_width = width;
    for (int i = 1; i <= MAX_AREA_H; i++) {
        for (int j = 1; j <= MAX_AREA_W; j++) {
//            area.lattices[i][j].h = i;
//            area.lattices[i][j].col = j;
            area.lattices[i][j].s = 0;
        }
    }
    for (int j = 1; j < MAX_AREA_W; j++) {
        area.col_h[j] = 0;
    }
    area.valid_left = width;
    area.valid_right = 1;
    area.num_stack = 0;

//    clear_time++;
//    clear_idx = 0;
}

void newBlock(int block[BLOCK_SIZE]) {
    block_col = 1;
    block_type = N_BLOCK;
    n_block[0] = block[0];
    n_block[1] = block[1];
    n_block[2] = block[2];
    update_valid_col_span();
}

// 9
void specialBlock(int type) {
    block_col = 1;
    s_block_type = type;
    block_type = S_BLOCK;
//    update_valid_col_span(); // don't care specialBlock col span
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
    if (block_type == N_BLOCK) { // don't care special_block
        update_valid_col_span();
    }
}

void print_lattice(char *position) {
    printf("%s,case_width:%d, block_col:%d\n", position, case_width, block_col);
    for (int h = 8; h >= 1; h--) {
        for (int col = 1; col <= case_width; col++) {
            printf("%d ", area.lattices[h][col].s);
        }
        printf("\n");
    }
    printf("\n");
}

int land() {
//    print_lattice("Before");

    // setup search_left & search_right in area
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
    // compute, clear, and rearrange area until stable
    // normal search and clear
    if (block_type == N_BLOCK) {
        // lay block and add col height, when block is normal(3 lattice) block
        int h = area.col_h[block_col];
        area.lattices[h + 1][block_col].s = n_block[2];
        area.lattices[h + 2][block_col].s = n_block[1];
        area.lattices[h + 3][block_col].s = n_block[0];
        area.col_h[block_col] += BLOCK_SIZE;
        normal_search(search_left, search_right);
    } else if (block_type == S_BLOCK) {
        int top = area.col_h[block_col];
        if (top != 0) {
            int top_symbol = area.lattices[top][block_col].s;
            clear_time++;
            clear_idx = 0;
            if (s_block_type == 1) {
                search_one_symbol_in_all(top_symbol);
            } else if (s_block_type == 2) {
                search_chain_symbol_in_all(search_left, search_right, top, top_symbol);
            }
            clear_lattices();
            shiftdown_area();
            normal_search(search_left, search_right);
        }
    }
    int result = 0;
    for (int col = 1; col <= case_width; col++) {
        result += area.col_h[col];
    }
//    print_lattice("After");
    return result;
}

void shiftdown_area() {
    for (int col = 1; col <= area.valid_right; col++) {
        int recompute_col_flag = 0;
        for (int h = 1; h <= area.col_h[col]; h++) {
            if (area.lattices[h][col].s == 0) {
                recompute_col_flag = 1;
                if (h != area.col_h[col]) {
                    for (int i = h + 1; i <= area.col_h[col]; i++) {
                        if (area.lattices[i][col].s != 0) {
                            area.lattices[h][col].s = area.lattices[i][col].s;
                            area.lattices[i][col].s = 0;
                            break;
                        }
                    }
                }
            }
        }
        if (recompute_col_flag == 1) {
            for (int h = 1; h < MAX_AREA_H; h++) {
                if (area.lattices[h][col].s == 0) {
                    area.col_h[col] = h - 1;
                    break;
                }
            }
        }
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
//        int ht = clear_table[i]->h;
//        int cl = clear_table[i]->col;
//        int hash = cl * 1000 + ht;
//        if (hash_clear_table[hash] == clear_time) {
        clear_table[i]->s = 0;
//        }
    }
}

void search_one_symbol_in_all(int s) {
    for (int col = area.valid_left; col <= area.valid_right; col++) {
        for (int h = 1; h <= area.col_h[col]; h++) {
            if (area.lattices[h][col].s == s) {
                add_clear_table(h, col);
            }
        }
    }
}

// 0 1 2
// 3   4
// 5 6 7
int eight_dir[8][2] = {{1,  -1},{1,  0},{1,  1},
                       {0,  -1},         {0,  1},
                       {-1, -1},{-1, 0},{-1, 1}};

Node *creat_new_node(int h, int col, int s) {
    auto *new_node = (Node *) malloc(sizeof(Node));
    if (new_node != nullptr) {
        new_node->next = nullptr;
        new_node->h = h;
        new_node->col = col;
        new_node->s = s;
    }
    return new_node;
}

void search_chain_symbol_in_all(int search_left, int search_right, int top, int s) {
    queue.next = nullptr;
    queue.h = top, queue.col = block_col, queue.s = s;
    Node *head = &queue;
    Node *tail = &queue;
    add_clear_table(head->h, head->col);
    int h, col;
    int h_d, col_d;
    while (head != nullptr) {
        h = head->h, col = head->col;
        for (auto &t : eight_dir) {
            h_d = h + t[0], col_d = col + t[1];
            if (area.lattices[h_d][col_d].s == s) {
                int hash = col_d * 1000 + h_d;
                if (hash_clear_table[hash] != clear_time) {
                    Node *new_node = creat_new_node(h_d, col_d, s);
                    tail->next = new_node;
                    add_clear_table(h_d, col_d);
                    tail = new_node;
                }
            }
        }
        head = head->next;
    }
}

void normal_search(int search_left, int search_right) {
    do { // normal search and clear
        clear_time++;
        clear_idx = 0;
        search_clear_lattice(search_left, search_right);
        if (clear_idx != 0) {
            clear_lattices();
            shiftdown_area();
        }
    } while (clear_idx != 0);
}

// --+ 1    |             +      \              +      |
//          | 2           | 4      \   5      /   6    | 7    --+ 8
//          +      +-- 3  |          +      /          +
int three_line[8][3][2] = {
        {{1,  -1}, {1,  0},  {1,  1}},
        {{1,  1},  {0,  1},  {-1, 1}},
        {{-1, 1},  {-1, 0},  {-1, -1}},
        {{-1, -1}, {0,  -1}, {1,  -1}},

        {{1,  -1}, {0,  0},  {-1, 1}},
        {{-1, -1}, {0,  0},  {1,  1}},

        {{1,  0},  {0,  0},  {-1, 0}},
        {{0,  -1}, {0,  0},  {0,  1}}
};

void search_clear_lattice(int search_left, int search_right) {
    int ah, bh, ch, max_h, s_1, s_2, s_3;
    for (int col = search_left; col <= search_right; col++) {
        ah = area.col_h[col - 1];
        bh = area.col_h[col];
        ch = area.col_h[col + 1];
        max_h = max(ah, bh, ch);
        if (max_h < 3) {
            max_h = 3;
        }
        for (int h = 2; h <= max_h - 1; h++) {
            for (auto &t : three_line) {
                s_1 = area.lattices[h + t[0][0]][col + t[0][1]].s;
                s_2 = area.lattices[h + t[1][0]][col + t[1][1]].s;
                s_3 = area.lattices[h + t[2][0]][col + t[2][1]].s;
                if (s_1 == s_2 && s_2 == s_3 && s_1 != 0) {
                    add_clear_table(h + t[0][0], col + t[0][1]);
                    add_clear_table(h + t[1][0], col + t[1][1]);
                    add_clear_table(h + t[2][0], col + t[2][1]);
                }
            }
        }
    }
}

void add_clear_table(int ht, int cl) {
    int hash = cl * 1000 + ht;
    if (hash_clear_table[hash] != clear_time) {
        hash_clear_table[hash] = clear_time;
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
    }
    if (b > c) {
        return b;
    }
    return c;
}