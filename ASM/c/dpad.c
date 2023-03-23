#include "gfx.h"
#include "text.h"
#include "dpad.h"
#include "dpad_paused.h"
#include "dpad_actions.h"
#include "buttons.h"
#include "fps.h"

extern uint8_t CFG_WS;
extern uint8_t CFG_TYCOON_WALLET;
extern uint8_t CFG_OPTIONS_MENU;

uint8_t  dpad_alt				= 0;
uint16_t dpad_x					= 0;
uint16_t dpad_y					= 0;
uint16_t last_mask				= 0;
uint8_t  options_menu			= 0;
uint8_t  options_menu_frames	= 0;
uint8_t  moved_x_axis_option	= 0;

char options[OPTIONS_SIZE_ALL][OPTIONS_LENGTH]	= { "30 FPS",  "D-Pad Config", "D-Pad Layout", "Hide HUD", "HUD Layout", "Inverse Aim", "No Idle Camera", "Keep Mask", "Tri-Swipe", "Unequip Item", "Unequip Gear", "Item on B", "Downgrade Item", "Crouch Stab Fix", "Weaker Swords", "Extra Abilities", "Rupee Drain", "Fog", "Inventory Editor", "Levitation", "Infinite Health", "Infinite Magic", "Infinite Rupees", "Infinite Ammo" };
uint8_t options_max[OPTIONS_SIZE_ALL]			= { 0,         2,              3,              4,          5,            0,             0,                0,           0,           0,              0,              0,           0,                0,                 0,               0,                 15,            15,    0,                  0,             0,                0,                0,                 0               };
uint8_t options_cursor							= 0;

extern uint8_t CHECKED_LENS;

void handle_dpad() {
	if (z64_file.game_mode != 0)
		return;
	
	handle_l_button();
	
	if (CFG_OPTIONS_MENU >= 1) {
		handle_dpad_ingame();
		handle_dpad_paused();
		handle_options_menu();
		handle_layout();
		handle_hud();
		handle_fps();
		
		if (SAVE_KEEP_MASK) {
			if (z64_change_scene == 0x20000001)
				last_mask = z64_mask_equipped;
			if (z64_change_scene == 0x20000000 && last_mask > 0)
				z64_mask_equipped = last_mask;
		}
		
		if (SAVE_TRISWIPE)
			if (z64_triswipe == 255)
				z64_triswipe = 1;
	}

	if (CFG_OPTIONS_MENU >= 2)  {
		set_b_button();
		handle_abilities_tunic_colors();
		handle_downgrading();
	}
	
	if (CFG_OPTIONS_MENU >= 3) {
		handle_infinite();		
		
		if (SAVE_FOG > 0) {
			if (SAVE_FOG < 15)
				z64_game.fog_distance = 0.65f * SAVE_FOG;
			else z64_game.fog_distance = 15.0f;
			if (z64_camera_view == 1 && z64_game.fog_distance < 5.0f)
				z64_game.fog_distance = 5.0f;
			if (z64_camera_view == 2 && z64_game.fog_distance < 10.0f)
				z64_game.fog_distance = 10.0f;
		}
	}
	
	if (CAN_CONTROL_LINK) {
		if (CFG_OPTIONS_MENU >= 1) {
			if (SAVE_INVERSE_AIM)
				if (z64_camera_view == 1 || z64_camera_view == 2)
					z64_y_axis_input *= -1;
		
			if (SAVE_NO_IDLE_CAMERA)
				if (z64_idle_camera_counter < 10)
					z64_idle_camera_counter = 10;
		}
		
		if (CFG_OPTIONS_MENU >= 2) {
			handle_rupee_dash();
			handle_power_crouch_stab_fix();
			handle_weaker_swords();
			handle_abilities();
		}
		
		if (CFG_OPTIONS_MENU >= 3)
			if (SAVE_LEVITATION)
				if (z64_game.common.input[0].raw.pad.l)
					z64_link_a_action = 0x40CB;
		
		if (CFG_TYCOON_WALLET && z64_file.gs_tokens >= 40 && z64_file.wallet == 2 && TYCOON_WALLET)
			z64_file.wallet = 3;
		
		if (!DPAD_INIT_SETUP) {
			EXTRA_SRAM_1 |= 1;      // Init
			EXTRA_SRAM_2 |= 1 << 6; // Keep Mask
			EXTRA_SRAM_4 |= 2;      // D-Pad
			EXTRA_SRAM_4 |= 1 << 2; // Show D-Pad
	
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
	if (z64_game.pause_ctxt.state != 6 || z64_game.pause_ctxt.unk_02_[1] != 3)
		return;
	
	pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	uint8_t size;
	if (CFG_OPTIONS_MENU == 1)
		size = OPTIONS_SIZE_CORE;
	else if (CFG_OPTIONS_MENU == 2)
		size = OPTIONS_SIZE_MAIN;
	else size = OPTIONS_SIZE_ALL;
	
	if (moved_x_axis_option)
		if (z64_x_axis_input > -10 && z64_x_axis_input < 10)
			moved_x_axis_option = 0;
	
	if (pad_pressed.dl || (z64_x_axis_input < -50 && !moved_x_axis_option) ) {
		if (options_cursor == 0)
			options_cursor = size - 1;
		else options_cursor--;
		z64_playsfx(0x4839, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		
		if (z64_x_axis_input < -50)
			moved_x_axis_option = 1;
	}
	else if (pad_pressed.dr || (z64_x_axis_input > 50 && !moved_x_axis_option) ) {
		if (options_cursor == size - 1)
			options_cursor = 0;
		else options_cursor++;
		z64_playsfx(0x4839, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		
		if (z64_x_axis_input > 50)
			moved_x_axis_option = 1;
	}
	else if (pad_pressed.a) {
		z64_playsfx(0x483B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		
		switch (options_cursor) {
			case OPTION_INVERSE_AIM:		EXTRA_SRAM_1 ^= 1 << 5; break;
			case OPTION_NO_IDLE_CAMERA:		EXTRA_SRAM_1 ^= 1 << 6; break;
			case OPTION_EXTRA_ABILITIES:	EXTRA_SRAM_1 ^= 1 << 7; break;
			case OPTION_UNEQUIP_GEAR:		EXTRA_SRAM_2 ^= 1;      break;
			case OPTION_UNEQUIP_ITEM:		EXTRA_SRAM_2 ^= 1 << 1; break;
			case OPTION_ITEM_ON_B:			EXTRA_SRAM_2 ^= 1 << 2; break;
			case OPTION_WEAKER_SWORDS:		EXTRA_SRAM_2 ^= 1 << 3; break;
			case OPTION_DOWNGRADE_ITEM:		EXTRA_SRAM_2 ^= 1 << 4; break;
			case OPTION_CROUCH_STAB_FIX:	EXTRA_SRAM_2 ^= 1 << 5; break;
			case OPTION_KEEP_MASK:			EXTRA_SRAM_2 ^= 1 << 6; break;
			case OPTION_TRISWIPE:			EXTRA_SRAM_2 ^= 1 << 7; break;
			case OPTION_INFINITE_HP:		EXTRA_SRAM_3 ^= 1 << 7; break;
			case OPTION_INFINITE_MP:		EXTRA_SRAM_4 ^= 1 << 7; break;
			case OPTION_LEVITATION:			EXTRA_SRAM_5 ^= 1 << 4; break;
			case OPTION_INFINITE_RUPEES:	EXTRA_SRAM_5 ^= 1 << 5; break;
			case OPTION_INFINITE_AMMO:		EXTRA_SRAM_5 ^= 1 << 6; break;
			
			case OPTION_30_FPS:
				EXTRA_SRAM_1 ^= 1 << 4;
				if (!SAVE_30_FPS)
					reset_fps_values();
				break;
				
			case OPTION_INVENTORY_EDITOR:
				z64_game.pause_ctxt.unk_02_[1] = 2;
				break;
			
			case OPTION_RUPEE_DRAIN:
				if (SAVE_RUPEE_DRAIN < options_max[options_cursor])
					EXTRA_SRAM_3++;
				else EXTRA_SRAM_3 -= options_max[options_cursor];
				break;
			
			case OPTION_HIDE_HUD:
				if (SAVE_HIDE_HUD < options_max[options_cursor])
					EXTRA_SRAM_3 += 1 << 4;
				else EXTRA_SRAM_3 -= options_max[options_cursor] << 4;
				break;
			
			case OPTION_DPAD:
				if (SAVE_DPAD < options_max[options_cursor])
					EXTRA_SRAM_4++;
				else EXTRA_SRAM_4 -= options_max[options_cursor];
				break;
			
			case OPTION_SHOW_DPAD:
				if (SAVE_SHOW_DPAD < options_max[options_cursor])
					EXTRA_SRAM_4 += 1 << 2;
				else EXTRA_SRAM_4 -= options_max[options_cursor] << 2;
				break;
				
			case OPTION_HUD_LAYOUT:
				if (SAVE_HUD_LAYOUT < options_max[options_cursor])
					EXTRA_SRAM_4 += 1 << 4;
				else EXTRA_SRAM_4 -= options_max[options_cursor] << 4;
				reset_layout();
				break;
				
			case OPTION_FOG:
				if (SAVE_FOG < options_max[options_cursor])
					EXTRA_SRAM_5++;
				else EXTRA_SRAM_5 -= options_max[options_cursor];
				if (SAVE_FOG == 0)
					z64_game.fog_distance = 10.0f;
				break;
		}
	}
	else if (pad_pressed.b) {
		switch (options_cursor) {
			case OPTION_RUPEE_DRAIN:
				if (SAVE_RUPEE_DRAIN > 0)
					EXTRA_SRAM_3--;
				else EXTRA_SRAM_3 += options_max[options_cursor];
				z64_playsfx(0x483B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				break;
			
			case OPTION_HIDE_HUD:
				if (SAVE_HIDE_HUD > 0)
					EXTRA_SRAM_3 -= 1 << 4;
				else EXTRA_SRAM_3 += options_max[options_cursor] << 4;
				z64_playsfx(0x483B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				break;
			
			case OPTION_DPAD:
				if (SAVE_DPAD > 0)
					EXTRA_SRAM_4--;
				else EXTRA_SRAM_4 += options_max[options_cursor];
				z64_playsfx(0x483B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				break;
			
			case OPTION_SHOW_DPAD:
				if (SAVE_SHOW_DPAD > 0)
					EXTRA_SRAM_4 -= 1 << 2;
				else EXTRA_SRAM_4 += options_max[options_cursor] << 2;
				z64_playsfx(0x483B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				break;
				
			case OPTION_HUD_LAYOUT:
				if (SAVE_HUD_LAYOUT > 0)
					EXTRA_SRAM_4 -= 1 << 4;
				else EXTRA_SRAM_4 += options_max[options_cursor] << 4;
				reset_layout();
				z64_playsfx(0x483B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				break;
				
			case OPTION_FOG:
				if (SAVE_FOG > 0)
					EXTRA_SRAM_5--;
				else EXTRA_SRAM_5 += options_max[options_cursor];
				if (SAVE_FOG == 0)
					z64_game.fog_distance = 10.0f;
				z64_playsfx(0x483B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				break;
		}
	}
	
	z64_x_axis_input = z64_y_axis_input = 0;
	z64_game.common.input[0].raw.pad.b = z64_game.common.input[0].pad_pressed.b = 0;
	z64_game.common.input[0].raw.pad.r = z64_game.common.input[0].pad_pressed.r = 0;
	z64_game.common.input[0].raw.pad.z = z64_game.common.input[0].pad_pressed.z = 0;
}

uint8_t draw_settings_menu(z64_disp_buf_t *db) {
	if (z64_game.pause_ctxt.state != 6 || z64_game.pause_ctxt.unk_02_[1] != 3)
		return 0;
	
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
	
	uint8_t x		= 40;
	uint8_t y		= 40;
	uint8_t width	= 80;
	uint8_t height	= 32;
	
	uint8_t left		= (320 / 2) - (144 / 2) + 10;
	uint8_t top			= 115;
	uint8_t setting		= 0;
	int8_t recenter 	= 0;
	char str_option[30]	= "";
	
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
		
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
	sprite_load(db, &font_en_sprite, 0, 1);
	sprite_draw(db, &font_en_sprite, 0, 0, 0, 16, 16);
	
	sprite_load(db, &subscreen_sprite, 4,  1);
	sprite_draw(db, &subscreen_sprite, 0, x,               y, width, height);
	sprite_load(db, &subscreen_sprite, 66, 1);
	sprite_draw(db, &subscreen_sprite, 0, x + width,       y, width, height);
	sprite_load(db, &subscreen_sprite, 5,  1);
	sprite_draw(db, &subscreen_sprite, 0, x + (width * 2), y, width, height);
	
	sprite_load(db, &title_sprite, 9, 1);
	sprite_draw(db, &title_sprite, 0, x + width + 16, y + 3, 128, 16);
	
	for (uint8_t i=0; i<3; i++)
		for (uint8_t j=1; j<=4; j++) {
			sprite_load(db, &subscreen_sprite, (i + 3) + (15 * j),  1);
			sprite_draw(db, &subscreen_sprite, 0, x + (width * i), y + (height * j), width, height);
		}
	
	options_menu_frames++;
	uint8_t compare_frames = 40;
	if (fps_limit == 2)
		compare_frames *= 1.5;

	if (options_menu_frames > compare_frames * 2)
		options_menu_frames = 0;
	if (options_menu_frames <= compare_frames)
		 gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xBF, 0x00, 0xFF);
	else gDPSetPrimColor(db->p++, 0, 0, 0x00, 0xFF, 0xFF, 0xFF);
	
	sprite_load(db, &button_sprite, 2, 1);
	sprite_draw(db, &button_sprite, 0, left - 35,  top - 10, 32, 32);
	sprite_load(db, &button_sprite, 4, 1);
	sprite_draw(db, &button_sprite, 0, left + 120, top - 10, 32, 32);
	
	switch (options_cursor) {
		case OPTION_30_FPS:				setting = SAVE_30_FPS;			recenter = 40;	break;
		case OPTION_DPAD:				setting = SAVE_DPAD;			recenter = 15;	break;
		case OPTION_SHOW_DPAD:			setting = SAVE_SHOW_DPAD;		recenter = 15;	break;
		case OPTION_HIDE_HUD:			setting = SAVE_HIDE_HUD;		recenter = 30;	break;
		case OPTION_HUD_LAYOUT:			setting = SAVE_HUD_LAYOUT;		recenter = 22;	break;
		case OPTION_INVERSE_AIM:		setting = SAVE_INVERSE_AIM;		recenter = 17;	break;
		case OPTION_NO_IDLE_CAMERA:		setting = SAVE_NO_IDLE_CAMERA;	recenter = 5;	break;
		case OPTION_KEEP_MASK:			setting = SAVE_KEEP_MASK;		recenter = 25;	break;
		case OPTION_TRISWIPE:			setting = SAVE_TRISWIPE;		recenter = 25;	break;
		case OPTION_UNEQUIP_ITEM:		setting = SAVE_UNEQUIP_ITEM;	recenter = 15;	break;
		case OPTION_UNEQUIP_GEAR:		setting = SAVE_UNEQUIP_GEAR;	recenter = 15;	break;
		case OPTION_ITEM_ON_B:			setting = SAVE_ITEM_ON_B;		recenter = 27;	break;
		case OPTION_DOWNGRADE_ITEM:		setting = SAVE_DOWNGRADE_ITEM;	recenter = 5;	break;
		case OPTION_CROUCH_STAB_FIX:	setting = SAVE_CROUCH_STAB_FIX;	recenter = 0;	break;
		case OPTION_WEAKER_SWORDS:		setting = SAVE_WEAKER_SWORDS;	recenter = 10;	break;
		case OPTION_EXTRA_ABILITIES:	setting = SAVE_EXTRA_ABILITIES; recenter = 0;	break;
		case OPTION_RUPEE_DRAIN:		setting = SAVE_RUPEE_DRAIN;		recenter = 20;	break;
		case OPTION_FOG:				setting = SAVE_FOG;				recenter = 50;	break;
		case OPTION_INVENTORY_EDITOR:	setting = 0;					recenter = -5;	break;
		case OPTION_LEVITATION:			setting = SAVE_LEVITATION;		recenter = 20;	break;
		case OPTION_INFINITE_HP:		setting = SAVE_INFINITE_HP;		recenter = 0;	break;
		case OPTION_INFINITE_MP:		setting = SAVE_INFINITE_MP;		recenter = 5;	break;
		case OPTION_INFINITE_RUPEES:	setting = SAVE_INFINITE_RUPEES;	recenter = 0;	break;
		case OPTION_INFINITE_AMMO:		setting = SAVE_INFINITE_AMMO;	recenter = 10;	break;
		default:						setting = 0;					recenter = 0;	break;
	}
	
	if (setting == 0)
		 gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0x00, 0x00, 0xFF);
	else gDPSetPrimColor(db->p++, 0, 0, 0x00, 0xFF, 0x00, 0xFF);
	
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
	
	text_print(options[options_cursor], left + recenter, top);
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