#include "gfx.h"
#include "text.h"
#include "options.h"
#include "buttons.h"
#include "fps.h"

extern uint8_t CFG_WS;
extern uint8_t CFG_OPTIONS_MENU;

uint8_t options_menu		= 0;
uint8_t moved_axis_option	= 0;
uint8_t options_frames		= 0;
uint8_t holding_stick		= 0;

char options[OPTIONS_SIZE_ALL][OPTIONS_LENGTH]	= { "30 FPS",  "D-Pad Config", "D-Pad Layout", "Hide HUD", "HUD Layout", "Inverse Aim", "No Idle Camera", "Keep Mask", "Tri-Swipe", "Unequip Item", "Unequip Gear", "Item on B", "Downgrade Item", "Crouch Stab Fix", "Weaker Swords", "Extra Abilities", "Rupee Drain", "Fog", "Inventory Editor", "Levitation", "Infinite Health", "Infinite Magic", "Infinite Rupees", "Infinite Ammo" };
uint8_t options_max[OPTIONS_SIZE_ALL]			= { 0,         2,              3,              4,          5,            0,             0,                0,           0,           0,              0,              0,           0,                0,                 0,               0,                 15,            15,    0,                  0,             0,                0,                0,                 0               };
int8_t  options_recenter[OPTIONS_SIZE_ALL]		= { 40,        15,             15,             30,         22,           17,            5,                25,          25,          15,             15,             27,          5,                0,                 10,              0,                 20,            50,    -5,                 20,            0,                5,                0,                 10              };
uint8_t options_cursor							= 0;

char tooltips1[OPTIONS_SIZE_ALL][30] = {
	"Allows 30 FPS, press L and",
	"0. Disabled",
	"0. Hidden",
	"Choose from four different",
	"Choose from five different",
	"Inverse y-axis for analog",
	"The camera no longer moves",
	"Mask remains equipped upon",
	"Replaces the transition",
	"Unassign items using the",
	"Unassign equipment by",
	"Assign items to the B",
	"Change between previously",
	"The Crouch Stab move no",
	"Sword slashes now deal one",
	"Obtain Spiritual Stones",
	"First drains rupees, then",
	"Adjust the level of fog",
	"Open the Inventory Editor",
	"Hold the L button to",
	"Your Health is kept at",
	"Your Magic is kept at",
	"Your Rupees are kept at",
	"Your Ammo is kept at"
};

char tooltips2[OPTIONS_SIZE_ALL][30] = {
	"Z to toggle between 30 FPS",
	"1. Enabled",
	"1. Left",
	"options to hide the HUD",
	"layouts to change the HUD",
	"controls when aiming in",
	"behind Link during idle",
	"entering another area",
	"effect with a twirling",
	"respective C button",
	"pressing A",
	"button by pressing A",
	"obtained items by pressing",
	"longer keeps the last",
	"less point of damage",
	"and Medallions to earn new",
	"health based on set",
	"applied, with higher",
	"subscreen and adjust all",
	"levitate",
	"current maximum limit",
	"current maximum limit",
	"current maximum limit",
	"current maximum limit"
};

char tooltips3[OPTIONS_SIZE_ALL][30] = {
	"and 20 FPS mode",
	"2. Dual Set",
	"2. Right",
	"",
	"",
	"first-person view",
	"stance",
	"",
	"Triforce animation",
	"",
	"",
	"",
	"the C-Up button",
	"dealt damage",
	"",
	"abilities",
	"seconds",
	"values increasing the fog",
	"of your items",
	"",
	"",
	"",
	"",
	""
};

void toggle_options_menu() {
	if (z64_game.pause_ctxt.state != 6 || !z64_game.common.input[0].pad_pressed.l)
		return;
	
	uint16_t sfx = 0x4813;
	if (z64_game.pause_ctxt.unk_02_[1] == 0)
		z64_game.pause_ctxt.unk_02_[1] = 3;
	else {
		z64_game.pause_ctxt.unk_02_[1] = 0;
		sfx = 0x4814;
	}
	z64_playsfx(sfx, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}

void handle_options_menu() {
	if (z64_game.pause_ctxt.state != 6 || z64_game.pause_ctxt.unk_02_[1] != 3)
		return;
	
	pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	uint8_t size;
	if (CFG_OPTIONS_MENU == 1)
		size = OPTIONS_SIZE_CORE;
	else if (CFG_OPTIONS_MENU == 2)
		size = OPTIONS_SIZE_MAIN;
	else size = OPTIONS_SIZE_ALL;
	
	if (moved_axis_option)
		if (z64_x_axis_input > -10 && z64_x_axis_input < 10)
			moved_axis_option = 0;
	
	if (pad_pressed.dl || (z64_x_axis_input < -50 && !moved_axis_option) ) {
		if (options_cursor == 0)
			options_cursor = size - 1;
		else options_cursor--;
		z64_playsfx(0x4839, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		
		if (z64_x_axis_input < -50)
			moved_axis_option = 1;
	}
	else if (pad_pressed.dr || (z64_x_axis_input > 50 && !moved_axis_option) ) {
		if (options_cursor == size - 1)
			options_cursor = 0;
		else options_cursor++;
		z64_playsfx(0x4839, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		
		if (z64_x_axis_input > 50)
			moved_axis_option = 1;
	}
	else if (pad_pressed.a  || pad_pressed.b) {
		z64_playsfx(0x483B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		handle_options_menu_input(pad_pressed);
	}
	
	z64_x_axis_input = z64_y_axis_input = 0;
	z64_game.common.input[0].raw.pad.b = z64_game.common.input[0].pad_pressed.b = 0;
	z64_game.common.input[0].raw.pad.r = z64_game.common.input[0].pad_pressed.r = 0;
	z64_game.common.input[0].raw.pad.z = z64_game.common.input[0].pad_pressed.z = 0;
}

void handle_options_menu_input(pad_t pad_pressed) {
	switch (options_cursor) {
		case OPTION_INVERSE_AIM:		EXTRA_SRAM_1 ^= 1 << 5; return;
		case OPTION_NO_IDLE_CAMERA:		EXTRA_SRAM_1 ^= 1 << 6; return;
		case OPTION_EXTRA_ABILITIES:	EXTRA_SRAM_1 ^= 1 << 7; return;
		case OPTION_UNEQUIP_GEAR:		EXTRA_SRAM_2 ^= 1;      return;
		case OPTION_UNEQUIP_ITEM:		EXTRA_SRAM_2 ^= 1 << 1; return;
		case OPTION_ITEM_ON_B:			EXTRA_SRAM_2 ^= 1 << 2; return;
		case OPTION_WEAKER_SWORDS:		EXTRA_SRAM_2 ^= 1 << 3; return;
		case OPTION_DOWNGRADE_ITEM:		EXTRA_SRAM_2 ^= 1 << 4; return;
		case OPTION_CROUCH_STAB_FIX:	EXTRA_SRAM_2 ^= 1 << 5; return;
		case OPTION_KEEP_MASK:			EXTRA_SRAM_2 ^= 1 << 6; return;
		case OPTION_TRISWIPE:			EXTRA_SRAM_2 ^= 1 << 7; return;
		case OPTION_INFINITE_HP:		EXTRA_SRAM_3 ^= 1 << 7; return;
		case OPTION_INFINITE_MP:		EXTRA_SRAM_4 ^= 1 << 7; return;
		case OPTION_LEVITATION:			EXTRA_SRAM_5 ^= 1 << 4; return;
		case OPTION_INFINITE_RUPEES:	EXTRA_SRAM_5 ^= 1 << 5; return;
		case OPTION_INFINITE_AMMO:		EXTRA_SRAM_5 ^= 1 << 6; return;
		
		case OPTION_30_FPS:
			EXTRA_SRAM_1 ^= 1 << 4;
			if (!SAVE_30_FPS)
				reset_fps_values();
			return;
			
		case OPTION_INVENTORY_EDITOR:
			z64_game.pause_ctxt.unk_02_[1] = 2;
			z64_game.common.input[0].raw.pad.a = z64_game.common.input[0].pad_pressed.a = 0;
			z64_game.common.input[0].raw.pad.b = z64_game.common.input[0].pad_pressed.b = 0;
			return;
	}
	
	if (pad_pressed.a) {
		switch (options_cursor) {
			case OPTION_RUPEE_DRAIN:
				if (SAVE_RUPEE_DRAIN < options_max[options_cursor])
					EXTRA_SRAM_3++;
				else EXTRA_SRAM_3 -= options_max[options_cursor];
				return;
			
			case OPTION_HIDE_HUD:
				if (SAVE_HIDE_HUD < options_max[options_cursor])
					EXTRA_SRAM_3 += 1 << 4;
				else EXTRA_SRAM_3 -= options_max[options_cursor] << 4;
				return;
			
			case OPTION_DPAD:
				if (SAVE_DPAD < options_max[options_cursor])
					EXTRA_SRAM_4++;
				else EXTRA_SRAM_4 -= options_max[options_cursor];
				return;
			
			case OPTION_SHOW_DPAD:
				if (SAVE_SHOW_DPAD < options_max[options_cursor])
					EXTRA_SRAM_4 += 1 << 2;
				else EXTRA_SRAM_4 -= options_max[options_cursor] << 2;
				return;
			
			case OPTION_HUD_LAYOUT:
				if (SAVE_HUD_LAYOUT < options_max[options_cursor])
					EXTRA_SRAM_4 += 1 << 4;
				else EXTRA_SRAM_4 -= options_max[options_cursor] << 4;
				reset_layout();
				return;
			
			case OPTION_FOG:
				if (SAVE_FOG < options_max[options_cursor])
					EXTRA_SRAM_5++;
				else EXTRA_SRAM_5 -= options_max[options_cursor];
				if (SAVE_FOG == 0)
					z64_game.fog_distance = 10.0f;
				return;
		}
	}
	else {
		switch (options_cursor) {
			case OPTION_RUPEE_DRAIN:
				if (SAVE_RUPEE_DRAIN > 0)
					EXTRA_SRAM_3--;
				else EXTRA_SRAM_3 += options_max[options_cursor];
				return;
			
			case OPTION_HIDE_HUD:
				if (SAVE_HIDE_HUD > 0)
					EXTRA_SRAM_3 -= 1 << 4;
				else EXTRA_SRAM_3 += options_max[options_cursor] << 4;
				return;
			
			case OPTION_DPAD:
				if (SAVE_DPAD > 0)
					EXTRA_SRAM_4--;
				else EXTRA_SRAM_4 += options_max[options_cursor];
				return;
			
			case OPTION_SHOW_DPAD:
				if (SAVE_SHOW_DPAD > 0)
					EXTRA_SRAM_4 -= 1 << 2;
				else EXTRA_SRAM_4 += options_max[options_cursor] << 2;
				return;
			
			case OPTION_HUD_LAYOUT:
				if (SAVE_HUD_LAYOUT > 0)
					EXTRA_SRAM_4 -= 1 << 4;
				else EXTRA_SRAM_4 += options_max[options_cursor] << 4;
				reset_layout();
				return;
				
			case OPTION_FOG:
				if (SAVE_FOG > 0)
					EXTRA_SRAM_5--;
				else EXTRA_SRAM_5 += options_max[options_cursor];
				if (SAVE_FOG == 0)
					z64_game.fog_distance = 10.0f;
				return;
		}
	}
}

uint8_t draw_settings_menu(z64_disp_buf_t *db) {
	if (z64_game.pause_ctxt.state != 6 || z64_game.pause_ctxt.unk_02_[1] != 3)
		return 0;
	
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
	
	uint8_t x			= 40;
	uint8_t y			= 40;
	uint8_t width		= 80;
	uint8_t height		= 32;
	uint8_t left		= 98;
	uint8_t top			= 75;
	uint8_t setting		= 0;
	
	if (CFG_WS) {
		x		+= 52;
		left	+= 52;
	}
	
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
		
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0);
	sprite_load(db, &font_en_sprite, 0, 1);
	sprite_draw(db, &font_en_sprite, 0, 0, 0, 16, 16);
	
	gDPSetPrimColor(db->p++, 0, 0, 0xA0, 0xA0, 0xA0, 0xFF);
	sprite_load(db, &subscreen_sprite, 4,  1);
	sprite_draw(db, &subscreen_sprite, 0, x,               y, width, height);
	sprite_load(db, &subscreen_sprite, 66, 1);
	sprite_draw(db, &subscreen_sprite, 0, x + width,       y, width, height);
	sprite_load(db, &subscreen_sprite, 5,  1);
	sprite_draw(db, &subscreen_sprite, 0, x + (width * 2), y, width, height);
	
	for (uint8_t i=0; i<3; i++)
		for (uint8_t j=1; j<=4; j++) {
			sprite_load(db, &subscreen_sprite, (i + 3) + (15 * j),  1);
			sprite_draw(db, &subscreen_sprite, 0, x + (width * i), y + (height * j), width, height);
		}
	
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
	sprite_load(db, &title_sprite, 9, 1);
	sprite_draw(db, &title_sprite, 0, x + width + 16, y + 3, 128, 16);
	
	gDPSetPrimColor(db->p++, 0, 0, 0xD0, 0xD0, 0xD0, 0xFF);
	sprite_load(db, &button_sprite, 2, 1);
	sprite_draw(db, &button_sprite, 0, left - 35,  top - 10, 32, 32);
	sprite_load(db, &button_sprite, 4, 1);
	sprite_draw(db, &button_sprite, 0, left + 120, top - 10, 32, 32);
	
	switch (options_cursor) {
		case OPTION_30_FPS:				setting = SAVE_30_FPS;			break;
		case OPTION_DPAD:				setting = SAVE_DPAD;			break;
		case OPTION_SHOW_DPAD:			setting = SAVE_SHOW_DPAD;		break;
		case OPTION_HIDE_HUD:			setting = SAVE_HIDE_HUD;		break;
		case OPTION_HUD_LAYOUT:			setting = SAVE_HUD_LAYOUT;		break;
		case OPTION_INVERSE_AIM:		setting = SAVE_INVERSE_AIM;		break;
		case OPTION_NO_IDLE_CAMERA:		setting = SAVE_NO_IDLE_CAMERA;	break;
		case OPTION_KEEP_MASK:			setting = SAVE_KEEP_MASK;		break;
		case OPTION_TRISWIPE:			setting = SAVE_TRISWIPE;		break;
		case OPTION_UNEQUIP_ITEM:		setting = SAVE_UNEQUIP_ITEM;	break;
		case OPTION_UNEQUIP_GEAR:		setting = SAVE_UNEQUIP_GEAR;	break;
		case OPTION_ITEM_ON_B:			setting = SAVE_ITEM_ON_B;		break;
		case OPTION_DOWNGRADE_ITEM:		setting = SAVE_DOWNGRADE_ITEM;	break;
		case OPTION_CROUCH_STAB_FIX:	setting = SAVE_CROUCH_STAB_FIX;	break;
		case OPTION_WEAKER_SWORDS:		setting = SAVE_WEAKER_SWORDS;	break;
		case OPTION_EXTRA_ABILITIES:	setting = SAVE_EXTRA_ABILITIES; break;
		case OPTION_RUPEE_DRAIN:		setting = SAVE_RUPEE_DRAIN;		break;
		case OPTION_FOG:				setting = SAVE_FOG;				break;
		case OPTION_LEVITATION:			setting = SAVE_LEVITATION;		break;
		case OPTION_INFINITE_HP:		setting = SAVE_INFINITE_HP;		break;
		case OPTION_INFINITE_MP:		setting = SAVE_INFINITE_MP;		break;
		case OPTION_INFINITE_RUPEES:	setting = SAVE_INFINITE_RUPEES;	break;
		case OPTION_INFINITE_AMMO:		setting = SAVE_INFINITE_AMMO;	break;
	}
	
	if (setting == 0)
		 gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
	else gDPSetPrimColor(db->p++, 0, 0, 0x00, 0xFF, 0x00, 0xFF);
	
	if (options_cursor != OPTION_INVENTORY_EDITOR) {
		if (setting < 10) {
			sprite_load(db, &ammo_digit_sprite, setting, 1);
			sprite_draw(db, &ammo_digit_sprite, 0, left + 56, top + 20, 16, 16);
		}
		else {
			sprite_load(db, &ammo_digit_sprite, 1, 1);
			sprite_draw(db, &ammo_digit_sprite, 0, left + 48, top + 20, 16, 16);
			sprite_load(db, &ammo_digit_sprite, (setting - 10), 1);
			sprite_draw(db, &ammo_digit_sprite, 0, left + 64, top + 20, 16, 16);
		}
	}
	
	text_print(options[options_cursor],  left + options_recenter[options_cursor], top);
	text_flush(db);
	
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
	text_print(tooltips1[options_cursor], left - 45, top + 50);
	text_print(tooltips2[options_cursor], left - 45, top + 70);
	text_print(tooltips3[options_cursor], left - 45, top + 90);
	text_flush(db);
	
	return 1;
}

uint8_t draw_abilities_info(z64_disp_buf_t *db) {
	if (!SAVE_EXTRA_ABILITIES || !IS_PAUSE_SCREEN_CURSOR || z64_game.pause_ctxt.screen_idx != 2 || !z64_game.common.input[0].raw.pad.a)
		return 0;
	
	uint8_t show         = 0;
	char str_item[26]    = "";
	char str_ability[25] = "";
	char str_desc1[30]   = "";
	char str_desc2[30]   = "";
	char str_desc3[30]   = "";
	char str_desc4[30]   = "";
	
	if (z64_game.pause_ctxt.quest_cursor == 5 && z64_file.light_medallion) { // Light Medallion
		strcpy(str_item,    "Item:    Light Medallion");
		strcpy(str_ability, "Ability: Long Jump");
		strcpy(str_desc1,   "Run, hold L and");
		strcpy(str_desc1,   "press A to jump");
		strcpy(str_desc3,   "Requires the Kokiri Boots");
		strcpy(str_desc4,   "for it to be effective");
		show = 1;
	}
	else if (z64_game.pause_ctxt.quest_cursor == 0 && z64_file.forest_medallion) { // Forest Medallion
		strcpy(str_item,    "Item:    Forest Medallion");
		strcpy(str_ability, "Ability: Magician Tunic");
		strcpy(str_desc1,   "Restores some magic when hit");
		strcpy(str_desc2,   "Replaces the Kokiri Tunic and");
		strcpy(str_desc3,   "requires to be equipped");
		show = 1;
	}
	else if (z64_game.pause_ctxt.quest_cursor == 1 && z64_file.fire_medallion) { // Fire Medallion
		strcpy(str_item,    "Item:    Fire Medallion");
		strcpy(str_ability, "Ability: Guardian Tunic");
		strcpy(str_desc1,   "Restores half of the damage");
		strcpy(str_desc2,   "taken at the cost of magic");
		strcpy(str_desc3,   "Replaces the Goron Tunic and");
		strcpy(str_desc4,   "requires to be equipped");
		show = 1;
	}
	else if (z64_game.pause_ctxt.quest_cursor == 2 && z64_file.water_medallion) { // Water Medallion
		strcpy(str_item,    "Item:    Water Medallion");
		strcpy(str_ability, "Ability: Hero Tunic");
		strcpy(str_desc1,   "Increases damage done by");
		strcpy(str_desc2,   "sword slashes by +1");
		strcpy(str_desc3,   "Replaces the Zora Tunic and");
		strcpy(str_desc4,   "requires to be equipped");
		show = 1;
	}
	else if (z64_game.pause_ctxt.quest_cursor == 4 && z64_file.shadow_medallion) { // Shadow Medallion
		strcpy(str_item,    "Item:    Shadow Medallion");
		strcpy(str_ability, "Ability: Shadow Tunic");
		strcpy(str_desc1,   "Restores some health over");
		strcpy(str_desc2,   "time at the cost of magic");
		strcpy(str_desc3,   "Unequip the tunic with C-Up");
		strcpy(str_desc4,   "for it to be effective");
		show = 1;
	}
	else if (z64_game.pause_ctxt.quest_cursor == 3 && z64_file.spirit_medallion) { // Spirit Medallion
		strcpy(str_item,    "Item:    Spirit Medallion");
		strcpy(str_ability, "Ability: Hover Dash Jump");
		strcpy(str_desc1,   "The Jump and Dash abilities");
		strcpy(str_desc1,   "can now be used with the");
		strcpy(str_desc3,   "Hover Boots as well");
		show = 1;
	}
	else if (z64_game.pause_ctxt.quest_cursor == 0x12 && z64_file.kokiris_emerald) { // Kokiri's Emerald
		strcpy(str_item,    "Item:    Kokiri's Emerald");
		strcpy(str_ability, "Ability: Dash");
		strcpy(str_desc1,   "Hold L when running to dash");
		strcpy(str_desc2,   "Consumes magic");
		strcpy(str_desc3,   "Requires the Kokiri Boots");
		strcpy(str_desc4,   "for it to be effective");
		show = 1;
	}
	else if (z64_game.pause_ctxt.quest_cursor == 0x13 && z64_file.gorons_ruby) { // Goron's Ruby
		strcpy(str_item,    "Item:    Goron's Ruby");
		strcpy(str_ability, "Ability: Faster Dash");
		strcpy(str_desc1,   "Upgrades the Dash Ability ");
		strcpy(str_desc2,   "to be faster");
		show = 1;
	}
	else if (z64_game.pause_ctxt.quest_cursor == 0x14 && z64_file.zoras_sapphire) { // Zora's Sapphire
		strcpy(str_item,    "Item:    Zora's Sapphire");
		strcpy(str_ability, "Ability: Cheaper Dash");
		strcpy(str_desc1,   "Lower the magic cost of");
		strcpy(str_desc2,   "the dash ability");
		show = 1;
	}
	
	if (show) {
		int left   = 10;
		int top    = 10;
		int width  = 240;
		int height = 115;
		
		gDPSetCombineMode(db->p++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
		
		gDPSetPrimColor(db->p++, 0, 0, 0x00, 0x00, 0x00, 0xD0);
		gSPTextureRectangle(db->p++,
			left<<2,            top<<2,
			(left + width)<<2, (top + height)<<2,
			0,
			0, 0,
			1<<10, 1<<10);
		
		gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
		
		gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0, 0, 0xFF);
		text_print(str_item,    left + 5, top + 5);
		text_flush(db);
		
		gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0, 0, 0xFF);
		text_print(str_ability, left + 5, top + 25);
		text_flush(db);
		
		gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
		text_print(str_desc1,   left + 5, top + 45);
		text_print(str_desc2,   left + 5, top + 62);
		text_print(str_desc3,   left + 5, top + 79);
		text_print(str_desc4,   left + 5, top + 96);
		text_flush(db);
		
		gDPFullSync(db->p++);
		gSPEndDisplayList(db->p++);
		
		return 1;
	}
	return 0;
}

void handle_inventory_editor() {
	if (z64_game.pause_ctxt.unk_02_[1] != 2 || CFG_OPTIONS_MENU < 3)
		return;
	
	pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (pad_pressed.a || pad_pressed.b) {
		z64_game.pause_ctxt.unk_02_[1] = 3;
		z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
	
	if (z64_x_axis_input > -10 && z64_x_axis_input < 10 && z64_y_axis_input > -10 && z64_y_axis_input < 10)
		moved_axis_option = holding_stick = 0;
	if (moved_axis_option) {
		options_frames++;
		uint8_t compare_frames = 15;
		if (holding_stick)
			compare_frames = 1;
		if (options_frames > compare_frames) {
			options_frames = moved_axis_option = 0;
			holding_stick  = 1;
		}
	}
	else options_frames = 0;
	
	if (!moved_axis_option) {
		if (z64_x_axis_input < -50) {
			moved_axis_option = 1;
			if (z64_inventory_editor_index == 0)
				z64_inventory_editor_index = 0x5C;
			else z64_inventory_editor_index--;
		}
		else if (z64_x_axis_input > 50) {
			moved_axis_option = 1;
			if (z64_inventory_editor_index == 0x5C)
				z64_inventory_editor_index = 0;
			else z64_inventory_editor_index++;
		}
		else if (z64_y_axis_input > 50) {
			moved_axis_option = 1;
			
			if (     z64_inventory_editor_index <= 0x5C && z64_inventory_editor_index > 0x59)
				z64_inventory_editor_index = 0x59;
			else if (z64_inventory_editor_index <= 0x59 && z64_inventory_editor_index > 0x56)
				z64_inventory_editor_index = 0x56;
			else if (z64_inventory_editor_index <= 0x56 && z64_inventory_editor_index > 0x4A)
				z64_inventory_editor_index = 0x4A;
			else if (z64_inventory_editor_index <= 0x4A && z64_inventory_editor_index > 0x44)
				z64_inventory_editor_index = 0x44;
			else if (z64_inventory_editor_index <= 0x44 && z64_inventory_editor_index > 0x38)
				z64_inventory_editor_index = 0x38;
			else if (z64_inventory_editor_index <= 0x38 && z64_inventory_editor_index > 0x34)
				z64_inventory_editor_index = 0x34;
			else if (z64_inventory_editor_index <= 0x34 && z64_inventory_editor_index > 0x2C)
				z64_inventory_editor_index = 0x2C;
			else if (z64_inventory_editor_index <= 0x2C && z64_inventory_editor_index > 0x1B)
				z64_inventory_editor_index = 0x1B;
			else if (z64_inventory_editor_index <= 0x1B && z64_inventory_editor_index > 3)
				z64_inventory_editor_index = 3;
			else if (z64_inventory_editor_index <= 3    && z64_inventory_editor_index > 0)
				z64_inventory_editor_index = 0;
			else if (z64_inventory_editor_index <= 0)
				z64_inventory_editor_index = 0x5C;
		}
		else if (z64_y_axis_input < -50) {
			moved_axis_option = 1;
			
			if (     z64_inventory_editor_index >= 0    && z64_inventory_editor_index < 3)
				z64_inventory_editor_index = 3;
			else if (z64_inventory_editor_index >= 3    && z64_inventory_editor_index < 0x1B)
				z64_inventory_editor_index = 0x1B;
			else if (z64_inventory_editor_index >= 0x1B && z64_inventory_editor_index < 0x2C)
				z64_inventory_editor_index = 0x2C;
			else if (z64_inventory_editor_index >= 0x2C && z64_inventory_editor_index < 0x34)
				z64_inventory_editor_index = 0x34;
			else if (z64_inventory_editor_index >= 0x34 && z64_inventory_editor_index < 0x38)
				z64_inventory_editor_index = 0x38;
			else if (z64_inventory_editor_index >= 0x38 && z64_inventory_editor_index < 0x44)
				z64_inventory_editor_index = 0x44;
			else if (z64_inventory_editor_index >= 0x44 && z64_inventory_editor_index < 0x4A)
				z64_inventory_editor_index = 0x4A;
			else if (z64_inventory_editor_index >= 0x4A && z64_inventory_editor_index < 0x56)
				z64_inventory_editor_index = 0x56;
			else if (z64_inventory_editor_index >= 0x56 && z64_inventory_editor_index < 0x59)
				z64_inventory_editor_index = 0x59;
			else if (z64_inventory_editor_index >= 0x59 && z64_inventory_editor_index < 0x5C)
				z64_inventory_editor_index = 0x5C;
			else if (z64_inventory_editor_index >= 0x5C)
				z64_inventory_editor_index = 0;
		}
	}
	
	z64_x_axis_input = z64_y_axis_input = 0;
	z64_game.common.input[0].raw.pad.b = z64_game.common.input[0].pad_pressed.b = 0;
	z64_game.common.input[0].raw.pad.r = z64_game.common.input[0].pad_pressed.r = 0;
	z64_game.common.input[0].raw.pad.z = z64_game.common.input[0].pad_pressed.z = 0;
	
}