//
// Created by zhangji on 4/7/18.
//

//#pragma GCC optimize("-Ofast")

#include <malloc.h>

#define MAXPAGES 50000
#define MAXWORDS 10000
#define MAXBLOCK 10000

struct Page {
    char kws[4][11];
    int isRemoved;
    int blockedNum;
};

struct KeyPosition {
    int pageId;
    struct KeyPosition *next;
};

struct KeyInfo {
    char word[11];
    int blockedNum;
    KeyPosition *key_pos;
};

struct HashNode {
    struct KeyInfo keyList[MAXWORDS];
    int listLen;
};


Page book[MAXPAGES];
int newPageId;
HashNode hashTable[MAXWORDS];

int equalStr(const char *dest, const char *src) {
    for (int i = 0; i < 11; i++) {
        if (src[i] == '\0') {
            return 1;// equals
        }
        if (src[i] == dest[i]) {
            continue;
        }
        return -1;// not equals
    }
    return 1;
}

void copyStr(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
}

struct KeyPosition *create_KeyPosition(int pageId) {
    auto *newKeyPosition = (struct KeyPosition *) malloc(sizeof(struct KeyPosition));
    if (newKeyPosition != nullptr) {
        newKeyPosition->pageId = pageId;
        newKeyPosition->next = nullptr;
    }
    return newKeyPosition;
}

//n page number
void init(int n) {
    newPageId = 0;
    for (auto &node : hashTable) {
        node.listLen = 0;
    }
}

int hashcode(const char kw[11]) {
    int ret = 1;
    for (int i = 0; i < 11; i++) {
        ret = ret * 17 + kw[i];
    }
    ret = ret < 0 ? -ret : ret;
    return ret % MAXWORDS;
}

// 60
void addPage(int mId, int m, char word[][11]) {
    // page process
    newPageId = mId;
    book[newPageId].isRemoved = 0;
    book[newPageId].blockedNum = 0;
    for (int i = 0; i < m; i++) {
        copyStr(book[newPageId].kws[i], word[i]);
    }
    // hashTable process
    for (int i = 0; i < m; i++) {
        int hash = hashcode(word[i]);
        HashNode &node = hashTable[hash];
        int listLen = node.listLen;
        if (listLen == 0) {
            KeyInfo &keyInfo = node.keyList[listLen];
            copyStr(keyInfo.word, word[i]);
            KeyPosition *newKeyPos = create_KeyPosition(mId);
            keyInfo.key_pos = newKeyPos;
            keyInfo.blockedNum = 0;
            node.listLen++;
        } else {// listLen not 0
            int k = 0;
            for (; k < listLen; k++) {
                if (1 == equalStr(word[i], node.keyList[k].word)) {
                    KeyPosition *newKeyPos = create_KeyPosition(mId);
                    book[newPageId].blockedNum += node.keyList[k].blockedNum;
                    KeyPosition *oldKeyPos = node.keyList[k].key_pos;
                    node.keyList[k].key_pos = newKeyPos;
                    newKeyPos->next = oldKeyPos;
                    break;
                }
            }
            if (k == listLen) {
                KeyInfo &keyInfo = node.keyList[listLen];
                copyStr(keyInfo.word, word[i]);
                KeyPosition *newKeyPos = create_KeyPosition(mId);
                keyInfo.key_pos = newKeyPos;
                keyInfo.blockedNum = 0;
                node.listLen++;
            }
        }
    }
}

// 5
void removePage(int mId) {
    book[mId].isRemoved = 1;
}

// 10
void blockWord(char word[]) {
    int hash = hashcode(word);
    HashNode &node = hashTable[hash];
    int listLen = node.listLen;
    for (int k = 0; k < listLen; k++) {
        KeyInfo &keyInfo = node.keyList[k];
        if (1 == equalStr(word, keyInfo.word)) {
            KeyPosition *keyPos = keyInfo.key_pos;
            keyInfo.blockedNum += 1;
            while (keyPos != nullptr) {
                book[keyPos->pageId].blockedNum++;
                keyPos = keyPos->next;
            }
            break;
        }
    }
}

// 15
void recoverWord(char word[]) {
    int hash = hashcode(word);
    HashNode &node = hashTable[hash];
    int listLen = node.listLen;
    for (int k = 0; k < listLen; k++) {
        KeyInfo &keyInfo = node.keyList[k];
        if (1 == equalStr(word, keyInfo.word)) {
            KeyPosition *keyPos = keyInfo.key_pos;
            while (keyPos != nullptr) {
                book[keyPos->pageId].blockedNum -= keyInfo.blockedNum;
                keyPos = keyPos->next;
            }
            keyInfo.blockedNum = 0;
            break;
        }
    }
}

void check(const char word[11], int pool[MAXPAGES]) {
    int hash = hashcode(word);
    HashNode &node = hashTable[hash];
    int listLen = node.listLen;
    for (int k = 0; k < listLen; k++) {
        KeyInfo &keyInfo = node.keyList[k];
        if (1 == equalStr(word, keyInfo.word)) {
            KeyPosition *keyPos = keyInfo.key_pos;
            while (keyPos != nullptr) {
                int pageId = keyPos->pageId;
                if (book[pageId].isRemoved != 1 && book[pageId].blockedNum == 0) {
                    pool[pageId] = 1;
                }
                keyPos = keyPos->next;
            }
            break;
        }
    }
}

int search(char word[][11], int mode) {
    int pool_0[MAXPAGES];
    int pool_1[MAXPAGES];
    for (int i = 0; i < MAXPAGES; i++) {
        pool_0[i] = 0;
        pool_1[i] = 0;
    }
    int ret = 0;
    if (mode == 2) { // OR
        check(word[0], pool_0);
        check(word[1], pool_1);
        for (int i = 0; i < MAXPAGES; i++) {
            if (!(pool_0[i] != 1 && pool_1[i] != 1)) {
                ret++;
            }
        }
    } else if (mode == 1) { // AND
        check(word[0], pool_0);
        check(word[1], pool_1);
        for (int i = 0; i < MAXPAGES; i++) {
            if (pool_0[i] == 1 && pool_1[i] == 1) {
                ret++;
            }
        }
    } else { // just 0th
        check(word[0], pool_0);
        for (int mark : pool_0) {
            if (mark == 1) {
                ret++;
            }
        }
    }
    return ret;
}
