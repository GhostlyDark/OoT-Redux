#ifndef DPAD_H
#define DPAD_H

#include "z64.h"
#include "z64_extended.h"

/* Functions */
#define z64_playsfx ((playsfx_t) 0x800C806C)

void handle_dpad();
void handle_restore_mask();
void run_default_options_setup();
void handle_dpad_ingame();
void draw_dpad();
void draw_name_panel(z64_disp_buf_t *db);
void draw_settings_label(z64_disp_buf_t *db);
void draw_swap_item_icons(z64_disp_buf_t *db, z64_item_t curr_item, z64_item_t next_item);
void draw_ammo_on_b_button();
void draw_dpad_icons(z64_disp_buf_t *db);
void check_lens();
uint8_t has_lens_on_button();

#endif