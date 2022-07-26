#ifndef DPAD_PAUSED_H
#define DPAD_PAUSED_H

#include "z64.h"
#include "z64_extended.h"

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

void handle_dpad_paused();
void handle_dpad_slots(pad_t pad_pressed);
void check_lens();
void unlock_lens(uint8_t button);
void set_dpad_action(pad_t pad_pressed, uint8_t action);
void handle_unequipping(pad_t pad_pressed);
void handle_downgrading(pad_t pad_pressed);
void swap_item(z64_slot_t slot, z64_item_t original, z64_item_t swap);
void unequip_sword(uint8_t play);
void unequip_shield();

#endif
