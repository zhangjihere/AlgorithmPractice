//
// Created by zhangji on 3/19/18.
//
/*
     |0|
      |
 |2|--_--|3|
      |
     |1|
*/

#ifndef __clang__

# include <malloc.h>

#else

# include <cstdlib>

#endif

enum {
    MAX_R_C = 700
};

struct Cell {
    char ch;
    struct Cell *left;
    struct Cell *right;
};

typedef struct {
    struct Cell *row;
    int last_col;
} RowMeta;

struct Cell *createNewCell(char ch);

void prepare_cell_db();
/**
 * shift the pos element in array down, and the backward elements all shift down one position
 * @param pos 
 */
void shiftDownArray(int pos);

struct Cell *cell_db[(MAX_R_C + 1) * (MAX_R_C + 1)];// for recycle memory usage, reusable Cell array
int db_idx; // each call of init function, it reset 0
int flag;   // represent for only once call of prepare_cell_db

RowMeta row_meta[MAX_R_C + 1];// row_meta array by row orber
static int r_c_num; // the number of inputed row
static int cur_row, cur_col; // current row & colomn position
static struct Cell *cur_cell;// current cell pointer

struct Cell *createNewCell(char ch) {
//    struct Cell *new_cell = (struct Cell *) malloc(sizeof(struct Cell));
    struct Cell *new_cell = cell_db[db_idx];
    if (new_cell) {
        new_cell->ch = ch;
        new_cell->left = nullptr;
        new_cell->right = nullptr;
    }
    db_idx++;
    return new_cell;
}

void prepare_cell_db() {
    int db_len = (MAX_R_C + 1) * (MAX_R_C + 1);
    for (int i = 0; i < db_len; i++) {
        cell_db[i] = (struct Cell *) malloc(sizeof(struct Cell));
    }
    flag = 1;
}

void shiftDownArray(int pos) {
    for (int i = r_c_num - 1; i >= pos; i--) {
        row_meta[i + 1] = row_meta[i];
    }
}

void init(int n) {
    db_idx = 0;
    if (flag != 1) {
        prepare_cell_db();
    }

    r_c_num = n;
    cur_row = cur_col = 1;
    for (int i = 1; i <= r_c_num; i++) {
        row_meta[i].row = nullptr;
        row_meta[i].last_col = 0;
    }
    struct Cell *new_cell = createNewCell(0);
    row_meta[cur_row].row = new_cell;
    cur_cell = new_cell;

}

void input_char(char in_char) {
    struct Cell *new_cell = createNewCell(in_char);
    if (cur_col == 1) { // current colomn is a current row first cell
        new_cell->right = cur_cell;
        cur_cell->left = new_cell;
        row_meta[cur_row].row = new_cell;
        row_meta[cur_row].last_col++;
    } else {
        new_cell->right = cur_cell;
        new_cell->left = cur_cell->left;
        struct Cell *old_left = cur_cell->left;
        cur_cell->left = new_cell;
        old_left->right = new_cell;
        row_meta[cur_row].last_col++;
    }
//    cur_cell = new_cell;
    cur_col++;
}

void input_newline() {
    int old_row_last_col = row_meta[cur_row].last_col;
    row_meta[cur_row].last_col = cur_col - 1;
    if (cur_col == 1) { // current colomn is a current row first cell
        struct Cell *new_cell = createNewCell(-1);
        row_meta[cur_row].row = new_cell;
    } else {
        struct Cell *temp = cur_cell->left;
        struct Cell *new_cell = createNewCell(-1);
        new_cell->left = temp;
        temp->right = new_cell;
    }

    shiftDownArray(cur_row + 1);
    cur_cell->left = nullptr;
    row_meta[cur_row + 1].row = cur_cell;
    row_meta[cur_row + 1].last_col = old_row_last_col - (cur_col - 1);

    cur_row++;
    cur_col = 1;
}

/**
 * @param direction 0: Up, 1: Down, 2: Left, 3: Right
 */
void move_cursor(int direction) {
    if (direction == 0) {// up
        if (cur_row != 1) {
            if (row_meta[cur_row - 1].last_col + 1 < cur_col) {
                struct Cell *temp = row_meta[cur_row - 1].row;
                int up_col = 1;
                for (; temp->right;) {
                    up_col++;
                    temp = temp->right;
                }
                cur_col = up_col;
                cur_row = cur_row - 1;
                cur_cell = temp;
            } else {
                struct Cell *temp = row_meta[cur_row - 1].row;
                int up_col = 1;
                for (; up_col <= cur_col - 1; up_col++) {
                    temp = temp->right;
                }
                cur_col = up_col;
                cur_row = cur_row - 1;
                cur_cell = temp;
            }
        }
    } else if (direction == 1) { //down
        if (cur_row != r_c_num && row_meta[cur_row + 1].row) {
            if (row_meta[cur_row + 1].last_col + 1 < cur_col) {
                struct Cell *temp = row_meta[cur_row + 1].row;
                int down_col = 1;
                for (; temp->right;) {
                    down_col++;
                    temp = temp->right;
                }
                cur_col = down_col;
                cur_row = cur_row + 1;
                cur_cell = temp;
            } else {
                struct Cell *temp = row_meta[cur_row + 1].row;
                int down_col = 1;
                for (; down_col <= cur_col - 1; down_col++) {
                    temp = temp->right;
                }
                cur_col = down_col;
                cur_row = cur_row + 1;
                cur_cell = temp;
            }
        }

    } else if (direction == 2) { // left
        if (cur_row != 1 || cur_col != 1) {
            if (cur_col == 1) {
                struct Cell *temp = row_meta[cur_row - 1].row;
                int up_col = 1;
                for (; temp->right;) {
                    up_col++;
                    temp = temp->right;
                }
                cur_col = up_col;
                cur_row = cur_row - 1;
                cur_cell = temp;
            } else {
                cur_col--;
                cur_row = cur_row;
                cur_cell = cur_cell->left;
            }
        }
    } else { // 3 right
        if (cur_cell->right) {
            cur_col++;
            cur_row = cur_row;
            cur_cell = cur_cell->right;
        } else {
            if (cur_row != r_c_num && row_meta[cur_row + 1].row) {
                struct Cell *temp = row_meta[cur_row + 1].row;
                cur_col = 1;
                cur_row = cur_row + 1;
                cur_cell = temp;
            }
        }
    }
}

char get_char(int r, int c) {
    struct Cell *target_row = row_meta[r].row;
    for (int i = 1; i < c; i++) {
        target_row = target_row->right;
    }
    return target_row->ch;
}
