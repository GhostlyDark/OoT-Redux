#ifndef DPAD_H
#define DPAD_H

#include "z64.h"
#include "z64_extended.h"

/* Functions */
#define z64_playsfx						((playsfx_t)			0x800C806C)

void handle_dpad();
void run_default_options_setup();
void handle_dpad_ingame();
void draw_dpad();
void draw_dpad_icons(z64_disp_buf_t *db);

#endif