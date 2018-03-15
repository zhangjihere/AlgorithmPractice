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
    int ject;
};

extern int observe(int top, int left, int size);

int power(int x, int n);

int next_power_by_2(int x);

int my_abs(int x);

void create4Tree(int x, int y, int size, Node *node, int no_scope);

Node *create_Node(int y, int x, int s, int ject);

int checkTree4(int x, int y, int size, Node *node, int new_obv);

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

Node *create_Node(int y, int x, int s, int ject) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode) {
        newNode->ject = ject;
    }
    return newNode;
}

void init(int cell[MAX_N][MAX_N], int N) {
    result.count = 0;

    target_size = N;
    scope_size = next_power_by_2(target_size);
    int ject = observe(0, 0, scope_size);
    tree4 = create_Node(0, 0, scope_size, ject);
    create4Tree(0, 0, scope_size, tree4, 0);
}

void create4Tree(int x, int y, int size, Node *node, int no_scope) {
    int x1 = x, y1 = y;
    int x3 = (2 * x + size) / 2, y3 = y;
    int x2 = x, y2 = (2 * y + size) / 2;
    int x4 = (2 * x + size) / 2, y4 = (2 * y + size) / 2;
    size = size / 2;
    if (size == 0) {
        return;
    }

    if (x1 < target_size && y1 < target_size) {
        int o1 = 0, no_scope1 = no_scope;
        if (no_scope1 == 0) {
            o1 = observe(y1, x1, size);
            if (o1 == 0) {
                no_scope1 = 1;
            }
        }
        Node *node1 = create_Node(y1, x2, size, o1);
        node->n1 = node1;
        create4Tree(x1, y1, size, node1, no_scope1);
    }

    if (x2 < target_size && y2 < target_size) {
        int o2 = 0, no_scope2 = no_scope;
        if (no_scope2 == 0) {
            o2 = observe(y2, x2, size);
            if (o2 == 0) {
                no_scope2 = 1;
            }
        }
        Node *node2 = create_Node(y2, x2, size, o2);
        node->n2 = node2;
        create4Tree(x2, y2, size, node2, no_scope2);
    }

    if (x3 < target_size && y3 < target_size) {
        int o3 = 0, no_scope3 = no_scope;
        if (no_scope3 == 0) {
            o3 = observe(y3, x3, size);
            if (o3 == 0) {
                no_scope3 = 1;
            }
        }
        Node *node3 = create_Node(y3, x3, size, o3);
        node->n3 = node3;
        create4Tree(x3, y3, size, node3, no_scope3);
    }

    if (x4 < target_size && y4 < target_size) {
        int o4 = 0, no_scope4 = no_scope;
        if (no_scope4 == 0) {
            o4 = observe(y4, x4, size);
            if (o4 == 0) {
                no_scope4 = 1;
            }
        }
        Node *node4 = create_Node(y4, x4, size, o4);
        node->n4 = node4;
        create4Tree(x4, y4, size, node4, no_scope4);
    }

}
// 1 3 2 4 better than 1 4 3 2, better than 1 2 3 4
int checkTree4(int x, int y, int size, Node *node, int new_obv) {
    int x1 = x, y1 = y;
    int x3 = (2 * x + size) / 2, y3 = y;
    int x2 = x, y2 = (2 * y + size) / 2;
    int x4 = (2 * x + size) / 2, y4 = (2 * y + size) / 2;
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

    int t_s = 0;
    int last_t_s = 0;
    int d_s = my_abs(new_obv - origin);

    if (x1 < target_size && y1 < target_size) {
        int o1 = 0;
        if (t_s != d_s) {
            if (size == 1) {
                o1 = observe(y1, x1, size);
                if (node->n1->ject != o1) {
                    last_t_s = checkTree4(x1, y1, size, node->n1, o1);
                    t_s += last_t_s;
                }
            } else {
                o1 = observe(y1, x1, size);
                if (node->n1->ject != o1) {
                    t_s += my_abs(o1 - node->n1->ject);
                    checkTree4(x1, y1, size, node->n1, o1);
                }
            }
        }
    }
    if (x2 < target_size && y2 < target_size) {
        int o2 = 0;
        if (size == 1) {
            if (t_s != d_s) {
                o2 = observe(y2, x2, size);
                if (node->n2->ject != o2) {
                    last_t_s = checkTree4(x2, y2, size, node->n2, o2);
                    t_s += last_t_s;
                }
            }
        } else {
            if (t_s != d_s) {
                o2 = observe(y2, x2, size);
                if (node->n2->ject != o2) {
                    t_s += my_abs(o2 - node->n2->ject);
                    checkTree4(x2, y2, size, node->n2, o2);
                }
            }
        }
    }
    if (x3 < target_size && y3 < target_size) {
        int o3 = 0;
        if (t_s != d_s) {
            if (size == 1) {
                o3 = observe(y3, x3, size);
                if (node->n3->ject != o3) {
                    last_t_s = checkTree4(x3, y3, size, node->n3, o3);
                    t_s += last_t_s;
                }
            } else {
                o3 = observe(y3, x3, size);
                if (node->n3->ject != o3) {
                    t_s += my_abs(o3 - node->n3->ject);
                    checkTree4(x3, y3, size, node->n3, o3);
                }
            }
        }
    }
    if (x4 < target_size && y4 < target_size) {
        int o4 = 0;
        if (t_s != d_s) {
            if (size == 1) {
                o4 = observe(y4, x4, size);
                if (node->n4->ject != o4) {
                    last_t_s = checkTree4(x4, y4, size, node->n4, o4);
                    t_s += last_t_s;
                }
            } else {
                o4 = observe(y4, x4, size);
                if (node->n4->ject != o4) {
                    t_s += my_abs(o4 - node->n4->ject);
                    checkTree4(x4, y4, size, node->n4, o4);
                }
            }
        }
    }
    return 0;
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