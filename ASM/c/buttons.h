#ifndef BUTTONS_H
#define BUTTONS_H

#include "z64.h"
#include "z64_extended.h"

#define CAN_DRAW_HUD        (((uint32_t)z64_ctxt.state_dtor==z64_state_ovl_tab[3].vram_dtor) && \
                            (z64_file.game_mode == 0) && \
                            ((z64_event_state_1 & 0x20) == 0))

void handle_layout();
void handle_hud();
void handle_buttons();

#endif
