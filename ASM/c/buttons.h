#ifndef BUTTONS_H
#define BUTTONS_H

#include "z64.h"
#include "z64_extended.h"

void handle_l_button();
void handle_layout();
void reset_layout();
void handle_hud();
void set_b_button();
void inventory_editor();
void settings_menu();
void swap_item(z64_slot_t slot, z64_item_t original, z64_item_t swap);
void handle_infinite();
void handle_rupee_dash();
void handle_weaker_swords();
void handle_abilities();
void handle_abilities_tunic_colors();
void handle_power_crouch_stab_fix();

#endif
