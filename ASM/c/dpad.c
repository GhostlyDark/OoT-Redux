#include "gfx.h"
#include "text.h"
#include "dpad.h"
#include "dpad_paused.h"
#include "dpad_actions.h"
#include "buttons.h"
#include "fps.h"

extern uint8_t CFG_WS;
extern uint8_t CFG_TYCOON_WALLET;

uint8_t  dpad_alt			= 0;
uint16_t dpad_x				= 0;
uint16_t dpad_y				= 0;
uint16_t last_mask			= 0;

char options[OPTIONS_SIZE][OPTIONS_LENGTH] = { "30 FPS", "Rupee Drain", "Hide HUD", "Layout", "D-Pad", "Show D-Pad", "Inverse Aim", "No Idle Camera", "Extra Abilities", "Unequip Gear", "Unequip Item", "Item on B",  "Weaker Swords", "Downgrade Item", "Crouch Stab Fix", "Keep Mask", "Tri-Swipe", "Inventory Editor", "Levitation", "Max HP", "Max MP", "Max Rupees", "Max Ammo" };
uint8_t options_cursor		               = 0;
uint8_t options_max[OPTIONS_SIZE]          = { 0, 10, 4, 5, 2, 2 };

extern uint8_t CHECKED_LENS;

void handle_dpad() {
	handle_options_menu();
	handle_layout();
	handle_hud();
	set_b_button();
	handle_dpad_ingame();
	handle_dpad_paused();
	handle_fps();
	handle_l_button();
	handle_infinite();
	handle_abilities_tunic_colors();
	handle_downgrading();
	
	if (CAN_CONTROL_LINK && z64_file.game_mode == 0) {
		handle_rupee_dash();
		handle_power_crouch_stab_fix();
		handle_weaker_swords();
		handle_abilities();
		
		if (SAVE_INVERSE_AIM)
			if (z64_camera_view == 1 || z64_camera_view == 2)
				z64_y_axis_input *= -1;
		
		if (SAVE_NO_IDLE_CAMERA)
			if (z64_idle_camera_counter < 10)
				z64_idle_camera_counter = 10;
			
		if (SAVE_LEVITATION)
			if (z64_game.common.input[0].raw.pad.l)
				z64_link_a_action = 0x40CB;
		
		if (CFG_TYCOON_WALLET && z64_file.gs_tokens >= 40 && z64_file.wallet == 2 && TYCOON_WALLET)
			z64_file.wallet = 3;
		
		if (!DPAD_INIT_SETUP) {
			EXTRA_SRAM_1 |= 1;
			EXTRA_SRAM_2 |= 0x40;
			EXTRA_SRAM_4 |= 6;
	
			DPAD_ADULT_UP		= DPAD_ARROWS		* 16 + DPAD_SWORD;
			DPAD_ADULT_RIGHT	= DPAD_HOVER_BOOTS  * 16 + DPAD_BOOTS;
			DPAD_ADULT_DOWN		= DPAD_OCARINA		* 16 + DPAD_SHIELD;
			DPAD_ADULT_LEFT		= DPAD_IRON_BOOTS	* 16 + DPAD_TUNIC;
			DPAD_CHILD_UP		= DPAD_LENS			* 16 + DPAD_SWORD;
			DPAD_CHILD_RIGHT	= DPAD_CHILD_TRADE	* 16 + DPAD_BOOTS;
			DPAD_CHILD_DOWN		= DPAD_OCARINA		* 16 + DPAD_SHIELD;
			DPAD_CHILD_LEFT		= DPAD_NULL			* 16 + DPAD_TUNIC;
		}
	}
	
	if (SAVE_KEEP_MASK) {
		if (z64_change_scene == 0x20000001)
			last_mask = z64_mask_equipped;
		if (z64_change_scene == 0x20000000 && last_mask > 0)
			z64_mask_equipped = last_mask;
	}
	
	if (SAVE_TRISWIPE && z64_triswipe == 255)
		z64_triswipe = 1;
	
	if (z64_game.pause_ctxt.state == 6 && z64_game.common.input[0].pad_pressed.s && z64_game.pause_ctxt.unk_02_[1])
		z64_game.pause_ctxt.unk_02_[1] = 0;
}

void handle_dpad_ingame() {
	if (!CAN_USE_DPAD || z64_camera_view != 0 || SAVE_DPAD == 0) 
		return;
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (SAVE_DPAD == 2) {
		if ( (z64_game.common.input[0].raw.pad.l && pad_pressed.r) || (z64_game.common.input[0].raw.pad.r && pad_pressed.l) ) {
			dpad_alt ^= 1;
			CHECKED_LENS = 0;
			if (dpad_alt)
				z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			else z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		}
	}
	
	if (z64_game.pause_ctxt.state == 0 && SAVE_DPAD > 0) 
		run_dpad_actions(pad_pressed);
}

void draw_dpad() {
	z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
	gSPDisplayList(db->p++, &setup_db);
	gDPPipeSync(db->p++);
	
	if (!draw_settings_menu(db))
		if (!draw_abilities_info(db))
			draw_dpad_icons(db);

}

void draw_dpad_icons(z64_disp_buf_t *db) {
	if (SAVE_DPAD == 0 || SAVE_SHOW_DPAD == 0 || (z64_game.pause_ctxt.state > 0x7 && z64_game.pause_ctxt.state < 0x12) )
		return;
	
	uint8_t *dpad_active = check_dpad_actions();
	if (z64_game.pause_ctxt.state == 0)
		if (!dpad_active[0] && !dpad_active[1] && !dpad_active[2] && !dpad_active[3])
			return;
	
	if (SAVE_SHOW_DPAD == 1) {
		dpad_x = 21;
		dpad_y = 44;
		if (z64_file.magic_acquired && z64_file.energy_capacity > 0xA0)
			dpad_y += 23;
		else if (z64_file.energy_capacity > 0xA0)
			dpad_y += 10;
		else if (z64_file.magic_acquired)
			dpad_y += 14;
		if (z64_file.timer_1_state > 0 || z64_file.timer_2_state > 0)
			dpad_y += 18;
	}
	else if (SAVE_SHOW_DPAD == 2) {
		dpad_x = 271;
		dpad_y = 64;
		if (SAVE_HUD_LAYOUT == 2 || SAVE_HUD_LAYOUT == 3) {
			dpad_x += 10;
			dpad_y += 14;
		}
		else if (SAVE_HUD_LAYOUT == 4 || SAVE_HUD_LAYOUT == 5)
			dpad_y += 15;
		if (CFG_WS)
			dpad_x += 104;
	}
	else if (SAVE_SHOW_DPAD == 3) {
		dpad_x = 35;
		dpad_y = 175;
		if (z64_dungeon_scene != 0xFF)
			if (z64_file.dungeon_keys[z64_dungeon_scene] > 0)
				dpad_y = 158;
	}
	
	uint16_t alpha = z64_game.hud_alpha_channels.minimap;
	
	if (z64_game.pause_ctxt.state != 0)
		alpha = 0xFF;
	if (alpha == 0xAA)
		alpha = 0xFF;
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
	sprite_load(db, &dpad_sprite, 0, 1);
	sprite_draw(db, &dpad_sprite, 0, dpad_x, dpad_y, 16, 16);
	
	draw_dpad_actions(db, alpha);
}

void handle_options_menu() {
	if (z64_game.pause_ctxt.unk_02_[1] != 3)
		return;
	
	pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (pad_pressed.du && options_cursor > 0) {
		options_cursor--;
		z64_playsfx(0x4839, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
	else if (pad_pressed.dd && options_cursor < OPTIONS_SIZE - 1) {
		options_cursor++;
		z64_playsfx(0x4839, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
	if (pad_pressed.dl && options_cursor >= OPTIONS_ROWS) {
		options_cursor -= OPTIONS_ROWS;
		z64_playsfx(0x4839, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
	else if (pad_pressed.dr && options_cursor < OPTIONS_ROWS) {
		options_cursor += OPTIONS_ROWS;
		z64_playsfx(0x4839, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
	else if (pad_pressed.a) {
		z64_playsfx(0x483B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		
		switch (options_cursor) {
			case OPTION_30_FPS:				EXTRA_SRAM_1 ^= 0x10; break;
			case OPTION_INVERSE_AIM:		EXTRA_SRAM_1 ^= 0x20; break;
			case OPTION_NO_IDLE_CAMERA:		EXTRA_SRAM_1 ^= 0x40; break;
			case OPTION_EXTRA_ABILITIES:	EXTRA_SRAM_1 ^= 0x80; break;
			case OPTION_UNEQUIP_GEAR:		EXTRA_SRAM_2 ^= 0x01; break;
			case OPTION_UNEQUIP_ITEM:		EXTRA_SRAM_2 ^= 0x02; break;
			case OPTION_ITEM_ON_B:			EXTRA_SRAM_2 ^= 0x04; break;
			case OPTION_WEAKER_SWORDS:		EXTRA_SRAM_2 ^= 0x08; break;
			case OPTION_DOWNGRADE_ITEM:		EXTRA_SRAM_2 ^= 0x10; break;
			case OPTION_CROUCH_STAB_FIX:	EXTRA_SRAM_2 ^= 0x20; break;
			case OPTION_KEEP_MASK:			EXTRA_SRAM_2 ^= 0x40; break;
			case OPTION_TRISWIPE:			EXTRA_SRAM_2 ^= 0x80; break;
			case OPTION_LEVITATION:			EXTRA_SRAM_5 ^= 0x01; break;
			case OPTION_INFINITE_HP:		EXTRA_SRAM_5 ^= 0x02; break;
			case OPTION_INFINITE_MP:		EXTRA_SRAM_5 ^= 0x04; break;
			case OPTION_INFINITE_RUPEES:	EXTRA_SRAM_5 ^= 0x08; break;
			case OPTION_INFINITE_AMMO:		EXTRA_SRAM_5 ^= 0x10; break;
			
			case OPTION_RUPEE_DRAIN:
				if (SAVE_RUPEE_DRAIN < 10)
					EXTRA_SRAM_3++;
				else EXTRA_SRAM_3 -= 10;
				break;
			
			case OPTION_HIDE_HUD:
				if (SAVE_HIDE_HUD < 4)
					EXTRA_SRAM_3 += 0x10;
				else EXTRA_SRAM_3 -= 0x40;
				break;
			
			case OPTION_DPAD:
				if (SAVE_DPAD < 2)
					EXTRA_SRAM_4++;
				else EXTRA_SRAM_4 -= 2;
				break;
			
			case OPTION_SHOW_DPAD:
				if (SAVE_SHOW_DPAD < 2)
					EXTRA_SRAM_4 += 4;
				else EXTRA_SRAM_4 -= 8;
				break;
				
			case OPTION_HUD_LAYOUT:
				if (SAVE_HUD_LAYOUT < 5)
					EXTRA_SRAM_4 += 0x10;
				else EXTRA_SRAM_4 -= 0x50;
				break;
		}
		
		if (options_cursor == OPTION_HUD_LAYOUT)
			reset_layout();
		else if (options_cursor == OPTION_INVENTORY_EDITOR)
			z64_game.pause_ctxt.unk_02_[1] = 2;
	}
		
	
	if (options_cursor >= OPTIONS_SIZE)
		options_cursor =  OPTIONS_SIZE - 1;
}

uint8_t draw_settings_menu(z64_disp_buf_t *db) {
	if (z64_game.pause_ctxt.state != 6 || z64_game.pause_ctxt.unk_02_[1] != 3)
		return 0;
	
	uint16_t bg_left   = 15;
	uint16_t bg_top    = 5;
	uint16_t bg_width  = 290;
	uint16_t bg_height = 227;
	
	uint16_t cursor_left, cursor_top;
	if (options_cursor < OPTIONS_ROWS) {
		cursor_left   = 0;
		cursor_top    = options_cursor * 17 + 11;
	}
	else {
		cursor_left   = 175;
		cursor_top    = (options_cursor - OPTIONS_ROWS) * 17 + 11;
	}
	uint16_t cursor_width  = 10;
	uint16_t cursor_height = 10;
	
	uint16_t left, top;
	
	gDPSetCombineMode(db->p++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
	
	// Background
	gDPSetPrimColor(db->p++, 0, 0, 0x00, 0x00, 0x00, 0xD0);
	gSPTextureRectangle(db->p++,
		 bg_left<<2,              bg_top<<2,
		(bg_left + bg_width)<<2, (bg_top + bg_height)<<2,
		0,
		0, 0,
		1<<10, 1<<10);
	
	// Cursor
	gDPSetPrimColor(db->p++, 0, 0, 0x00, 0x00, 0xFF, 0xFF);
	gSPTextureRectangle(db->p++,
		 cursor_left<<2,                  cursor_top<<2,
		(cursor_left + cursor_width)<<2, (cursor_top + cursor_height)<<2,
		0,
		0, 0,
		1<<10, 1<<10);
	
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	
	for (uint8_t on=0; on<=1; on++) {
		uint8_t draw = 0;
		for (uint8_t i=0; i<OPTIONS_SIZE; i++) {
			uint8_t setting;
			switch (i) {
				case OPTION_30_FPS:				setting = SAVE_30_FPS;			break;
				case OPTION_INVERSE_AIM:		setting = SAVE_INVERSE_AIM;		break;
				case OPTION_NO_IDLE_CAMERA:		setting = SAVE_NO_IDLE_CAMERA;	break;
				case OPTION_EXTRA_ABILITIES:	setting = SAVE_EXTRA_ABILITIES; break;
				case OPTION_UNEQUIP_GEAR:		setting = SAVE_UNEQUIP_GEAR;	break;
				case OPTION_UNEQUIP_ITEM:		setting = SAVE_UNEQUIP_ITEM;	break;
				case OPTION_ITEM_ON_B:			setting = SAVE_ITEM_ON_B;		break;
				case OPTION_WEAKER_SWORDS:		setting = SAVE_WEAKER_SWORDS;	break;
				case OPTION_DOWNGRADE_ITEM:		setting = SAVE_DOWNGRADE_ITEM;	break;
				case OPTION_CROUCH_STAB_FIX:	setting = SAVE_CROUCH_STAB_FIX;	break;
				case OPTION_KEEP_MASK:			setting = SAVE_KEEP_MASK;		break;
				case OPTION_TRISWIPE:			setting = SAVE_TRISWIPE;		break;
				case OPTION_LEVITATION:			setting = SAVE_LEVITATION;		break;
				case OPTION_INFINITE_HP:		setting = SAVE_INFINITE_HP;		break;
				case OPTION_INFINITE_MP:		setting = SAVE_INFINITE_MP;		break;
				case OPTION_INFINITE_RUPEES:	setting = SAVE_INFINITE_RUPEES;	break;
				case OPTION_INFINITE_AMMO:		setting = SAVE_INFINITE_AMMO;	break;
				case OPTION_RUPEE_DRAIN:		setting = SAVE_RUPEE_DRAIN;		break;
				case OPTION_HIDE_HUD:			setting = SAVE_HIDE_HUD;		break;
				case OPTION_HUD_LAYOUT:			setting = SAVE_HUD_LAYOUT;		break;
				case OPTION_DPAD:				setting = SAVE_DPAD;			break;
				case OPTION_SHOW_DPAD:			setting = SAVE_SHOW_DPAD;		break;
			}
			
			if (i < OPTIONS_ROWS) {
				left = 20;
				top  = 10 + i * 17;
			}
			else {
				left = 190;
				top  = 10 + (i - OPTIONS_ROWS) * 17;
			}
			
			if (setting == 0 && !on) {
				text_print(options[i], left, top);
				draw = 1;
			}
			else if (setting > 0 && on) {
				text_print(options[i], left, top);
				if (options_max[i] > 1) {
					uint8_t addLeft = 100;
					switch (setting) {
						case 1:  text_print("1",  left + addLeft, top); break;
						case 2:  text_print("2",  left + addLeft, top); break;
						case 3:  text_print("3",  left + addLeft, top); break;
						case 4:  text_print("4",  left + addLeft, top); break;
						case 5:  text_print("5",  left + addLeft, top); break;
						case 6:  text_print("6",  left + addLeft, top); break;
						case 7:  text_print("7",  left + addLeft, top); break;
						case 8:  text_print("8",  left + addLeft, top); break;
						case 9:  text_print("9",  left + addLeft, top); break;
						case 10: text_print("10", left + addLeft, top); break;
					}	
				}
				draw = 1;
			}
		}
		
		if (draw) {
			if (on == 0)
				gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0x00, 0x00, 0xFF);
			else gDPSetPrimColor(db->p++, 0, 0, 0x00, 0xFF, 0x00, 0xFF);
			text_flush(db);
		}
	}
	
	gDPFullSync(db->p++);
	gSPEndDisplayList(db->p++);
		
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