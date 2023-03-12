#ifndef DPAD_H
#define DPAD_H

#include "z64.h"
#include "z64_extended.h"

#include <string.h>
#include <stdio.h>

void handle_dpad();
void handle_dpad_buttons();
void handle_dpad_ingame();
void handle_dpad_paused();
void handle_hud();
void handle_l_button();
void draw_dpad();
void draw_dpad_icons(z64_disp_buf_t *db);
uint8_t draw_abilities_info(z64_disp_buf_t *db);

#endif
