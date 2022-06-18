#include "gfx.h"
#include "dpad_actions.h"

extern uint8_t CFG_UNEQUIP_GEAR_ENABLED;

extern uint8_t CFG_DPAD_ADULT_SET1_UP;
extern uint8_t CFG_DPAD_ADULT_SET1_RIGHT;
extern uint8_t CFG_DPAD_ADULT_SET1_DOWN;
extern uint8_t CFG_DPAD_ADULT_SET1_LEFT;
extern uint8_t CFG_DPAD_ADULT_SET2_UP;
extern uint8_t CFG_DPAD_ADULT_SET2_RIGHT;
extern uint8_t CFG_DPAD_ADULT_SET2_DOWN;
extern uint8_t CFG_DPAD_ADULT_SET2_LEFT;
extern uint8_t CFG_DPAD_CHILD_SET1_UP;
extern uint8_t CFG_DPAD_CHILD_SET1_RIGHT;
extern uint8_t CFG_DPAD_CHILD_SET1_DOWN;
extern uint8_t CFG_DPAD_CHILD_SET1_LEFT;
extern uint8_t CFG_DPAD_CHILD_SET2_UP;
extern uint8_t CFG_DPAD_CHILD_SET2_RIGHT;
extern uint8_t CFG_DPAD_CHILD_SET2_DOWN;
extern uint8_t CFG_DPAD_CHILD_SET2_LEFT;

extern uint8_t CFG_ALLOW_KOKIRI_SWORD;
extern uint8_t CFG_ALLOW_MASTER_SWORD;
extern uint8_t CFG_ALLOW_GIANTS_KNIFE;
extern uint8_t CFG_ALLOW_DEKU_SHIELD;
extern uint8_t CFG_ALLOW_MIRROR_SHIELD;
extern uint8_t CFG_ALLOW_TUNIC;
extern uint8_t CFG_ALLOW_BOOTS;

extern uint8_t DPAD_ALT;
extern uint16_t DPAD_X;
extern uint16_t DPAD_Y;

//unknown 00 is a pointer to some vector transformation when the sound is tied to an actor. actor + 0x3E, when not tied to an actor (map), always 80104394
//unknown 01 is always 4 in my testing
//unknown 02 is a pointer to some kind of audio configuration Always 801043A0 in my testing
//unknown 03 is always a3 in my testing
//unknown 04 is always a3 + 0x08 in my testing (801043A8)
typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);
typedef void(*usebutton_t)(z64_game_t *game, z64_link_t *link, uint8_t item, uint8_t button);

#define z64_playsfx   ((playsfx_t)      0x800C806C)
#define z64_usebutton ((usebutton_t)    0x8038C9A0)

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

void change_arrow(uint8_t button, z64_item_t item, uint16_t sfx) {
	z64_file.button_items[button]	= item;
	if (!z64_file.link_age)
		z64_file.adult_button_items[button]	= item;
	else z64_file.child_button_items[button]	= item;
	z64_UpdateItemButton(&z64_game, button);
	z64_playsfx(sfx, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}

void change_equipment() {
	z64_UpdateEquipment(&z64_game, &z64_link);
	z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}

void run_dpad_actions(pad_t pad_pressed) {
	if (pad_pressed.du) {
		if (!z64_file.link_age && !DPAD_ALT)
			run_action(CFG_DPAD_ADULT_SET1_UP);
		else if (!z64_file.link_age &&  DPAD_ALT)
			run_action(CFG_DPAD_ADULT_SET2_UP);
		else if ( z64_file.link_age && !DPAD_ALT)
			run_action(CFG_DPAD_CHILD_SET1_UP);
		else if ( z64_file.link_age &&  DPAD_ALT)
			run_action(CFG_DPAD_CHILD_SET2_UP);
	}
	else if (pad_pressed.dr) {
		if (!z64_file.link_age && !DPAD_ALT)
			run_action(CFG_DPAD_ADULT_SET1_RIGHT);
		else if (!z64_file.link_age &&  DPAD_ALT)
			run_action(CFG_DPAD_ADULT_SET2_RIGHT);
		else if ( z64_file.link_age && !DPAD_ALT)
			run_action(CFG_DPAD_CHILD_SET1_RIGHT);
		else if ( z64_file.link_age &&  DPAD_ALT)
			run_action(CFG_DPAD_CHILD_SET2_RIGHT);
	}
	else if (pad_pressed.dd) {
		if (!z64_file.link_age && !DPAD_ALT)
			run_action(CFG_DPAD_ADULT_SET1_DOWN);
		else if (!z64_file.link_age &&  DPAD_ALT)
			run_action(CFG_DPAD_ADULT_SET2_DOWN);
		else if ( z64_file.link_age && !DPAD_ALT)
			run_action(CFG_DPAD_CHILD_SET1_DOWN);
		else if ( z64_file.link_age &&  DPAD_ALT)
			run_action(CFG_DPAD_CHILD_SET2_DOWN);
	}
	else if (pad_pressed.dl) {
		if (!z64_file.link_age && !DPAD_ALT)
			run_action(CFG_DPAD_ADULT_SET1_LEFT);
		else if (!z64_file.link_age &&  DPAD_ALT)
			run_action(CFG_DPAD_ADULT_SET2_LEFT);
		else if ( z64_file.link_age && !DPAD_ALT)
			run_action(CFG_DPAD_CHILD_SET1_LEFT);
		else if ( z64_file.link_age &&  DPAD_ALT)
			run_action(CFG_DPAD_CHILD_SET2_LEFT);
	}
}

void draw_dpad_actions(z64_disp_buf_t *db, uint16_t alpha) {
	if (!z64_file.link_age && !DPAD_ALT)
		draw_action(CFG_DPAD_ADULT_SET1_UP, db, alpha, 2, -10);
	else if (!z64_file.link_age &&  DPAD_ALT)
		draw_action(CFG_DPAD_ADULT_SET2_UP, db, alpha, 2, -10);
	else if ( z64_file.link_age && !DPAD_ALT)
		draw_action(CFG_DPAD_CHILD_SET1_UP, db, alpha, 2, -10);
	else if ( z64_file.link_age &&  DPAD_ALT)
		draw_action(CFG_DPAD_CHILD_SET2_UP, db, alpha, 2, -10);
	
	if (!z64_file.link_age && !DPAD_ALT)
		draw_action(CFG_DPAD_ADULT_SET1_RIGHT, db, alpha, 14, 2);
	else if (!z64_file.link_age &&  DPAD_ALT)
		draw_action(CFG_DPAD_ADULT_SET2_RIGHT, db, alpha, 14, 2);
	else if ( z64_file.link_age && !DPAD_ALT)
		draw_action(CFG_DPAD_CHILD_SET1_RIGHT, db, alpha, 14, 2);
	else if ( z64_file.link_age &&  DPAD_ALT)
		draw_action(CFG_DPAD_CHILD_SET2_RIGHT, db, alpha, 14, 2);
	
	if (!z64_file.link_age && !DPAD_ALT)
		draw_action(CFG_DPAD_ADULT_SET1_DOWN, db, alpha, 2, 15);
	else if (!z64_file.link_age &&  DPAD_ALT)
		draw_action(CFG_DPAD_ADULT_SET2_DOWN, db, alpha, 2, 15);
	else if ( z64_file.link_age && !DPAD_ALT)
		draw_action(CFG_DPAD_CHILD_SET1_DOWN, db, alpha, 2, 15);
	else if ( z64_file.link_age &&  DPAD_ALT)
		draw_action(CFG_DPAD_CHILD_SET2_DOWN, db, alpha, 2, 15);
	
	if (!z64_file.link_age && !DPAD_ALT)
		draw_action(CFG_DPAD_ADULT_SET1_LEFT, db, alpha, -11, 2);
	else if (!z64_file.link_age &&  DPAD_ALT)
		draw_action(CFG_DPAD_ADULT_SET2_LEFT, db, alpha, -11, 2);
	else if ( z64_file.link_age && !DPAD_ALT)
		draw_action(CFG_DPAD_CHILD_SET1_LEFT, db, alpha, -11, 2);
	else if ( z64_file.link_age &&  DPAD_ALT)
		draw_action(CFG_DPAD_CHILD_SET2_LEFT, db, alpha, -11, 2);
}

uint8_t * check_dpad_actions() {
	if (!z64_file.link_age && !DPAD_ALT)
		check_action(0, CFG_DPAD_ADULT_SET1_UP);
	else if (!z64_file.link_age &&  DPAD_ALT)
		check_action(0, CFG_DPAD_ADULT_SET2_UP);
	else if ( z64_file.link_age && !DPAD_ALT)
		check_action(0, CFG_DPAD_CHILD_SET1_UP);
	else if ( z64_file.link_age &&  DPAD_ALT)
		check_action(0, CFG_DPAD_CHILD_SET2_UP);
	
	if (!z64_file.link_age && !DPAD_ALT)
		check_action(1, CFG_DPAD_ADULT_SET1_RIGHT);
	else if (!z64_file.link_age &&  DPAD_ALT)
		check_action(1, CFG_DPAD_ADULT_SET2_RIGHT);
	else if ( z64_file.link_age && !DPAD_ALT)
		check_action(1, CFG_DPAD_CHILD_SET1_RIGHT);
	else if ( z64_file.link_age &&  DPAD_ALT)
		check_action(1, CFG_DPAD_CHILD_SET2_RIGHT);
	
	if (!z64_file.link_age && !DPAD_ALT)
		check_action(2, CFG_DPAD_ADULT_SET1_DOWN);
	else if (!z64_file.link_age &&  DPAD_ALT)
		check_action(2, CFG_DPAD_ADULT_SET2_DOWN);
	else if ( z64_file.link_age && !DPAD_ALT)
		check_action(2, CFG_DPAD_CHILD_SET1_DOWN);
	else if ( z64_file.link_age &&  DPAD_ALT)
		check_action(2, CFG_DPAD_CHILD_SET2_DOWN);
	
	if (!z64_file.link_age && !DPAD_ALT)
		check_action(3, CFG_DPAD_ADULT_SET1_LEFT);
	else if (!z64_file.link_age &&  DPAD_ALT)
		check_action(3, CFG_DPAD_ADULT_SET2_LEFT);
	else if ( z64_file.link_age && !DPAD_ALT)
		check_action(3, CFG_DPAD_CHILD_SET1_LEFT);
	else if ( z64_file.link_age &&  DPAD_ALT)
		check_action(3, CFG_DPAD_CHILD_SET2_LEFT);
	
	return DPAD_ACTIVE;
}

void run_action(uint8_t action) {
	if (action == 0x01)
		toggle_sword();
	else if (action == 0x02)
		toggle_boots();
	else if (action == 0x03)
		toggle_shield();
	else if (action == 0x04)
		toggle_tunic();
	else if (action == 0x05)
		toggle_arrow();
	else if (action == 0x06)
		swap_iron_boots();
	else if (action == 0x07)
		swap_hover_boots();
	else if (action == 0x08)
		use_child_trade();
	else if (action == 0x09)
		use_ocarina();
}

void draw_action(uint8_t action, z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y) {
	if (action == 0x01)
		draw_sword_icon(db, alpha, icon_x, icon_y);
	else if (action == 0x02)
		draw_boots_icon(db, alpha, icon_x, icon_y);
	else if (action == 0x03)
		draw_shield_icon(db, alpha, icon_x, icon_y);
	else if (action == 0x04)
		draw_tunic_icon(db, alpha, icon_x, icon_y);
	else if (action == 0x05)
		draw_arrow_icon(db, alpha, icon_x, icon_y);
	else if (action == 0x06)
		draw_iron_boots_icon(db, alpha, icon_x, icon_y);
	else if (action == 0x07)
		draw_hover_boots_icon(db, alpha, icon_x, icon_y);
	else if (action == 0x08)
		draw_child_trade_icon(db, alpha, icon_x, icon_y);
	else if (action == 0x09)
		draw_ocarina_icon(db, alpha, icon_x, icon_y);
}

void check_action(uint8_t button, uint8_t action) {
	if (action == 0x01) { // Sword
		if (z64_file.kokiri_sword || z64_file.master_sword || z64_file.giants_knife)
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == 0x02) { // Boots
		if (z64_file.kokiri_boots && (z64_file.iron_boots || z64_file.hover_boots) )
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == 0x03) { // Shield
		if (z64_file.deku_shield || z64_file.hylian_shield || z64_file.mirror_shield)
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == 0x04) { // Tunic
		if (z64_file.kokiri_tunic && (z64_file.goron_tunic || z64_file.zora_tunic) )
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == 0x05) { // Arrow
		if (z64_file.items[Z64_SLOT_BOW] == Z64_ITEM_BOW && (z64_file.items[Z64_SLOT_FIRE_ARROW] == Z64_ITEM_FIRE_ARROW || z64_file.items[Z64_SLOT_ICE_ARROW] == Z64_ITEM_ICE_ARROW || z64_file.items[Z64_SLOT_LIGHT_ARROW] == Z64_ITEM_LIGHT_ARROW) )
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == 0x06) { // Iron Boots
		if (z64_file.iron_boots && (!z64_file.link_age || CFG_ALLOW_BOOTS) )
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == 0x07) { // Hover Boots
		if (z64_file.hover_boots && (!z64_file.link_age || CFG_ALLOW_BOOTS) )
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == 0x08) { // Child Trade
		if (z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH && z64_file.link_age)
			DPAD_ACTIVE[button] = 1;
		else DPAD_ACTIVE[button] = 0;
	}
	else if (action == 0x09) { // Ocarina
		if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME)
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
		if (!CFG_UNEQUIP_GEAR_ENABLED)
			sword++;
	}
	
	if (sword == 1 && (!z64_file.kokiri_sword || (!z64_file.link_age && !CFG_ALLOW_KOKIRI_SWORD) ) )
		sword++;
	if (sword == 2 && (!z64_file.master_sword || ( z64_file.link_age && !CFG_ALLOW_MASTER_SWORD) ) )
		sword++;
	if (sword == 3 && (!z64_file.giants_knife || ( z64_file.link_age && !CFG_ALLOW_GIANTS_KNIFE) ) ) {
		if (CFG_UNEQUIP_GEAR_ENABLED)
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
		if (!CFG_UNEQUIP_GEAR_ENABLED)
			shield++;
	}
	
	if (shield == 1 && (!z64_file.deku_shield || (!z64_file.link_age && !CFG_ALLOW_DEKU_SHIELD) ) )
		shield++;
	if (shield == 2 && !z64_file.hylian_shield)
		shield++;
	if (shield == 3 && (!z64_file.mirror_shield || (z64_file.link_age && !CFG_ALLOW_MIRROR_SHIELD) ) ) {
		if (CFG_UNEQUIP_GEAR_ENABLED)
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
	if (z64_file.link_age && CFG_ALLOW_TUNIC)
		return;
	if (!z64_file.kokiri_tunic || (!z64_file.goron_tunic && !z64_file.zora_tunic) )
			return;
	
	uint8_t tunic = z64_file.equip_tunic;
	tunic++;
	if (tunic > 3)
		tunic = 1;
	
	if (tunic == 2 && !z64_file.goron_tunic)
		tunic++;
	if (tunic == 3 && !z64_file.zora_tunic)
		tunic = 1;
	
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
	
	if (arrow == Z64_ITEM_BOW)
		arrow = Z64_ITEM_BOW_FIRE_ARROW;
	else if (arrow == Z64_ITEM_BOW_FIRE_ARROW)
		arrow = Z64_ITEM_BOW_ICE_ARROW;
	else if (arrow == Z64_ITEM_BOW_ICE_ARROW)
		arrow = Z64_ITEM_BOW_LIGHT_ARROW;
	else arrow = Z64_ITEM_BOW;
	
	if (arrow == Z64_ITEM_BOW && z64_file.items[Z64_SLOT_FIRE_ARROW] != Z64_ITEM_FIRE_ARROW)
		arrow = Z64_ITEM_BOW_ICE_ARROW;
	else if (arrow == Z64_ITEM_BOW_FIRE_ARROW && z64_file.items[Z64_SLOT_ICE_ARROW] != Z64_ITEM_ICE_ARROW)
		arrow = Z64_ITEM_BOW_LIGHT_ARROW;
	else if (arrow == Z64_ITEM_BOW_ICE_ARROW && z64_file.items[Z64_SLOT_LIGHT_ARROW] != Z64_ITEM_LIGHT_ARROW)
		arrow = Z64_ITEM_BOW;
	
	if (arrow != z64_file.button_items[slot]) {
		if (arrow == 03)
			change_arrow(slot, arrow, 0x4808);
		else change_arrow(slot, arrow, (0x4806 + arrow) );
	}
}

void swap_iron_boots() {
	if (!z64_file.iron_boots)
		return;
	
	if (z64_file.equip_boots == 2)
		z64_file.equip_boots = 1;
	else z64_file.equip_boots = 2;
	z64_UpdateEquipment(&z64_game, &z64_link);
	z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}

void swap_hover_boots() {
	if (!z64_file.hover_boots)
		return;
	
	if (z64_file.equip_boots == 3)
		z64_file.equip_boots = 1;
	else z64_file.equip_boots = 3;
	z64_UpdateEquipment(&z64_game, &z64_link);
	z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}

void use_child_trade() {
	if (CAN_USE_CHILD_TRADE)
		z64_usebutton(&z64_game,&z64_link,z64_file.items[Z64_SLOT_CHILD_TRADE], 2);
}

void use_ocarina() {
	if (CAN_USE_OCARINA)
		z64_usebutton(&z64_game,&z64_link,z64_file.items[Z64_SLOT_OCARINA], 2);
}

void draw_sword_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.equip_sword == 0)
		return;
	sprite_load(db, &items_sprite, (58 + z64_file.equip_sword), 1);
	sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x), (DPAD_Y + icon_y), 12, 12);
}

void draw_shield_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.equip_shield == 0)
		return;
	sprite_load(db, &items_sprite, (61 + z64_file.equip_shield), 1);
	sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x), (DPAD_Y + icon_y), 12, 12);
}

void draw_tunic_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.equip_tunic == 0)
		return;
	sprite_load(db, &items_sprite, (64 + z64_file.equip_tunic), 1);
	sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x), (DPAD_Y + icon_y), 12, 12);
}


void draw_boots_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.equip_boots == 0)
		return;
	sprite_load(db, &items_sprite, (67 + z64_file.equip_boots), 1);
	sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x), (DPAD_Y + icon_y), 12, 12);
}

void draw_arrow_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y) {
	for (uint8_t i=1; i<=3; i++) {
		if (z64_file.button_items[i] == Z64_ITEM_BOW || z64_file.button_items[i] == Z64_ITEM_BOW_FIRE_ARROW || z64_file.button_items[i] == Z64_ITEM_BOW_ICE_ARROW || z64_file.button_items[i] == Z64_ITEM_BOW_LIGHT_ARROW) {
			if (z64_file.button_items[i] == Z64_ITEM_BOW)
				sprite_load(db, &items_sprite, 0x03, 1);
			if (z64_file.button_items[i] == Z64_ITEM_BOW_FIRE_ARROW)
				sprite_load(db, &items_sprite, 0x38, 1);
			if (z64_file.button_items[i] == Z64_ITEM_BOW_ICE_ARROW)
				sprite_load(db, &items_sprite, 0x39, 1);
			if (z64_file.button_items[i] == Z64_ITEM_BOW_LIGHT_ARROW)
				sprite_load(db, &items_sprite, 0x3A, 1);
			sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x), (DPAD_Y + icon_y), 12, 12);
			break;
		}
	}
}

void draw_iron_boots_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.iron_boots) {
		sprite_load(db, &items_sprite, 69, 1);
		if (z64_file.equip_boots == 2)
			sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x - 2), (DPAD_Y + icon_y - 2), 16, 16);
		else sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x), (DPAD_Y + icon_y), 12, 12);
	}
}

void draw_hover_boots_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.hover_boots) {
		sprite_load(db, &items_sprite, 70, 1);
		if (z64_file.equip_boots == 3)
			sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x - 2), (DPAD_Y + icon_y - 2), 16, 16);
		else sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x), (DPAD_Y + icon_y), 12, 12);
	}
}

void draw_child_trade_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH) {
		if(alpha==0xFF && !CAN_USE_CHILD_TRADE)
			gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
		else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
		sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_CHILD_TRADE], 1);
		sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x), (DPAD_Y + icon_y), 12, 12);
	}
}

void draw_ocarina_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y) {
	if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) {
		if (alpha==0xFF && !CAN_USE_OCARINA)
			gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
		else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
		sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_OCARINA], 1);
		sprite_draw(db, &items_sprite, 0, (DPAD_X + icon_x), (DPAD_Y + icon_y), 12,12);
	}
}