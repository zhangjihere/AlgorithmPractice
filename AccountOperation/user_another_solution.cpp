#define MAX_ACCOUNT 50000
#define MAX_TIME 30000

#define MAX_TICKTIME 30000+50000+1

void str_cp(char *dst,const char *src)
{
    for(;*src!=0;src++){
        *dst++=*src;
    }
    *dst='\0';
}

int str_cmp(char *p1, char*p2)
{
    if(p1==0||p2==0){
        return -1;
    }
    for(;*p1!=0 && *p2!=0;p1++,p2++){
        if(*p1-*p2 != 0){
            return *p1-*p2;
        }
    }
    return 0;
}

int hash_code(char * key)
{
    int h = 0;
    if (key != 0 ) {
        for (char* p = key; *p!=0; p++) {
            h = 23 * h + *p;
        }
    }
    h=h<0?-h:h;
    return h%MAX_ACCOUNT;
}

struct Account{
    char id[11];
    char password[11];
    int defaulttime;
    int intime;
    bool islogout;
    Account():defaulttime(0),intime(0),islogout(false){};
    Account(char*myid,char*mypwd,int dt,int it):defaulttime(dt),intime(it),islogout(false){
        str_cp(id,myid);
        str_cp(password,mypwd);
    };
};

struct IDList{
    int ids[50];
    int pos;
};

Account account[MAX_ACCOUNT+1];
int account_pos = 0;
IDList hash2id[MAX_ACCOUNT+1];
int ticktime[MAX_TICKTIME+1];
int currenttime = 0;

void Init()
{
    account_pos=0;
    currenttime=0;

    for(int *p=ticktime; p-ticktime<MAX_TICKTIME;p++){
        (*p)^=(*p);
    }
    for(IDList* p=hash2id; p-hash2id<MAX_ACCOUNT; p++){
        (*p).pos^=(*p).pos;
    }
}

void NewAccount(char id[11], char password[11], int defaulttime)
{
    account[account_pos] = Account(id,password,defaulttime,currenttime);
    int hc = hash_code(id);

    hash2id[hc].ids[hash2id[hc].pos++]=account_pos++;

    int tt = defaulttime+currenttime;
    ticktime[tt]++;
}

Account* getAccount(char id[11]){
    int hc = hash_code(id);
    IDList* p=&hash2id[hc];
    if(p->pos>0){
        for(int i=0; i<p->pos; i++){
            if(str_cmp(id,account[p->ids[i]].id)==0){
                return &account[p->ids[i]];
            }
        }
    }
    return 0;
}

void Logout(char id[11])
{
    Account* p=getAccount(id);
    if(p!=0 && !p->islogout){
        p->islogout = true;
        int tt = p->defaulttime+p->intime;
        if(currenttime<=tt && ticktime[tt]>0) ticktime[tt]--;
    }
}

void Connect(char id[11], char password[11])
{
    Account* p=getAccount(id);
    if(p!=0 && !p->islogout && str_cmp(password,p->password) == 0 &&
       p->intime!=currenttime){
        int tt = p->defaulttime+p->intime;
        if(ticktime[tt] > 0){
            ticktime[tt]--;
            p->intime = currenttime;
            tt = currenttime+p->defaulttime;
            ticktime[tt]++;
        }
    }
}

int Tick()
{
    currenttime++;
    int ret = ticktime[currenttime];
    ticktime[currenttime]=0;
    return ret;
}

