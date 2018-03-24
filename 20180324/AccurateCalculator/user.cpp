#define MAX_N 1000

#include <malloc.h>

extern int my_strlen(const char *a);

extern void my_strcpy(char *a, const char *b);

extern int my_strcmp(const char *a, const char *b);

extern char *my_strcat(char *a, const char *b);

int numcmp(const char *a, const char *b);

void add(char *r, const char *a, const char *b);

void sub(char *r, const char *a, const char *b);

void mul(char *r, const char *a, const char *b);

void div(char *r, const char *a, const char *b);

void process(char *rst);

enum {
    NN = 0, ADD = 1, SUB = 2, MUL = 3, DIV = 4
};

struct Stat {
    int sig_prv;
    char n[500];
    Stat *next;
};

struct Quation {
    int sig_prv;
    Quation *sub_qua;
    Stat stat;
};

struct Quation qua[300];
int qua_idx;

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

char up_cur[100];
char down_cur[100];

char up_ret[100];
char down_ret[100];
char integer_part[100];
char decimal[100];
char zeros[21];

void run(char *rst, const char *str) {
    qua_idx = 0;
    int equ_len = my_strlen(str);
    Quation *cur_qua = getQuation(qua_idx, NN);
    root = cur_qua;
    Stat *tail_stat = &root->stat;
    for (int i = 0, sub_prv; i < equ_len; i++) {
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
            Stat *new_stat = createNewStat(sub_prv, '\0');
            tail_stat->next = new_stat;
            tail_stat = new_stat;
        } else { // ADD + 43 SUB - 45
            qua_idx++;
            if (str[i] == 43) {
                cur_qua->sub_qua = getQuation(qua_idx, ADD);
            } else { // 45
                cur_qua->sub_qua = getQuation(qua_idx, SUB);
            }
            cur_qua = cur_qua->sub_qua;
            tail_stat = &cur_qua->stat;
        }
    }

    cur_qua = root;
    Stat *cur_stat = &cur_qua->stat;
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
        cur_stat = &cur_qua->stat;
    } while (cur_qua != nullptr);
    process(rst);
}

void process(char *rst) {
    int dot_pos = 0;
    for (; dot_pos < MAX_N && up_ret[dot_pos] != '\0'; dot_pos++) {
    }

    div(integer_part, up_ret, down_ret);
    int int_len = 0;
    for (; int_len < MAX_N && integer_part[int_len] != '\0'; int_len++) {
    }

    mul(decimal, down_ret, integer_part);
    sub(up_ret, up_ret, decimal);

    zeros[0] = '1';
    int k = 1;
    for (; k <= 20 + 1; k++) {
        zeros[k] = '0';
    }
    zeros[k] = '\0';
    mul(up_ret, up_ret, zeros);

    div(decimal, up_ret, down_ret);
    if (!(decimal[18] != '0'
          && decimal[19] == '0'
          && decimal[20] != '0')) { // check decimal[] idx 19 is available number 0, shouldn't be clear. 
        int z = (20) - 1;
        for (; z >= 0 && decimal[z] == '0'; z--) {
        }
        decimal[z + 1] = '\0';
    }

    int i = 0;
    for (; i < int_len + 1 + (20 + 1); i++) {
        if (i < int_len) {
            rst[i] = integer_part[i];
        } else if (i == int_len) {
            if (decimal[0] != '0') {
                rst[i] = '.';
            } else {
                rst[i] = '\0';
                break;
            }
        } else {
            rst[i] = decimal[(i - int_len) - 1];
        }
    }
    rst[i] = '\0';
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
