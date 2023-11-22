#include "gfx.h"
#include "dpad.h"
#include "dpad_paused.h"
#include "dpad_actions.h"
#include "options.h"
#include "buttons.h"
#include "fps.h"
#include "actor.h"

extern uint8_t CFG_WS;
extern uint8_t CFG_TYCOON_WALLET;
extern uint8_t CFG_OPTIONS_MENU;

// Default Options
extern uint8_t CFG_DEFAULT_30_FPS;
extern uint8_t CFG_DEFAULT_ARROW_TOGGLE;
extern uint8_t CFG_DEFAULT_DPAD;
extern uint8_t CFG_DEFAULT_SHOW_DPAD;
extern uint8_t CFG_DEFAULT_HIDE_HUD;
extern uint8_t CFG_DEFAULT_HUD_LAYOUT;
extern uint8_t CFG_DEFAULT_SHOW_HEALTH;
extern uint8_t CFG_DEFAULT_CHEST_CONTENTS;
extern uint8_t CFG_DEFAULT_A_BUTTON_SCALE;
extern uint8_t CFG_DEFAULT_B_BUTTON_SCALE;
extern uint8_t CFG_DEFAULT_C_LEFT_BUTTON_SCALE;
extern uint8_t CFG_DEFAULT_C_DOWN_BUTTON_SCALE;
extern uint8_t CFG_DEFAULT_C_RIGHT_BUTTON_SCALE;
extern uint8_t CFG_DEFAULT_INVERSE_AIM;
extern uint8_t CFG_DEFAULT_NO_IDLE_CAMERA;
extern uint8_t CFG_DEFAULT_KEEP_MASK;
extern uint8_t CFG_DEFAULT_TRISWIPE;
extern uint8_t CFG_DEFAULT_DAMAGE_TAKEN;
extern uint8_t CFG_DEFAULT_RANDOM_ENEMIES;
extern uint8_t CFG_DEFAULT_UNEQUIP_ITEM;
extern uint8_t CFG_DEFAULT_UNEQUIP_GEAR;
extern uint8_t CFG_DEFAULT_ITEM_ON_B;
extern uint8_t CFG_DEFAULT_DOWNGRADE_ITEM;
extern uint8_t CFG_DEFAULT_CROUCH_STAB_FIX;
extern uint8_t CFG_DEFAULT_WEAKER_SWORDS;
extern uint8_t CFG_DEFAULT_EXTRA_ABILITIES;
extern uint8_t CFG_DEFAULT_RUPEE_DRAIN;
extern uint8_t CFG_DEFAULT_FOG;
extern uint8_t CFG_DEFAULT_LEVITATION;
extern uint8_t CFG_DEFAULT_INFINITE_HP;
extern uint8_t CFG_DEFAULT_INFINITE_MP;
extern uint8_t CFG_DEFAULT_INFINITE_RUPEES;
extern uint8_t CFG_DEFAULT_INFINITE_AMMO;

uint8_t  dpad_alt        = 0;
uint16_t dpad_x          = 0;
uint16_t dpad_y          = 0;
uint8_t last_mask        = 0;
int8_t  last_mask_age    = -1;
uint16_t original_damage = 0;
uint16_t play_sfx        = 0;
uint8_t compare_frames;

extern uint8_t CHECKED_LENS;

void handle_dpad() {
	if (z64_file.game_mode != 0)
		return;
	
	compare_frames = 60 / fps_limit;
	
	if (play_sfx != 0) {
		z64_playsfx(play_sfx, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		play_sfx = 0;
	}
	
	if (CFG_OPTIONS_MENU >= 1) {
		toggle_options_menu();
		handle_dpad_ingame();
		handle_dpad_paused();
		handle_options_menu();
		handle_button_scaling();
		handle_layout();
		handle_hud();
		handle_fps();
		
		if (SAVE_DOWNGRADE_ITEM)
			if (!z64_file.bgs_flag && z64_file.equip_sword == 3)
				SWORD_HEALTH = z64_file.bgs_hits_left;
		
		if (SAVE_KEEP_MASK) {
			if (z64_file.items[Z64_SLOT_CHILD_TRADE] != last_mask + 0x23)
				last_mask = 0;
			if (last_mask_age != -1 && last_mask_age != z64_file.link_age) {
				last_mask     = 0;
				last_mask_age = -1;
			}
			if (z64_change_scene == 0x20000001)
				last_mask = z64_mask_equipped;
			if (z64_change_scene == 0x20000000 && last_mask > 0) {
				z64_mask_equipped = last_mask;
				last_mask_age     = z64_file.link_age;
			}
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
	
	handle_inventory_editor();
	handle_l_button();
	
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
				if (z64_MessageGetState(((uint8_t *)(&z64_game)) + 0x20D8) != 4 && (z64_game.camera_mode == CAM_MODE_FIRST_PERSON || z64_game.camera_mode == CAM_MODE_AIM_ADULT || z64_game.camera_mode == CAM_MODE_AIM_BOOMERANG || z64_game.camera_mode == CAM_MODE_AIM_CHILD) ) {
					z64_game.common.input[0].raw.y      = -z64_game.common.input[0].raw.y;
					z64_game.common.input[0].y_diff     = -z64_game.common.input[0].y_diff;
					z64_game.common.input[0].adjusted_y = -z64_game.common.input[0].adjusted_y;
				}
		
			if (SAVE_NO_IDLE_CAMERA)
				if (z64_idle_camera_counter < 10)
					z64_idle_camera_counter = 10;
		}
		
		if (CFG_OPTIONS_MENU >= 2) {
			handle_rupee_drain();
			handle_power_crouch_stab_fix();
			handle_weaker_swords();
			handle_abilities();
			
			if (original_damage == 0)
				original_damage = z64_damage_taken_modifier_3;
			else if (SAVE_DAMAGE_TAKEN == 0)
				z64_damage_taken_modifier_3 = original_damage;
			else z64_damage_taken_modifier_3 = 0x2C43 - 0x40 * SAVE_DAMAGE_TAKEN;
		}
		
		if (CFG_OPTIONS_MENU >= 3)
			if (SAVE_LEVITATION)
				if (z64_game.common.input[0].raw.pad.l)
					z64_link_a_action = 0x40CB;
		
		if (CFG_TYCOON_WALLET && z64_file.gs_tokens >= 40 && z64_file.wallet == 2 && TYCOON_WALLET)
			z64_file.wallet = 3;
		
		if (!DPAD_INIT_SETUP)
			run_default_options_setup();
	}
}

void run_default_options_setup() {
	z64_file.inf_table[0x15] |= 1 << 0; // Init
	
	z64_file.inf_table[0x15] |= CFG_DEFAULT_30_FPS               << 4;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_ARROW_TOGGLE         << 5;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_SHOW_HEALTH          << 6;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_CHEST_CONTENTS       << 7;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_INVERSE_AIM          << 8;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_NO_IDLE_CAMERA       << 9;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_KEEP_MASK            << 10;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_TRISWIPE             << 11;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_UNEQUIP_ITEM         << 12;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_UNEQUIP_GEAR         << 13;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_ITEM_ON_B            << 14;
	z64_file.inf_table[0x15] |= CFG_DEFAULT_DOWNGRADE_ITEM       << 15;
	z64_file.inf_table[0x14] |= CFG_DEFAULT_CROUCH_STAB_FIX      << 8;
	z64_file.inf_table[0x14] |= CFG_DEFAULT_WEAKER_SWORDS        << 9;
	z64_file.inf_table[0x14] |= CFG_DEFAULT_EXTRA_ABILITIES      << 10;
	z64_file.inf_table[0x14] |= CFG_DEFAULT_LEVITATION           << 11;
	z64_file.inf_table[0x14] |= CFG_DEFAULT_INFINITE_HP          << 12;
	z64_file.inf_table[0x14] |= CFG_DEFAULT_INFINITE_MP          << 13;
	z64_file.inf_table[0x14] |= CFG_DEFAULT_INFINITE_RUPEES      << 14;
	z64_file.inf_table[0x14] |= CFG_DEFAULT_INFINITE_AMMO        << 15;
	
	z64_file.inf_table[0x1B] |= CFG_DEFAULT_DPAD                 << 0;
	z64_file.inf_table[0x1B] |= CFG_DEFAULT_SHOW_DPAD            << 2;
	z64_file.inf_table[0x1B] |= CFG_DEFAULT_HIDE_HUD             << 4;
	z64_file.inf_table[0x1B] |= CFG_DEFAULT_HUD_LAYOUT           << 7;
	z64_file.inf_table[0x1B] |= CFG_DEFAULT_A_BUTTON_SCALE       << 10;
	z64_file.inf_table[0x1B] |= CFG_DEFAULT_B_BUTTON_SCALE       << 12;
	z64_file.inf_table[0x1B] |= CFG_DEFAULT_RANDOM_ENEMIES       << 15;
	z64_file.inf_table[0x1C] |= CFG_DEFAULT_C_LEFT_BUTTON_SCALE  << 0;
	z64_file.inf_table[0x1C] |= CFG_DEFAULT_C_DOWN_BUTTON_SCALE  << 4;
	z64_file.inf_table[0x1C] |= CFG_DEFAULT_C_RIGHT_BUTTON_SCALE << 8;
	z64_file.inf_table[0x1C] |= CFG_DEFAULT_DAMAGE_TAKEN         << 12;
	z64_file.inf_table[0x18] |= CFG_DEFAULT_RUPEE_DRAIN          << 0;
	z64_file.inf_table[0x18] |= CFG_DEFAULT_FOG                  << 4;
	
	DPAD_ADULT_UP		= DPAD_ARROWS		* 16 + DPAD_SWORD;
	DPAD_ADULT_RIGHT	= DPAD_HOVER_BOOTS  * 16 + DPAD_BOOTS;
	DPAD_ADULT_DOWN		= DPAD_OCARINA		* 16 + DPAD_SHIELD;
	DPAD_ADULT_LEFT		= DPAD_IRON_BOOTS	* 16 + DPAD_TUNIC;
	DPAD_CHILD_UP		= DPAD_LENS			* 16 + DPAD_SWORD;
	DPAD_CHILD_RIGHT	= DPAD_CHILD_TRADE	* 16 + DPAD_BOOTS;
	DPAD_CHILD_DOWN		= DPAD_OCARINA		* 16 + DPAD_SHIELD;
	DPAD_CHILD_LEFT		= DPAD_NULL			* 16 + DPAD_TUNIC;
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
				play_sfx = 0x4813;
			else play_sfx = 0x4814;
		}
	}
	
	if (z64_game.pause_ctxt.state == 0 && SAVE_DPAD > 0) 
		run_dpad_actions(pad_pressed);
}

void draw_dpad() {
	z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
	gSPDisplayList(db->p++, &setup_db);
	gDPPipeSync(db->p++);
	
	draw_health(db);
	if (!draw_settings_menu(db))
		if (!draw_abilities_info(db))
			draw_dpad_icons(db);
}

void draw_dpad_icons(z64_disp_buf_t *db) {
	if (!CAN_DRAW_HUD || SAVE_DPAD == 0 || (SAVE_SHOW_DPAD == 0 && z64_game.pause_ctxt.state != 6) || (z64_game.pause_ctxt.state > 0x7 && z64_game.pause_ctxt.state < 0x12) )
		return;
	
	uint8_t *dpad_active = check_dpad_actions();
	if (z64_game.pause_ctxt.state == 0 && !dpad_active[0] && !dpad_active[1] && !dpad_active[2] && !dpad_active[3])
		return;
	
	if (SAVE_SHOW_DPAD == 2) {
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
	else {
		dpad_x = 21;
		dpad_y = 44;
		if (z64_file.magic_acquired && z64_file.energy_capacity > 0xA0)
			dpad_y += 23;
		else if (z64_file.energy_capacity > 0xA0)
			dpad_y += 10;
		else if (z64_file.magic_acquired)
			dpad_y += 14;
		if (z64_game.pause_ctxt.state == 0)
			if (z64_file.timer_1_state > 0 || z64_file.timer_2_state > 0)
				dpad_y += 18;
	}
	
	uint8_t alpha = 255;
	if (alpha > z64_game.hud_alpha_channels.hearts_navi)
		alpha = z64_game.hud_alpha_channels.hearts_navi;
	
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
	sprite_load(db, &dpad_sprite, 0, 1);
	sprite_draw(db, &dpad_sprite, 0, dpad_x, dpad_y, 16, 16);
	
	draw_dpad_actions(db, alpha);
}