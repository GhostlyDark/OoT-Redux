#ifndef DPAD_PAUSED_H
#define DPAD_PAUSED_H

#include "z64.h"
#include "z64_extended.h"

void handle_dpad_paused();
void handle_dpad_slots(pad_t pad_pressed);
void check_lens();
void run_unlock_lens(uint8_t up, uint8_t right, uint8_t down, uint8_t left);
void unlock_lens(uint8_t button);
void set_dpad_action(pad_t pad_pressed, dpad_action_t action, limit_item_t restriction);
uint8_t run_set_dpad_action(dpad_action_t action, uint8_t set1, uint8_t set2);
uint8_t run_set_dpad_action_set(dpad_action_t action, uint8_t set, uint8_t btn1, uint8_t btn2, uint8_t btn3, uint8_t btn4);
void handle_unequipping(pad_t pad_pressed);
void handle_downgrading();
void swap_item(z64_slot_t slot, z64_item_t original, z64_item_t swap);
void unequip_gear(uint8_t play);
void unequip_sword(uint8_t play);
void unequip_shield();
void unequip_tunic();

#endif
