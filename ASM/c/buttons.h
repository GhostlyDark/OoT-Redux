#ifndef BUTTONS_H
#define BUTTONS_H

#include "z64.h"
#include "z64_extended.h"

void handle_l_button();
void handle_l_button_paused();
void toggle_options_menu();
void toggle_minimap();
uint16_t get_scale_value(uint8_t scale);
void set_scale_values(uint8_t option, uint8_t button, int8_t icon_size, int8_t c_size);
void handle_button_scaling();
void handle_layout();
void reset_layout();
void handle_hud();
void set_hide_hud(uint8_t value);
void set_b_button();
void handle_rupee_drain();
void handle_weaker_swords();
void handle_abilities();
void handle_abilities_tunic_colors();
void handle_power_crouch_stab_fix();
void handle_infinite();

#endif
