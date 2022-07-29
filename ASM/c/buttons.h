#ifndef BUTTONS_H
#define BUTTONS_H

#include "z64.h"
#include "z64_extended.h"
#include "dpad_paused.h"

#define CAN_DRAW_HUD ( ( (uint32_t)z64_ctxt.state_dtor==z64_state_ovl_tab[3].vram_dtor) && (z64_file.game_mode == 0) && ( (z64_event_state_1 & 0x20) == 0) )

void handle_buttons();
void handle_l_button();
void handle_layout();
void handle_hud();
void toggle_minimap();
void hide_hud();
void set_b_button(pad_t pad_pressed);
void inventory_editor();
void handle_downgrading();
void swap_item(z64_slot_t slot, z64_item_t original, z64_item_t swap);

#endif
