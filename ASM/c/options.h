#ifndef OPTIONS_H
#define OPTIONS_H

#include "z64.h"
#include "z64_extended.h"

#include <string.h>
#include <stdio.h>

void handle_options_menu();
void handle_options_menu_input(pad_t pad_pressed);
uint8_t write_option(uint8_t index, uint8_t shift, pad_t pad_pressed, uint16_t save);
uint8_t draw_settings_menu(z64_disp_buf_t *db);
uint8_t draw_abilities_info(z64_disp_buf_t *db);
void handle_inventory_editor();

#endif
