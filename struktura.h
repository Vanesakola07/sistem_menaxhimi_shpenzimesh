#ifndef STRUKTURA_H
#define STRUKTURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_USERS      "user.txt"
#define FILE_SHPENZIME  "shpenzime.txt"
#define FILE_TEARDHURA  "teArdhura.txt"
#define FILE_KATEGORITE "kategorite.txt"

#define ADMIN_USER "admin"
#define ADMIN_PASS "Admin1!"

#define MAX_USERS 500
#define MAX_SHPENZIME 2000
#define MAX_TEARDHURA 2000
#define MAX_KATEGORI 200

/* Struktura per kategorite e shpenzimeve */
typedef struct {
    int  id_kategori;
    char emertimi[50];
    char pershkrimi[100];
} Kategori;

/* Struktura per perdoruesit e sistemit */
typedef struct {
    int   id_user;
    char  emri[50];
    char  username[50];
    char  password[50];
    float buxheti_mujor;
} User;

/* Struktura per shpenzimet. Kategoria eshte strukture me vete. */
typedef struct {
    int      id_shpenzim;
    int      id_user;
    char     pershkrim[100];
    Kategori kategori;
    float    shuma;
    char     data[20]; /* DD/MM/YYYY */
} Shpenzim;

/* Struktura per te ardhurat e perdoruesit */
typedef struct {
    int   id_hyrje;
    int   id_user;
    char  burimi[100];
    float shuma;
    char  data[20];
} TeArdhura;


/* utils */
void  lexoStr(char *buf, int n);
int   lexoInt(void);
float lexoFloat(void);
int   validoPass(const char *p);
int   userEkziston(int id);
int   usernameEkziston(const char *u);
int   shpenzimEkziston(int id);
int   teArdhuratEkziston(int id);
int   kategoriaEkziston(int id);

/* skedar — users */
void lexoUsers(User *arr, int *n);
void ruajUsers(User *arr, int n);

/* skedar — shpenzime */
void lexoShpenzime(Shpenzim *arr, int *n);
void ruajShpenzime(Shpenzim *arr, int n);

/* skedar — te ardhura */
void lexoTeArdhura(TeArdhura *arr, int *n);
void ruajTeArdhura(TeArdhura *arr, int n);

/* skedar — kategori */
void lexoKategori(Kategori *arr, int *n);
void ruajKategori(Kategori *arr, int n);

/* auth */
int loginAdmin(void);
int loginUser(void); /* kthen id_user ose -1 */

/* admin */
void menuAdmin(void);

/* perdorues */
void menuUser(int id_user);

#endif
