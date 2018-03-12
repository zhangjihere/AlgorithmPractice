//
// Created by zhangji on 3/12/18.
//

#include <stdio.h>

extern int CalculateRelation(char *src, char *dst);



struct SInfo {
    char first[11];
    char second[11];
    char third[11];
    char fourth[11];
    char fifth[11];
};

struct Entry {
    struct SInfo info;
    struct Entry *next;
    int valid;
};

unsigned long getHash(const SInfo &info, unsigned long ret);
void set_empty(char *comp);

const static int COMP_MAX_LEN = 11;
const static int HASH = 17;
const static int HASHTABLE_SIZE = 1009;
struct Entry db[HASHTABLE_SIZE];


void set_empty(char *comp) {
    for (int i = 0; i < COMP_MAX_LEN; i++) {
        comp[i] = '\0';
    }
}

void init() {
    for (int i = 0; i < HASHTABLE_SIZE; i++) {
        set_empty(db[i].info.first);
        set_empty(db[i].info.second);
        set_empty(db[i].info.third);
        set_empty(db[i].info.fourth);
        set_empty(db[i].info.fifth);
        db[i].next = nullptr;
        db[i].valid = 0;
    }
}

void AddDB(SInfo info) {
    unsigned long ret = getHash(info, ret);
    int pos = ret % 1009;
    if (db[pos].valid == 0) {
        db[pos].info = info;
        db[pos].valid = 1;
    } else {
        struct Entry nextE = db[pos];
        while (nextE.next != nullptr) {
            nextE = *(nextE.next);
        }
        static struct Entry temp;
        temp.info = info;
        temp.next = nullptr;
        temp.valid = 1;
        nextE.next = &temp;
    }

}

unsigned long getHash(const SInfo &info, unsigned long ret) {
    for (int i = 0; i < COMP_MAX_LEN && info.first[i] != '\0'; i++) {
        ret = ret * HASH + info.first[i];
    }
    for (int i = 0; i < COMP_MAX_LEN && info.second[i] != '\0'; i++) {
        ret = ret * HASH + info.second[i];
    }
    for (int i = 0; i < COMP_MAX_LEN && info.third[i] != '\0'; i++) {
        ret = ret * HASH + info.third[i];
    }
    for (int i = 0; i < COMP_MAX_LEN && info.fourth[i] != '\0'; i++) {
        ret = ret * HASH + info.fourth[i];
    }
    for (int i = 0; i < COMP_MAX_LEN && info.fifth[i] != '\0'; i++) {
        ret = ret * HASH + info.fifth[i];
    }
    return ret;
}

int SearchBestCase(SInfo info) {
//    printf("%d,%s", 1, "SBC");
    return 0;
}