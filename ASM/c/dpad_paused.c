#include "dpad_paused.h"

extern uint8_t CFG_INVENTORY_EDITOR_ENABLED;
extern uint8_t CFG_DPAD_ENABLED;
extern uint8_t CFG_UNEQUIP_GEAR_ENABLED;
extern uint8_t CFG_UNEQUIP_ITEM_ENABLED;
extern uint8_t CFG_SWAP_ITEM_ENABLED;

typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);

uint8_t KNIFE_COUNTER = 0xFF;

#define z64_playsfx   ((playsfx_t)      0x800C806C)

void swap_item(z64_slot_t slot, z64_item_t item, z64_item_t swap) {
	if (z64_file.items[slot] == item) {
		for (uint8_t i=0; i<4; i++) {
			if (z64_file.button_items[i] == item) {
				z64_file.button_items[i]			= swap;
				if (!z64_file.link_age)
					z64_file.adult_button_items[i]	= swap;
				else z64_file.child_button_items[i]	= swap;
				z64_UpdateItemButton(&z64_game, i);
			}
		}
		z64_file.items[slot] = swap;
		z64_playsfx(0x4808, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
}

void unequip_sword() {
	z64_file.equip_sword     = 0;
	z64_file.inf_table[29]   = 1;
	z64_file.button_items[0] = -1;
	z64_UpdateEquipment(&z64_game, &z64_link);
	z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}

void handle_dpad_paused() {
	if (!CAN_USE_DPAD || z64_game.pause_ctxt.state != 6 || CFG_DPAD_ENABLED == 0) 
		return;
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (CFG_INVENTORY_EDITOR_ENABLED) {
		if (pad_pressed.dr && z64_game.pause_ctxt.unk_02_[1] == 0)
			z64_game.pause_ctxt.unk_02_[1] = 2;
		else if ( (pad_pressed.l || pad_pressed.s) && z64_game.pause_ctxt.unk_02_[1] == 2)
			z64_game.pause_ctxt.unk_02_[1] = 0;
	}
	
	if (z64_game.pause_ctxt.unk_02_[1] == 2 || z64_game.pause_ctxt.cursor_pos == 0x0A || z64_game.pause_ctxt.cursor_pos == 0x0B)
		return;
	
	if (pad_pressed.du) {
		if (z64_game.pause_ctxt.screen_idx == 3 && CFG_UNEQUIP_GEAR_ENABLED) { // Unequip gear
			if (z64_game.pause_ctxt.equip_cursor == 1 && z64_file.equip_sword == 1)
				unequip_sword();
			if (z64_game.pause_ctxt.equip_cursor == 2 && z64_file.equip_sword == 2)
				unequip_sword();
			if (z64_game.pause_ctxt.equip_cursor == 3 && z64_file.equip_sword == 3)
				unequip_sword();
			
			if (z64_game.pause_ctxt.equip_cursor == 5 && z64_file.equip_shield == 1) {
				z64_file.equip_shield = 0;
				z64_UpdateEquipment(&z64_game, &z64_link);
				z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
			if (z64_game.pause_ctxt.equip_cursor == 6 && z64_file.equip_shield == 2) {
				z64_file.equip_shield = 0;
				z64_UpdateEquipment(&z64_game, &z64_link);
				z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
			if (z64_game.pause_ctxt.equip_cursor == 7 && z64_file.equip_shield == 3) {
				z64_file.equip_shield = 0;
				z64_UpdateEquipment(&z64_game, &z64_link);
				z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
		}
		
		
		if (z64_game.pause_ctxt.screen_idx == 0 && CFG_UNEQUIP_ITEM_ENABLED) { // Unset item from C button
			for (uint8_t item=0; item<24; item++) {
				if (z64_game.pause_ctxt.item_cursor == item) {
					for (uint8_t button=1; button<=3; button++) {
						if (z64_file.button_items[button] == z64_file.items[item]) {
							z64_file.button_items[button]					= 0xFF;
							z64_file.c_button_slots[button - 1]				= 0xFF;
							if (!z64_file.link_age) {
								z64_file.adult_button_items[button]			= 0xFF;
								z64_file.adult_c_button_slots[button - 1]	= 0xFF;
							}
							else {
								z64_file.child_button_items[button]			= 0xFF;
								z64_file.child_c_button_slots[button - 1]	= 0xFF;
							}
							z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
							break;
						}
					}
				}
			}
		}
	}
	
	if (pad_pressed.dd) {
		if (z64_game.pause_ctxt.screen_idx == 3 && CFG_SWAP_ITEM_ENABLED) { // Swap knife
			if (z64_game.pause_ctxt.equip_cursor == 3 && (z64_file.ammo[4] == 1 || z64_file.bgs_flag) ) {
				z64_file.ammo[4] = 1;
				z64_file.bgs_flag ^= 1;
				if (!z64_file.bgs_flag) {
					if (KNIFE_COUNTER != 0xFF)
						z64_file.bgs_hits_left = KNIFE_COUNTER;
				}
				else KNIFE_COUNTER = z64_file.bgs_hits_left;
				if (z64_file.equip_sword == 3) {
					z64_file.equip_sword     = 0;
					z64_file.inf_table[29]   = 1;
					z64_file.button_items[0] = -1;
				}
				z64_UpdateEquipment(&z64_game, &z64_link);
				z64_playsfx(0x4808, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
		}
			
		if (z64_game.pause_ctxt.screen_idx == 0) { // Downgrade / Upgrade items
			if (z64_game.pause_ctxt.item_cursor == 7 && (z64_file.ammo[5] == 1 || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) ) {
				z64_file.ammo[5] = 1;
				if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME)
					swap_item(Z64_SLOT_OCARINA, Z64_ITEM_OCARINA_OF_TIME, Z64_ITEM_FAIRY_OCARINA);
				else if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA)
					swap_item(Z64_SLOT_OCARINA, Z64_ITEM_FAIRY_OCARINA, Z64_ITEM_OCARINA_OF_TIME);
			}
			
			if (z64_game.pause_ctxt.item_cursor == 9 && (z64_file.ammo[7] == 1 || z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_LONGSHOT) ) {
				z64_file.ammo[7] = 1;
				if (z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_LONGSHOT)
					swap_item(Z64_SLOT_HOOKSHOT, Z64_ITEM_LONGSHOT, Z64_ITEM_HOOKSHOT);
				else if (z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_HOOKSHOT)
					swap_item(Z64_SLOT_HOOKSHOT, Z64_ITEM_HOOKSHOT, Z64_ITEM_LONGSHOT);
			}
		}
	}
}
