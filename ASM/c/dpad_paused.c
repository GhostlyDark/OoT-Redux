#include "dpad_paused.h"

extern uint8_t  dpad_alt;
extern uint16_t play_sfx;

uint8_t checked_lens	= 0;

void handle_dpad_paused() {
	check_lens();
	
	if (IS_PAUSE_SCREEN_CURSOR) {
		pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
		if (CAN_USE_DPAD && SAVE_DPAD > 0)
			handle_dpad_slots(pad_pressed);
		handle_unequipping(pad_pressed);
	}
}

void handle_dpad_slots(pad_t pad_pressed) {
	if (SAVE_DPAD == 0)
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

void check_lens() {
	if (checked_lens || !CAN_USE_DPAD || SAVE_DPAD == 0)
		return;
	checked_lens = 1;
	
	z64_dpad_lens_1 = 0x504E;
	z64_dpad_lens_2 = 0x504F;
	z64_dpad_lens_3 = 0x5458;
	
	if (!z64_file.link_age) {
		if (!dpad_alt) {
			unlock_lens(DPAD_ADULT_SET1_UP);
			unlock_lens(DPAD_ADULT_SET1_RIGHT);
			unlock_lens(DPAD_ADULT_SET1_DOWN);
			unlock_lens(DPAD_ADULT_SET1_LEFT);
		}
		else {
			unlock_lens(DPAD_ADULT_SET2_UP);
			unlock_lens(DPAD_ADULT_SET2_RIGHT);
			unlock_lens(DPAD_ADULT_SET2_DOWN);
			unlock_lens(DPAD_ADULT_SET2_LEFT);
		}
	}
	else {
		if (!dpad_alt) {
			unlock_lens(DPAD_CHILD_SET1_UP);
			unlock_lens(DPAD_CHILD_SET1_RIGHT);
			unlock_lens(DPAD_CHILD_SET1_DOWN);
			unlock_lens(DPAD_CHILD_SET1_LEFT);
		}
		else {
			unlock_lens(DPAD_CHILD_SET2_UP);
			unlock_lens(DPAD_CHILD_SET2_RIGHT);
			unlock_lens(DPAD_CHILD_SET2_DOWN);
			unlock_lens(DPAD_CHILD_SET2_LEFT);
		}
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
	
	if (pad_pressed.du) {
		if (!z64_file.link_age)
			DPAD_ADULT_UP = run_set_dpad_action(action, DPAD_ADULT_SET1_UP, DPAD_ADULT_SET2_UP);
		else DPAD_CHILD_UP = run_set_dpad_action(action, DPAD_CHILD_SET1_UP, DPAD_CHILD_SET2_UP);
	}
	else if (pad_pressed.dr) {
		if (!z64_file.link_age)
			DPAD_ADULT_RIGHT = run_set_dpad_action(action, DPAD_ADULT_SET1_RIGHT, DPAD_ADULT_SET2_RIGHT);
		else DPAD_CHILD_RIGHT = run_set_dpad_action(action, DPAD_CHILD_SET1_RIGHT, DPAD_CHILD_SET2_RIGHT);
	}
	else if (pad_pressed.dd) {
		if (!z64_file.link_age)
			DPAD_ADULT_DOWN = run_set_dpad_action(action, DPAD_ADULT_SET1_DOWN, DPAD_ADULT_SET2_DOWN);
		else DPAD_CHILD_DOWN = run_set_dpad_action(action, DPAD_CHILD_SET1_DOWN, DPAD_CHILD_SET2_DOWN);
	}
	else if (pad_pressed.dl) {
		if (!z64_file.link_age)
			DPAD_ADULT_LEFT = run_set_dpad_action(action, DPAD_ADULT_SET1_LEFT, DPAD_ADULT_SET2_LEFT);
		else DPAD_CHILD_LEFT = run_set_dpad_action(action, DPAD_CHILD_SET1_LEFT, DPAD_CHILD_SET2_LEFT);
	}
}

uint8_t run_set_dpad_action(dpad_action_t action, uint8_t set1, uint8_t set2) {
	if (action == DPAD_LENS)
		checked_lens = 0;
	
	if (!dpad_alt)
		 return run_set_dpad_action_set(action, set1, DPAD_NULL, set2, action, set2);
	return run_set_dpad_action_set(action, set2, set1, DPAD_NULL, set1, action);
}

uint8_t run_set_dpad_action_set(dpad_action_t action, uint8_t set, uint8_t btn1, uint8_t btn2, uint8_t btn3, uint8_t btn4) {
	if (set == action) {
		play_sfx = 0x480A;
		return btn1 * 16 + btn2;
	}
	play_sfx = 0x4808;
	return btn3 * 16 + btn4;
}

void handle_unequipping(pad_t pad_pressed) {
	if (z64_game.pause_ctxt.screen_idx == 3 && pad_pressed.a) {
		if (SAVE_UNEQUIP_GEAR) { // Unequip gear
			if (z64_game.pause_ctxt.equip_cursor     == z64_file.equip_sword)
				unequip_sword(1);
			if (z64_game.pause_ctxt.equip_cursor - 4 == z64_file.equip_shield)
				unequip_shield();
		}
		if (SAVE_EXTRA_ABILITIES || SAVE_UNEQUIP_GEAR)
			if (z64_game.pause_ctxt.equip_cursor - 8 == z64_file.equip_tunic)
				unequip_tunic();
	}
	
	if (z64_game.pause_ctxt.screen_idx == 0 && SAVE_UNEQUIP_ITEM) { // Unset item from C button
		uint8_t button;
		if (pad_pressed.cl)
			button = 1;
		else if (pad_pressed.cd)
			button = 2;
		else if (pad_pressed.cr)
			button = 3;
		else return;
	
		for (uint8_t cursor=0; cursor<24; cursor++)
			if (z64_game.pause_ctxt.item_cursor == cursor)
				for (uint8_t item=0; item<=0x3A; item++) {
					uint8_t compare_cursor = cursor;
					uint8_t compare_item   = item;
					if (cursor == 0x4) {
						compare_cursor = 0x3;
						compare_item   = 0x38;
					}
					else if (cursor == 0xA) {
						compare_cursor = 0x3;
						compare_item   = 0x39;
					}
					else if (cursor == 0x10) {
						compare_cursor = 0x3;
						compare_item   = 0x3A;
					}
					
					if (z64_file.button_items[button] == compare_item && z64_file.c_button_slots[button - 1] == compare_cursor) {
						z64_game.common.input[0].pad_pressed.cl = z64_game.common.input[0].pad_pressed.cd = z64_game.common.input[0].pad_pressed.cr = 0;
						z64_file.button_items[button] = z64_file.c_button_slots[button - 1] = 0xFF;
						if (!z64_file.link_age)
							z64_file.adult_button_items[button]	= z64_file.adult_c_button_slots[button - 1] = 0xFF;
						else z64_file.child_button_items[button] = z64_file.child_c_button_slots[button - 1] = 0xFF;
						play_sfx = 0x480A;
						break;
					}
				}
	}
}

void handle_downgrading() {
	if (!SAVE_DOWNGRADE_ITEM || z64_game.pause_ctxt.state != 6 || z64_game.pause_ctxt.unk_02_[1] != 0)
		return;
	
	if (z64_file.giants_knife)
		if ( (z64_file.broken_giants_knife && z64_file.bgs_hits_left > 0) ||  (!z64_file.broken_giants_knife && z64_file.bgs_hits_left == 0) )
			z64_file.broken_giants_knife ^= 1;
	
	if (!z64_game.common.input[0].pad_pressed.cu)
		return;
	
	if (z64_game.pause_ctxt.screen_idx == 3) { // Swap knife
		if (z64_game.pause_ctxt.equip_cursor == 3 && (DOWNGRADE_GIANTS_KNIFE || z64_file.bgs_flag) ) {
			if (!DOWNGRADE_GIANTS_KNIFE)
				EXTRA_SRAM_1 |= 2;
			
			z64_file.bgs_flag ^= 1;
			if (!z64_file.bgs_flag)
				z64_file.bgs_hits_left = SWORD_HEALTH;

			if (z64_file.equip_sword == 3)
				unequip_sword(0);
			z64_UpdateEquipment(&z64_game, &z64_link);
			play_sfx = 0x4808;
		}
	}
			
	if (z64_game.pause_ctxt.screen_idx == 0) { // Downgrade / Upgrade items
		if (z64_game.pause_ctxt.item_cursor == 7 && (DOWNGRADE_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) ) {
			EXTRA_SRAM_1 |= 4;
			if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME)
				swap_item(Z64_SLOT_OCARINA, Z64_ITEM_OCARINA_OF_TIME, Z64_ITEM_FAIRY_OCARINA);
			else if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA)
				swap_item(Z64_SLOT_OCARINA, Z64_ITEM_FAIRY_OCARINA, Z64_ITEM_OCARINA_OF_TIME);
		}
		
		if (z64_game.pause_ctxt.item_cursor == 9 && (DOWNGRADE_HOOKSHOT || z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_LONGSHOT) ) {
			EXTRA_SRAM_1 |= 8;
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
				z64_file.button_items[i]            = swap;
				if (!z64_file.link_age)
					z64_file.adult_button_items[i]  = swap;
				else z64_file.child_button_items[i] = swap;
				z64_UpdateItemButton(&z64_game, i);
			}
		}
		z64_file.items[slot] = swap;
		play_sfx = 0x4808;
	}
}

void unequip_gear(uint8_t play) {
	z64_game.common.input[0].raw.pad.a = z64_game.common.input[0].pad_pressed.a = 0;
	z64_UpdateEquipment(&z64_game, &z64_link);
	if (play)
		play_sfx = 0x480A;
}

void unequip_sword(uint8_t play) {
	z64_file.equip_sword            = 0;
	if (z64_file.link_age)
		z64_file.child_equip_sword  = 0;
	else z64_file.adult_equip_sword = 0;
	z64_file.sword_flag             = 1;
	z64_file.button_items[0]        = -1;
	unequip_gear(play);
}

void unequip_shield() {
	z64_file.equip_shield            = 0;
	if (z64_file.link_age)
		z64_file.child_equip_shield  = 0;
	else z64_file.adult_equip_shield = 0;
	unequip_gear(1);
}

void unequip_tunic() {
	z64_file.equip_tunic            = 0;
	if (z64_file.link_age)
		z64_file.child_equip_tunic  = 0;
	else z64_file.adult_equip_tunic = 0;
	unequip_gear(1);
}