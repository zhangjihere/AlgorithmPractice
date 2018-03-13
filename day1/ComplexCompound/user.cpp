//
// Created by zhangji on 3/12/18.
//

#include <malloc.h>

extern int CalculateRelation(char *src, char *dst);


const static int COMP_SIG_MAX_LEN = 11;
const static int COMP_MAX_NUM = 5000;
const static int COMP_MAX_SCORE = 500;
const static int HASH = 13;
const static int HASHTABLE_SIZE = 1009;

struct SInfo {
    char first[11];
    char second[11];
    char third[11];
    char fourth[11];
    char fifth[11];
};

struct Entry {
    char str[COMP_SIG_MAX_LEN];
    int db_idx;
    struct Entry *next;
    int valid;
};

void add2Bucket(char *str, struct Entry *bucket, int db_idx);

void set_empty(struct Entry *entry);

void copyStr(char *dest, const char *src);

unsigned long getHashcode(const char *str);

int equalStr(const char *str1, const char *base);

int computeScore(SInfo &info, int comp_max_score, char *comp_str, struct Entry *bucket);

struct Entry *create_Entry(int db_idx, int valid);

void destroy_Entry(struct Entry *entry);

struct Entry str1_bucket[HASHTABLE_SIZE];
struct Entry str2_bucket[HASHTABLE_SIZE];
struct Entry str3_bucket[HASHTABLE_SIZE];
struct Entry str4_bucket[HASHTABLE_SIZE];
struct Entry str5_bucket[HASHTABLE_SIZE];

struct SInfo comp_db[COMP_MAX_NUM];
int db_idx = 0;

void set_empty(struct Entry *entry) {
    for (int i = 0; i < COMP_SIG_MAX_LEN; i++) {
        entry->str[i] = '\0';
    }
    entry->db_idx = 0;
    entry->valid = 0;
    entry->next = 0;
//    struct Entry *temp = 0;
//    while (entry->next) {
//        temp = entry;
//        entry = entry->next;
//        destroy_Entry(temp);
//    }
}

void copyStr(char *dest, const char *src) {
    for (int i = 0; i < COMP_SIG_MAX_LEN; i++) {
        dest[i] = src[i];
    }
}

int equalStr(const char *str1, const char *base) {
    for (int i = 0; i < COMP_SIG_MAX_LEN; i++) {
        if (base[i] == '\0') {
            return 1;// equals
        } else {
            if (base[i] == str1[i]) {
                continue;
            }
            return -1;// not equals
        }
    }
}

struct Entry *create_Entry(int db_idx, int valid) {
    struct Entry *newEntry = (struct Entry *) malloc(sizeof(struct Entry));
    if (newEntry) {
        newEntry->db_idx = db_idx;
        newEntry->valid = valid;
    }
    return newEntry;
}

void destroy_Entry(struct Entry *entry) {
    if (entry) {
        free(entry);
    }
}

void add2Bucket(char *str, struct Entry *bucket, int db_idx) {
    unsigned long ret = getHashcode(str);
    unsigned long pos = ret % HASHTABLE_SIZE;
    if (bucket[pos].valid == 0) {
        copyStr(bucket[pos].str, str);
        bucket[pos].db_idx = db_idx;
        bucket[pos].valid = 1;
    } else {
        struct Entry *new_entry = create_Entry(0, 0);
        copyStr(new_entry->str, str);
        new_entry->db_idx = db_idx;
        new_entry->valid = 1;
        new_entry->next = bucket[pos].next;
        bucket[pos].next = new_entry;
    }
}

unsigned long getHashcode(const char *str) {
    unsigned long ret = 0;
    for (int i = 0; i < COMP_SIG_MAX_LEN && str[i] != '\0'; i++) {
        ret = ret * HASH + str[i];
    }
    return ret;
}

void init() {
    for (int i = 0; i < HASHTABLE_SIZE; i++) {
        set_empty(&str1_bucket[i]);
        set_empty(&str2_bucket[i]);
        set_empty(&str3_bucket[i]);
        set_empty(&str4_bucket[i]);
        set_empty(&str5_bucket[i]);
    }

    db_idx = 0;
}

void AddDB(SInfo info) {
    comp_db[db_idx] = info;
    add2Bucket(info.first, str1_bucket, db_idx);
    add2Bucket(info.second, str2_bucket, db_idx);
    add2Bucket(info.third, str3_bucket, db_idx);
    add2Bucket(info.fourth, str4_bucket, db_idx);
    add2Bucket(info.fifth, str5_bucket, db_idx);
    db_idx++;
}


int SearchBestCase(SInfo info) {

    int comp_max_score = 0;

    comp_max_score = computeScore(info, comp_max_score, info.first, str1_bucket);
    comp_max_score = computeScore(info, comp_max_score, info.second, str2_bucket);
    comp_max_score = computeScore(info, comp_max_score, info.third, str3_bucket);
    comp_max_score = computeScore(info, comp_max_score, info.fourth, str4_bucket);
    comp_max_score = computeScore(info, comp_max_score, info.fifth, str5_bucket);

    return comp_max_score;
}

int computeScore(SInfo &info, int comp_max_score, char *comp_str, struct Entry *bucket) {
    unsigned long hashcode = getHashcode(comp_str);
    unsigned long pos = hashcode % HASHTABLE_SIZE;
    struct Entry *locEntry = &bucket[pos];
    struct Entry *previousEntry = &bucket[pos];
    int new_score = 0;
    while (locEntry && locEntry->valid == 1) {
        if (equalStr(locEntry->str, comp_str) == 1) {
            int loc_db_idx = locEntry->db_idx;
            struct SInfo loc_sinfo = comp_db[loc_db_idx];

            int s1 = CalculateRelation(info.first, loc_sinfo.first);
            int s2 = CalculateRelation(info.second, loc_sinfo.second);
            int s3 = CalculateRelation(info.third, loc_sinfo.third);
            int s4 = CalculateRelation(info.fourth, loc_sinfo.fourth);
            int s5 = CalculateRelation(info.fifth, loc_sinfo.fifth);
            new_score = s1 + s2 + s3 + s4 + s5;
            if (new_score > comp_max_score) {
                comp_max_score = new_score;
            }
        }
        locEntry = locEntry->next;
    }
    return comp_max_score;
}