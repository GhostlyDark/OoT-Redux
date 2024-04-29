#ifndef DPAD_PAUSED_H
#define DPAD_PAUSED_H

#include "z64.h"
#include "z64_extended.h"

void handle_dpad_paused();
void handle_dpad_slots(pad_t pad_pressed);
void set_dpad_action(pad_t pad_pressed, z64_slot_t slot);
uint8_t run_set_dpad_action(z64_slot_t slot, uint8_t button);
void handle_unequipping(pad_t pad_pressed);
void handle_switch_mask(pad_t pad_pressed);
uint8_t get_next_mask();
void update_trade_equips(uint16_t item, int16_t slot);
void handle_downgrading(pad_t pad_pressed);
void swap_item(z64_slot_t slot, z64_item_t original, z64_item_t swap);
void unequip_gear(uint8_t play);
void unequip_sword(uint8_t play);
void unequip_shield();
void unequip_tunic();

#endif
