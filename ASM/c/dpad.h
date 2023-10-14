#ifndef DPAD_H
#define DPAD_H

#include "z64.h"
#include "z64_extended.h"

#include <string.h>
#include <stdio.h>

/* Functions */
#define z64_playsfx						((playsfx_t)			0x800C806C)

void handle_dpad();
void handle_dpad_ingame();
void draw_dpad();
void draw_dpad_icons(z64_disp_buf_t *db);
void draw_health(z64_disp_buf_t *db, z64_actor_t* actor);

#endif