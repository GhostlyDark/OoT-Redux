#include "gfx.h"
#include "dpad.h"
#include "dpad_paused.h"
#include "dpad_actions.h"
#include "buttons.h"
#include "fps.h"

extern uint8_t CFG_DISPLAY_DPAD;
extern uint8_t CFG_DPAD_ENABLED;
extern uint8_t CFG_HUD_LAYOUT;
extern uint8_t CFG_KEEP_MASK;
extern uint8_t CFG_WS;

uint8_t dpad_alt			= 0;
uint16_t dpad_x				= 0;
uint16_t dpad_y				= 0;
uint16_t last_mask			= 0;

extern uint8_t CHECKED_LENS;

void handle_dpad() {
	handle_buttons();
	handle_dpad_ingame();
	handle_dpad_paused();
	handle_fps();
	handle_l_button();
	handle_infinite();
	handle_rupee_dash();
	handle_weaker_swords();
	handle_abilities();
	
	if (CFG_KEEP_MASK) {
		if (z64_change_scene == 0x20000001)
			last_mask = z64_mask_equipped;
		if (z64_change_scene == 0x20000000 && last_mask > 0)
			z64_mask_equipped = last_mask;
	}
}

void handle_dpad_ingame() {
	if (!CAN_USE_DPAD || z64_camera_view != 0 || CFG_DPAD_ENABLED == 0) 
		return;
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (CFG_DPAD_ENABLED == 2) {
		if ( (z64_game.common.input[0].raw.pad.l && z64_game.common.input[0].pad_pressed.r) || (z64_game.common.input[0].raw.pad.r && z64_game.common.input[0].pad_pressed.l) ) {
			dpad_alt ^= 1;
			CHECKED_LENS = 0;
			if (dpad_alt)
				z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			else z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		}
	}
	
	if (z64_game.pause_ctxt.state == 0 && CFG_DPAD_ENABLED > 0) 
		run_dpad_actions(pad_pressed);
}

void draw_dpad() {
    if (CFG_DISPLAY_DPAD == 0 || CFG_DPAD_ENABLED == 0 || (z64_game.pause_ctxt.state > 0x7 && z64_game.pause_ctxt.state < 0x12) )
		return;
	
	uint8_t *dpad_active = check_dpad_actions();
	if (z64_game.pause_ctxt.state == 0)
		if (!dpad_active[0] && !dpad_active[1] && !dpad_active[2] && !dpad_active[3])
			return;
	
	z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
	
	if (CFG_DISPLAY_DPAD == 1) {
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
	else {
		dpad_x = 271;
		dpad_y = 64;
		if (CFG_HUD_LAYOUT == 2 || CFG_HUD_LAYOUT == 3) {
			dpad_x += 10;
			dpad_y += 14;
		}
		else if (CFG_HUD_LAYOUT == 4 || CFG_HUD_LAYOUT == 5)
			dpad_y += 15;
		if (CFG_WS)
			dpad_x += 104;
	}
	if (CFG_DISPLAY_DPAD == 3) {
		dpad_x = 35;
		dpad_y = 175;
		if (z64_dungeon_scene != 0xFF)
			if (z64_file.dungeon_keys[z64_dungeon_scene] > 0)
				dpad_y = 158;
	}
	
	gSPDisplayList(db->p++, &setup_db);
	gDPPipeSync(db->p++);
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	uint16_t alpha = z64_game.hud_alpha_channels.minimap;
	
	if (z64_game.pause_ctxt.state != 0)
		alpha = 0xFF;
	if (alpha == 0xAA)
		alpha = 0xFF;
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
	sprite_load(db, &dpad_sprite, 0, 1);
	sprite_draw(db, &dpad_sprite, 0, dpad_x, dpad_y, 16, 16);
	
	draw_dpad_actions(db, alpha);
	gDPPipeSync(db->p++);
}