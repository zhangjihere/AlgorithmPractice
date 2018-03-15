//
// Created by zhangji on 3/14/18.
//

# include <malloc.h>

enum {
    MAX_N = 100
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
    struct Node *n1;
    struct Node *n2;
    struct Node *n3;
    struct Node *n4;
    int y, x, s;
    int ject;
};

extern int observe(int top, int left, int size);

int power(int x, int n);

int next_power_by_2(int x);

void create4Tree(int x, int y, int size, Node *node, int no_scope);

Node *create_Node(int y, int x, int s, int ject);

void checkTree4(int x, int y, int size, Node *node, int new_obv);

void init(int cell[MAX_N][MAX_N], int N);

RESULT infect();

RESULT recover();


RESULT result;
static int scope_edge = 1;
int odd_hist[100][100][100];
int case_side_len;
Node tree4;

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

Node *create_Node(int y, int x, int s, int ject) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode) {
        newNode->y = y;
        newNode->x = x;
        newNode->s = s;
        newNode->ject = ject;
    }
    return newNode;
}

void init(int cell[MAX_N][MAX_N], int N) {
    tree4.y = 0;
    tree4.x = 0;
    tree4.s = 0;
    tree4.ject = 0;
    tree4.n1 = nullptr;
    tree4.n2 = nullptr;
    tree4.n3 = nullptr;
    tree4.n4 = nullptr;

    result.count = 0;
//    for (int i = 0; i < MAX_N; i++) {
//        result.cell[i].x = -1;
//        result.cell[i].y = -1;
//    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            for (int k = 0; k < 100; k++) {
                odd_hist[i][j][k] = 0;
            }
        }
    }

    int x = 0, y = 0;
    case_side_len = N;
    scope_edge = next_power_by_2(case_side_len);
    int ject = observe(y, x, scope_edge);
    odd_hist[y][x][scope_edge] = ject;
    tree4.ject = ject;
    tree4.y = y;
    tree4.x = x;
    tree4.s = scope_edge;
    create4Tree(x, y, scope_edge, &tree4, 0);
    printf("%s", "create OK!");
}

void create4Tree(int x, int y, int size, Node *node, int no_scope) {
    int x1 = x, y1 = y;
    int x2 = (2 * x + size) / 2, y2 = y;
    int x3 = x, y3 = (2 * y + size) / 2;
    int x4 = (2 * x + size) / 2, y4 = (2 * y + size) / 2;
    size = size / 2;
    if (size == 0) {
        return;
    }

    if (x1 < case_side_len && y1 < case_side_len) {
        int o1 = 0, no_scope1 = no_scope;
        if (no_scope1 == 0) {
            o1 = observe(y1, x1, size);
            if (o1 == 0) {
                no_scope1 = 1;
            }
        }
        odd_hist[y1][x1][size] = o1;
        Node *node1 = create_Node(y1, x2, size, o1);
        node->n1 = node1;
        create4Tree(x1, y1, size, node1, no_scope1);
    }

    if (x2 < case_side_len && y2 < case_side_len) {
        int o2 = 0, no_scope2 = no_scope;
        if (no_scope2 == 0) {
            o2 = observe(y2, x2, size);
            if (o2 == 0) {
                no_scope2 = 1;
            }
        }
        odd_hist[y2][x2][size] = o2;
        Node *node2 = create_Node(y2, x2, size, o2);
        node->n2 = node2;
        create4Tree(x2, y2, size, node2, no_scope2);
    }

    if (x3 < case_side_len && y3 < case_side_len) {
        int o3 = 0, no_scope3 = no_scope;
        if (no_scope3 == 0) {
            o3 = observe(y3, x3, size);
            if (o3 == 0) {
                no_scope3 = 1;
            }
        }
        odd_hist[y3][x3][size] = o3;
        Node *node3 = create_Node(y3, x3, size, o3);
        node->n3 = node3;
        create4Tree(x3, y3, size, node3, no_scope3);
    }

    if (x4 < case_side_len && y4 < case_side_len) {
        int o4 = 0, no_scope4 = no_scope;
        if (no_scope4 == 0) {
            o4 = observe(y4, x4, size);
            if (o4 == 0) {
                no_scope4 = 1;
            }
        }
        odd_hist[y4][x4][size] = o4;
        Node *node4 = create_Node(y4, x4, size, o4);
        node->n4 = node4;
        create4Tree(x4, y4, size, node4, no_scope4);
    }

}

void checkTree4(int x, int y, int size, Node *node, int new_obv) {
    int x1 = x, y1 = y;
    int x2 = (2 * x + size) / 2, y2 = y;
    int x3 = x, y3 = (2 * y + size) / 2;
    int x4 = (2 * x + size) / 2, y4 = (2 * y + size) / 2;
    size = size / 2;

    int origin = node->ject;
    node->ject = new_obv;
    if (size == 0) {
        if (new_obv != origin) {
            result.cell[result.count].x = x;
            result.cell[result.count].y = y;
            result.count++;
        }
        return;
    }
    if (x1 < case_side_len && y1 < case_side_len) {
        int o1 = observe(y1, x1, size);
        if (odd_hist[y1][x1][size] != o1) {
            odd_hist[y1][x1][size] = o1;
            checkTree4(x1, y1, size, node->n1, o1);
        }
    }
    if (x2 < case_side_len && y2 < case_side_len) {
        int o2 = observe(y2, x2, size);
        if (odd_hist[y2][x2][size] != o2) {
            odd_hist[y2][x2][size] = o2;
            checkTree4(x2, y2, size, node->n2, o2);
        }
    }
    if (x3 < case_side_len && y3 < case_side_len) {
        int o3 = observe(y3, x3, size);
        if (odd_hist[y3][x3][size] != o3) {
            odd_hist[y3][x3][size] = o3;
            checkTree4(x3, y3, size, node->n3, o3);
        }
    }
    if (x4 < case_side_len && y4 < case_side_len) {
        int o4 = observe(y4, x4, size);
        if (odd_hist[y4][x4][size] != o4) {
            odd_hist[y4][x4][size] = o4;
            checkTree4(x4, y4, size, node->n4, o4);
        }
    }

}

RESULT infect() {
//    for (int i = 0; i < MAX_N; i++) {
//        result.cell[i].x = -1;
//        result.cell[i].y = -1;
//    }
    result.count = 0;
    
    int new_obv_ject = observe(0, 0, scope_edge);
    if (odd_hist[0][0][scope_edge] != new_obv_ject) {
        odd_hist[0][0][scope_edge] = new_obv_ject;
        checkTree4(0, 0, scope_edge, &tree4, new_obv_ject);
    }
    return result;
}


RESULT recover() {
//    for (int i = 0; i < MAX_N; i++) {
//        result.cell[i].x = -1;
//        result.cell[i].y = -1;
//    }
    result.count = 0;
    
    int new_obv_ject = observe(0, 0, scope_edge);
    if (odd_hist[0][0][scope_edge] != new_obv_ject) {
        odd_hist[0][0][scope_edge] = new_obv_ject;
        checkTree4(0, 0, scope_edge, &tree4, new_obv_ject);
    }

    return result;
}