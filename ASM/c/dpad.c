#include "gfx.h"
#include "dpad.h"
#include "dpad_paused.h"
#include "dpad_actions.h"
#include "buttons.h"
#include "l_button.h"
#include "fps.h"

extern uint8_t CFG_DISPLAY_DPAD;
extern uint8_t CFG_DPAD_ENABLED;
extern uint8_t CFG_HUD_LAYOUT;
extern uint8_t CFG_KEEP_MASK;
extern uint8_t CFG_WS;

uint8_t DPAD_ALT			= 0;
uint16_t DPAD_X				= 0;
uint16_t DPAD_Y				= 0;
uint16_t LAST_MASK			= 0;
uint16_t LAST_MASK_SCENE	= 0xFFFF;

void handle_dpad() {
	handle_layout();
	handle_buttons();
	handle_dpad_ingame();
	handle_dpad_paused();
    handle_hud();
	handle_fps();
	handle_l_button();
	
	if (CFG_KEEP_MASK) {
		if (z64_mask_equipped > 0)
			LAST_MASK = z64_mask_equipped;
		if (z64_game.scene_index != LAST_MASK_SCENE && LAST_MASK > 0) {
			z64_mask_equipped = LAST_MASK;
			LAST_MASK_SCENE = z64_game.scene_index;
		}
	}
}

void handle_dpad_ingame() {
	if (!CAN_USE_DPAD || z64_game.pause_ctxt.state != 0 || z64_camera_view != 0 || CFG_DPAD_ENABLED == 0) 
		return;
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (CFG_DPAD_ENABLED == 2) {
		if ( (z64_game.common.input[0].raw.pad.l && z64_game.common.input[0].pad_pressed.r) || (z64_game.common.input[0].raw.pad.r && z64_game.common.input[0].pad_pressed.l) )
			DPAD_ALT ^= 1;
	}
	
	run_dpad_actions(pad_pressed);
}

void draw_dpad() {
    if (CFG_DISPLAY_DPAD == 0 || CFG_DPAD_ENABLED == 0)
		return;
	
	uint8_t *dpad_active  = check_dpad_actions();
	if (!dpad_active[0] && !dpad_active[1] && !dpad_active[2] && !dpad_active[3])
		return;
	
	z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
	
	if (CFG_DISPLAY_DPAD == 1) {
		DPAD_X = 21;
		DPAD_Y = 44;
		if (z64_file.magic_acquired && z64_file.energy_capacity > 0xA0)
			DPAD_Y += 23;
		else if (z64_file.energy_capacity > 0xA0)
			DPAD_Y += 10;
		else if (z64_file.magic_acquired)
			DPAD_Y += 14;
	}
	else {
		DPAD_X = 271;
		DPAD_Y = 64;
		if (CFG_HUD_LAYOUT == 2 || CFG_HUD_LAYOUT == 3) {
			DPAD_X += 10;
			DPAD_Y += 14;
		}
		else if (CFG_HUD_LAYOUT == 4 || CFG_HUD_LAYOUT == 5)
			DPAD_Y += 15;
		if (CFG_WS)
			DPAD_X += 104;
	}
	if (CFG_DISPLAY_DPAD == 3) {
		DPAD_X = 35;
		DPAD_Y = 175;
		if (z64_dungeon_scene != 0xFF)
			if (z64_file.dungeon_keys[z64_dungeon_scene] > 0)
				DPAD_Y = 158;
	}
	
	gSPDisplayList(db->p++, &setup_db);
	gDPPipeSync(db->p++);
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	uint16_t alpha = z64_game.hud_alpha_channels.minimap;
	
	if (alpha == 0xAA)
		alpha = 0xFF;
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
	sprite_load(db, &dpad_sprite, 0, 1);
	sprite_draw(db, &dpad_sprite, 0, DPAD_X, DPAD_Y, 16, 16);
	
	if (alpha == 0xFF && z64_game.pause_ctxt.state != 0)
		gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
	
	draw_dpad_actions(db, alpha);
	gDPPipeSync(db->p++);
}

