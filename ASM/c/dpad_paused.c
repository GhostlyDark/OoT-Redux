#include "dpad_paused.h"

typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);

extern uint8_t CFG_DPAD_ENABLED;
extern uint8_t CFG_UNEQUIP_GEAR_ENABLED;
extern uint8_t CFG_UNEQUIP_ITEM_ENABLED;
extern uint8_t CFG_SWAP_ITEM_ENABLED;

uint8_t knife_counter		= 0xFF;
uint8_t checked_lens		= 0;

extern uint8_t DPAD_ALT;

void handle_dpad_paused() {
	check_lens();
	check_default_dpad_actions();
	
	if (!CAN_USE_DPAD || z64_game.pause_ctxt.state != 6 || CFG_DPAD_ENABLED == 0 || z64_game.pause_ctxt.unk_02_[1] != 0 || z64_game.pause_ctxt.cursor_pos == 0x0A || z64_game.pause_ctxt.cursor_pos == 0x0B) 
		return;
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	handle_dpad_slots(pad_pressed);
	handle_unequipping(pad_pressed);
}

void handle_dpad_slots(pad_t pad_pressed) {
	if (CFG_DPAD_ENABLED == 0)
		return;
	
	if (z64_game.pause_ctxt.screen_idx == 3) {
		if (z64_game.pause_ctxt.equip_cursor == 0x01)
			set_dpad_action(pad_pressed, DPAD_SWORD, LIMIT_KOKIRI_SWORD);
		else if (z64_game.pause_ctxt.equip_cursor == 0x02)
			set_dpad_action(pad_pressed, DPAD_SWORD, LIMIT_MASTER_SWORD);
		else if (z64_game.pause_ctxt.equip_cursor == 0x03)
			set_dpad_action(pad_pressed, DPAD_SWORD, LIMIT_GIANTS_KNIFE);
		else if (z64_game.pause_ctxt.equip_cursor == 0x0D) 
			set_dpad_action(pad_pressed, DPAD_BOOTS, LIMIT_KOKIRI_BOOTS);
		else if (z64_game.pause_ctxt.equip_cursor == 0x05)
			set_dpad_action(pad_pressed, DPAD_SHIELD, LIMIT_DEKU_SHIELD);
		else if (z64_game.pause_ctxt.equip_cursor == 0x06)
			set_dpad_action(pad_pressed, DPAD_SHIELD, LIMIT_HYLIAN_SHIELD);
		else if (z64_game.pause_ctxt.equip_cursor == 0x07)
			set_dpad_action(pad_pressed, DPAD_SHIELD, LIMIT_MIRROR_SHIELD);
		else if (z64_game.pause_ctxt.equip_cursor == 0x09)
			set_dpad_action(pad_pressed, DPAD_TUNIC, LIMIT_KOKIRI_TUNIC);
		else if (z64_game.pause_ctxt.equip_cursor == 0x0A)
			set_dpad_action(pad_pressed, DPAD_TUNIC, LIMIT_GORON_TUNIC);
		else if (z64_game.pause_ctxt.equip_cursor == 0x0B)
			set_dpad_action(pad_pressed, DPAD_TUNIC, LIMIT_ZORA_TUNIC);
		else if (z64_game.pause_ctxt.equip_cursor == 0x0E)
			set_dpad_action(pad_pressed, DPAD_IRON_BOOTS, LIMIT_IRON_BOOTS);
		else if (z64_game.pause_ctxt.equip_cursor == 0x0F)
			set_dpad_action(pad_pressed, DPAD_HOVER_BOOTS, LIMIT_HOVER_BOOTS);
	}
	else if (z64_game.pause_ctxt.screen_idx == 0) {
		if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_OCARINA)
			set_dpad_action(pad_pressed, DPAD_OCARINA, LIMIT_OCARINA);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_LENS)
			set_dpad_action(pad_pressed, DPAD_LENS, LIMIT_LENS);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_NUT)
			set_dpad_action(pad_pressed, DPAD_NUT, LIMIT_NUT);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_DINS_FIRE)
			set_dpad_action(pad_pressed, DPAD_DINS_FIRE, LIMIT_DINS_FIRE);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_FARORES_WIND)
			set_dpad_action(pad_pressed, DPAD_FARORES_WIND, LIMIT_FARORES_WIND);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_NAYRUS_LOVE)
			set_dpad_action(pad_pressed, DPAD_NAYRUS_LOVE, LIMIT_NAYRUS_LOVE);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_BOW)
			set_dpad_action(pad_pressed, DPAD_ARROWS, LIMIT_BOW);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_ADULT_TRADE)
			set_dpad_action(pad_pressed, DPAD_ADULT_TRADE, LIMIT_ADULT_TRADE);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_CHILD_TRADE)
			set_dpad_action(pad_pressed, DPAD_CHILD_TRADE, LIMIT_CHILD_TRADE);
	}
}

void check_default_dpad_actions() {
	if (DPAD_INIT_SETUP)
		return;
	EXTRA_SRAM |= 1;
	
	DPAD_ADULT_UP		= DPAD_ARROWS		* 16 + DPAD_SWORD;
	DPAD_ADULT_RIGHT	= DPAD_HOVER_BOOTS  * 16 + DPAD_BOOTS;
	DPAD_ADULT_DOWN		= DPAD_OCARINA		* 16 + DPAD_SHIELD;
	DPAD_ADULT_LEFT		= DPAD_IRON_BOOTS	* 16 + DPAD_TUNIC;
	DPAD_CHILD_UP		= DPAD_LENS			* 16 + DPAD_SWORD;
	DPAD_CHILD_RIGHT	= DPAD_CHILD_TRADE	* 16 + DPAD_BOOTS;
	DPAD_CHILD_DOWN		= DPAD_OCARINA		* 16 + DPAD_SHIELD;
	DPAD_CHILD_LEFT		= DPAD_NULL			* 16 + DPAD_TUNIC;
}

void check_lens() {
	if (checked_lens || !CAN_USE_DPAD || CFG_DPAD_ENABLED == 0)
		return;
	checked_lens = 1;
	
	z64_dpad_lens_1 = 0x504E;
	z64_dpad_lens_2 = 0x504F;
	z64_dpad_lens_3 = 0x5458;
	
	if (!z64_file.link_age && !DPAD_ALT) {
		unlock_lens(DPAD_ADULT_SET1_UP);
		unlock_lens(DPAD_ADULT_SET1_RIGHT);
		unlock_lens(DPAD_ADULT_SET1_DOWN);
		unlock_lens(DPAD_ADULT_SET1_LEFT);
	}
	else if (!z64_file.link_age && DPAD_ALT) {
		unlock_lens(DPAD_ADULT_SET2_UP);
		unlock_lens(DPAD_ADULT_SET2_RIGHT);
		unlock_lens(DPAD_ADULT_SET2_DOWN);
		unlock_lens(DPAD_ADULT_SET2_LEFT);
	}
	else if (z64_file.link_age && !DPAD_ALT) {
		unlock_lens(DPAD_CHILD_SET1_UP);
		unlock_lens(DPAD_CHILD_SET1_RIGHT);
		unlock_lens(DPAD_CHILD_SET1_DOWN);
		unlock_lens(DPAD_CHILD_SET1_LEFT);
	}
	else if (z64_file.link_age && DPAD_ALT) {
		unlock_lens(DPAD_CHILD_SET2_UP);
		unlock_lens(DPAD_CHILD_SET2_RIGHT);
		unlock_lens(DPAD_CHILD_SET2_DOWN);
		unlock_lens(DPAD_CHILD_SET2_LEFT);
	}
}

void unlock_lens(uint8_t button) {
	if (button == DPAD_LENS) {
		z64_dpad_lens_1 = 0x1000;
		z64_dpad_lens_2 = 0x1000;
		z64_dpad_lens_3 = 0x1000;
	}
}

void set_dpad_action(pad_t pad_pressed, dpad_action_t action, limit_item_t limit) {
	if ( (!z64_file.link_age && z64_usability.item[limit] == 1) || (z64_file.link_age && z64_usability.item[limit] == 0) )
		return;
	uint16_t sfx = 0x4808;
	
	if (pad_pressed.du) {
		if (!z64_file.link_age && !DPAD_ALT) {
			if (DPAD_ADULT_SET1_UP == action) {
				DPAD_ADULT_UP = DPAD_NULL * 16 + DPAD_ADULT_SET2_UP;
				sfx = 0x480A;
			}
			else DPAD_ADULT_UP = action * 16 + DPAD_ADULT_SET2_UP;
		}
		else if (!z64_file.link_age && DPAD_ALT) {
			if (DPAD_ADULT_SET2_UP == action) {
				DPAD_ADULT_UP = DPAD_ADULT_SET1_UP * 16 + DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_UP = DPAD_ADULT_SET1_UP * 16 + action;
		}
		else if (z64_file.link_age  && !DPAD_ALT) {
			if (DPAD_CHILD_SET1_UP == action) {
				DPAD_CHILD_UP = DPAD_NULL * 16 + DPAD_CHILD_SET2_UP;
				sfx = 0x480A;
			}
			else DPAD_CHILD_UP = action * 16 + DPAD_CHILD_SET2_UP;
		}
		else if (z64_file.link_age  && DPAD_ALT) {
			if (DPAD_CHILD_SET2_UP == action) {
				DPAD_CHILD_UP = DPAD_CHILD_SET1_UP * 16 + DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_UP = DPAD_CHILD_SET1_UP * 16 + action;
		}
	}
	else if (pad_pressed.dr) {
		if (!z64_file.link_age && !DPAD_ALT) {
			if (DPAD_ADULT_SET1_RIGHT == action) {
				DPAD_ADULT_RIGHT = DPAD_NULL * 16 + DPAD_ADULT_SET2_RIGHT;
				sfx = 0x480A;
			}
			else DPAD_ADULT_RIGHT = action * 16 + DPAD_ADULT_SET2_RIGHT;
		}
		else if (!z64_file.link_age && DPAD_ALT) {
			if (DPAD_ADULT_SET2_RIGHT == action) {
				DPAD_ADULT_RIGHT = DPAD_ADULT_SET1_RIGHT * 16 + DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_RIGHT = DPAD_ADULT_SET1_RIGHT * 16 + action;
		}
		else if (z64_file.link_age  && !DPAD_ALT) {
			if (DPAD_CHILD_SET1_RIGHT == action) {
				DPAD_CHILD_RIGHT = DPAD_NULL * 16 + DPAD_CHILD_SET2_RIGHT;
				sfx = 0x480A;
			}
			else DPAD_CHILD_RIGHT = action * 16 + DPAD_CHILD_SET2_RIGHT;
		}
		else if (z64_file.link_age  && DPAD_ALT) {
			if (DPAD_CHILD_SET2_RIGHT == action) {
				DPAD_CHILD_RIGHT = DPAD_CHILD_SET1_RIGHT * 16 + DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_RIGHT = DPAD_CHILD_SET1_RIGHT * 16 + action;
		}
	}
	else if (pad_pressed.dd) {
		if (!z64_file.link_age && !DPAD_ALT) {
			if (DPAD_ADULT_SET1_DOWN == action) {
				DPAD_ADULT_DOWN = DPAD_NULL * 16 + DPAD_ADULT_SET2_DOWN;
				sfx = 0x480A;
			}
			else DPAD_ADULT_DOWN = action * 16 + DPAD_ADULT_SET2_DOWN;
		}
		else if (!z64_file.link_age && DPAD_ALT) {
			if (DPAD_ADULT_SET2_DOWN == action) {
				DPAD_ADULT_DOWN = DPAD_ADULT_SET1_DOWN * 16 + DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_DOWN = DPAD_ADULT_SET1_DOWN * 16 + action;
		}
		else if (z64_file.link_age  && !DPAD_ALT) {
			if (DPAD_CHILD_SET1_DOWN == action) {
				DPAD_CHILD_DOWN = DPAD_NULL * 16 + DPAD_CHILD_SET2_DOWN;
				sfx = 0x480A;
			}
			else DPAD_CHILD_DOWN = action * 16 + DPAD_CHILD_SET2_DOWN;
		}
		else if (z64_file.link_age  && DPAD_ALT) {
			if (DPAD_CHILD_SET2_DOWN == action) {
				DPAD_CHILD_DOWN = DPAD_CHILD_SET1_DOWN * 16 + DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_DOWN = DPAD_CHILD_SET1_DOWN * 16 + action;
		}
	}
	else if (pad_pressed.dl) {
		if (!z64_file.link_age && !DPAD_ALT) {
			if (DPAD_ADULT_SET1_LEFT == action) {
				DPAD_ADULT_LEFT = DPAD_NULL * 16 + DPAD_ADULT_SET2_LEFT;
				sfx = 0x480A;
			}
			else DPAD_ADULT_LEFT = action * 16 + DPAD_ADULT_SET2_LEFT;
		}
		else if (!z64_file.link_age && DPAD_ALT) {
			if (DPAD_ADULT_SET2_LEFT == action) {
				DPAD_ADULT_LEFT = DPAD_ADULT_SET1_LEFT * 16 + DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_LEFT = DPAD_ADULT_SET1_LEFT * 16 + action;
		}
		else if (z64_file.link_age  && !DPAD_ALT) {
			if (DPAD_CHILD_SET1_LEFT == action) {
				DPAD_CHILD_LEFT = DPAD_NULL * 16 + DPAD_CHILD_SET2_LEFT;
				sfx = 0x480A;
			}
			else DPAD_CHILD_LEFT = action * 16 + DPAD_CHILD_SET2_LEFT;
		}
		else if (z64_file.link_age  && DPAD_ALT) {
			if (DPAD_CHILD_SET2_LEFT == action) {
				DPAD_CHILD_LEFT = DPAD_CHILD_SET1_LEFT * 16 + DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_LEFT = DPAD_CHILD_SET1_LEFT * 16 + action;
		}
	}
	
	if (pad_pressed.du || pad_pressed.dr || pad_pressed.dd || pad_pressed.dl) {
		if (action == DPAD_LENS)
			checked_lens = 0;
		z64_playsfx(sfx, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
}

void handle_unequipping(pad_t pad_pressed) {
	if (!pad_pressed.cu)
		return;
	
	if (z64_game.pause_ctxt.screen_idx == 3 && CFG_UNEQUIP_GEAR_ENABLED) { // Unequip gear
		if ( (z64_game.pause_ctxt.equip_cursor == 1 && z64_file.equip_sword  == 1) || (z64_game.pause_ctxt.equip_cursor == 2 && z64_file.equip_sword  == 2) || (z64_game.pause_ctxt.equip_cursor == 3 && z64_file.equip_sword  == 3) )
			unequip_sword(1);
		if ( (z64_game.pause_ctxt.equip_cursor == 5 && z64_file.equip_shield == 1) || (z64_game.pause_ctxt.equip_cursor == 6 && z64_file.equip_shield == 2) || (z64_game.pause_ctxt.equip_cursor == 7 && z64_file.equip_shield == 3) )
			unequip_shield();
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

void handle_downgrading() {
	if (!CFG_SWAP_ITEM_ENABLED || z64_game.pause_ctxt.unk_02_[1] != 0)
		return;
	
	if (z64_game.pause_ctxt.screen_idx == 3) { // Swap knife
		if (z64_game.pause_ctxt.equip_cursor == 3 && (DOWNGRADE_GIANTS_KNIFE || z64_file.bgs_flag) ) {
			EXTRA_SRAM			|= 2;
			z64_file.bgs_flag	^= 1;
			if (!z64_file.bgs_flag)
				if (knife_counter != 0xFF)
					z64_file.bgs_hits_left = knife_counter;
			else knife_counter = z64_file.bgs_hits_left;
			if (z64_file.equip_sword == 3)
				unequip_sword(0);
			z64_UpdateEquipment(&z64_game, &z64_link);
			z64_playsfx(0x4808, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		}
	}
			
	if (z64_game.pause_ctxt.screen_idx == 0) { // Downgrade / Upgrade items
		if (z64_game.pause_ctxt.item_cursor == 7 && (DOWNGRADE_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) ) {
			EXTRA_SRAM |= 4;
			if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME)
				swap_item(Z64_SLOT_OCARINA, Z64_ITEM_OCARINA_OF_TIME, Z64_ITEM_FAIRY_OCARINA);
			else if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA)
				swap_item(Z64_SLOT_OCARINA, Z64_ITEM_FAIRY_OCARINA, Z64_ITEM_OCARINA_OF_TIME);
		}
		
		if (z64_game.pause_ctxt.item_cursor == 9 && (DOWNGRADE_HOOKSHOT || z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_LONGSHOT) ) {
			EXTRA_SRAM |= 8;
			if (z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_LONGSHOT)
				swap_item(Z64_SLOT_HOOKSHOT, Z64_ITEM_LONGSHOT, Z64_ITEM_HOOKSHOT);
			else if (z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_HOOKSHOT)
				swap_item(Z64_SLOT_HOOKSHOT, Z64_ITEM_HOOKSHOT, Z64_ITEM_LONGSHOT);
		}
	}
}

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

void unequip_sword(uint8_t play) {
	z64_file.equip_sword				= 0;
	if (z64_file.link_age)
		z64_file.child_equip_sword		= 0;
	else z64_file.adult_equip_sword		= 0;
	z64_file.inf_table[29]				= 1;
	z64_file.button_items[0]			= -1;
	z64_UpdateEquipment(&z64_game, &z64_link);
	if (play)
		z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}

void unequip_shield() {
	z64_file.equip_shield				= 0;
	if (z64_file.link_age)
		z64_file.child_equip_shield		= 0;
	else z64_file.adult_equip_shield	= 0;
	z64_UpdateEquipment(&z64_game, &z64_link);
	z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}