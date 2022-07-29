#include "dpad_paused.h"

typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);

extern uint8_t CFG_DPAD_ENABLED;
extern uint8_t CFG_UNEQUIP_GEAR_ENABLED;
extern uint8_t CFG_UNEQUIP_ITEM_ENABLED;
extern uint8_t CFG_SWAP_ITEM_ENABLED;

extern uint8_t DPAD_ADULT_SET1_UP;
extern uint8_t DPAD_ADULT_SET1_RIGHT;
extern uint8_t DPAD_ADULT_SET1_DOWN;
extern uint8_t DPAD_ADULT_SET1_LEFT;
extern uint8_t DPAD_ADULT_SET2_UP;
extern uint8_t DPAD_ADULT_SET2_RIGHT;
extern uint8_t DPAD_ADULT_SET2_DOWN;
extern uint8_t DPAD_ADULT_SET2_LEFT;
extern uint8_t DPAD_CHILD_SET1_UP;
extern uint8_t DPAD_CHILD_SET1_RIGHT;
extern uint8_t DPAD_CHILD_SET1_DOWN;
extern uint8_t DPAD_CHILD_SET1_LEFT;
extern uint8_t DPAD_CHILD_SET2_UP;
extern uint8_t DPAD_CHILD_SET2_RIGHT;
extern uint8_t DPAD_CHILD_SET2_DOWN;
extern uint8_t DPAD_CHILD_SET2_LEFT;

uint8_t KNIFE_COUNTER		= 0xFF;
uint8_t CHECKED_LENS		= 0;
uint16_t LAST_DPAD_SCENE	= 0xFFFF;

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
		if (z64_game.pause_ctxt.equip_cursor == 0x01 || z64_game.pause_ctxt.equip_cursor == 0x02 || z64_game.pause_ctxt.equip_cursor == 0x03)
			set_dpad_action(pad_pressed, DPAD_SWORD);
		else if (z64_game.pause_ctxt.equip_cursor == 0x0D) 
			set_dpad_action(pad_pressed, DPAD_BOOTS);
		else if (z64_game.pause_ctxt.equip_cursor == 0x05 || z64_game.pause_ctxt.equip_cursor == 0x06 || z64_game.pause_ctxt.equip_cursor == 0x07)
			set_dpad_action(pad_pressed, DPAD_SHIELD);
		else if (z64_game.pause_ctxt.equip_cursor == 0x09 || z64_game.pause_ctxt.equip_cursor == 0x0A || z64_game.pause_ctxt.equip_cursor == 0x0B)
			set_dpad_action(pad_pressed, DPAD_TUNIC);
		else if (z64_game.pause_ctxt.equip_cursor == 0x0E)
			set_dpad_action(pad_pressed, DPAD_IRON_BOOTS);
		else if (z64_game.pause_ctxt.equip_cursor == 0x0F)
			set_dpad_action(pad_pressed, DPAD_HOVER_BOOTS);
	}
	else if (z64_game.pause_ctxt.screen_idx == 0) {
		if (!z64_file.link_age) {
			if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_BOW)
				set_dpad_action(pad_pressed, DPAD_ARROWS);
			else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_ADULT_TRADE)
				set_dpad_action(pad_pressed, DPAD_ADULT_TRADE);
		}
		else if(z64_file.link_age) {
			if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_CHILD_TRADE)
			set_dpad_action(pad_pressed, DPAD_CHILD_TRADE);
		}
		if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_OCARINA)
			set_dpad_action(pad_pressed, DPAD_OCARINA);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_LENS)
			set_dpad_action(pad_pressed, DPAD_LENS);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_NUT)
			set_dpad_action(pad_pressed, DPAD_NUT);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_DINS_FIRE)
			set_dpad_action(pad_pressed, DPAD_DINS_FIRE);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_FARORES_WIND)
			set_dpad_action(pad_pressed, DPAD_FARORES_WIND);
		else if (z64_game.pause_ctxt.item_cursor == Z64_SLOT_NAYRUS_LOVE)
			set_dpad_action(pad_pressed, DPAD_NAYRUS_LOVE);
	}
}

void check_default_dpad_actions() {
	if (CFG_DPAD_ENABLED == 0 || z64_game.scene_index == LAST_DPAD_SCENE)
		return;
	LAST_DPAD_SCENE = z64_game.scene_index;
	
	DPAD_ADULT_SET1_UP		= z64_file.ammo[DPAD_ADULT_UP] >> 4;
	DPAD_ADULT_SET2_UP		= z64_file.ammo[DPAD_ADULT_UP] & 0xF;
	DPAD_CHILD_SET1_UP		= z64_file.ammo[DPAD_ADULT_UP] >> 4;
	DPAD_CHILD_SET2_UP		= z64_file.ammo[DPAD_ADULT_UP] & 0xF;
	
	DPAD_ADULT_SET1_RIGHT	= z64_file.ammo[DPAD_ADULT_RIGHT] >> 4;
	DPAD_ADULT_SET2_RIGHT	= z64_file.ammo[DPAD_ADULT_RIGHT] & 0xF;
	DPAD_CHILD_SET1_RIGHT	= z64_file.ammo[DPAD_CHILD_RIGHT] >> 4;
	DPAD_CHILD_SET2_RIGHT	= z64_file.ammo[DPAD_CHILD_RIGHT] & 0xF;
	
	DPAD_ADULT_SET1_DOWN	= z64_file.ammo[DPAD_ADULT_DOWN] >> 4;
	DPAD_ADULT_SET2_DOWN	= z64_file.ammo[DPAD_ADULT_DOWN] & 0xF;
	DPAD_CHILD_SET1_DOWN	= z64_file.ammo[DPAD_CHILD_DOWN] >> 4;
	DPAD_CHILD_SET2_DOWN	= z64_file.ammo[DPAD_CHILD_DOWN] & 0xF;
	
	DPAD_ADULT_SET1_LEFT	= z64_file.ammo[DPAD_ADULT_LEFT] >> 4;
	DPAD_ADULT_SET2_LEFT	= z64_file.ammo[DPAD_ADULT_LEFT] & 0xF;
	DPAD_CHILD_SET1_LEFT	= z64_file.ammo[DPAD_CHILD_LEFT] >> 4;
	DPAD_CHILD_SET2_LEFT	= z64_file.ammo[DPAD_CHILD_LEFT] & 0xF;
}

void check_lens() {
	if (CHECKED_LENS || !CAN_USE_DPAD || CFG_DPAD_ENABLED == 0)
		return;
	CHECKED_LENS = 1;
	
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

void set_dpad_action(pad_t pad_pressed, uint8_t action) {
	uint16_t sfx = 0x4808;
	
	if (pad_pressed.du) {
		if (!z64_file.link_age && !DPAD_ALT) {
			if (DPAD_ADULT_SET1_UP == action) {
				DPAD_ADULT_SET1_UP = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_SET1_UP = action;
			z64_file.ammo[DPAD_ADULT_UP] = DPAD_ADULT_SET1_UP * 16 + DPAD_ADULT_SET2_UP;
		}
		else if (!z64_file.link_age && DPAD_ALT) {
			if (DPAD_ADULT_SET2_UP == action) {
				DPAD_ADULT_SET2_UP = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_SET2_UP = action;
			z64_file.ammo[DPAD_ADULT_UP] = DPAD_ADULT_SET1_UP * 16 + DPAD_ADULT_SET2_UP;
		}
		else if (z64_file.link_age  && !DPAD_ALT) {
			if (DPAD_CHILD_SET1_UP == action) {
				DPAD_CHILD_SET1_UP = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_SET1_UP = action;
			z64_file.ammo[DPAD_CHILD_UP] = DPAD_CHILD_SET1_UP * 16 + DPAD_CHILD_SET2_UP;
		}
		else if (z64_file.link_age  && DPAD_ALT) {
			if (DPAD_CHILD_SET2_UP == action) {
				DPAD_CHILD_SET2_UP = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_SET2_UP = action;
			z64_file.ammo[DPAD_CHILD_UP] = DPAD_CHILD_SET1_UP * 16 + DPAD_CHILD_SET2_UP;
		}
	}
	else if (pad_pressed.dr) {
		if (!z64_file.link_age && !DPAD_ALT) {
			if (DPAD_ADULT_SET1_RIGHT == action) {
				DPAD_ADULT_SET1_RIGHT = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_SET1_RIGHT = action;
			z64_file.ammo[DPAD_ADULT_RIGHT] = DPAD_ADULT_SET1_RIGHT * 16 + DPAD_ADULT_SET2_RIGHT;
		}
		else if (!z64_file.link_age && DPAD_ALT) {
			if (DPAD_ADULT_SET2_RIGHT == action) {
				DPAD_ADULT_SET2_RIGHT = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_SET2_RIGHT = action;
			z64_file.ammo[DPAD_ADULT_RIGHT] = DPAD_ADULT_SET1_RIGHT * 16 + DPAD_ADULT_SET2_RIGHT;
		}
		else if (z64_file.link_age  && !DPAD_ALT) {
			if (DPAD_CHILD_SET1_RIGHT == action) {
				DPAD_CHILD_SET1_RIGHT = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_SET1_RIGHT = action;
			z64_file.ammo[DPAD_CHILD_RIGHT] = DPAD_CHILD_SET1_RIGHT * 16 + DPAD_CHILD_SET2_RIGHT;
		}
		else if (z64_file.link_age  && DPAD_ALT) {
			if (DPAD_CHILD_SET2_RIGHT == action) {
				DPAD_CHILD_SET2_RIGHT = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_SET2_RIGHT = action;
			z64_file.ammo[DPAD_CHILD_RIGHT] = DPAD_CHILD_SET1_RIGHT * 16 + DPAD_CHILD_SET2_RIGHT;
		}
	}
	else if (pad_pressed.dd) {
		if (!z64_file.link_age && !DPAD_ALT) {
			if (DPAD_ADULT_SET1_DOWN == action) {
				DPAD_ADULT_SET1_DOWN = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_SET1_DOWN = action;
			z64_file.ammo[DPAD_ADULT_DOWN] = DPAD_ADULT_SET1_DOWN * 16 + DPAD_ADULT_SET2_DOWN;
		}
		else if (!z64_file.link_age && DPAD_ALT) {
			if (DPAD_ADULT_SET2_DOWN == action) {
				DPAD_ADULT_SET2_DOWN = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_SET2_DOWN = action;
			z64_file.ammo[DPAD_ADULT_DOWN] = DPAD_ADULT_SET1_DOWN * 16 + DPAD_ADULT_SET2_DOWN;
		}
		else if (z64_file.link_age  && !DPAD_ALT) {
			if (DPAD_CHILD_SET1_DOWN == action) {
				DPAD_CHILD_SET1_DOWN = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_SET1_DOWN = action;
			z64_file.ammo[DPAD_CHILD_DOWN] = DPAD_CHILD_SET1_DOWN * 16 + DPAD_CHILD_SET2_DOWN;
		}
		else if (z64_file.link_age  && DPAD_ALT) {
			if (DPAD_CHILD_SET2_DOWN == action) {
				DPAD_CHILD_SET2_DOWN = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_SET2_DOWN = action;
			z64_file.ammo[DPAD_CHILD_DOWN] = DPAD_CHILD_SET1_DOWN * 16 + DPAD_CHILD_SET2_DOWN;
		}
	}
	else if (pad_pressed.dl) {
		if (!z64_file.link_age && !DPAD_ALT) {
			if (DPAD_ADULT_SET1_LEFT == action) {
				DPAD_ADULT_SET1_LEFT = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_SET1_LEFT = action;
			z64_file.ammo[DPAD_ADULT_LEFT] = DPAD_ADULT_SET1_LEFT * 16 + DPAD_ADULT_SET2_LEFT;
		}
		else if (!z64_file.link_age &&  DPAD_ALT) {
			if (DPAD_ADULT_SET2_LEFT == action) {
				DPAD_ADULT_SET2_LEFT = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_ADULT_SET2_LEFT = action;
			z64_file.ammo[DPAD_ADULT_LEFT] = DPAD_ADULT_SET1_LEFT * 16 + DPAD_ADULT_SET2_LEFT;
		}
		else if (z64_file.link_age  && !DPAD_ALT) {
			if (DPAD_CHILD_SET1_LEFT == action) {
				DPAD_CHILD_SET1_LEFT = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_SET1_LEFT = action;
			z64_file.ammo[DPAD_CHILD_LEFT] = DPAD_CHILD_SET1_LEFT * 16 + DPAD_CHILD_SET2_LEFT;
		}
		else if (z64_file.link_age  &&  DPAD_ALT) {
			if (DPAD_CHILD_SET2_LEFT == action) {
				DPAD_CHILD_SET2_LEFT = DPAD_NULL;
				sfx = 0x480A;
			}
			else DPAD_CHILD_SET2_LEFT = action;
			z64_file.ammo[DPAD_CHILD_LEFT] = DPAD_CHILD_SET1_LEFT * 16 + DPAD_CHILD_SET2_LEFT;
		}
	}
	
	if (pad_pressed.du || pad_pressed.dr || pad_pressed.dd || pad_pressed.dl) {
		if (action == DPAD_LENS)
			CHECKED_LENS = 0;
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

/*void handle_downgrading() {
	if (!CFG_SWAP_ITEM_ENABLED || z64_game.pause_ctxt.unk_02_[1] != 0)
		return;
	
	if (z64_game.pause_ctxt.screen_idx == 3) { // Swap knife
		if (z64_game.pause_ctxt.equip_cursor == 3 && (DOWNGRADE_GIANTS_KNIFE] || z64_file.bgs_flag) ) {
			DOWNGRADE_GIANTS_KNIFE] ^= 1; 
			z64_file.bgs_flag ^= 1;
			if (!z64_file.bgs_flag)
				if (KNIFE_COUNTER != 0xFF)
					z64_file.bgs_hits_left = KNIFE_COUNTER;
			else KNIFE_COUNTER = z64_file.bgs_hits_left;
			if (z64_file.equip_sword == 3)
				unequip_sword(0);
			z64_UpdateEquipment(&z64_game, &z64_link);
			z64_playsfx(0x4808, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		}
	}
			
	if (z64_game.pause_ctxt.screen_idx == 0) { // Downgrade / Upgrade items
		if (z64_game.pause_ctxt.item_cursor == 7 && (DOWNGRADE_OCARINA] || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) ) {
			DOWNGRADE_OCARINA] ^= 1;
			if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME)
				swap_item(Z64_SLOT_OCARINA, Z64_ITEM_OCARINA_OF_TIME, Z64_ITEM_FAIRY_OCARINA);
			else if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA)
				swap_item(Z64_SLOT_OCARINA, Z64_ITEM_FAIRY_OCARINA, Z64_ITEM_OCARINA_OF_TIME);
		}
		
		if (z64_game.pause_ctxt.item_cursor == 9 && (DOWNGRADE_HOOKSHOT] || z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_LONGSHOT) ) {
			DOWNGRADE_HOOKSHOT] ^= 1;
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
}*/

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