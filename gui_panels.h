#ifndef GUI_PANELS_H
#define GUI_PANELS_H

/*
 * gui_panels.h
 * ------------
 * One function per screen. Each function:
 *   - Receives (AppState *st) — reads cache, writes form buffers and screen
*   - Calls your EXISTING lexo_x/ruaj_x functions directly (no wrappers)
 *   - Returns void; screen transitions happen by setting st->screen
 *
 * Raylib/raygui IMGUI: call these inside the BeginDrawing()/EndDrawing() block.
 * Nothing here touches struktura.h types or file I/O logic.
 */

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "gui_state.h"

/* ── Design tokens ───────────────────────────────────────────────────────── */
#define COL_BG       (Color){255,244,248,  255}   /* soft pink background */
#define COL_SURFACE  (Color){255,255,255,  255}   /* clean card surface   */
#define COL_SURFACE2 (Color){255,236,244,  255}   /* pale pink panels     */
#define COL_BORDER   (Color){244,174,204,  255}   /* soft rose border     */
#define COL_ACCENT   (Color){190, 31, 96,  255}   /* rose accent          */
#define COL_ACCENT2  (Color){255, 91,161,  255}   /* bright pink accent   */
#define COL_OK       (Color){42, 157,101,  255}   /* success green        */
#define COL_ERR      (Color){205, 55, 79,  255}   /* error red            */
#define COL_TEXT     (Color){41,  25, 34,  255}   /* body text            */
#define COL_MUTED    (Color){122, 83,101,  255}   /* secondary text       */
#define COL_ROW      (Color){255,247,250,  255}   /* table row fill       */

#define FONT_SIZE    16
#define PAD          20
#define BTN_H        36
#define INPUT_H      32
#define ROW_H        28

/* Screen dimensions — set once in main, read here via GetScreenWidth/Height */
#define SW  ((float)GetScreenWidth())
#define SH  ((float)GetScreenHeight())

static Font ui_font = {0};
static int ui_font_loaded = 0;

static void ui_load_font(void) {
    ui_font = LoadFontEx("Figtree-Regular.ttf", 36, NULL, 0);
    if (ui_font.texture.id > 0) {
        SetTextureFilter(ui_font.texture, TEXTURE_FILTER_BILINEAR);
        GuiSetFont(ui_font);
        ui_font_loaded = 1;
    }
}

static void ui_unload_font(void) {
    if (ui_font_loaded) {
        UnloadFont(ui_font);
        ui_font_loaded = 0;
    }
}

static int ui_measure_text(const char *txt, int size) {
    if (ui_font_loaded) {
        return (int)MeasureTextEx(ui_font, txt, (float)size, 1.0f).x;
    }
    return MeasureText(txt, size);
}

static void ui_draw_text(const char *txt, int x, int y, int size, Color color) {
    if (ui_font_loaded) {
        DrawTextEx(ui_font, txt, (Vector2){(float)x, (float)y}, (float)size, 1.0f, color);
    } else {
        DrawText(txt, x, y, size, color);
    }
}

/* ── Internal draw helpers ───────────────────────────────────────────────── */

/* Filled rounded card */
static void draw_card(float x, float y, float w, float h) {
    DrawRectangleRounded((Rectangle){x + 2, y + 4, w, h}, 0.08f, 10,
                         (Color){215, 90, 140, 32});
    DrawRectangleRounded((Rectangle){x, y, w, h}, 0.08f, 10, COL_SURFACE);
    DrawRectangleRoundedLines((Rectangle){x, y, w, h}, 0.08f, 10, COL_BORDER);
}

/* Centred title text */
static void draw_title(const char *txt, float y) {
    int tw = ui_measure_text(txt, 22);
    ui_draw_text(txt, (int)(SW/2 - tw/2), (int)y, 22, COL_ACCENT);
}

/* Status bar at bottom of card */
static void draw_status(const FormState *f, float card_x, float card_y,
                        float card_w, float card_h) {
    if (f->msg[0] == '\0') return;
    Color c = f->msg_is_ok ? COL_OK : COL_ERR;
    int tw = ui_measure_text(f->msg, FONT_SIZE);
    ui_draw_text(f->msg,
             (int)(card_x + card_w/2 - tw/2),
             (int)(card_y + card_h - PAD - FONT_SIZE),
             FONT_SIZE, c);
}

/* raygui style reset to our dark theme (call once before panels) */
static void theme_apply(void) {
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR,   0xFFF4F8FF);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL,  0xFFFFFFFF);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0xFFE7F0FF);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0xFFD5E5FF);
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL,  0xF4AECCFF);
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0xBE1F60FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL,  0x291922FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0x291922FF);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,   0xBE1F60FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED,  0xD83278FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED,  0x9F174FFF);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0xBE1F60FF);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED,0x9F174FFF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,   0xFFFFFFFF);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED,  0xFFFFFFFF);
    GuiSetStyle(DEFAULT, TEXT_SIZE, FONT_SIZE);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiSetStyle(TEXTBOX, TEXT_PADDING, 6);
}

#define DrawText(txt, x, y, size, color) ui_draw_text((txt), (x), (y), (size), (color))
#define MeasureText(txt, size) ui_measure_text((txt), (size))

static void login_textbox(Rectangle bounds, char *text, int text_size,
                          int field_id, FormState *form) {
    int editing = (form->login_field == field_id);
    if (GuiTextBox(bounds, text, text_size, editing)) {
        form->login_field = editing ? 0 : field_id;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SCREEN 1 — MAIN MENU
 * Replaces: your do/while printf menu in main.c
 * ═══════════════════════════════════════════════════════════════════════════ */
void panel_main(AppState *st) {
    float cw = 340, ch = 280;
    float cx = SW/2 - cw/2, cy = SH/2 - ch/2;

    draw_card(cx, cy, cw, ch);
    draw_title("Menaxhimi i Shpenzimeve", cy + PAD);

    int prompt_w = MeasureText("Zgjidhni opsionin:", FONT_SIZE);
    DrawText("Zgjidhni opsionin:", (int)(cx + cw/2 - prompt_w/2), (int)(cy + 70),
             FONT_SIZE, COL_MUTED);

    /* Button: Administrator */
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,  0xFFE7F0FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xFFD5E5FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0xF4AECCFF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,  0xBE1F60FF);
    if (GuiButton((Rectangle){cx+PAD, cy+104, cw-PAD*2, BTN_H},
                  "Administrator")) {
        form_clear(&st->form);
        st->screen = SCR_LOGIN_ADMIN;
    }

    /* Button: Perdorues */
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,  0xBE1F60FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xD83278FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x9F174FFF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,  0xFFFFFFFF);
    if (GuiButton((Rectangle){cx+PAD, cy+150, cw-PAD*2, BTN_H},
                  "Perdorues")) {
        form_clear(&st->form);
        st->screen = SCR_LOGIN_USER;
    }

    /* Button: Dil */
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,  0xFFFFFFFF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xFFE7F0FF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0xFFD5E5FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,  0xCD374FFF);
    if (GuiButton((Rectangle){cx+PAD, cy+196, cw-PAD*2, BTN_H},
                  "Dil nga programi")) {
        st->screen = SCR_QUIT;
    }
    theme_apply(); /* restore normal button style */

    /* Version tag */
    DrawText("v1.0 - Universiteti Politeknik i Tiranes", (int)(cx+PAD), (int)(cy+ch-26),
             12, COL_MUTED);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SCREEN 2 — ADMIN LOGIN
 * Replaces: loginAdmin() terminal prompts.
 * Your credential constants (ADMIN_USER / ADMIN_PASS) are used directly.
 * ═══════════════════════════════════════════════════════════════════════════ */
void panel_login_admin(AppState *st) {
    float cw = 380, ch = 290;
    float cx = SW/2 - cw/2, cy = SH/2 - ch/2;

    draw_card(cx, cy, cw, ch);
    draw_title("Hyrja e Administratorit", cy + PAD);

    float iy = cy + 72;

    DrawText("Username", (int)(cx+PAD), (int)(iy), FONT_SIZE, COL_MUTED);
    login_textbox((Rectangle){cx+PAD, iy+18, cw-PAD*2, INPUT_H},
                  st->form.inp_user, 49, 1, &st->form);

    iy += 68;
    DrawText("Password", (int)(cx+PAD), (int)(iy), FONT_SIZE, COL_MUTED);
    /* raygui has no built-in password masking; mask display manually */
    char masked[50] = {0};
    int plen = (int)strlen(st->form.inp_pass);
    for (int i = 0; i < plen && i < 49; i++) masked[i] = '*';
    /* We render a read-only masked box and a hidden edit box on top */
    login_textbox((Rectangle){cx+PAD, iy+18, cw-PAD*2, INPUT_H},
                  st->form.inp_pass, 49, 2, &st->form);
    /* Overdraw with asterisks (visual only) */
    DrawRectangle((int)(cx+PAD+2), (int)(iy+20), (int)(cw-PAD*2-4), INPUT_H-4,
                  COL_SURFACE);
    DrawText(masked, (int)(cx+PAD+6), (int)(iy+22), FONT_SIZE, COL_TEXT);

    iy += 72;

    /* Kyçu button */
    if (GuiButton((Rectangle){cx+PAD, iy, cw-PAD*2, BTN_H}, "Kyçu")) {
        /* ── Your existing credential check, no logic changed ── */
        if (strcmp(st->form.inp_user, ADMIN_USER) == 0 &&
            strcmp(st->form.inp_pass, ADMIN_PASS) == 0) {
            set_msg(&st->form, 1, "Mirë se erdhe, Administrator!");
            cache_reload(&st->cache);
            st->screen = SCR_MENU_ADMIN;
        } else {
            set_msg(&st->form, 0, "Kredenciale të gabuara!");
        }
    }

    /* Back */
    DrawText("← Kthehu", (int)(cx+PAD), (int)(cy+ch-40), FONT_SIZE, COL_MUTED);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (mp.x >= cx+PAD && mp.x <= cx+PAD+90 &&
            mp.y >= cy+ch-40 && mp.y <= cy+ch-20) {
            form_clear(&st->form);
            st->screen = SCR_MAIN;
        }
    }

    draw_status(&st->form, cx, cy, cw, ch);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SCREEN 3 — USER LOGIN
 * Replaces: loginUser() terminal prompts.
 * Uses your existing lexoUsers() to verify — not rewritten.
 * ═══════════════════════════════════════════════════════════════════════════ */
void panel_login_user(AppState *st) {
    float cw = 380, ch = 290;
    float cx = SW/2 - cw/2, cy = SH/2 - ch/2;

    draw_card(cx, cy, cw, ch);
    draw_title("Hyrja e Përdoruesit", cy + PAD);

    float iy = cy + 72;

    DrawText("Username", (int)(cx+PAD), (int)(iy), FONT_SIZE, COL_MUTED);
    login_textbox((Rectangle){cx+PAD, iy+18, cw-PAD*2, INPUT_H},
                  st->form.inp_user, 49, 1, &st->form);

    iy += 68;
    DrawText("Password", (int)(cx+PAD), (int)(iy), FONT_SIZE, COL_MUTED);
    login_textbox((Rectangle){cx+PAD, iy+18, cw-PAD*2, INPUT_H},
                  st->form.inp_pass, 49, 2, &st->form);

    /* Same asterisk overdraw trick as admin login */
    char masked[50] = {0};
    int plen = (int)strlen(st->form.inp_pass);
    for (int i = 0; i < plen && i < 49; i++) masked[i] = '*';
    DrawRectangle((int)(cx+PAD+2), (int)(iy+20), (int)(cw-PAD*2-4), INPUT_H-4,
                  COL_SURFACE);
    DrawText(masked, (int)(cx+PAD+6), (int)(iy+22), FONT_SIZE, COL_TEXT);

    iy += 72;

    if (GuiButton((Rectangle){cx+PAD, iy, cw-PAD*2, BTN_H}, "Kyçu")) {
        /* ── Your existing loginUser() logic, inlined — file I/O unchanged ── */
        lexoUsers(st->cache.users, &st->cache.n_users);
        int found = 0;
        for (int i = 0; i < st->cache.n_users; i++) {
            if (strcmp(st->cache.users[i].username, st->form.inp_user) == 0 &&
                strcmp(st->cache.users[i].password, st->form.inp_pass) == 0) {
                st->form.active_uid = st->cache.users[i].id_user;
                strncpy(st->form.active_name, st->cache.users[i].emri,
                        sizeof(st->form.active_name)-1);
                cache_reload(&st->cache);
                char buf[80];
                snprintf(buf, sizeof(buf), "Mirë se erdhe, %s!",
                         st->form.active_name);
                set_msg(&st->form, 1, buf);
                st->form.user_tab = 0;
                st->screen = SCR_MENU_USER;
                found = 1;
                break;
            }
        }
        if (!found) set_msg(&st->form, 0, "Username ose password gabim!");
    }

    DrawText("← Kthehu", (int)(cx+PAD), (int)(cy+ch-40), FONT_SIZE, COL_MUTED);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        if (mp.x >= cx+PAD && mp.x <= cx+PAD+90 &&
            mp.y >= cy+ch-40 && mp.y <= cy+ch-20) {
            form_clear(&st->form);
            st->screen = SCR_MAIN;
        }
    }

    draw_status(&st->form, cx, cy, cw, ch);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SCREEN 4 — USER MENU  (tabbed layout)
 *
 * Tabs:   [0] Shpenzime   [1] Të Ardhura   [2] Shto   [3] Raport
 *
 * Each tab body calls your existing u_* static functions OR replicates their
 * file I/O inline (since they're static in funksione.c and can't be forward-
 * declared without making them non-static — see NOTES in main.c).
 * ═══════════════════════════════════════════════════════════════════════════ */

/* ── Tab 0: list of expenses for active user ── */
static void tab_shpenzime(AppState *st) {
    float lx = PAD, ly = 100, lw = SW - PAD*2, row = ROW_H;
    int uid = st->form.active_uid;

    /* Header row */
    DrawText("ID",   (int)lx,      (int)ly, 13, COL_MUTED);
    DrawText("Pershkrim", (int)(lx+40),  (int)ly, 13, COL_MUTED);
    DrawText("Kategori",  (int)(lx+240), (int)ly, 13, COL_MUTED);
    DrawText("Shuma",     (int)(lx+390), (int)ly, 13, COL_MUTED);
    DrawText("Data",      (int)(lx+490), (int)ly, 13, COL_MUTED);
    DrawLine((int)lx, (int)(ly+16), (int)(lx+lw), (int)(ly+16), COL_BORDER);

    ly += 20;
    int shown = 0;
    for (int i = 0; i < st->cache.n_shp; i++) {
        Shpenzim *s = &st->cache.shp[i];
        if (s->id_user != uid) continue;

        /* Alternating row background */
        if (shown % 2 == 0)
            DrawRectangle((int)lx, (int)ly, (int)lw, (int)row, COL_ROW);

        DrawText(TextFormat("%d",  s->id_shpenzim), (int)lx,      (int)(ly+6), 13, COL_TEXT);
        DrawText(s->pershkrim,                       (int)(lx+40), (int)(ly+6), 13, COL_TEXT);
        DrawText(s->kategori.emertimi,               (int)(lx+240),(int)(ly+6), 13, COL_TEXT);
        DrawText(TextFormat("%.0f L", s->shuma),     (int)(lx+390),(int)(ly+6), 13, COL_ACCENT);
        DrawText(s->data,                            (int)(lx+490),(int)(ly+6), 13, COL_MUTED);

        ly += row;
        shown++;
        if (ly > SH - 80) break; /* simple clamp — add scroll if needed */
    }

    if (shown == 0)
        DrawText("(Asnjë shpenzim i regjistruar)", (int)(lx+40), (int)ly,
                 FONT_SIZE, COL_MUTED);
}

/* ── Tab 1: list of incomes for active user ── */
static void tab_teardhura(AppState *st) {
    float lx = PAD, ly = 100, lw = SW - PAD*2, row = ROW_H;
    int uid = st->form.active_uid;

    DrawText("ID",     (int)lx,      (int)ly, 13, COL_MUTED);
    DrawText("Burimi", (int)(lx+40), (int)ly, 13, COL_MUTED);
    DrawText("Shuma",  (int)(lx+300),(int)ly, 13, COL_MUTED);
    DrawText("Data",   (int)(lx+400),(int)ly, 13, COL_MUTED);
    DrawLine((int)lx, (int)(ly+16), (int)(lx+lw), (int)(ly+16), COL_BORDER);
    ly += 20;

    int shown = 0;
    for (int i = 0; i < st->cache.n_ta; i++) {
        TeArdhura *t = &st->cache.ta[i];
        if (t->id_user != uid) continue;
        if (shown % 2 == 0)
            DrawRectangle((int)lx,(int)ly,(int)lw,(int)row,COL_ROW);
        DrawText(TextFormat("%d", t->id_hyrje), (int)lx,       (int)(ly+6), 13, COL_TEXT);
        DrawText(t->burimi,                      (int)(lx+40),  (int)(ly+6), 13, COL_TEXT);
        DrawText(TextFormat("%.0f L", t->shuma), (int)(lx+300), (int)(ly+6), 13, COL_OK);
        DrawText(t->data,                        (int)(lx+400), (int)(ly+6), 13, COL_MUTED);
        ly += row; shown++;
        if (ly > SH - 80) break;
    }
    if (shown == 0)
        DrawText("(Asnjë e ardhur e regjistruar)", (int)(lx+40), (int)ly,
                 FONT_SIZE, COL_MUTED);
}

/* ── Tab 2: add expense + add income forms side by side ── */
static void tab_shto(AppState *st) {
    int uid = st->form.active_uid;
    float half = (SW - PAD*3) / 2.0f;

    /* ── Left: Shto Shpenzim ── */
    float cx = PAD, cy = 90;
    draw_card(cx, cy, half, 280);
    DrawText("Shto Shpenzim", (int)(cx+PAD), (int)(cy+PAD), FONT_SIZE, COL_ACCENT);

    float iy = cy + 50;
    DrawText("Pershkrim", (int)(cx+PAD), (int)iy, 13, COL_MUTED);
    GuiTextBox((Rectangle){cx+PAD, iy+16, half-PAD*2, INPUT_H},
               st->form.f_pershkrim, 99, true);

    iy += 56;
    DrawText("Shuma (ALL)", (int)(cx+PAD), (int)iy, 13, COL_MUTED);
    GuiTextBox((Rectangle){cx+PAD, iy+16, half-PAD*2, INPUT_H},
               st->form.f_shuma, 31, true);

    iy += 56;
    DrawText("Data (DD/MM/YYYY)", (int)(cx+PAD), (int)iy, 13, COL_MUTED);
    GuiTextBox((Rectangle){cx+PAD, iy+16, half-PAD*2, INPUT_H},
               st->form.f_data, 19, true);

    iy += 56;
    /* Kategori dropdown label */
    DrawText("Kategori:", (int)(cx+PAD), (int)iy, 13, COL_MUTED);
    if (st->cache.n_kat > 0) {
        const char *kname = st->cache.kat[st->form.f_kat_idx].emertimi;
        /* Simple prev/next selector (raygui DropdownBox needs active flag) */
        if (GuiButton((Rectangle){cx+PAD+80, iy-4, 20, 24}, "<") &&
            st->form.f_kat_idx > 0) st->form.f_kat_idx--;
        DrawText(kname, (int)(cx+PAD+108), (int)iy, 13, COL_TEXT);
        if (GuiButton((Rectangle){cx+half-PAD-24, iy-4, 20, 24}, ">") &&
            st->form.f_kat_idx < st->cache.n_kat-1) st->form.f_kat_idx++;
    } else {
        DrawText("(Nuk ka kategori)", (int)(cx+PAD+80), (int)iy, 13, COL_MUTED);
    }

    iy += 32;
    if (GuiButton((Rectangle){cx+PAD, iy, half-PAD*2, BTN_H}, "Regjistro")) {
        /* ── Uses your existing file I/O — just like u_shtoShpenzim() ── */
        if (strlen(st->form.f_pershkrim) == 0 ||
            strlen(st->form.f_shuma)     == 0 ||
            strlen(st->form.f_data)      != 10) {
            set_msg(&st->form, 0, "Plotëso të gjitha fushat!");
        } else {
            Shpenzim arr[MAX_SHPENZIME]; int n;
            lexoShpenzime(arr, &n);

            /* Generate next ID */
            int max_id = 0;
            for (int i = 0; i < n; i++)
                if (arr[i].id_shpenzim > max_id) max_id = arr[i].id_shpenzim;

            Shpenzim ns = {0};
            ns.id_shpenzim = max_id + 1;
            ns.id_user     = uid;
            strncpy(ns.pershkrim, st->form.f_pershkrim, 99);
            ns.shuma = (float)atof(st->form.f_shuma);
            strncpy(ns.data, st->form.f_data, 19);

            if (st->cache.n_kat > 0) {
                ns.kategori = st->cache.kat[st->form.f_kat_idx];
            } else {
                ns.kategori.id_kategori = 0;
                strncpy(ns.kategori.emertimi,  "Tjeter", 49);
                strncpy(ns.kategori.pershkrimi, "E paspecifikuar", 99);
            }

            arr[n++] = ns;
            ruajShpenzime(arr, n);
            cache_reload(&st->cache);
            st->form.f_pershkrim[0] = st->form.f_shuma[0] = st->form.f_data[0] = '\0';
            set_msg(&st->form, 1, "Shpenzimi u regjistrua!");
        }
    }

    /* ── Right: Shto Të Ardhura ── */
    cx = PAD*2 + half; cy = 90;
    draw_card(cx, cy, half, 280);
    DrawText("Shto Të Ardhura", (int)(cx+PAD), (int)(cy+PAD), FONT_SIZE, COL_OK);

    iy = cy + 50;
    DrawText("Burimi", (int)(cx+PAD), (int)iy, 13, COL_MUTED);
    GuiTextBox((Rectangle){cx+PAD, iy+16, half-PAD*2, INPUT_H},
               st->form.f_burimi, 99, true);

    iy += 56;
    DrawText("Shuma (ALL)", (int)(cx+PAD), (int)iy, 13, COL_MUTED);
    GuiTextBox((Rectangle){cx+PAD, iy+16, half-PAD*2, INPUT_H},
               st->form.f_ta_shuma, 31, true);

    iy += 56;
    DrawText("Data (DD/MM/YYYY)", (int)(cx+PAD), (int)iy, 13, COL_MUTED);
    GuiTextBox((Rectangle){cx+PAD, iy+16, half-PAD*2, INPUT_H},
               st->form.f_ta_data, 19, true);

    iy += 64;
    if (GuiButton((Rectangle){cx+PAD, iy, half-PAD*2, BTN_H}, "Regjistro")) {
        if (strlen(st->form.f_burimi)   == 0 ||
            strlen(st->form.f_ta_shuma) == 0 ||
            strlen(st->form.f_ta_data)  != 10) {
            set_msg(&st->form, 0, "Plotëso të gjitha fushat!");
        } else {
            TeArdhura arr[MAX_TEARDHURA]; int n;
            lexoTeArdhura(arr, &n);

            int max_id = 0;
            for (int i = 0; i < n; i++)
                if (arr[i].id_hyrje > max_id) max_id = arr[i].id_hyrje;

            TeArdhura nt = {0};
            nt.id_hyrje = max_id + 1;
            nt.id_user  = uid;
            strncpy(nt.burimi, st->form.f_burimi, 99);
            nt.shuma = (float)atof(st->form.f_ta_shuma);
            strncpy(nt.data, st->form.f_ta_data, 19);

            arr[n++] = nt;
            ruajTeArdhura(arr, n);
            cache_reload(&st->cache);
            st->form.f_burimi[0] = st->form.f_ta_shuma[0] = st->form.f_ta_data[0] = '\0';
            set_msg(&st->form, 1, "Të ardhurat u regjistruan!");
        }
    }

    /* Shared status bar centred below both cards */
    draw_status(&st->form, PAD, 90, SW-PAD*2, 300);
}

/* ── Tab 3: summary report (monthly totals) ── */
static void tab_raport(AppState *st) {
    int uid = st->form.active_uid;
    float total_shp = 0, total_ta = 0;

    for (int i = 0; i < st->cache.n_shp; i++)
        if (st->cache.shp[i].id_user == uid)
            total_shp += st->cache.shp[i].shuma;

    for (int i = 0; i < st->cache.n_ta; i++)
        if (st->cache.ta[i].id_user == uid)
            total_ta += st->cache.ta[i].shuma;

    float balance = total_ta - total_shp;
    Color bal_col = balance >= 0 ? COL_OK : COL_ERR;

    /* Three metric cards */
    float cw = (SW - PAD*4) / 3.0f, ch = 90, cy = 100;

    draw_card(PAD,           cy, cw, ch);
    draw_card(PAD*2+cw,      cy, cw, ch);
    draw_card(PAD*3+cw*2,    cy, cw, ch);

    DrawText("Të Ardhura Totale",  (int)(PAD+12),        (int)(cy+14), 13, COL_MUTED);
    DrawText("Shpenzime Totale",   (int)(PAD*2+cw+12),   (int)(cy+14), 13, COL_MUTED);
    DrawText("Bilanc",             (int)(PAD*3+cw*2+12), (int)(cy+14), 13, COL_MUTED);

    DrawText(TextFormat("%.0f L", total_ta),
             (int)(PAD+12), (int)(cy+42), 24, COL_OK);
    DrawText(TextFormat("%.0f L", total_shp),
             (int)(PAD*2+cw+12), (int)(cy+42), 24, COL_ERR);
    DrawText(TextFormat("%.0f L", balance),
             (int)(PAD*3+cw*2+12), (int)(cy+42), 24, bal_col);

    /* Budget check for current user */
    User *usr = NULL;
    for (int i = 0; i < st->cache.n_users; i++)
        if (st->cache.users[i].id_user == uid) { usr = &st->cache.users[i]; break; }

    if (usr) {
        float bux = usr->buxheti_mujor;
        float ratio = (bux > 0) ? (total_shp / bux) : 0;
        if (ratio > 1.0f) ratio = 1.0f;

        DrawText(TextFormat("Buxheti Mujor: %.0f L", bux),
                 (int)PAD, 230, FONT_SIZE, COL_TEXT);

        /* Simple progress bar */
        float bw = SW - PAD*2;
        DrawRectangle((int)PAD, 258, (int)bw, 18, COL_SURFACE2);
        Color bar_col = (total_shp > bux) ? COL_ERR : COL_ACCENT;
        DrawRectangle((int)PAD, 258, (int)(bw * ratio), 18, bar_col);
        DrawRectangleLines((int)PAD, 258, (int)bw, 18, COL_BORDER);
        DrawText(TextFormat("%.0f%%", ratio*100),
                 (int)(PAD + bw*ratio - 30), 260, 13, COL_TEXT);

        if (total_shp > bux)
            DrawText(TextFormat("⚠ Tejkalim buxheti me %.0f L!", total_shp - bux),
                     (int)PAD, 288, FONT_SIZE, COL_ERR);
    }
}

/* ── Top-level user menu: draws navbar then dispatches tabs ── */
void panel_menu_user(AppState *st) {
    /* Nav bar */
    DrawRectangle(0, 0, (int)SW, 52, COL_SURFACE);
    DrawLine(0, 52, (int)SW, 52, COL_BORDER);
    DrawText(TextFormat("▸ %s", st->form.active_name),
             (int)PAD, 16, FONT_SIZE, COL_ACCENT);

    /* Tabs */
    const char *tabs[] = {"Shpenzime", "Të Ardhura", "Shto", "Raport"};
    float tw = 130, tx = SW/2 - (tw*4)/2;
    for (int i = 0; i < 4; i++) {
        bool active = (st->form.user_tab == i);
        if (active)
            DrawRectangle((int)(tx + tw*i), 30, (int)tw, 22, COL_SURFACE2);
        if (GuiButton((Rectangle){tx + tw*i, 30, tw, 22}, tabs[i]))
            st->form.user_tab = i;
        if (active)
            DrawLine((int)(tx+tw*i), 52, (int)(tx+tw*i+tw), 52, COL_ACCENT);
    }

    /* Logout */
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xFFE7F0FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xCD374FFF);
    if (GuiButton((Rectangle){SW-100, 14, 80, 26}, "Dil"))
    {
        form_clear(&st->form);
        st->form.active_uid = -1;
        st->screen = SCR_MAIN;
    }
    theme_apply();

    /* Dispatch active tab */
    switch (st->form.user_tab) {
        case 0: tab_shpenzime(st); break;
        case 1: tab_teardhura(st); break;
        case 2: tab_shto(st);      break;
        case 3: tab_raport(st);    break;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SCREEN 5 — ADMIN MENU  (tabbed layout)
 *
 * Tabs: [0] Perdoruesit   [1] Shto Perdorues   [2] Kategori
 * ═══════════════════════════════════════════════════════════════════════════ */

static void atab_users(AppState *st) {
    float lx = PAD, ly = 100, lw = SW - PAD*2;

    DrawText("ID", (int)lx,       (int)ly, 13, COL_MUTED);
    DrawText("Emri",  (int)(lx+40), (int)ly, 13, COL_MUTED);
    DrawText("Username",(int)(lx+200),(int)ly, 13, COL_MUTED);
    DrawText("Buxhet", (int)(lx+380),(int)ly, 13, COL_MUTED);
    DrawLine((int)lx, (int)(ly+16), (int)(lx+lw), (int)(ly+16), COL_BORDER);
    ly += 20;

    for (int i = 0; i < st->cache.n_users; i++) {
        User *u = &st->cache.users[i];
        if (i%2==0) DrawRectangle((int)lx,(int)ly,(int)lw,ROW_H,COL_ROW);
        DrawText(TextFormat("%d", u->id_user),   (int)lx,       (int)(ly+6), 13, COL_TEXT);
        DrawText(u->emri,                         (int)(lx+40),  (int)(ly+6), 13, COL_TEXT);
        DrawText(u->username,                     (int)(lx+200), (int)(ly+6), 13, COL_TEXT);
        DrawText(TextFormat("%.0f L", u->buxheti_mujor),(int)(lx+380),(int)(ly+6),13,COL_ACCENT);

        /* Delete button per row */
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xFFFFFFFF);
        GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xFFE7F0FF);
        GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xCD374FFF);
        if (GuiButton((Rectangle){lx+lw-60, ly+2, 54, 22}, "Fshi")) {
            /* Remove user — your ruajUsers() handles persistence */
            User arr[MAX_USERS]; int n;
            lexoUsers(arr, &n);
            for (int j = i; j < n-1; j++) arr[j] = arr[j+1];
            ruajUsers(arr, n-1);
            cache_reload(&st->cache);
            set_msg(&st->form, 1, "Perdoruesi u fshi!");
            break;
        }
        theme_apply();
        ly += ROW_H;
        if (ly > SH - 80) break;
    }
}

static void atab_add_user(AppState *st) {
    float cw = 400, ch = 320;
    float cx = SW/2 - cw/2, cy = 90;
    draw_card(cx, cy, cw, ch);
    DrawText("Regjistro Perdorues të Ri", (int)(cx+PAD), (int)(cy+PAD),
             FONT_SIZE, COL_ACCENT);

    float iy = cy + 56;
    const char *labels[] = {"Emri", "Username", "Password", "Buxheti (ALL)"};
    char *bufs[] = {st->form.a_emri, st->form.a_username,
                    st->form.a_pass, st->form.a_buxhet};
    int   lens[] = {49, 49, 49, 31};

    for (int i = 0; i < 4; i++) {
        DrawText(labels[i], (int)(cx+PAD), (int)iy, 13, COL_MUTED);
        GuiTextBox((Rectangle){cx+PAD, iy+16, cw-PAD*2, INPUT_H},
                   bufs[i], lens[i], true);
        iy += 52;
    }

    if (GuiButton((Rectangle){cx+PAD, iy, cw-PAD*2, BTN_H}, "Regjistro")) {
        /* validoPass() from funksione.c — untouched */
        if (!validoPass(st->form.a_pass)) {
            set_msg(&st->form, 0,
                    "Password: >=4 kar, 1 madhe, 1 numer, 1 simbol");
        } else if (usernameEkziston(st->form.a_username)) {
            set_msg(&st->form, 0, "Username ekziston tashmë!");
        } else {
            User arr[MAX_USERS]; int n;
            lexoUsers(arr, &n);
            int max_id = 0;
            for (int i = 0; i < n; i++)
                if (arr[i].id_user > max_id) max_id = arr[i].id_user;

            User nu = {0};
            nu.id_user = max_id + 1;
            strncpy(nu.emri,       st->form.a_emri,     49);
            strncpy(nu.username,   st->form.a_username,  49);
            strncpy(nu.password,   st->form.a_pass,      49);
            nu.buxheti_mujor = (float)atof(st->form.a_buxhet);
            arr[n++] = nu;
            ruajUsers(arr, n);
            cache_reload(&st->cache);
            st->form.a_emri[0] = st->form.a_username[0] =
            st->form.a_pass[0] = st->form.a_buxhet[0] = '\0';
            set_msg(&st->form, 1, "Perdoruesi u regjistrua!");
        }
    }
    draw_status(&st->form, cx, cy, cw, ch);
}

static void atab_kategori(AppState *st) {
    float lx = PAD, ly = 100, lw = SW - PAD*2;
    DrawText("ID", (int)lx, (int)ly, 13, COL_MUTED);
    DrawText("Emërtimi", (int)(lx+40), (int)ly, 13, COL_MUTED);
    DrawText("Pershkrim", (int)(lx+200), (int)ly, 13, COL_MUTED);
    DrawLine((int)lx,(int)(ly+16),(int)(lx+lw),(int)(ly+16),COL_BORDER);
    ly += 20;
    for (int i = 0; i < st->cache.n_kat; i++) {
        Kategori *k = &st->cache.kat[i];
        if (i%2==0) DrawRectangle((int)lx,(int)ly,(int)lw,ROW_H,COL_ROW);
        DrawText(TextFormat("%d", k->id_kategori),(int)lx,      (int)(ly+6),13,COL_TEXT);
        DrawText(k->emertimi,                      (int)(lx+40), (int)(ly+6),13,COL_TEXT);
        DrawText(k->pershkrimi,                    (int)(lx+200),(int)(ly+6),13,COL_MUTED);
        ly += ROW_H;
        if (ly > SH - 80) break;
    }
    if (st->cache.n_kat == 0)
        DrawText("(Nuk ka kategori të regjistruara)", (int)(lx+40),(int)ly,
                 FONT_SIZE, COL_MUTED);
}

void panel_menu_admin(AppState *st) {
    DrawRectangle(0, 0, (int)SW, 52, COL_SURFACE);
    DrawLine(0, 52, (int)SW, 52, COL_BORDER);
    DrawText("▸ Administrator", (int)PAD, 16, FONT_SIZE, COL_ACCENT);

    const char *tabs[] = {"Perdoruesit", "Shto Perdorues", "Kategori"};
    float tw = 160, tx = SW/2 - (tw*3)/2;
    for (int i = 0; i < 3; i++) {
        bool active = (st->form.admin_tab == i);
        if (active)
            DrawRectangle((int)(tx+tw*i),30,(int)tw,22,COL_SURFACE2);
        if (GuiButton((Rectangle){tx+tw*i,30,tw,22}, tabs[i]))
            st->form.admin_tab = i;
        if (active)
            DrawLine((int)(tx+tw*i),52,(int)(tx+tw*i+tw),52,COL_ACCENT);
    }

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xFFE7F0FF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xCD374FFF);
    if (GuiButton((Rectangle){SW-100,14,80,26},"Dil")) {
        form_clear(&st->form);
        st->screen = SCR_MAIN;
    }
    theme_apply();

    switch (st->form.admin_tab) {
        case 0: atab_users(st);    break;
        case 1: atab_add_user(st); break;
        case 2: atab_kategori(st); break;
    }
}

#endif /* GUI_PANELS_H */

