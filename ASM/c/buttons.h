#ifndef BUTTONS_H
#define BUTTONS_H

#include "z64.h"
#include "z64_extended.h"

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
void handle_infinite();
void handle_rupee_dash();
void handle_weaker_swords();
void handle_abilities();

#endif
