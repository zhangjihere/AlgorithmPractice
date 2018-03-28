//
// Created by zhangji on 3/16/18.
//

#ifndef __clang__

//#pragma GCC optimize ("-Ofast")

# include <malloc.h>

#else

# include <cstdlib>

#endif

# define MAX_NODE       10000
enum {
    DR = 1,
    FL = 2
};
enum {
    ADD = 1,
    SUB = -1
};
enum {
    INFECT = 1,
    RECOVER = -1
};


struct Node {
    int id;
    int type;
    int file_size;

    int total_f_cnt;
    int total_f_size;

    int origin_f_size;

    Node *parent;
    Node *prev_bro;
    Node *next_bro;
    Node *first_chd;
};


int make_hash(int id);

Node *get_node(int id, int fileSize);

Node *find_node(Node *_node, int *id);

void *update_file_info(Node *node, int symbol);

void connect_to_new_parent(Node *src_node, Node *tgt_node);

void inject_node(Node *node, int infect_size);

void recover_node(Node *node);

void file_change_until_parents(Node *node, int change_size, int change_type);

Node *remove_node(int &id);


Node root;
int case_time;
int hash_table[MAX_NODE];
Node node_pool[MAX_NODE];

int make_hash(int id) {
    int hash = id / MAX_NODE;
    do {
        if (hash == MAX_NODE) {
            hash = 0;
        }
        if (hash_table[hash] != case_time) {
            hash_table[hash] = case_time;
            return hash;
        }
        hash++;
    } while (hash > 0);
}

Node *get_node(int id, int fileSize) {
    Node *node = &node_pool[make_hash(id)];
//    auto node = (Node *) malloc(sizeof(Node));
    node->id = id;
    if (fileSize == 0) {
        node->type = DR;
    } else {
        node->type = FL;
    }
    node->file_size = fileSize;
    node->total_f_cnt = 0;
    node->total_f_size = 0;
    node->origin_f_size = fileSize;
    node->parent = nullptr;
    node->prev_bro = nullptr;
    node->next_bro = nullptr;
    node->first_chd = nullptr;
    return node;
}

void init(void) {
    case_time++;
    root.id = MAX_NODE;
    root.type = DR;
    root.first_chd = nullptr;
    root.file_size = 0;
    root.total_f_size = 0;
    root.total_f_cnt = 0;
    root.origin_f_size = 0;
}

Node *find_node(Node *_node, int *id) {
    if (*id == _node->id) {
        return _node;
    }
    Node *ret_node;
    if (_node->next_bro != nullptr) {
        ret_node = find_node(_node->next_bro, id);
        if (ret_node != nullptr) {
            return ret_node;
        }
    }
    if (_node->first_chd != nullptr) {
        ret_node = find_node(_node->first_chd, id);
        if (ret_node != nullptr) {
            return ret_node;
        }
    }
    return nullptr;
}

void *update_file_info(Node *node, int symbol) {
    int tf_size, tf_cnt, origin_f_size;
    origin_f_size = node->origin_f_size;
    if (node->type == DR) {
        tf_size = symbol * node->total_f_size;
        tf_cnt = symbol * node->total_f_cnt;
    } else { // node->type == FL
        tf_size = symbol * node->file_size;
        tf_cnt = symbol * 1;
    }
    Node *p_node = node->parent;
    while (p_node != nullptr) {
        p_node->total_f_cnt += tf_cnt;
        p_node->total_f_size += tf_size;
        p_node->origin_f_size += origin_f_size;
        p_node = p_node->parent;
    }
}

// 1
int add(int id, int pid, int fileSize) {
    Node *p_node = find_node(&root, &pid);
    if (p_node == nullptr) {
        return 0;
    }
    Node *new_node = get_node(id, fileSize);
    connect_to_new_parent(new_node, p_node);
    return p_node->total_f_size;
}

// 2
int move(int id, int pid) {
    Node *node = remove_node(id);
    Node *tgt_node = find_node(&root, &pid);
    connect_to_new_parent(node, tgt_node);
    return tgt_node->total_f_size;
}

void connect_to_new_parent(Node *src_node, Node *tgt_node) {
    Node *old_first_child = tgt_node->first_chd;
    tgt_node->first_chd = src_node;
    src_node->parent = tgt_node;
    src_node->prev_bro = nullptr;
    if (old_first_child != nullptr) {
        src_node->next_bro = old_first_child;
        old_first_child->prev_bro = src_node;
    }
    update_file_info(src_node, ADD);
}

// node should be child node
void inject_node(Node *node, int infect_size) {
    if (node->type == FL) {
        file_change_until_parents(node, infect_size, INFECT);
    }
    if (node->next_bro != nullptr) {
        inject_node(node->next_bro, infect_size);
    }
    if (node->first_chd != nullptr) {
        inject_node(node->first_chd, infect_size);
    }
}

//3
int infect(int id) {
    int infect_size = root.total_f_cnt != 0 ? root.total_f_size / root.total_f_cnt : 0;
    Node *node = find_node(&root, &id);
    if (infect_size != 0) {
        if (node->type == FL) {
            file_change_until_parents(node, infect_size, INFECT);
        } else {
            if (node->first_chd != nullptr) {
                inject_node(node->first_chd, infect_size);
            }
        }
        if (node->type == FL) {
            return node->file_size;
        } else {
            return node->total_f_size;
        }
    } else {
        return 0;
    }
}

// node should be child node
void recover_node(Node *node) {
    if (node->type == FL) {
        int recover_size = node->file_size - node->origin_f_size;
        file_change_until_parents(node, recover_size, RECOVER);
    }
    if (node->next_bro != nullptr) {
        recover_node(node->next_bro);
    }
    if (node->first_chd != nullptr) {
        recover_node(node->first_chd);
    }
}

//4
int recover(int id) {
    Node *node = find_node(&root, &id);
    if (node->type == FL) {
        if (node->origin_f_size != node->file_size) {
            int recover_size = node->file_size - node->origin_f_size;
            file_change_until_parents(node, recover_size, RECOVER);
        }
    } else {
        if (node->origin_f_size != node->total_f_size) {
            if (node->first_chd != nullptr) {
                recover_node(node->first_chd);
            }
        }
    }
    if (node->type == FL) {
        return node->file_size;
    } else {
        return node->total_f_size;
    }
}

void file_change_until_parents(Node *node, int change_size, int change_type) {
    node->file_size += change_type * change_size;
    Node *p_node = node->parent;
    while (p_node != nullptr) {
        p_node->total_f_size += change_type * change_size;
        p_node = p_node->parent;
    }
}

//5
int remove(int id) {
    Node *src_node = remove_node(id);
    if (src_node->type == DR) {
        return src_node->total_f_size;
    } else {
        return src_node->file_size;
    }
}

Node *remove_node(int &id) {
    Node *src_node = find_node(&root, &id);
    Node *src_parent_n = src_node->parent;
    Node *src_prev_bro = src_node->prev_bro;
    Node *src_next_bro = src_node->next_bro;
    if (src_prev_bro == nullptr) { // src_node is fisrt child
        src_parent_n->first_chd = src_next_bro;
        if (src_next_bro != nullptr) {
            src_next_bro->prev_bro = nullptr;
        }
    } else if (src_next_bro == nullptr) {
        src_prev_bro->next_bro = src_next_bro;
    } else {
        src_prev_bro->next_bro = src_next_bro;
        src_next_bro->prev_bro = src_prev_bro;
    }
    src_node->prev_bro = nullptr;
    src_node->next_bro = nullptr;
    update_file_info(src_node, SUB);
    return src_node;
}
