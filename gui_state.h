#ifndef GUI_STATE_H
#define GUI_STATE_H

/*
 * gui_state.h
 * -----------
 * Bridges your existing struktura.h types into the GUI render loop.
 * DO NOT touch struktura.h or funksione.c logic — this file only adds.
 *
 * Screens map 1-to-1 with your original do/while branches in main.c.
 */

#include "struktura.h"   /* User, Shpenzim, TeArdhura, Kategori — untouched */

/* ── Screen IDs ──────────────────────────────────────────────────────────── */
typedef enum {
    SCR_MAIN        = 0,
    SCR_LOGIN_ADMIN,
    SCR_LOGIN_USER,
    SCR_MENU_ADMIN,
    SCR_MENU_USER,
    SCR_QUIT
} Screen;

/* ── Per-screen sub-state for forms ──────────────────────────────────────── */
typedef struct {
    /* Login form buffers (replaces your lexoStr() prompts) */
    char inp_user[50];
    char inp_pass[50];
    int  login_field;  /* 0 = none, 1 = username, 2 = password */

    /* Status bar — set after any action */
    char msg[160];
    int  msg_is_ok;    /* 1 = green success, 0 = red error */

    /* Currently logged-in user */
    int  active_uid;   /* -1 = none */
    char active_name[50];

    /* Add-expense form */
    char f_pershkrim[100];
    char f_shuma[32];
    char f_data[20];
    int  f_kat_idx;    /* index into cached kategori array */

    /* Add-income form */
    char f_burimi[100];
    char f_ta_shuma[32];
    char f_ta_data[20];

    /* Admin: add-user form */
    char a_emri[50];
    char a_username[50];
    char a_pass[50];
    char a_buxhet[32];

    /* Scroll offsets for list panels */
    int  scroll_shp;
    int  scroll_ta;
    int  scroll_users;

    /* Tab inside user menu (0-3) */
    int  user_tab;
    /* Tab inside admin menu (0-2) */
    int  admin_tab;

} FormState;

/* ── Cached data (reloaded on every screen transition) ───────────────────── */
typedef struct {
    User      users[MAX_USERS];        int n_users;
    Shpenzim  shp[MAX_SHPENZIME];      int n_shp;
    TeArdhura ta[MAX_TEARDHURA];       int n_ta;
    Kategori  kat[MAX_KATEGORI];       int n_kat;
} Cache;

/* ── Top-level application state ─────────────────────────────────────────── */
typedef struct {
    Screen     screen;
    FormState  form;
    Cache      cache;
} AppState;

/* ── Helper: reload all files into cache ─────────────────────────────────── */
static inline void cache_reload(Cache *c) {
    lexoUsers(c->users,   &c->n_users);
    lexoShpenzime(c->shp, &c->n_shp);
    lexoTeArdhura(c->ta,  &c->n_ta);
    lexoKategori(c->kat,  &c->n_kat);
}

/* ── Helper: set status message ──────────────────────────────────────────── */
static inline void set_msg(FormState *f, int ok, const char *txt) {
    f->msg_is_ok = ok;
    strncpy(f->msg, txt, sizeof(f->msg) - 1);
    f->msg[sizeof(f->msg) - 1] = '\0';
}

/* ── Helper: clear all form input buffers ────────────────────────────────── */
static inline void form_clear(FormState *f) {
    f->inp_user[0] = f->inp_pass[0] = '\0';
    f->f_pershkrim[0] = f->f_shuma[0] = f->f_data[0] = '\0';
    f->f_burimi[0] = f->f_ta_shuma[0] = f->f_ta_data[0] = '\0';
    f->a_emri[0] = f->a_username[0] = f->a_pass[0] = f->a_buxhet[0] = '\0';
    f->f_kat_idx = 0;
    f->login_field = 0;
    f->scroll_shp = f->scroll_ta = f->scroll_users = 0;
    f->msg[0] = '\0';
}

#endif /* GUI_STATE_H */
