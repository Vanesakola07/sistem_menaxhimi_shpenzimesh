#include "struktura.h"


/* Lexon nje tekst nga tastiera dhe heq karakterin \n ne fund */
void lexoStr(char *buf, int n) {
    if (fgets(buf, n, stdin))
        buf[strcspn(buf, "\n")] = '\0';
}

/* Lexon nje numer te plote nga tastiera */
int lexoInt(void) {
    char t[32]; lexoStr(t, sizeof(t)); return atoi(t);
}

/* Lexon nje numer me presje dhjetore nga tastiera */
float lexoFloat(void) {
    char t[32]; lexoStr(t, sizeof(t)); return (float)atof(t);
}

/* Password: fillon madhe, >=4 kar, >=1 numer, >=1 simbol */
int validoPass(const char *p) {
    int g = (int)strlen(p);
    if (g < 4 || !isupper((unsigned char)p[0])) return 0;
    int num = 0, sim = 0;
    for (int i = 0; i < g; i++) {
        if (isdigit((unsigned char)p[i])) num = 1;
        if (!isalnum((unsigned char)p[i]))  sim = 1;
    }
    return num && sim;
}


int userEkziston(int id) {
    /* Lexojme perdoruesit dhe kontrollojme nese ID ekziston */
    User a[MAX_USERS]; int n; lexoUsers(a, &n);
    for (int i = 0; i < n; i++) if (a[i].id_user == id) return 1;
    return 0;
}
int usernameEkziston(const char *u) {
    /* Kontrollon nese username eshte perdorur me pare */
    User a[MAX_USERS]; int n; lexoUsers(a, &n);
    for (int i = 0; i < n; i++) if (strcmp(a[i].username, u) == 0) return 1;
    return 0;
}
int shpenzimEkziston(int id) {
    /* Kontrollon nese ekziston nje shpenzim me kete ID */
    Shpenzim a[MAX_SHPENZIME]; int n; lexoShpenzime(a, &n);
    for (int i = 0; i < n; i++) if (a[i].id_shpenzim == id) return 1;
    return 0;
}
int teArdhuratEkziston(int id) {
    /* Kontrollon nese ekziston nje hyrje te ardhurash me kete ID */
    TeArdhura a[MAX_TEARDHURA]; int n; lexoTeArdhura(a, &n);
    for (int i = 0; i < n; i++) if (a[i].id_hyrje == id) return 1;
    return 0;
}
int kategoriaEkziston(int id) {
    /* Kontrollon nese ekziston nje kategori me kete ID */
    Kategori a[MAX_KATEGORI]; int n; lexoKategori(a, &n);
    for (int i = 0; i < n; i++) if (a[i].id_kategori == id) return 1;
    return 0;
}


/* Lexon te gjithe perdoruesit nga skedari user.txt */
void lexoUsers(User *a, int *n) {
    *n = 0;
    FILE *f = fopen(FILE_USERS, "r"); if (!f) return;
    while (fscanf(f, "%d|%49[^|]|%49[^|]|%49[^|]|%f\n",
                  &a[*n].id_user, a[*n].emri, a[*n].username,
                  a[*n].password, &a[*n].buxheti_mujor) == 5) (*n)++;
    fclose(f);
}
/* Ruajme perseri listen e perdoruesve ne skedar */
void ruajUsers(User *a, int n) {
    FILE *f = fopen(FILE_USERS, "w"); if (!f) return;
    for (int i = 0; i < n; i++)
        fprintf(f, "%d|%s|%s|%s|%.2f\n",
                a[i].id_user, a[i].emri, a[i].username,
                a[i].password, a[i].buxheti_mujor);
    fclose(f);
}


/* Lexon shpenzimet nga skedari shpenzime.txt */
void lexoShpenzime(Shpenzim *a, int *n) {
    *n = 0;
    FILE *f = fopen(FILE_SHPENZIME, "r"); if (!f) return;
    while (fscanf(f, "%d|%d|%99[^|]|%d|%49[^|]|%99[^|]|%f|%19[^\n]\n",
                  &a[*n].id_shpenzim, &a[*n].id_user, a[*n].pershkrim,
                  &a[*n].kategori.id_kategori, a[*n].kategori.emertimi,
                  a[*n].kategori.pershkrimi, &a[*n].shuma, a[*n].data) == 8) (*n)++;
    fclose(f);
}
/* Ruajme shpenzimet ne skedar pas shtimit, ndryshimit ose fshirjes */
void ruajShpenzime(Shpenzim *a, int n) {
    FILE *f = fopen(FILE_SHPENZIME, "w"); if (!f) return;
    for (int i = 0; i < n; i++)
        fprintf(f, "%d|%d|%s|%d|%s|%s|%.2f|%s\n",
                a[i].id_shpenzim, a[i].id_user, a[i].pershkrim,
                a[i].kategori.id_kategori, a[i].kategori.emertimi,
                a[i].kategori.pershkrimi, a[i].shuma, a[i].data);
    fclose(f);
}


/* Lexon te ardhurat nga skedari teArdhura.txt */
void lexoTeArdhura(TeArdhura *a, int *n) {
    *n = 0;
    FILE *f = fopen(FILE_TEARDHURA, "r"); if (!f) return;
    while (fscanf(f, "%d|%d|%99[^|]|%f|%19[^\n]\n",
                  &a[*n].id_hyrje, &a[*n].id_user,
                  a[*n].burimi, &a[*n].shuma, a[*n].data) == 5) (*n)++;
    fclose(f);
}
/* Ruajme te ardhurat ne skedar */
void ruajTeArdhura(TeArdhura *a, int n) {
    FILE *f = fopen(FILE_TEARDHURA, "w"); if (!f) return;
    for (int i = 0; i < n; i++)
        fprintf(f, "%d|%d|%s|%.2f|%s\n",
                a[i].id_hyrje, a[i].id_user,
                a[i].burimi, a[i].shuma, a[i].data);
    fclose(f);
}


/* Lexon kategorite nga skedari kategorite.txt */
void lexoKategori(Kategori *a, int *n) {
    *n = 0;
    FILE *f = fopen(FILE_KATEGORITE, "r"); if (!f) return;
    while (fscanf(f, "%d|%49[^|]|%99[^\n]\n",
                  &a[*n].id_kategori, a[*n].emertimi, a[*n].pershkrimi) == 3) (*n)++;
    fclose(f);
}
/* Ruajme kategorite ne skedar */
void ruajKategori(Kategori *a, int n) {
    FILE *f = fopen(FILE_KATEGORITE, "w"); if (!f) return;
    for (int i = 0; i < n; i++)
        fprintf(f, "%d|%s|%s\n", a[i].id_kategori, a[i].emertimi, a[i].pershkrimi);
    fclose(f);
}


/* Kontrollon login-in e administratorit */
int loginAdmin(void) {
    char u[50], p[50];
    printf("\n  Username: "); lexoStr(u, sizeof(u));
    printf("  Password: "); lexoStr(p, sizeof(p));
    if (strcmp(u, ADMIN_USER) == 0 && strcmp(p, ADMIN_PASS) == 0) {
        printf("  [OK] Mire se erdhe, Administrator!\n"); return 1;
    }
    printf("  [GABIM] Kredenciale te gabuara!\n"); return 0;
}

/* Kontrollon login-in e perdoruesit dhe kthen ID e tij */
int loginUser(void) {
    char u[50], p[50];
    printf("\n  Username: "); lexoStr(u, sizeof(u));
    printf("  Password: "); lexoStr(p, sizeof(p));
    User a[MAX_USERS]; int n; lexoUsers(a, &n);
    for (int i = 0; i < n; i++)
        if (strcmp(a[i].username, u) == 0 && strcmp(a[i].password, p) == 0) {
            printf("  [OK] Mire se erdhe, %s!\n", a[i].emri);
            return a[i].id_user;
        }
    printf("  [GABIM] Username ose password gabim!\n"); return -1;
}


/* Perdoret kur perdoruesi zgjedh kategorine per nje shpenzim */
static Kategori zgjedhKategori(void) {
    Kategori a[MAX_KATEGORI]; int n; lexoKategori(a, &n);
    Kategori def = {0, "Tjeter", "E paspecifikuar"};
    if (n == 0) { printf("  (Ska kategori, perdoret 'Tjeter')\n"); return def; }
    printf("  -- Kategorite --\n");
    for (int i = 0; i < n; i++)
        printf("  %d. %s\n", a[i].id_kategori, a[i].emertimi);
    printf("  Zgjidh ID: "); int id = lexoInt();
    for (int i = 0; i < n; i++) if (a[i].id_kategori == id) return a[i];
    printf("  (Nuk u gjet, perdoret 'Tjeter')\n"); return def;
}

/* Krahaso data DD/MM/YYYY si numër YYYYMMDD */
static int krahasoData(const char *d1, const char *d2) {
    if (strlen(d1) < 10 || strlen(d2) < 10) return 0;
    char s1[9], s2[9];
    snprintf(s1,9,"%c%c%c%c%c%c%c%c",d1[6],d1[7],d1[8],d1[9],d1[3],d1[4],d1[0],d1[1]);
    snprintf(s2,9,"%c%c%c%c%c%c%c%c",d2[6],d2[7],d2[8],d2[9],d2[3],d2[4],d2[0],d2[1]);
    return strcmp(s1, s2);
}

static void printShpenzimHeader(void) {
    printf("  %-5s %-24s %-14s %-9s %s\n","ID","Pershkrim","Kategori","Shuma","Data");
    }
/* Printon nje shpenzim ne forme tabele */
static void printShpenzim(Shpenzim *s) {
    printf("  %-5d %-24s %-14s %-9.2f %s\n",
           s->id_shpenzim, s->pershkrim, s->kategori.emertimi, s->shuma, s->data);
}


/* 1 */
static void a_shtoUser(void) {
    /* Shton nje perdorues te ri nga ana e administratorit */
    printf("\nSHTO PERDORUES\n");
    User u;
    do {
        printf("  ID (pozitiv unik): "); u.id_user = lexoInt();
        if (u.id_user <= 0)            { printf("  [GABIM] ID duhet pozitive!\n"); continue; }
        if (userEkziston(u.id_user))   { printf("  [GABIM] ID ekziston!\n"); continue; }
        break;
    } while (1);
    printf("  Emri: ");     lexoStr(u.emri, sizeof(u.emri));
    do {
        printf("  Username: "); lexoStr(u.username, sizeof(u.username));
        if (usernameEkziston(u.username)) printf("  [GABIM] Username ekziston!\n");
        else break;
    } while (1);
    do {
        printf("  Password (Madhe+4kar+num+simbol): "); lexoStr(u.password, sizeof(u.password));
        if (!validoPass(u.password)) printf("  [GABIM] Password invalid!\n");
        else break;
    } while (1);
    printf("  Buxheti mujor (ALL): "); u.buxheti_mujor = lexoFloat();

    User a[MAX_USERS]; int n; lexoUsers(a, &n);
    a[n++] = u; ruajUsers(a, n);
    printf("  [OK] Perdoruesi u shtua!\n");
}

/* 2 */
static void a_shfaqUsers(void) {
    /* Shfaq listen e te gjithe perdoruesve */
    printf("\nLISTA PERDORUESVE\n");
    printf("  %-5s %-20s %-20s %s\n","ID","Emri","Username","Buxheti");
        User a[MAX_USERS]; int n; lexoUsers(a, &n);
    for (int i = 0; i < n; i++)
        printf("  %-5d %-20s %-20s %.2f\n",
               a[i].id_user, a[i].emri, a[i].username, a[i].buxheti_mujor);
    if (!n) printf("  (Ska perdorues)\n");
}

/* 3 */
static void a_kerkoSipasID(void) {
    /* Kerkojme perdoruesin duke perdorur ID */
    printf("\n  ID: "); int id = lexoInt();
    User a[MAX_USERS]; int n; lexoUsers(a, &n);
    for (int i = 0; i < n; i++)
        if (a[i].id_user == id) {
            printf("  ID:%d  Emri:%s  Username:%s  Buxheti:%.2f\n",
                   a[i].id_user, a[i].emri, a[i].username, a[i].buxheti_mujor);
            return;
        }
    printf("  [INFO] Nuk u gjet.\n");
}

/* 4 */
static void a_fshiUser(void) {
    /* Fshin perdoruesin dhe te dhenat e tij */
    printf("\n  ID per fshirje: "); int id = lexoInt();
    User a[MAX_USERS]; int n; lexoUsers(a, &n);
    int idx = -1;
    for (int i = 0; i < n; i++) if (a[i].id_user == id) { idx = i; break; }
    if (idx < 0) { printf("  [GABIM] Nuk u gjet!\n"); return; }
    printf("  Fshi '%s'? (1=Po): ", a[idx].emri); if (lexoInt() != 1) { printf("  [ANULUAR]\n"); return; }
    for (int i = idx; i < n-1; i++) a[i] = a[i+1];
    n--;
    ruajUsers(a, n);

    /* Fshi edhe shpenzimet dhe te ardhurat */
    Shpenzim sh[MAX_SHPENZIME]; int ns; lexoShpenzime(sh, &ns);
    int m = 0;
    for (int i = 0; i < ns; i++) if (sh[i].id_user != id) sh[m++] = sh[i];
    ruajShpenzime(sh, m);

    TeArdhura ta[MAX_TEARDHURA]; int nt; lexoTeArdhura(ta, &nt);
    m = 0;
    for (int i = 0; i < nt; i++) if (ta[i].id_user != id) ta[m++] = ta[i];
    ruajTeArdhura(ta, m);

    printf("  [OK] Perdoruesi u fshi!\n");
}

/* 5 */
static void a_ndryshoUser(void) {
    /* Ndryshon te dhenat e perdoruesit nese ai ekziston */
    printf("\n  ID per ndryshim: "); int id = lexoInt();
    User a[MAX_USERS]; int n; lexoUsers(a, &n);
    int idx = -1;
    for (int i = 0; i < n; i++) if (a[i].id_user == id) { idx = i; break; }
    if (idx < 0) { printf("  [GABIM] Nuk u gjet!\n"); return; }

    char t[100];
    printf("  Emri i ri (bosh=pa ndryshim): ");    lexoStr(t,sizeof(t)); if(strlen(t)) strcpy(a[idx].emri,t);
    printf("  Username i ri (bosh=pa ndryshim): "); lexoStr(t,sizeof(t));
    if (strlen(t)) {
        if (usernameEkziston(t) && strcmp(t,a[idx].username)!=0) printf("  [GABIM] Ekziston, ruhet i vjetri.\n");
        else strcpy(a[idx].username, t);
    }
    printf("  Password i ri (bosh=pa ndryshim): "); lexoStr(t,sizeof(t));
    if (strlen(t)) {
        if (!validoPass(t)) printf("  [GABIM] Invalid, ruhet i vjetri.\n");
        else strcpy(a[idx].password, t);
    }
    printf("  Buxheti i ri (-1=pa ndryshim): "); float b = lexoFloat();
    if (b >= 0) a[idx].buxheti_mujor = b;
    ruajUsers(a, n);
    printf("  [OK] Perdoruesi u ndryshua!\n");
}

/* 6 */
static void a_printTeDhena(void) {
    /* Printon perdoruesit, shpenzimet dhe te ardhurat */
    a_shfaqUsers();

    printf("\n-- SHPENZIMET --\n"); printShpenzimHeader();
    Shpenzim sh[MAX_SHPENZIME]; int ns; lexoShpenzime(sh, &ns);
    for (int i = 0; i < ns; i++) printShpenzim(&sh[i]);
    if (!ns) printf("  (Ska)\n");

    printf("\n-- TE ARDHURAT --\n");
    printf("  %-5s %-5s %-24s %-9s %s\n","ID_h","ID_u","Burimi","Shuma","Data");
        TeArdhura ta[MAX_TEARDHURA]; int nt; lexoTeArdhura(ta, &nt);
    for (int i = 0; i < nt; i++)
        printf("  %-5d %-5d %-24s %-9.2f %s\n",
               ta[i].id_hyrje, ta[i].id_user, ta[i].burimi, ta[i].shuma, ta[i].data);
    if (!nt) printf("  (Ska)\n");
}

/* 7 */
static void a_kerkoUsername(void) {
    /* Kerkojme perdoruesin sipas username */
    printf("\n  Username: "); char u[50]; lexoStr(u,sizeof(u));
    User a[MAX_USERS]; int n; lexoUsers(a,&n);
    for (int i = 0; i < n; i++)
        if (strcmp(a[i].username,u)==0) {
            printf("  ID:%d Emri:%s Buxheti:%.2f\n",a[i].id_user,a[i].emri,a[i].buxheti_mujor);
            return;
        }
    printf("  [INFO] Nuk u gjet.\n");
}

/* 8 */
static void a_radhitUsername(void) {
    /* Rendit perdoruesit sipas username */
    User a[MAX_USERS]; int n; lexoUsers(a,&n);
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-1-i; j++)
            if (strcmp(a[j].username,a[j+1].username)>0) { User t=a[j];a[j]=a[j+1];a[j+1]=t; }
    printf("\nPERDORUESIT (sipas username)\n");
    printf("  %-5s %-20s %-20s %s\n","ID","Emri","Username","Buxheti");
        for (int i = 0; i < n; i++)
        printf("  %-5d %-20s %-20s %.2f\n",a[i].id_user,a[i].emri,a[i].username,a[i].buxheti_mujor);
    if (!n) printf("  (Ska perdorues)\n");
}

/* 9 */
static void a_menaxhoKategori(void) {
    /* Menu e vogel per shtim, shfaqje, ndryshim dhe fshirje kategorish */
    int z;
    do {
        printf("\n  1.Shto  2.Shfaq  3.Ndrysho  4.Fshi  0.Kthehu\n  Zgjedhja: ");
        z = lexoInt();
        Kategori a[MAX_KATEGORI]; int n; lexoKategori(a,&n);
        if (z==1) {
            Kategori k;
            do {
                printf("  ID: "); k.id_kategori = lexoInt();
                if (k.id_kategori<=0) { printf("  [GABIM] Pozitive!\n"); continue; }
                if (kategoriaEkziston(k.id_kategori)) { printf("  [GABIM] Ekziston!\n"); continue; }
                break;
            } while(1);
            printf("  Emertimi: "); lexoStr(k.emertimi,sizeof(k.emertimi));
            printf("  Pershkrimi: "); lexoStr(k.pershkrimi,sizeof(k.pershkrimi));
            a[n++]=k; ruajKategori(a,n); printf("  [OK] Shtuar!\n");
        } else if (z==2) {
            printf("  %-5s %-20s %s\n","ID","Emertimi","Pershkrimi");
            for (int i=0;i<n;i++) printf("  %-5d %-20s %s\n",a[i].id_kategori,a[i].emertimi,a[i].pershkrimi);
            if(!n) printf("  (Ska)\n");
        } else if (z==3) {
            printf("  ID: "); int id=lexoInt(); int idx=-1;
            for(int i=0;i<n;i++) if(a[i].id_kategori==id){idx=i;break;}
            if(idx<0){printf("  [GABIM] Nuk u gjet!\n");}
            else {
                char t[100];
                printf("  Emertim i ri (bosh=pa ndryshim): "); lexoStr(t,sizeof(t)); if(strlen(t)) strcpy(a[idx].emertimi,t);
                printf("  Pershkrim i ri: "); lexoStr(t,sizeof(t)); if(strlen(t)) strcpy(a[idx].pershkrimi,t);
                ruajKategori(a,n); printf("  [OK] Ndryshuar!\n");
            }
        } else if (z==4) {
            printf("  ID: "); int id=lexoInt(); int idx=-1;
            for(int i=0;i<n;i++) if(a[i].id_kategori==id){idx=i;break;}
            if(idx<0){printf("  [GABIM] Nuk u gjet!\n");}
            else { for(int i=idx;i<n-1;i++) a[i]=a[i+1]; n--; ruajKategori(a,n); printf("  [OK] Fshire!\n"); }
        } else if (z!=0) printf("  [GABIM] Opsion invalid!\n");
    } while(z!=0);
}

/* 10 */
static void a_statistika(void) {
    /* Llogarit disa statistika te pergjithshme per sistemin */
    printf("\nSTATISTIKAT E SISTEMIT\n");
    User u[MAX_USERS]; int nu; lexoUsers(u,&nu);
    printf("  Perdorues total : %d\n", nu);

    Shpenzim sh[MAX_SHPENZIME]; int ns; lexoShpenzime(sh,&ns);
    printf("  Shpenzime total : %d\n", ns);

    float totalSh=0;
    float shPerU[MAX_USERS]={0};
    for(int i=0;i<ns;i++){
        totalSh+=sh[i].shuma;
        for(int j=0;j<nu;j++) if(u[j].id_user==sh[i].id_user){shPerU[j]+=sh[i].shuma;break;}
    }
    printf("  Totali shpenzimeve (ALL): %.2f\n", totalSh);

    if(nu>0){
        int imax=0,imin=0;
        for(int i=1;i<nu;i++){
            if(shPerU[i]>shPerU[imax]) imax=i;
            if(shPerU[i]<shPerU[imin]) imin=i;
        }
        printf("  Me shume shpenzime: %s (%.2f)\n", u[imax].emri, shPerU[imax]);
        printf("  Me pak  shpenzime : %s (%.2f)\n", u[imin].emri, shPerU[imin]);
    }

    TeArdhura ta[MAX_TEARDHURA]; int nt; lexoTeArdhura(ta,&nt);
    float totalTA=0; for(int i=0;i<nt;i++) totalTA+=ta[i].shuma;
    printf("  Te ardhura total  : %d  (%.2f ALL)\n", nt, totalTA);

    Kategori ka[MAX_KATEGORI]; int nk; lexoKategori(ka,&nk);
    printf("  Kategori total    : %d\n", nk);
}

void menuAdmin(void) {
    /* Menuja kryesore e administratorit */
    int z;
    do {
        printf("\nMENUJA E ADMINISTRATORIT\n");
        printf("1. Shto perdorues te ri\n");
        printf("2. Shfaq perdoruesit\n");
        printf("3. Kerko perdorues sipas ID\n");
        printf("4. Fshi perdoruesin\n");
        printf("5. Ndrysho perdoruesin\n");
        printf("6. Printo te gjitha te dhenat\n");
        printf("7. Kerko perdorues (username)\n");
        printf("8. Radhit perdoruesit (username)\n");
        printf("9. Menaxho kategorite\n");
        printf("10. Statistika sistemi\n");
        printf("11. Dil (Menu 1)\n");
        printf("  Zgjedhja: "); z = lexoInt();
        switch(z){
            case 1:  a_shtoUser(); break;
            case 2:  a_shfaqUsers(); break;
            case 3:  a_kerkoSipasID(); break;
            case 4:  a_fshiUser(); break;
            case 5:  a_ndryshoUser(); break;
            case 6:  a_printTeDhena(); break;
            case 7:  a_kerkoUsername(); break;
            case 8:  a_radhitUsername(); break;
            case 9:  a_menaxhoKategori(); break;
            case 10: a_statistika(); break;
            case 11: printf("  Kthehuni ne Menu 1...\n"); break;
            default: printf("  [GABIM] Opsion invalid!\n"); break;
        }
    } while(z!=11);
}


/* 1 */
static void u_shtoShpenzim(int uid) {
    /* Shton nje shpenzim per perdoruesin qe eshte futur ne sistem */
    printf("\nSHTO SHPENZIM\n");
    Shpenzim s; s.id_user=uid;
    do {
        printf("  ID (pozitiv unik): "); s.id_shpenzim=lexoInt();
        if(s.id_shpenzim<=0){printf("  [GABIM] Pozitive!\n");continue;}
        if(shpenzimEkziston(s.id_shpenzim)){printf("  [GABIM] Ekziston!\n");continue;}
        break;
    } while(1);
    printf("  Pershkrim: "); lexoStr(s.pershkrim,sizeof(s.pershkrim));
    s.kategori = zgjedhKategori();
    printf("  Shuma (ALL): "); s.shuma=lexoFloat();
    printf("  Data (DD/MM/YYYY): "); lexoStr(s.data,sizeof(s.data));

    Shpenzim a[MAX_SHPENZIME]; int n; lexoShpenzime(a,&n);
    a[n++]=s; ruajShpenzime(a,n);
    printf("  [OK] Shpenzimi u shtua!\n");
}

/* 2 */
static void u_shtoTeArdhura(int uid) {
    /* Shton nje te ardhur per perdoruesin */
    printf("\nSHTO TE ARDHURA\n");
    TeArdhura t; t.id_user=uid;
    do {
        printf("  ID (pozitiv unik): "); t.id_hyrje=lexoInt();
        if(t.id_hyrje<=0){printf("  [GABIM] Pozitive!\n");continue;}
        if(teArdhuratEkziston(t.id_hyrje)){printf("  [GABIM] Ekziston!\n");continue;}
        break;
    } while(1);
    printf("  Burimi: "); lexoStr(t.burimi,sizeof(t.burimi));
    printf("  Shuma (ALL): "); t.shuma=lexoFloat();
    printf("  Data (DD/MM/YYYY): "); lexoStr(t.data,sizeof(t.data));

    TeArdhura a[MAX_TEARDHURA]; int n; lexoTeArdhura(a,&n);
    a[n++]=t; ruajTeArdhura(a,n);
    printf("  [OK] Te ardhurat u shtuan!\n");
}

/* 3 */
static void u_shfaqShpenzime(int uid) {
    /* Shfaq vetem shpenzimet e perdoruesit aktual */
    printf("\nSHPENZIMET E MIA\n"); printShpenzimHeader();
    Shpenzim a[MAX_SHPENZIME]; int n; lexoShpenzime(a,&n);
    int f=0;
    for(int i=0;i<n;i++) if(a[i].id_user==uid){printShpenzim(&a[i]);f=1;}
    if(!f) printf("  (Ska shpenzime)\n");
}

/* 4 */
static void u_shfaqTeArdhura(int uid) {
    /* Shfaq vetem te ardhurat e perdoruesit aktual */
    printf("\nTE ARDHURAT E MIA\n");
    printf("  %-5s %-24s %-9s %s\n","ID","Burimi","Shuma","Data");
        TeArdhura a[MAX_TEARDHURA]; int n; lexoTeArdhura(a,&n);
    int f=0;
    for(int i=0;i<n;i++) if(a[i].id_user==uid){
        printf("  %-5d %-24s %-9.2f %s\n",a[i].id_hyrje,a[i].burimi,a[i].shuma,a[i].data);f=1;}
    if(!f) printf("  (Ska te ardhura)\n");
}

/* 5 */
static void u_raport(int uid) {
    /* Krijon raport mujor ose vjetor per perdoruesin */
    printf("  1.Mujor  2.Vjetor\n  Zgjedhja: "); int lloji=lexoInt();
    char filtri[16]="";
    if(lloji==1){
        printf("  Muaji (MM): "); char m[4]; lexoStr(m,sizeof(m));
        printf("  Viti (YYYY): "); char v[6]; lexoStr(v,sizeof(v));
        snprintf(filtri,sizeof(filtri),"%s/%s",m,v);
    } else {
        printf("  Viti (YYYY): "); lexoStr(filtri,sizeof(filtri));
    }

    /* shpenzime */
    Shpenzim sh[MAX_SHPENZIME]; int ns; lexoShpenzime(sh,&ns);
    float totalSh=0;
    char katArr[50][50]; float katSh[50]={0}; int nk=0;
    for(int i=0;i<ns;i++){
        if(sh[i].id_user!=uid) continue;
        int ok=0;
        if(lloji==1 && strlen(sh[i].data)>=10){
            char tmp[8]; snprintf(tmp,8,"%c%c/%c%c%c%c",sh[i].data[3],sh[i].data[4],sh[i].data[6],sh[i].data[7],sh[i].data[8],sh[i].data[9]);
            if(strcmp(tmp,filtri)==0) ok=1;
        } else if(lloji==2 && strlen(sh[i].data)>=10){
            char tmp[5]; snprintf(tmp,5,"%c%c%c%c",sh[i].data[6],sh[i].data[7],sh[i].data[8],sh[i].data[9]);
            if(strcmp(tmp,filtri)==0) ok=1;
        }
        if(!ok) continue;
        totalSh+=sh[i].shuma;
        int found=0;
        for(int j=0;j<nk;j++) if(strcmp(katArr[j],sh[i].kategori.emertimi)==0){katSh[j]+=sh[i].shuma;found=1;break;}
        if(!found && nk<50){strcpy(katArr[nk],sh[i].kategori.emertimi);katSh[nk]=sh[i].shuma;nk++;}
    }
    char katMax[50]=""; float kmSh=0;
    for(int j=0;j<nk;j++) if(katSh[j]>kmSh){kmSh=katSh[j];strcpy(katMax,katArr[j]);}

    /* te ardhura */
    TeArdhura ta[MAX_TEARDHURA]; int nt; lexoTeArdhura(ta,&nt);
    float totalTA=0;
    for(int i=0;i<nt;i++){
        if(ta[i].id_user!=uid) continue;
        int ok=0;
        if(lloji==1 && strlen(ta[i].data)>=10){
            char tmp[8]; snprintf(tmp,8,"%c%c/%c%c%c%c",ta[i].data[3],ta[i].data[4],ta[i].data[6],ta[i].data[7],ta[i].data[8],ta[i].data[9]);
            if(strcmp(tmp,filtri)==0) ok=1;
        } else if(lloji==2 && strlen(ta[i].data)>=10){
            char tmp[5]; snprintf(tmp,5,"%c%c%c%c",ta[i].data[6],ta[i].data[7],ta[i].data[8],ta[i].data[9]);
            if(strcmp(tmp,filtri)==0) ok=1;
        }
        if(ok) totalTA+=ta[i].shuma;
    }

    printf("\n  RAPORTI [%s]\n", filtri);
    printf("  Te ardhura  : %.2f ALL\n", totalTA);
    printf("  Shpenzime   : %.2f ALL\n", totalSh);
    printf("  Kursime     : %.2f ALL\n", totalTA-totalSh);
    if(nk>0) printf("  Kat. max shpenzim: %s (%.2f ALL)\n",katMax,kmSh);
    else printf("  (Ska shpenzime per kete periudhe)\n");
}

/* 6 */
static void u_buxheti(int uid) {
    /* Kontrollon nese perdoruesi ka kaluar buxhetin mujor */
    printf("  Muaji (MM): "); char m[4]; lexoStr(m,sizeof(m));
    printf("  Viti (YYYY): "); char v[6]; lexoStr(v,sizeof(v));
    char filtri[16]; snprintf(filtri,sizeof(filtri),"%s/%s",m,v);

    /* buxheti */
    User a[MAX_USERS]; int n; lexoUsers(a,&n);
    float bux=0;
    for(int i=0;i<n;i++) if(a[i].id_user==uid){bux=a[i].buxheti_mujor;break;}

    Shpenzim sh[MAX_SHPENZIME]; int ns; lexoShpenzime(sh,&ns);
    float tot=0;
    for(int i=0;i<ns;i++){
        if(sh[i].id_user!=uid) continue;
        if(strlen(sh[i].data)>=10){
            char tmp[8]; snprintf(tmp,8,"%c%c/%c%c%c%c",sh[i].data[3],sh[i].data[4],sh[i].data[6],sh[i].data[7],sh[i].data[8],sh[i].data[9]);
            if(strcmp(tmp,filtri)==0) tot+=sh[i].shuma;
        }
    }
    printf("\n  Buxheti mujor     : %.2f ALL\n", bux);
    printf("  Shpenzuar [%s]: %.2f ALL\n", filtri, tot);
    printf("  Mbetur            : %.2f ALL\n", bux-tot);
    if(tot>bux) printf("  [KUJDES] Tejkalim buxheti me %.2f ALL!\n",tot-bux);
    else printf("  [OK] Brenda buxhetit.\n");
}

/* 7 */
static void u_kerkoKategori(int uid) {
    /* Kerkojme shpenzimet sipas emrit te kategorise */
    printf("  Kategoria: "); char k[50]; lexoStr(k,sizeof(k));
    Shpenzim a[MAX_SHPENZIME]; int n; lexoShpenzime(a,&n);
    printShpenzimHeader(); int f=0;
    for(int i=0;i<n;i++)
        if(a[i].id_user==uid && strcmp(a[i].kategori.emertimi,k)==0){printShpenzim(&a[i]);f=1;}
    if(!f) printf("  (Asnje shpenzim per kategorine '%s')\n",k);
}

/* 8 */
static void u_fshiShpenzim(int uid) {
    /* Fshin nje shpenzim vetem nese i perket perdoruesit aktual */
    printf("  ID shpenzimit per fshirje: "); int id=lexoInt();
    Shpenzim a[MAX_SHPENZIME]; int n; lexoShpenzime(a,&n);
    int idx=-1;
    for(int i=0;i<n;i++) if(a[i].id_shpenzim==id && a[i].id_user==uid){idx=i;break;}
    if(idx<0){printf("  [GABIM] Nuk u gjet!\n");return;}
    printf("  Fshi '%s'? (1=Po): ",a[idx].pershkrim); if(lexoInt()!=1){printf("  [ANULUAR]\n");return;}
    for(int i=idx;i<n-1;i++) a[i]=a[i+1];
    n--;
    ruajShpenzime(a,n); printf("  [OK] Shpenzimi u fshi!\n");
}

/* 9 */
static void u_renditShuma(int uid) {
    /* Rendit shpenzimet nga shuma me e madhe tek me e vogla */
    Shpenzim all[MAX_SHPENZIME]; int n; lexoShpenzime(all,&n);
    Shpenzim a[MAX_SHPENZIME]; int m=0;
    for(int i=0;i<n;i++) if(all[i].id_user==uid) a[m++]=all[i];
    for(int i=0;i<m-1;i++)
        for(int j=0;j<m-1-i;j++)
            if(a[j].shuma<a[j+1].shuma){Shpenzim t=a[j];a[j]=a[j+1];a[j+1]=t;}
    printf("\nSHPENZIMET (sipas shumes, zbritas)\n"); printShpenzimHeader();
    for(int i=0;i<m;i++) printShpenzim(&a[i]);
    if(!m) printf("  (Ska)\n");
}

/* 10 */
static void u_renditData(int uid) {
    /* Rendit shpenzimet sipas dates */
    Shpenzim all[MAX_SHPENZIME]; int n; lexoShpenzime(all,&n);
    Shpenzim a[MAX_SHPENZIME]; int m=0;
    for(int i=0;i<n;i++) if(all[i].id_user==uid) a[m++]=all[i];
    for(int i=0;i<m-1;i++)
        for(int j=0;j<m-1-i;j++)
            if(krahasoData(a[j].data,a[j+1].data)>0){Shpenzim t=a[j];a[j]=a[j+1];a[j+1]=t;}
    printf("\nSHPENZIMET (sipas dates, ngjites)\n"); printShpenzimHeader();
    for(int i=0;i<m;i++) printShpenzim(&a[i]);
    if(!m) printf("  (Ska)\n");
}

/* 11 */
static void u_kerkoAvancuar(int uid) {
    /* Kerkim me disa menyra: ID, date, kategori ose interval datash */
    printf("  1.ID  2.Data  3.Kategori  4.Interval datash\n  Zgjedhja: "); int z=lexoInt();
    Shpenzim all[MAX_SHPENZIME]; int n; lexoShpenzime(all,&n);
    printShpenzimHeader(); int f=0;
    if(z==1){
        printf("  ID: "); int id=lexoInt();
        for(int i=0;i<n;i++) if(all[i].id_user==uid && all[i].id_shpenzim==id){printShpenzim(&all[i]);f=1;}
    } else if(z==2){
        printf("  Data (DD/MM/YYYY): "); char d[20]; lexoStr(d,sizeof(d));
        for(int i=0;i<n;i++) if(all[i].id_user==uid && strcmp(all[i].data,d)==0){printShpenzim(&all[i]);f=1;}
    } else if(z==3){
        printf("  Kategoria: "); char k[50]; lexoStr(k,sizeof(k));
        for(int i=0;i<n;i++) if(all[i].id_user==uid && strcmp(all[i].kategori.emertimi,k)==0){printShpenzim(&all[i]);f=1;}
    } else if(z==4){
        printf("  Data fillimit (DD/MM/YYYY): "); char d1[20]; lexoStr(d1,sizeof(d1));
        printf("  Data mbarimit (DD/MM/YYYY): "); char d2[20]; lexoStr(d2,sizeof(d2));
        for(int i=0;i<n;i++)
            if(all[i].id_user==uid && krahasoData(all[i].data,d1)>=0 && krahasoData(all[i].data,d2)<=0){printShpenzim(&all[i]);f=1;}
    } else { printf("  [GABIM] Opsion invalid!\n"); return; }
    if(!f) printf("  (Asnje rezultat)\n");
}

void menuUser(int uid) {
    /* Menuja kryesore per perdoruesin normal */
    int z;
    do {
        printf("\nMENUJA E PERDORUESIT\n");
        printf("1. Shto shpenzim te ri\n");
        printf("2. Shto te ardhura\n");
        printf("3. Shfaq shpenzimet e mia\n");
        printf("4. Shfaq te ardhurat e mia\n");
        printf("5. Raport mujor/vjetor\n");
        printf("6. Kontrollo buxhetin\n");
        printf("7. Kerko sipas kategorise\n");
        printf("8. Fshi shpenzim\n");
        printf("9. Rendit sipas shumes\n");
        printf("10. Rendit sipas dates\n");
        printf("11. Kerkimi avancuar\n");
        printf("12. Dil (Menu 1)\n");
        printf("  Zgjedhja: "); z=lexoInt();
        switch(z){
            case 1:  u_shtoShpenzim(uid); break;
            case 2:  u_shtoTeArdhura(uid); break;
            case 3:  u_shfaqShpenzime(uid); break;
            case 4:  u_shfaqTeArdhura(uid); break;
            case 5:  u_raport(uid); break;
            case 6:  u_buxheti(uid); break;
            case 7:  u_kerkoKategori(uid); break;
            case 8:  u_fshiShpenzim(uid); break;
            case 9:  u_renditShuma(uid); break;
            case 10: u_renditData(uid); break;
            case 11: u_kerkoAvancuar(uid); break;
            case 12: printf("  Kthehuni ne Menu 1...\n"); break;
            default: printf("  [GABIM] Opsion invalid!\n"); break;
        }
    } while(z!=12);
}

