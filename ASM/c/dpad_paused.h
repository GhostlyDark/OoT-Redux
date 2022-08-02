#ifndef DPAD_PAUSED_H
#define DPAD_PAUSED_H

#include "z64.h"
#include "z64_extended.h"

void handle_dpad_paused();
void handle_dpad_slots(pad_t pad_pressed);
void check_default_dpad_actions();
void check_lens();
void unlock_lens(uint8_t button);
void set_dpad_action(pad_t pad_pressed, dpad_action_t action, limit_item_t restriction);
void handle_unequipping(pad_t pad_pressed);
void handle_downgrading();
void swap_item(z64_slot_t slot, z64_item_t original, z64_item_t swap);
void unequip_sword(uint8_t play);
void unequip_shield();

#endif
