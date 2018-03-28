//
// Created by zhangji on 3/28/18.
//

#include <malloc.h>

#define MAXACCOUNT  50000
#define MAXTIME     30000

struct Account {
    char id[11];
    char pwd[11];
    long default_t;

    struct Book *curr_page;
    Account *next;

    int hash;
    Account *hashtbl_next;
};

struct Book {
    int num;
    long logout_t;
    Account *account;
    Book *next_page;
};

void add_new_page(Account *new_account, long new_logout_t, Book *prev_page);

void clear_account_in_hashtable(int hash, char *tgt_id);

Book *book;
Account *hash_table[MAXACCOUNT];
long tick_time;

void mystr_cpy(char *src, char *dst) {
    int i = 0;
    while (src[i]) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
}

int mystr_equal(char *src, char *dst) {
    int i = 0;
    while (src[i] && dst[i]) {
        if (src[i] != dst[i])
            return 0;
        i++;
    }
    if (src[i] == 0 && dst[i] == 0)
        return 1;
    return 0;
}

int hash_code(const char *src) {
    int i = 0;
    int hash_code = 1;
    while (src[i] != 0) {
        hash_code = hash_code * 31 + src[i];
        i++;
    }
    return (hash_code > 0 ? hash_code : -hash_code) / MAXACCOUNT;
}

//Book

Account *get_new_account(char *id, char *pwd, long default_t) {
    auto account = (Account *) malloc(sizeof(Account));
    if (account != nullptr) {
        mystr_cpy(id, account->id);
        mystr_cpy(pwd, account->pwd);
        account->default_t = default_t;

        account->next = nullptr;
        account->curr_page = nullptr;

        account->hash = hash_code(id);
        account->hashtbl_next = nullptr;
    }
    return account;
}

Book *get_new_book(int num, long logout_t, Account *account) {
    auto book = (Book *) malloc(sizeof(Book));
    if (book != nullptr) {
        book->num = num;
        book->logout_t = logout_t;
        book->account = account;
        book->next_page = nullptr;
    }
    return book;
}

void Init() {
    tick_time = 0;
    for (auto &i : hash_table) {
        i = nullptr;
    }
    book = nullptr;
}


void NewAccount(char id[11], char password[11], int defaulttime) {
    Account *new_account = get_new_account(id, password, defaulttime);
    Account *old_hash_account = hash_table[new_account->hash];
    if (old_hash_account != nullptr) {
        hash_table[new_account->hash] = new_account;
        new_account->hashtbl_next = old_hash_account;
    }
    //search book page
    long new_logout_t = tick_time + new_account->default_t;
    if (book == nullptr) { // init
        book = get_new_book(1, new_logout_t, new_account);
        new_account->curr_page = book;
    } else {
        Book *page = book;
        Book *prev_page = nullptr;
        while (page != nullptr) {
            long page_logout_t = page->logout_t;
            if (new_logout_t > page_logout_t) {
                prev_page = page;
                page = page->next_page;
                if (page == nullptr) {
                    add_new_page(new_account, new_logout_t, prev_page);
                    break;
                }
            } else {
                if (new_logout_t < page_logout_t) {
                    if (prev_page == nullptr) {
                        Book *new_page = get_new_book(1, new_logout_t, new_account);
                        new_account->curr_page = new_page;
                        new_page->next_page = page;
                        book = new_page;
                    } else {
                        add_new_page(new_account, new_logout_t, prev_page);
                    }
                    break;
                } else { // new_logout_t == page->logout_t
                    new_account->curr_page = page;
                    Account *old_first_account = page->account;
                    page->account = new_account;
                    new_account->next = old_first_account;
                    page->num++;
                    break;
                }
            }
        }
    }
}

void add_new_page(Account *new_account, long new_logout_t, Book *prev_page) {
    Book *new_page = get_new_book(1, new_logout_t, new_account);
    new_account->curr_page = new_page;
    Book *old_next_page = prev_page->next_page;
    prev_page->next_page = new_page;
    new_page->next_page = old_next_page;
}

void Logout(char id[11]) {
    Account *hash_account = hash_table[hash_code(id)];
    clear_account_in_hashtable(hash_code(id), id);
}

void Connect(char id[11], char password[11]) {
    int hash = hash_code(id);
    Account *hash_account = hash_table[hash];
    while (hash_account != nullptr && mystr_equal(hash_account->id, id) != 1) {
        hash_account = hash_account->hashtbl_next;
    }
    // remove account in book
    if (hash_account != nullptr) {
        if (mystr_equal(hash_account->pwd, password) == 1) {
            Book *the_page = hash_account->curr_page;
            Account *temp_account = the_page->account;
            Account *prev_account = the_page->account;
            while (temp_account != nullptr && temp_account != hash_account) {
                prev_account = temp_account;
                temp_account = temp_account->next;
            }
            if (temp_account != nullptr) { // temp_account == hash_account
                Account *next_account = temp_account->next;
                prev_account->next = next_account;
                the_page->num--;
                //
                hash_account->next = nullptr;
                hash_account->curr_page = nullptr;
                long new_logout_t = tick_time + hash_account->default_t;

                Book *page = the_page;
                Book *prev_page = the_page;
                while (page != nullptr) {
                    long page_logout_t = page->logout_t;
                    if (new_logout_t > page_logout_t) {
                        prev_page = page;
                        page = page->next_page;
                        if (page == nullptr) {
                            add_new_page(hash_account, new_logout_t, prev_page);
                            break;
                        }
                    } else {
                        if (new_logout_t < page_logout_t) {
                            add_new_page(hash_account, new_logout_t, prev_page);
                            break;
                        } else { // new_logout_t == page->logout_t
                            hash_account->curr_page = page;
                            Account *old_first_account = page->account;
                            page->account = hash_account;
                            hash_account->next = old_first_account;
                            page->num++;
                            break;
                        }
                    }
                }

            }
        }
    }
}

int Tick() {
    tick_time++;
    Book *page = book;
    int num = 0;
    while (page != nullptr && page->logout_t < tick_time) {
        Account *account = page->account;
        while (account != nullptr) {
            clear_account_in_hashtable(account->hash, account->id);
        }
        num += page->num;
        page = page->next_page;
    }
    book = page;

    return num;
}

void clear_account_in_hashtable(int hash, char *tgt_id) {
    Account *hash_account = hash_table[hash];
    Account *prev_hash_account = nullptr;
    while (hash_account != nullptr) {
        if (mystr_equal(hash_account->id, tgt_id) == 1) {
            if (prev_hash_account != nullptr) {
                prev_hash_account->hashtbl_next = hash_account->hashtbl_next;
            } else {
                hash_table[hash] = hash_account->hashtbl_next;
            }
            break;
        } else {
            prev_hash_account = hash_account;
            hash_account = hash_account->hashtbl_next;
        }
    }
}
