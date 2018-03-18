//
// Created by zhangji on 3/14/18.
//

#ifndef __clang__
# include <malloc.h>
#else

# include <cstdlib>

#endif

enum {
    MAX_N = 100,
    TR_B = 4
};

typedef struct {
    int x;
    int y;
} Cell;

typedef struct {
    int count;
    Cell cell[MAX_N];
} RESULT;

struct Node {
    struct Node *branch[TR_B];
    int ject;
};

extern int observe(int top, int left, int size);

int power(int x, int n);

int next_power_by_2(int x);

int my_abs(int x);

void create4Tree(int x, int y, int size, Node *node, int child_branch_ignore);

Node *create_Node(int ject);

int checkTree4(int x, int y, int size, Node *node, int new_obv);

void compute4Coordinate(int x, int y, int size, int left_top[][2]);

void init(int cell[MAX_N][MAX_N], int N);

RESULT infect();

RESULT recover();

RESULT process();


RESULT result;
static int scope_size;
int target_size;
Node *tree4;


int power(int x, int n) {
    int result;
    if (n == 1) {
        return x;
    }
    if (n % 2 == 0) {
        result = power(x, n / 2) * power(x, n / 2);
    } else
        result = power(x, (n + 1) / 2) * power(x, (n - 1) / 2);
    return result;
}

int next_power_by_2(int x) {
    if (x == 1) {
        return 1;
    }
    int ret = 1;
    int s = x;
    while (s > 1) {
        s = s >> 1;
        ret = ret * 2;
    }
    if (ret != x)
        ret = ret * 2;
    return ret;
}

int my_abs(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

Node *create_Node(int ject) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode) {
        newNode->ject = ject;
    }
    return newNode;
}

// no_scope is 1 means , the branch's child branches all don't need to call observe(y,x,size) function.
void create4Tree(int x, int y, int size, Node *node, int child_branch_ignore) {
    int left_top[TR_B][2];
    compute4Coordinate(x, y, size, left_top);
    size = size / 2;
    if (size == 0) {
        return;
    }

    for (int i = 0; i < TR_B; i++) {
        if (left_top[i][0] < target_size && left_top[i][1] < target_size) { //optim
            int obv = 0, child_branch_ignore_i = child_branch_ignore;
            if (child_branch_ignore_i == 0) {  //optim
                obv = observe(left_top[i][1], left_top[i][0], size);
                if (obv == 0) {
                    child_branch_ignore_i = 1;
                }
            }
            Node *new_node = create_Node(obv);
            node->branch[i] = new_node;
            create4Tree(left_top[i][0], left_top[i][1], size, new_node, child_branch_ignore_i);
        }
    }

}

void compute4Coordinate(int x, int y, int size, int left_top[][2]) {
    left_top[0][0] = x, left_top[0][1] = y;
    left_top[2][0] = (2 * x + size) / 2, left_top[2][1] = y;
    left_top[1][0] = x, left_top[1][1] = (2 * y + size) / 2;
    left_top[3][0] = (2 * x + size) / 2, left_top[3][1] = (2 * y + size) / 2;
}

// 1 3 2 4 better than 1 4 3 2, better than 1 2 3 4
int checkTree4(int x, int y, int size, Node *node, int new_obv) {
    int left_top[TR_B][2];
    compute4Coordinate(x, y, size, left_top);
    size = size / 2;

    int origin = node->ject;
    node->ject = new_obv;
    if (size == 0) {
        if (new_obv != origin) {
            result.cell[result.count].x = x;
            result.cell[result.count].y = y;
            result.count++;
            return 1;
        }
        return 0;
    }

    int t_s = 0;        // stat scope square
    int last_t_s = 0;   // stat last single cell
    int d_s = my_abs(new_obv - origin);

    for (int i = 0; i < TR_B; i++) {
        if (left_top[i][0] < target_size && left_top[i][1] < target_size) { //optim
            int obv = 0;
            if (t_s != d_s) { //optim
                if (size == 1) {
                    obv = observe(left_top[i][1], left_top[i][0], size);
                    if (node->branch[i]->ject != obv) { //optim
                        last_t_s = checkTree4(left_top[i][0], left_top[i][1], size, node->branch[i], obv);
                        t_s += last_t_s;
                    }
                } else {
                    obv = observe(left_top[i][1], left_top[i][0], size);
                    if (node->branch[i]->ject != obv) {
                        t_s += my_abs(obv - node->branch[i]->ject);
                        checkTree4(left_top[i][0], left_top[i][1], size, node->branch[i], obv);
                    }
                }
            }
        }
    }
    return 0;
}

void init(int cell[MAX_N][MAX_N], int N) {
    result.count = 0;

    target_size = N;
    scope_size = next_power_by_2(target_size);
    int ject = observe(0, 0, scope_size);
    tree4 = create_Node(ject);
    create4Tree(0, 0, scope_size, tree4, 0);
}

RESULT infect() {
    return process();
}

RESULT recover() {
    return process();
}

RESULT process() {
    result.count = 0;

    int new_obv_ject = observe(0, 0, scope_size);
    if (tree4->ject != new_obv_ject) {
        checkTree4(0, 0, scope_size, tree4, new_obv_ject);
    }
    return result;
}