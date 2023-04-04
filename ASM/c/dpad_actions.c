#include "gfx.h"
#include "dpad_actions.h"

extern uint8_t CFG_ALLOW_KOKIRI_SWORD;
extern uint8_t CFG_ALLOW_MASTER_SWORD;
extern uint8_t CFG_ALLOW_GIANTS_KNIFE;
extern uint8_t CFG_ALLOW_DEKU_SHIELD;
extern uint8_t CFG_ALLOW_MIRROR_SHIELD;
extern uint8_t CFG_ALLOW_TUNIC;
extern uint8_t CFG_ALLOW_BOOTS;

extern uint8_t  dpad_alt;
extern uint16_t dpad_x;
extern uint16_t dpad_y;
extern uint16_t play_sfx;

static uint8_t DPAD_ACTIVE[4] = {0, 0, 0, 0};

void change_sword(uint8_t sword) {
	z64_file.equip_sword				= sword;
		if (z64_file.link_age)
		z64_file.child_equip_sword		= sword;
	else z64_file.adult_equip_sword		= sword;
	change_equipment();
	if (z64_file.equip_sword == 0) {
		z64_file.inf_table[29]			= 1;
		z64_file.button_items[0]		= -1;
	}
	else {
		z64_file.inf_table[29]			= 0;
		z64_file.button_items[0]		= z64_file.equip_sword + 0x3A;
		z64_UpdateItemButton(&z64_game, 0);
	}
}

void change_shield(uint8_t shield) {
	z64_file.equip_shield				= shield;
	if (z64_file.link_age)
		z64_file.child_equip_shield		= shield;
	else z64_file.adult_equip_shield	= shield;
	change_equipment();
}

void change_tunic(uint8_t tunic) {
	z64_file.equip_tunic				= tunic;
	if (z64_file.link_age)
		z64_file.child_equip_tunic		= tunic;
	else z64_file.adult_equip_tunic		= tunic;
	change_equipment();
}

void change_boots(uint8_t boots) {
	z64_file.equip_boots				= boots;
	if (z64_file.link_age)
		z64_file.child_equip_boots		= boots;
	else z64_file.adult_equip_boots		= boots;
	change_equipment();
}

void change_arrow(uint8_t button, z64_item_t item) {
	z64_file.button_items[button]	= item;
	if (!z64_file.link_age)
		z64_file.adult_button_items[button]	= item;
	else z64_file.child_button_items[button]	= item;
	z64_UpdateItemButton(&z64_game, button);
}

void change_equipment() {
	z64_UpdateEquipment(&z64_game, &z64_link);
	play_sfx = 0x835;
}

void run_dpad_actions(pad_t pad_pressed) {
	if (pad_pressed.du) {
		if (!z64_file.link_age && !dpad_alt)
			run_action(DPAD_ADULT_SET1_UP);
		else if (!z64_file.link_age &&  dpad_alt)
			run_action(DPAD_ADULT_SET2_UP);
		else if ( z64_file.link_age && !dpad_alt)
			run_action(DPAD_CHILD_SET1_UP);
		else if ( z64_file.link_age &&  dpad_alt)
			run_action(DPAD_CHILD_SET2_UP);
	}
	else if (pad_pressed.dr) {
		if (!z64_file.link_age && !dpad_alt)
			run_action(DPAD_ADULT_SET1_RIGHT);
		else if (!z64_file.link_age &&  dpad_alt)
			run_action(DPAD_ADULT_SET2_RIGHT);
		else if ( z64_file.link_age && !dpad_alt)
			run_action(DPAD_CHILD_SET1_RIGHT);
		else if ( z64_file.link_age &&  dpad_alt)
			run_action(DPAD_CHILD_SET2_RIGHT);
	}
	else if (pad_pressed.dd) {
		if (!z64_file.link_age && !dpad_alt)
			run_action(DPAD_ADULT_SET1_DOWN);
		else if (!z64_file.link_age &&  dpad_alt)
			run_action(DPAD_ADULT_SET2_DOWN);
		else if ( z64_file.link_age && !dpad_alt)
			run_action(DPAD_CHILD_SET1_DOWN);
		else if ( z64_file.link_age &&  dpad_alt)
			run_action(DPAD_CHILD_SET2_DOWN);
	}
	else if (pad_pressed.dl) {
		if (!z64_file.link_age && !dpad_alt)
			run_action(DPAD_ADULT_SET1_LEFT);
		else if (!z64_file.link_age &&  dpad_alt)
			run_action(DPAD_ADULT_SET2_LEFT);
		else if ( z64_file.link_age && !dpad_alt)
			run_action(DPAD_CHILD_SET1_LEFT);
		else if ( z64_file.link_age &&  dpad_alt)
			run_action(DPAD_CHILD_SET2_LEFT);
	}
}

void draw_dpad_actions(z64_disp_buf_t *db) {
	if (!z64_file.link_age && !dpad_alt)
		draw_action(DPAD_ADULT_SET1_UP, db, 2, -10);
	else if (!z64_file.link_age &&  dpad_alt)
		draw_action(DPAD_ADULT_SET2_UP, db, 2, -10);
	else if ( z64_file.link_age && !dpad_alt)
		draw_action(DPAD_CHILD_SET1_UP, db, 2, -10);
	else if ( z64_file.link_age &&  dpad_alt)
		draw_action(DPAD_CHILD_SET2_UP, db, 2, -10);
	
	if (!z64_file.link_age && !dpad_alt)
		draw_action(DPAD_ADULT_SET1_RIGHT, db, 14, 2);
	else if (!z64_file.link_age &&  dpad_alt)
		draw_action(DPAD_ADULT_SET2_RIGHT, db, 14, 2);
	else if ( z64_file.link_age && !dpad_alt)
		draw_action(DPAD_CHILD_SET1_RIGHT, db, 14, 2);
	else if ( z64_file.link_age &&  dpad_alt)
		draw_action(DPAD_CHILD_SET2_RIGHT, db, 14, 2);
	
	if (!z64_file.link_age && !dpad_alt)
		draw_action(DPAD_ADULT_SET1_DOWN, db, 2, 15);
	else if (!z64_file.link_age &&  dpad_alt)
		draw_action(DPAD_ADULT_SET2_DOWN, db, 2, 15);
	else if ( z64_file.link_age && !dpad_alt)
		draw_action(DPAD_CHILD_SET1_DOWN, db, 2, 15);
	else if ( z64_file.link_age &&  dpad_alt)
		draw_action(DPAD_CHILD_SET2_DOWN, db, 2, 15);
	
	if (!z64_file.link_age && !dpad_alt)
		draw_action(DPAD_ADULT_SET1_LEFT, db, -11, 2);
	else if (!z64_file.link_age &&  dpad_alt)
		draw_action(DPAD_ADULT_SET2_LEFT, db, -11, 2);
	else if ( z64_file.link_age && !dpad_alt)
		draw_action(DPAD_CHILD_SET1_LEFT, db, -11, 2);
	else if ( z64_file.link_age &&  dpad_alt)
		draw_action(DPAD_CHILD_SET2_LEFT, db, -11, 2);
}

uint8_t * check_dpad_actions() {
	if (!z64_file.link_age && !dpad_alt)
		check_action(0, DPAD_ADULT_SET1_UP);
	else if (!z64_file.link_age &&  dpad_alt)
		check_action(0, DPAD_ADULT_SET2_UP);
	else if ( z64_file.link_age && !dpad_alt)
		check_action(0, DPAD_CHILD_SET1_UP);
	else if ( z64_file.link_age &&  dpad_alt)
		check_action(0, DPAD_CHILD_SET2_UP);
	
	if (!z64_file.link_age && !dpad_alt)
		check_action(1, DPAD_ADULT_SET1_RIGHT);
	else if (!z64_file.link_age &&  dpad_alt)
		check_action(1, DPAD_ADULT_SET2_RIGHT);
	else if ( z64_file.link_age && !dpad_alt)
		check_action(1, DPAD_CHILD_SET1_RIGHT);
	else if ( z64_file.link_age &&  dpad_alt)
		check_action(1, DPAD_CHILD_SET2_RIGHT);
	
	if (!z64_file.link_age && !dpad_alt)
		check_action(2, DPAD_ADULT_SET1_DOWN);
	else if (!z64_file.link_age &&  dpad_alt)
		check_action(2, DPAD_ADULT_SET2_DOWN);
	else if ( z64_file.link_age && !dpad_alt)
		check_action(2, DPAD_CHILD_SET1_DOWN);
	else if ( z64_file.link_age &&  dpad_alt)
		check_action(2, DPAD_CHILD_SET2_DOWN);
	
	if (!z64_file.link_age && !dpad_alt)
		check_action(3, DPAD_ADULT_SET1_LEFT);
	else if (!z64_file.link_age &&  dpad_alt)
		check_action(3, DPAD_ADULT_SET2_LEFT);
	else if ( z64_file.link_age && !dpad_alt)
		check_action(3, DPAD_CHILD_SET1_LEFT);
	else if ( z64_file.link_age &&  dpad_alt)
		check_action(3, DPAD_CHILD_SET2_LEFT);
	
	return DPAD_ACTIVE;
}

void run_action(uint8_t action) {
	if (action == DPAD_SWORD)
		toggle_sword();
	else if (action == DPAD_BOOTS)
		toggle_boots();
	else if (action == DPAD_SHIELD)
		toggle_shield();
	else if (action == DPAD_TUNIC)
		toggle_tunic();
	else if (action == DPAD_ARROWS)
		toggle_arrow();
	else if (action == DPAD_IRON_BOOTS)
		swap_iron_boots();
	else if (action == DPAD_HOVER_BOOTS)
		swap_hover_boots();
	else if (action == DPAD_CHILD_TRADE)
		use_item(Z64_SLOT_CHILD_TRADE, CAN_USE_CHILD_TRADE);
	else if (action == DPAD_ADULT_TRADE)
		use_item(Z64_SLOT_ADULT_TRADE, CAN_USE_ADULT_TRADE);
	else if (action == DPAD_OCARINA)
		use_item(Z64_SLOT_OCARINA, CAN_USE_OCARINA);
	else if (action == DPAD_LENS)
		use_item(Z64_SLOT_LENS, CAN_USE_ITEMS);
	else if (action == DPAD_NUT && !z64_throwing_nut)
		use_item(Z64_SLOT_NUT, CAN_USE_ITEMS);
	else if (action == DPAD_DINS_FIRE)
		use_item(Z64_SLOT_DINS_FIRE, CAN_USE_ITEMS);
	else if (action == DPAD_FARORES_WIND)
		use_item(Z64_SLOT_FARORES_WIND, CAN_USE_FARORES_WIND);
	else if (action == DPAD_NAYRUS_LOVE)
		use_item(Z64_SLOT_NAYRUS_LOVE, CAN_USE_ITEMS);
	
}

void draw_action(uint8_t action, z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	if (action == DPAD_SWORD)
		draw_sword_icon(db, icon_x, icon_y);
	else if (action == DPAD_BOOTS)
		draw_boots_icon(db, icon_x, icon_y);
	else if (action == DPAD_SHIELD)
		draw_shield_icon(db, icon_x, icon_y);
	else if (action == DPAD_TUNIC)
		draw_tunic_icon(db, icon_x, icon_y);
	else if (action == DPAD_ARROWS)
		draw_arrow_icon(db, icon_x, icon_y);
	else if (action == DPAD_IRON_BOOTS)
		draw_iron_boots_icon(db, icon_x, icon_y);
	else if (action == DPAD_HOVER_BOOTS)
		draw_hover_boots_icon(db, icon_x, icon_y);
	else if (action == DPAD_CHILD_TRADE)
		draw_child_trade_icon(db, icon_x, icon_y);
	else if (action == DPAD_ADULT_TRADE)
		draw_adult_trade_icon(db, icon_x, icon_y);
	else if (action == DPAD_OCARINA)
		draw_ocarina_icon(db, icon_x, icon_y);
	else if (action == DPAD_LENS)
		draw_item_icon(db, icon_x, icon_y, Z64_SLOT_LENS, Z64_ITEM_LENS, CAN_USE_LENS);
	else if (action == DPAD_NUT)
		draw_item_icon(db, icon_x, icon_y, Z64_SLOT_NUT, Z64_ITEM_NUT, CAN_USE_ITEMS);
	else if (action == DPAD_DINS_FIRE)
		draw_item_icon(db, icon_x, icon_y, Z64_SLOT_DINS_FIRE, Z64_ITEM_DINS_FIRE, CAN_USE_ITEMS);
	else if (action == DPAD_FARORES_WIND)
		draw_item_icon(db, icon_x, icon_y, Z64_SLOT_FARORES_WIND, Z64_ITEM_FARORES_WIND, CAN_USE_FARORES_WIND);
	else if (action == DPAD_NAYRUS_LOVE)
		draw_item_icon(db, icon_x, icon_y, Z64_SLOT_NAYRUS_LOVE, Z64_ITEM_NAYRUS_LOVE, CAN_USE_ITEMS);
}

void check_action(uint8_t button, uint8_t action) {
	if (action == DPAD_SWORD) { // Sword
		if (z64_file.kokiri_sword || z64_file.master_sword || z64_file.giants_knife)
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == DPAD_BOOTS) { // Boots
		if (z64_file.kokiri_boots && (z64_file.iron_boots || z64_file.hover_boots) )
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == DPAD_SHIELD) { // Shield
		if (z64_file.deku_shield || z64_file.hylian_shield || z64_file.mirror_shield)
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == DPAD_TUNIC) { // Tunic
		if (z64_file.kokiri_tunic && (z64_file.goron_tunic || z64_file.zora_tunic) )
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == DPAD_ARROWS) { // Arrow
		if (z64_file.items[Z64_SLOT_BOW] == Z64_ITEM_BOW && (z64_file.items[Z64_SLOT_FIRE_ARROW] == Z64_ITEM_FIRE_ARROW || z64_file.items[Z64_SLOT_ICE_ARROW] == Z64_ITEM_ICE_ARROW || z64_file.items[Z64_SLOT_LIGHT_ARROW] == Z64_ITEM_LIGHT_ARROW) )
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == DPAD_IRON_BOOTS) { // Iron Boots
		if (z64_file.iron_boots && (!z64_file.link_age || CFG_ALLOW_BOOTS) )
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == DPAD_HOVER_BOOTS) { // Hover Boots
		if (z64_file.hover_boots && (!z64_file.link_age || CFG_ALLOW_BOOTS) )
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == DPAD_CHILD_TRADE) { // Child Trade
		if (z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH && z64_file.link_age)
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == DPAD_ADULT_TRADE) { // Adult Trade
		if (z64_file.items[Z64_SLOT_ADULT_TRADE] >= Z64_ITEM_POCKET_EGG && z64_file.items[Z64_SLOT_ADULT_TRADE] <= Z64_ITEM_CLAIM_CHECK && !z64_file.link_age)
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == DPAD_OCARINA) { // Ocarina
		if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME)
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	
	check_action_item(button, action, DPAD_LENS,         Z64_SLOT_LENS,         Z64_ITEM_LENS);
	check_action_item(button, action, DPAD_NUT,          Z64_SLOT_NUT,          Z64_SLOT_NUT);
	check_action_item(button, action, DPAD_DINS_FIRE,    Z64_SLOT_DINS_FIRE,    Z64_ITEM_DINS_FIRE);
	check_action_item(button, action, DPAD_FARORES_WIND, Z64_SLOT_FARORES_WIND, Z64_ITEM_FARORES_WIND);
	check_action_item(button, action, DPAD_NAYRUS_LOVE,  Z64_SLOT_NAYRUS_LOVE , Z64_ITEM_NAYRUS_LOVE);
	
	if (action == DPAD_NULL)
		DPAD_ACTIVE[button] = 0;
}

void check_action_item(uint8_t button, uint8_t action, uint8_t dpad, z64_slot_t slot, z64_item_t item) {
	if (action == dpad) { // Nayru's Love
		if (z64_file.items[slot] == item)
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
}

void toggle_sword() {
	if (!z64_file.kokiri_sword && !z64_file.master_sword && !z64_file.giants_knife)
		return;
	
	uint8_t sword = z64_file.equip_sword;
	sword++;
	
	if (sword > 3) {
		sword = 0;
		if (!SAVE_UNEQUIP_GEAR)
			sword++;
	}
	
	if (sword == 1 && (!z64_file.kokiri_sword || (!z64_file.link_age && !CFG_ALLOW_KOKIRI_SWORD) ) )
		sword++;
	if (sword == 2 && (!z64_file.master_sword || ( z64_file.link_age && !CFG_ALLOW_MASTER_SWORD) ) )
		sword++;
	if (sword == 3 && (!z64_file.giants_knife || ( z64_file.link_age && !CFG_ALLOW_GIANTS_KNIFE) ) ) {
		if (SAVE_UNEQUIP_GEAR)
			sword = 0;
		else if (z64_file.kokiri_sword && ( z64_file.link_age || CFG_ALLOW_KOKIRI_SWORD) )
			sword = 1;
		else if (z64_file.master_sword && (!z64_file.link_age || CFG_ALLOW_MASTER_SWORD) )
			sword = 2;
		else sword = 0;
	}
			
	if (sword != z64_file.equip_sword)
		change_sword(sword);
}

void toggle_shield() {
	if (!z64_file.deku_shield && !z64_file.hylian_shield && !z64_file.mirror_shield)
		return;
	
	uint8_t shield = z64_file.equip_shield;
	shield++;
	
	if (shield > 3) {
		shield = 0;
		if (!SAVE_UNEQUIP_GEAR)
			shield++;
	}
	
	if (shield == 1 && (!z64_file.deku_shield || (!z64_file.link_age && !CFG_ALLOW_DEKU_SHIELD) ) )
		shield++;
	if (shield == 2 && !z64_file.hylian_shield)
		shield++;
	if (shield == 3 && (!z64_file.mirror_shield || (z64_file.link_age && !CFG_ALLOW_MIRROR_SHIELD) ) ) {
		if (SAVE_UNEQUIP_GEAR)
			shield = 0;
		else if (z64_file.deku_shield && (z64_file.link_age || CFG_ALLOW_DEKU_SHIELD) )
			shield = 1;
		else if (z64_file.hylian_shield)
			shield = 2;
		else shield = 0;
	}
	
	if (shield != z64_file.equip_shield)
		change_shield(shield);
}

void toggle_tunic() {
	if (!z64_file.kokiri_tunic && !z64_file.goron_tunic && !z64_file.zora_tunic)
		return;
	
	uint8_t not_allowed = z64_file.link_age && !CFG_ALLOW_TUNIC;
	uint8_t can_unequip = SAVE_UNEQUIP_GEAR || SAVE_EXTRA_ABILITIES;
	uint8_t tunic       = z64_file.equip_tunic;
	tunic++;
	
	if (tunic > 3) {
		tunic = 0;
		if (!can_unequip)
			tunic++;
	}
	
	if (tunic == 1 && !z64_file.kokiri_tunic)
		tunic++;
	if (tunic == 2 && (!z64_file.goron_tunic || not_allowed) )
		tunic++;
	if (tunic == 3 && (!z64_file.zora_tunic  || not_allowed) ) {
		if (can_unequip)
			tunic = 0;
		else if (z64_file.kokiri_tunic)
			tunic = 1;
		else if (z64_file.goron_tunic && (!z64_file.link_age || !CFG_ALLOW_TUNIC) )
			tunic = 2;
		else tunic = 0;
	}
	
	if (tunic != z64_file.equip_tunic)
		change_tunic(tunic);
}

void toggle_boots() {
	if (z64_file.link_age && !CFG_ALLOW_BOOTS)
		return;
	if (!z64_file.kokiri_boots || (!z64_file.iron_boots && !z64_file.hover_boots) )
		return;
	
	uint8_t boots = z64_file.equip_boots;
	boots++;
	if (boots > 3)
		boots = 1;
	
	if (boots == 2 && !z64_file.iron_boots)
		boots++;
	if (boots == 3 && !z64_file.hover_boots)
		boots = 1;
	
	if (boots != z64_file.equip_boots)
		change_boots(boots);
}

void toggle_arrow() {
	if (z64_file.items[Z64_SLOT_BOW] != Z64_ITEM_BOW)
		return;
	
	uint8_t slot;
	uint8_t arrow = 0;
	for (slot=1; slot<=3; slot++) {
		if (z64_file.button_items[slot] == Z64_ITEM_BOW || z64_file.button_items[slot] == Z64_ITEM_BOW_FIRE_ARROW || z64_file.button_items[slot] == Z64_ITEM_BOW_ICE_ARROW || z64_file.button_items[slot] == Z64_ITEM_BOW_LIGHT_ARROW) {
			arrow = z64_file.button_items[slot];
			break;
		}
	}
		
	if (arrow == 0)
		return;
	
	if (arrow == Z64_ITEM_BOW) {
		if (z64_file.items[Z64_SLOT_FIRE_ARROW] == Z64_ITEM_FIRE_ARROW)
			arrow = Z64_ITEM_BOW_FIRE_ARROW;
		else if (z64_file.items[Z64_SLOT_ICE_ARROW] == Z64_ITEM_ICE_ARROW)
			arrow = Z64_ITEM_BOW_ICE_ARROW;
		else if (z64_file.items[Z64_SLOT_LIGHT_ARROW] == Z64_ITEM_LIGHT_ARROW)
			arrow = Z64_ITEM_BOW_LIGHT_ARROW;
	}
	else if (arrow == Z64_ITEM_BOW_FIRE_ARROW) {
		if (z64_file.items[Z64_SLOT_ICE_ARROW] == Z64_ITEM_ICE_ARROW)
			arrow = Z64_ITEM_BOW_ICE_ARROW;
		else if (z64_file.items[Z64_SLOT_LIGHT_ARROW] == Z64_ITEM_LIGHT_ARROW)
			arrow = Z64_ITEM_BOW_LIGHT_ARROW;
		else arrow = Z64_ITEM_BOW;
	}
	else if (arrow == Z64_ITEM_BOW_ICE_ARROW) {
		if (z64_file.items[Z64_SLOT_LIGHT_ARROW] == Z64_ITEM_LIGHT_ARROW)
			arrow = Z64_ITEM_BOW_LIGHT_ARROW;
		else arrow = Z64_ITEM_BOW;
	}
	else arrow = Z64_ITEM_BOW;
	
	if (arrow != z64_file.button_items[slot]) {
		if (arrow == 0x03) {
			change_arrow(slot, arrow);
			play_sfx = 0x4808;
		}
		else {
			change_arrow(slot, arrow);
			play_sfx = 0x4806 + arrow;
		}
	}
}

void swap_iron_boots() {
	if (!z64_file.iron_boots)
		return;
	
	if (z64_file.equip_boots == 2)
		z64_file.equip_boots = 1;
	else z64_file.equip_boots = 2;
	change_equipment();
}

void swap_hover_boots() {
	if (!z64_file.hover_boots)
		return;
	
	if (z64_file.equip_boots == 3)
		z64_file.equip_boots = 1;
	else z64_file.equip_boots = 3;
	change_equipment();
}

void use_item(z64_slot_t slot, uint8_t usability) {
	if (usability)
		z64_usebutton(&z64_game,&z64_link,z64_file.items[slot], 2);
}

void draw_sword_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.equip_sword == 0)
		return;
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
	sprite_load(db, &items_sprite, (58 + z64_file.equip_sword), 1);
	sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12, 12);
}

void draw_shield_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.equip_shield == 0)
		return;
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
	sprite_load(db, &items_sprite, (61 + z64_file.equip_shield), 1);
	sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12, 12);
}

void draw_tunic_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.equip_tunic == 0)
		return;
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
	sprite_load(db, &items_sprite, (64 + z64_file.equip_tunic), 1);
	sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12, 12);
}


void draw_boots_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.equip_boots == 0)
		return;
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
	sprite_load(db, &items_sprite, (67 + z64_file.equip_boots), 1);
	sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12, 12);
}

void draw_arrow_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	for (uint8_t i=1; i<=3; i++) {
		if (z64_file.button_items[i] == Z64_ITEM_BOW || z64_file.button_items[i] == Z64_ITEM_BOW_FIRE_ARROW || z64_file.button_items[i] == Z64_ITEM_BOW_ICE_ARROW || z64_file.button_items[i] == Z64_ITEM_BOW_LIGHT_ARROW) {
			gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
			if (z64_file.button_items[i] == Z64_ITEM_BOW)
				sprite_load(db, &items_sprite, Z64_ITEM_BOW, 1);
			if (z64_file.button_items[i] == Z64_ITEM_BOW_FIRE_ARROW)
				sprite_load(db, &items_sprite, Z64_ITEM_BOW_FIRE_ARROW, 1);
			if (z64_file.button_items[i] == Z64_ITEM_BOW_ICE_ARROW)
				sprite_load(db, &items_sprite, Z64_ITEM_BOW_ICE_ARROW, 1);
			if (z64_file.button_items[i] == Z64_ITEM_BOW_LIGHT_ARROW)
				sprite_load(db, &items_sprite, Z64_ITEM_BOW_LIGHT_ARROW, 1);
			sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12, 12);
			return;
		}
	}
	if (z64_game.pause_ctxt.state == 6) {
		sprite_load(db, &items_sprite, Z64_ITEM_BOW, 1);
		gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
		sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12, 12);
	}
}

void draw_iron_boots_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.iron_boots) {
		gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
		sprite_load(db, &items_sprite, 69, 1);
		if (z64_file.equip_boots == 2)
			sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x - 2), (dpad_y + icon_y - 2), 16, 16);
		else sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12, 12);
	}
}

void draw_hover_boots_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.hover_boots) {
		gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
		sprite_load(db, &items_sprite, 70, 1);
		if (z64_file.equip_boots == 3)
			sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x - 2), (dpad_y + icon_y - 2), 16, 16);
		else sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12, 12);
	}
}

void draw_child_trade_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH) {
		if (!CAN_USE_CHILD_TRADE && z64_game.pause_ctxt.state == 0)
			gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
		else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
		sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_CHILD_TRADE], 1);
		if (z64_mask_equipped > 0)
			sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x - 2), (dpad_y + icon_y - 2), 16, 16);
		else sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12, 12);
	}
}

void draw_adult_trade_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.items[Z64_SLOT_ADULT_TRADE] >= Z64_ITEM_POCKET_EGG && z64_file.items[Z64_SLOT_ADULT_TRADE] <= Z64_ITEM_CLAIM_CHECK) {
		if (!CAN_USE_CHILD_TRADE && z64_game.pause_ctxt.state == 0)
			gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
		else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
		sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_ADULT_TRADE], 1);
		if (z64_mask_equipped > 0)
			sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x - 2), (dpad_y + icon_y - 2), 16, 16);
		else sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12, 12);
	}
}

void draw_ocarina_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) {
		if (!CAN_USE_OCARINA && z64_game.pause_ctxt.state == 0)
			gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
		else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
		sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_OCARINA], 1);
		sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12,12);
	}
}

void draw_item_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, z64_slot_t slot, z64_item_t item, uint8_t usability) {
	if (z64_file.items[slot] == item) {
		if (!usability && z64_game.pause_ctxt.state == 0)
			gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
		else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
		sprite_load(db, &items_sprite, z64_file.items[slot], 1);
		sprite_draw(db, &items_sprite, 0, (dpad_x + icon_x), (dpad_y + icon_y), 12,12);
	}
}