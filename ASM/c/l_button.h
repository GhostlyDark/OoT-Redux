#ifndef L_BUTTON_H
#define L_BUTTON_H

#include "z64.h"

#define BLOCK_DPAD (0x00000001 | \
	                0x00000002 | \
                    0x00000080 | \
                    0x00000400 | \
                    0x10000000 | \
                    0x20000000)

#define CAN_USE_DPAD        (((z64_link.state_flags_1 & BLOCK_DPAD) == 0) && \
                            ((uint32_t)z64_ctxt.state_dtor==z64_state_ovl_tab[3].vram_dtor) && \
                            (z64_file.game_mode == 0) && \
                            ((z64_event_state_1 & 0x20) == 0))

void handle_l_button();

#endif
