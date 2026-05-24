#include "raylib.h"
 
/* ── Pull in our GUI state + all panel functions ──────────────────────────
 * gui_state.h  →  AppState, FormState, Cache, cache_reload(), set_msg()
 * gui_panels.h →  panel_main(), panel_login_admin(), panel_login_user(),
 *                 panel_menu_user(), panel_menu_admin()
 *
 * raygui.h is #included inside gui_panels.h with RAYGUI_IMPLEMENTATION,
 * so do NOT include it here a second time.                               */
#include "gui_state.h"
#include "gui_panels.h"   /* includes raygui.h once with RAYGUI_IMPLEMENTATION */
 
/* ── Window constants — tweak freely ──────────────────────────────────────*/
#define WIN_W    900
#define WIN_H    580
#define WIN_TITLE "Sistem për Menaxhimin e Shpenzimeve"
#define TARGET_FPS 60
 
int main(void) {
 
    /* ── Window init ──────────────────────────────────────────────────── */
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(WIN_W, WIN_H, WIN_TITLE);
    SetTargetFPS(TARGET_FPS);
    SetExitKey(KEY_NULL);   /* disable ESC-to-quit so we handle it ourselves */
 
    /* ── Application state — zero-initialised ────────────────────────── */
    AppState st = {0};
    st.screen          = SCR_MAIN;
    st.form.active_uid = -1;
 
    ui_load_font();

    /* Apply theme to raygui before first frame */
    theme_apply();
 
    /* ── Render loop ──────────────────────────────────────────────────── *
     * This is the direct replacement for:                                 *
     *   do { printf menu; z = lexoInt(); if(z==1)... } while(z!=3);      *
     *                                                                     *
     * Screen routing replaces the if/else chain.                          *
     * Each panel_*() function is the GUI equivalent of one menu branch.  */
    while (!WindowShouldClose() && st.screen != SCR_QUIT) {
 
        BeginDrawing();
            ClearBackground(COL_BG);
 
            /* ── Background grid (decorative, purely visual) ─────────── */
 
            /* ── Screen dispatch ──────────────────────────────────────── */
            switch (st.screen) {
 
                case SCR_MAIN:
                    panel_main(&st);
                    break;
 
                case SCR_LOGIN_ADMIN:
                    panel_login_admin(&st);
                    break;
 
                case SCR_LOGIN_USER:
                    panel_login_user(&st);
                    break;
 
                case SCR_MENU_USER:
                    panel_menu_user(&st);
                    break;
 
                case SCR_MENU_ADMIN:
                    panel_menu_admin(&st);
                    break;
 
                case SCR_QUIT:
                    break;   /* loop exits at while condition */
            }
 
            /* ── Global ESC: go back one level ───────────────────────── */
            if (IsKeyPressed(KEY_ESCAPE)) {
                if (st.screen == SCR_MENU_USER || st.screen == SCR_MENU_ADMIN) {
                    form_clear(&st.form);
                    st.form.active_uid = -1;
                    st.screen = SCR_MAIN;
                } else if (st.screen == SCR_LOGIN_ADMIN ||
                           st.screen == SCR_LOGIN_USER) {
                    form_clear(&st.form);
                    st.screen = SCR_MAIN;
                } else if (st.screen == SCR_MAIN) {
                    st.screen = SCR_QUIT;
                }
            }
 
            /* ── Global FPS counter (remove in final submission) ──────── */
        EndDrawing();
    }
 
    /* ── Cleanup ──────────────────────────────────────────────────────── */
    ui_unload_font();
    CloseWindow();
    return 0;
}
