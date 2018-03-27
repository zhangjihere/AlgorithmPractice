#define MAX_N 1001

#ifndef __clang__

#pragma GCC optimize ("-Ofast")

# include <malloc.h>

#else

# include <cstdlib>

#endif

extern int my_strlen(const char *a);

extern void my_strcpy(char *a, const char *b);

extern int my_strcmp(const char *a, const char *b);

extern char *my_strcat(char *a, const char *b);

int numcmp(const char *a, const char *b);

void add(char *r, const char *a, const char *b);

void sub(char *r, const char *a, const char *b);

void mul(char *r, const char *a, const char *b);

void div(char *r, const char *a, const char *b);

// computation signal
enum {
    NN = 0, ADD = 1, SUB = 2, MUL = 3, DIV = 4
};

struct Stat {
    int sig_prv;
    char n[MAX_N];
    Stat *next;
};

struct Quation {
    int sig_prv;
    Quation *sub_qua;
    Stat stat;
};

struct Quation qua[MAX_N];
//int qua_idx;

struct Quation *root;

Stat *createNewStat(int sig_prv, const char *n) {
    auto *new_stat = (Stat *) malloc(sizeof(Stat));
    if (new_stat != nullptr) {
        new_stat->sig_prv = sig_prv;
        new_stat->next = nullptr;
        if (n != nullptr) {
            my_strcat(new_stat->n, n);
        }
    }
    return new_stat;
}

Quation *getQuation(int qua_idx, int sig_prv) {
    qua[qua_idx].sig_prv = sig_prv;
    qua[qua_idx].stat = *createNewStat(NN, nullptr);
    qua[qua_idx].sub_qua = nullptr;
    return &qua[qua_idx];
}

void parse_quation(const char *str, Quation *qua);

void make_final_fraction_optimized(Quation *qua);

void make_final_fraction_deprecated(Quation *qua);

void compute(char *rst);

char zeros[] = {'1',
                '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
                '0'};//1 + 20 + 1
char temp[MAX_N];
char up_cur[MAX_N], down_cur[MAX_N];
char integer_part[MAX_N];
char integer_part_abs[MAX_N] = "";// integer_part's absolutive value
char decimal_part[MAX_N];
char up_ret[MAX_N], down_ret[MAX_N];

void run(char *rst, const char *str) {
    root = getQuation(0, NN);
    parse_quation(str, root);
    make_final_fraction_optimized(root);
    compute(rst);
}

void parse_quation(const char *str, Quation *qua) {
    Quation *cur_qua = qua;
    Stat *tail_stat = &(cur_qua->stat);
    int len = my_strlen(str);
    for (int i = 0, sub_prv = 0, sub_qua_idx = 0; i < len; i++) {
        if (48 <= str[i] && str[i] <= 57) {// 0~9
            char ct[2];
            ct[0] = str[i];
            ct[1] = '\0';
            my_strcat(tail_stat->n, ct);
        } else if (str[i] == 42 || str[i] == 47) { // MUL * 42   DIV / 47
            if (str[i] == 42) {
                sub_prv = MUL;
            } else {// 47
                sub_prv = DIV;
            }
            Stat *new_stat = createNewStat(sub_prv, "");
            tail_stat->next = new_stat;
            tail_stat = new_stat;
        } else { // ADD + 43 SUB - 45
            sub_qua_idx++;
            if (str[i] == 43) {
                cur_qua->sub_qua = getQuation(sub_qua_idx, ADD);
            } else { // 45
                cur_qua->sub_qua = getQuation(sub_qua_idx, SUB);
            }
            cur_qua = cur_qua->sub_qua;
            tail_stat = &(cur_qua->stat);
        }
    }
}

void make_final_fraction_optimized(Quation *qua) {
    Quation *cur_qua = qua;
    Stat *cur_stat;
    int qua_sig_prv, stat_sig_prv;
    while (cur_qua != nullptr) {
        qua_sig_prv = cur_qua->sig_prv;
        cur_stat = &(cur_qua->stat);
        while (cur_stat != nullptr) {
            stat_sig_prv = cur_stat->sig_prv;
            if (stat_sig_prv == MUL) {
                mul(up_cur, up_cur, cur_stat->n);
            } else if (cur_stat->sig_prv == DIV) {
                mul(down_cur, down_cur, cur_stat->n);
            } else {
                my_strcpy(up_cur, cur_stat->n);
                my_strcpy(down_cur, "1");
            }
            cur_stat = cur_stat->next;
        }
        if (qua_sig_prv == ADD) {
            mul(up_ret, up_ret, down_cur);
            mul(up_cur, up_cur, down_ret);
            add(up_ret, up_ret, up_cur);
            mul(down_ret, down_ret, down_cur);
        } else if (qua_sig_prv == SUB) {
            mul(up_ret, up_ret, down_cur);
            mul(up_cur, up_cur, down_ret);
            sub(up_ret, up_ret, up_cur);
            mul(down_ret, down_ret, down_cur);
        } else {
            my_strcpy(up_ret, up_cur);
            my_strcpy(down_ret, down_cur);
        }
        cur_qua = cur_qua->sub_qua;
    }
}

void compute(char *rst) {
    // get result's integer part
    int int_len = 0;// before decimal dot integer_part len and it also is the dot_position
    if (up_ret[0] == '-') {// up_ret is negative
        mul(up_ret, up_ret, "-1");
        div(integer_part_abs, up_ret, down_ret);
        int_len = my_strlen(integer_part_abs) + 1;// 1 represent '-' signal
        if (integer_part_abs[0] == '0') {//up_ret absolutive value less than down_ret
            integer_part[0] = '-';
            integer_part[1] = '0';
            integer_part[2] = '\0';
        } else {
            for (int i = int_len; i > 0; i--) {
                integer_part[i] = integer_part_abs[i - 1];
            }
            integer_part[0] = '-';
        }
    } else {
        div(integer_part_abs, up_ret, down_ret);
        int_len = my_strlen(integer_part_abs);
        my_strcpy(integer_part, integer_part_abs);
    }
    // up_ret substract integer_part*down_ret, and remains result's decimal dividend
    mul(temp, down_ret, integer_part_abs);
    sub(up_ret, up_ret, temp);
    // get 0 numbers after decimal dot
    div(temp, down_ret, up_ret);
    int diff = my_strlen(temp);
    int zero_num_aft_dot = (diff > 1) ? diff - 1 : 0;
    // up_ret multiple 10^(20+1)
    mul(up_ret, up_ret, zeros);
    // check the available
    div(decimal_part, up_ret, down_ret);
    // check decimal_part[] index 19's available number 0 and 20 index is not 0, remain index 19 available number 0
    if (decimal_part[19] == '0' && decimal_part[20] != '0') {
        decimal_part[20] = '\0';
    } else {
        int z = (20) - 1;
        for (; z >= 0 && decimal_part[z] == '0'; z--) {
        }
        decimal_part[z + 1] = '\0';
    }
    // combine integer_part, decimal dot and decimal_part to rst
    int i = 0;
    for (; i < int_len + 1 + (20); i++) {
        if (i < int_len) {
            rst[i] = integer_part[i];
        } else if (i == int_len) { // check decimal dot
            if (decimal_part[0] != '0' && decimal_part[1] != '\0') {
                rst[i] = '.';
            } else {
                rst[i] = '\0';
                break;
            }
        } else if (i < int_len + 1 + zero_num_aft_dot) { // check zero after decimal dot
            rst[i] = '0';
        } else {
            rst[i] = decimal_part[i - int_len - 1 - zero_num_aft_dot];
        }
    }
    rst[i] = '\0';
}

void make_final_fraction_deprecated(Quation *qua) {
    Quation *cur_qua = qua;
    Stat *cur_stat = &(cur_qua->stat);
    my_strcpy(up_ret, "1");
    my_strcpy(down_ret, "1");
    int stat_sig_prv;
    int qua_sig_prv;
    do {
        my_strcpy(up_cur, "1");
        my_strcpy(down_cur, "1");
        qua_sig_prv = cur_qua->sig_prv;
        do {
            stat_sig_prv = cur_stat->sig_prv;
            if (stat_sig_prv == MUL || stat_sig_prv == NN) {
                mul(up_cur, up_cur, cur_stat->n);
            } else if (cur_stat->sig_prv == DIV) {
                mul(down_cur, down_cur, cur_stat->n);
            }
            cur_stat = cur_stat->next;

        } while (cur_stat != nullptr);
        if (qua_sig_prv == ADD) {
            mul(up_ret, up_ret, down_cur);
            mul(up_cur, up_cur, down_ret);
            add(up_ret, up_ret, up_cur);
            mul(down_ret, down_ret, down_cur);
        } else if (qua_sig_prv == SUB) {
            mul(up_ret, up_ret, down_cur);
            mul(up_cur, up_cur, down_ret);
            sub(up_ret, up_ret, up_cur);
            mul(down_ret, down_ret, down_cur);
        } else {
            my_strcpy(up_ret, up_cur);
            my_strcpy(down_ret, down_cur);
        }
        cur_qua = cur_qua->sub_qua;
        cur_stat = &(cur_qua->stat);
    } while (cur_qua != nullptr);
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

int numcmp(const char *a, const char *b) {
    if (a[0] != '-' && b[0] != '-') {
        int c = my_strlen(a);
        int d = my_strlen(b);
        if (c > d)
            return 1;
        if (c < d)
            return -1;
        do {
            if (*b < *a)
                return 1;
            if (*a < *b++)
                return -1;
        } while (*++a != 0);
    } else if (a[0] == '-' && b[0] == '-')
        return numcmp(&b[1], &a[1]);
    else if (a[0] == '-')
        return -1;
    else
        return 1;
    return 0;
}

void add(char *r, const char *a, const char *b) {
    if (a[0] != '-' && b[0] != '-') {
        if (0 <= numcmp(a, b)) {
            int tmp[MAX_N];
            int pos_a = my_strlen(a);
            int pos_b = my_strlen(b);
            int carry = 0;
            int len = 0;
            while (0 < pos_a) {
                int num1 = a[--pos_a] - '0';
                int num2 = (0 < pos_b) ? b[--pos_b] - '0' : 0;
                int num3 = carry + num1 + num2;
                tmp[len++] = num3 % 10;
                carry = num3 / 10;
            }
            if (0 < carry)
                tmp[len++] = 1;
            for (int i = 0; i < len; ++i)
                r[i] = tmp[len - 1 - i] + '0';
            r[len] = 0;
        } else
            add(r, b, a);
    } else if (a[0] == '-' && b[0] == '-') {
        add(&r[1], &a[1], &b[1]);
        r[0] = '-';
    } else if (a[0] == '-')
        sub(r, b, &a[1]);
    else
        sub(r, a, &b[1]);
}

void sub(char *r, const char *a, const char *b) {
    if (a[0] != '-' && b[0] != '-') {
        if (0 <= numcmp(a, b)) {
            int tmp[MAX_N];
            int pos_a = my_strlen(a);
            int pos_b = my_strlen(b);
            int carry = 0;
            int len = 0;
            while (0 < pos_a) {
                int num1 = a[--pos_a] - '0';
                int num2 = (0 < pos_b) ? b[--pos_b] - '0' : 0;
                int num3 = 10 - carry + num1 - num2;
                tmp[len++] = num3 % 10;
                carry = 1 - (num3 / 10);
            }
            while (1 < len && tmp[len - 1] == 0)
                --len;
            for (int i = 0; i < len; ++i)
                r[i] = tmp[len - 1 - i] + '0';
            r[len] = 0;
        } else {
            sub(&r[1], b, a);
            r[0] = '-';
        }
    } else if (a[0] == '-' && b[0] == '-')
        sub(r, &b[1], &a[1]);
    else if (a[0] == '-') {
        add(&r[1], &a[1], b);
        r[0] = '-';
    } else
        add(r, a, &b[1]);
}

void mul(char *r, const char *a, const char *b) {
    if (numcmp(a, "0") == 0 || numcmp(b, "0") == 0)
        my_strcpy(r, "0");
    else if (a[0] != '-' && b[0] != '-') {
        char cpy_a[MAX_N + 1];
        my_strcpy(cpy_a, a);
        my_strcpy(r, "0");
        for (int i = my_strlen(b) - 1; i >= 0; --i) {
            for (int j = 0; j < b[i] - '0'; ++j)
                add(r, r, cpy_a);
            my_strcat(cpy_a, "0");
        }
    } else if (a[0] == '-' && b[0] == '-')
        mul(r, &a[1], &b[1]);
    else {
        if (a[0] == '-')
            mul(&r[1], &a[1], b);
        else
            mul(&r[1], a, &b[1]);
        r[0] = '-';
    }
}

void div(char *r, const char *a, const char *b) {
    if (numcmp(a, "0") == 0 || numcmp(b, "0") == 0)
        my_strcpy(r, "0");
    else if (numcmp(((a[0] != '-') ? a : &a[1]), ((b[0] != '-') ? b : &b[1])) == -1)
        my_strcpy(r, "0");
    else if (a[0] != '-' && b[0] != '-') {
        char cpy_a[MAX_N + 1];
        char cpy_b[MAX_N + 1];
        char itr[MAX_N + 1];
        my_strcpy(cpy_a, a);
        my_strcpy(r, "");
        int c = (my_strlen(a) > my_strlen(b)) ? my_strlen(a) - my_strlen(b) : 1;
        for (int i = 0; i < c; ++i) {
            my_strcpy(cpy_b, b);
            for (int j = 0; j < c - i - 1; ++j)
                my_strcat(cpy_b, "0");
            my_strcpy(itr, "0");
            while (0 <= numcmp(cpy_a, "0")) {
                add(itr, itr, "1");
                sub(cpy_a, cpy_a, cpy_b);
            }
            sub(itr, itr, "1");
            add(cpy_a, cpy_a, cpy_b);
            my_strcat(r, itr);
        }
    } else if (a[0] == '-' && b[0] == '-')
        div(r, &a[1], &b[1]);
    else {
        if (a[0] == '-')
            div(&r[1], &a[1], b);
        else
            div(&r[1], a, &b[1]);
        r[0] = '-';
    }
}
