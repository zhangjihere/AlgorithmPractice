//
// Created by zhangji on 3/20/18.
//
#ifndef __clang__

// Maybe thie can speed up waste time more than several hundreds miliseconds
//#pragma GCC optimize ("-Ofast")

# include <malloc.h>

#else

# include <cstdlib>

#endif

#define STRINGTYPE         0x01
#define SETTYPE            0x02

#define MAX_KEY_LENGTH     15
#define MAX_VALUE_LENGTH   127
#define MAX_MESSAGE_LENGTH 81920

typedef struct entity_t {
    char type;
    char key_len_high;
    char key_len_low;
    char key_val[MAX_KEY_LENGTH];

    char val_len_high;
    char val_len_low;
    char val_val[MAX_VALUE_LENGTH];

    entity_t *sub_ent[MAX_VALUE_LENGTH];
    entity_t *parent_ent;

} Entity;

Entity *root;

Entity *createNewEntity(Entity *parent_ent);

int assemble(Entity *ent, char *in, int offset);

Entity *search_entity(Entity *ent, char *targetkey);

int output(char *out, int offset, const char *src, int n);

int search_n_generate(Entity *ent, char *out, int offset);


//the below functions are provided for your convenience
const char hex[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

int printHex(const char message[], const int length, char *out, int offset) {
    for (int i = 0; i < length; i += 16) {
        for (int j = 0; j < 16 && i + j < length; ++j) {
            out[offset++] = hex[message[i + j] >> 4];
            out[offset++] = hex[message[i + j] & 0x0f];
//			printf("%c%c ", hex[message[i + j] >> 4], hex[message[i + j] & 0x0f]);
//		printf("\n");
        }
    }
    return offset;
}

int ztrncpy(char dest[], const char src[], const int offset, const int length) {
    int pos = 0;
    while (pos < length && src[pos + offset] != '\0') {
        dest[pos] = src[pos + offset];
        ++pos;
    }
    dest[pos] = '\0';
    return pos;
}

int ztrncpy_append(char dest[], const int offset, const char src[], const int length) {
    int pos = 0;
    while (pos < length && src[pos] != '\0') {
        dest[offset + pos] = src[pos];
        ++pos;
    }
    dest[offset + pos] = '\0';
    return offset + pos;
}

int ztrlen(const char str[]) {
    int pos = 0;
    while (str[pos] != '\0')
        ++pos;
    return pos;
}

int equalStr(const char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0' && dest[i] != '\0') {
        if (src[i] != dest[i])
            return 0;
        i++;
    }
    if (src[i] == '\0' && dest[i] == '\0')
        return 1;
    return 0;
}

int chars2int(const char c1, const char c2) {
    return (c1 - '0') * 256 + (c2 - '0');
}

Entity *createNewEntity(Entity *parent_ent) {
    auto *new_entity = (Entity *) malloc(sizeof(Entity));
    if (new_entity != nullptr) {
        new_entity->parent_ent = parent_ent;
    }
    return new_entity;
}

//100
void parse(char in[MAX_MESSAGE_LENGTH], int size) { // 100
    root = createNewEntity(nullptr);
    assemble(root, in, 0);
}

int assemble(Entity *ent, char *in, int offset) {
    int type = in[offset];
    if (type == STRINGTYPE) {
        ent->type = in[offset];
        ent->key_len_high = in[offset + 1];
        ent->key_len_low = in[offset + 2];
        ztrncpy(ent->key_val, in, offset + 1 + 2, ent->key_len_low);
        ent->val_len_high = in[offset + 1 + 2 + ent->key_len_low];
        ent->val_len_low = in[offset + 1 + 2 + ent->key_len_low + 1];
        ztrncpy(ent->val_val, in, offset + 1 + 2 + ent->key_len_low + 1 + 1, ent->val_len_low);
        
        return (offset + 1 + 2 + ent->key_len_low + 1 + 1) + ent->val_len_low;
    }
    if (type == SETTYPE) {
        ent->type = in[offset];
        ent->key_len_high = in[offset + 1];
        ent->key_len_low = in[offset + 2];
        int set_first_3_offset = offset + 1 + 2;
        ztrncpy(ent->key_val, in, set_first_3_offset, ent->key_len_low);
        ent->val_len_high = in[set_first_3_offset + ent->key_len_low];
        ent->val_len_low = in[set_first_3_offset + ent->key_len_low + 1];

        int set_key_last_pos = (set_first_3_offset + ent->key_len_low + 1) + 1;
        for (int i = 0; i < ent->val_len_low; i++) {
            Entity *new_entity = createNewEntity(ent);
            ent->sub_ent[i] = new_entity;
            set_key_last_pos = assemble(new_entity, in, set_key_last_pos);
        }
        return set_key_last_pos;
    }
    return 0;
}

//200
void set(char targetkey[MAX_KEY_LENGTH + 1], char newvalue[MAX_VALUE_LENGTH + 1]) {
    Entity *ent = search_entity(root, targetkey);
    if (ent != nullptr) {
        int num = ztrncpy(ent->val_val, newvalue, 0, 128);
        ent->val_len_low = static_cast<char>(num);
    }
}

//300
void add(char parentkey[MAX_KEY_LENGTH + 1], char childkey[MAX_KEY_LENGTH + 1], char childvalue[MAX_VALUE_LENGTH + 1]) {
    Entity *parent_entity = search_entity(root, parentkey);
    if (parent_entity != nullptr) {
        Entity *new_child_entity = createNewEntity(parent_entity);
        new_child_entity->type = 0x01;
        new_child_entity->key_len_high = 0x00;
        new_child_entity->key_len_low = static_cast<char>(ztrlen(childkey));
        ztrncpy(new_child_entity->key_val, childkey, 0, 128);
        new_child_entity->val_len_high = 0x00;
        new_child_entity->val_len_low = static_cast<char>(ztrlen(childvalue));;
        ztrncpy(new_child_entity->val_val, childvalue, 0, 128);

        parent_entity->sub_ent[parent_entity->val_len_low] = new_child_entity;
        parent_entity->val_len_low++;
    }
}

//400
void erase(char targetkey[MAX_KEY_LENGTH + 1]) {
    Entity *ent = search_entity(root, targetkey);
    if (ent != nullptr) {
        Entity *parent_ent = ent->parent_ent;
        int pos = 0;
        for (; pos < parent_ent->val_len_low; pos++) {
            if (parent_ent->sub_ent[pos] == ent) {
                parent_ent->sub_ent[pos] = nullptr;
                break;
            }
        }
        // shiftup sub_ent array, complemt delete positon entity 
        for (; pos < parent_ent->val_len_low - 1; pos++) {
            parent_ent->sub_ent[pos] = parent_ent->sub_ent[pos + 1];
        }
        parent_ent->sub_ent[pos] = nullptr;

        parent_ent->val_len_low--;
    }
}


//500
int generate(char targetkey[MAX_KEY_LENGTH + 1], char out[MAX_MESSAGE_LENGTH]) {
    Entity *ent = search_entity(root, targetkey);
    int size = search_n_generate(ent, out, 0);
    return size;  // return the size of the encoded IoT Message which is stored in the array of out. 
}

Entity *search_entity(Entity *ent, char *targetkey) {
    int type = ent->type;
    if (type == STRINGTYPE) {
        if (equalStr(ent->key_val, targetkey) == 1) {
            return ent;
        }
    }
    if (type == SETTYPE) {
        if (equalStr(ent->key_val, targetkey) == 1) {
            return ent;
        } else {
            Entity *sub_ent = nullptr;
            for (int i = 0; i < ent->val_len_low; i++) {
                sub_ent = search_entity(ent->sub_ent[i], targetkey);
                if (sub_ent != nullptr) {
                    return sub_ent;
                }
            }
            return sub_ent;
        }
    }
}

int search_n_generate(Entity *ent, char *out, int offset) {
    int type = ent->type;
    offset = output(out, offset, &ent->type, 1);
    offset = output(out, offset, &ent->key_len_high, 1);
    offset = output(out, offset, &ent->key_len_low, 1);
    offset = output(out, offset, ent->key_val, ent->key_len_low);
    offset = output(out, offset, &ent->val_len_high, 1);
    offset = output(out, offset, &ent->val_len_low, 1);

    if (type == STRINGTYPE) {
        offset = output(out, offset, ent->val_val, ent->val_len_low);
        return offset;
    }
    if (type == SETTYPE) {
        for (int i = 0; i < ent->val_len_low; i++) {
            Entity *sub_ent = ent->sub_ent[i];
            offset = search_n_generate(sub_ent, out, offset);
        }
        return offset;
    }
}

int output(char *out, int offset, const char *src, int n) {
    for (int i = 0; i < n; i++, offset++) {
        out[offset] = src[i];
    }
    return offset;
}
