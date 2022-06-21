#include "gfx.h"
#include "dpad.h"
#include "dpad_actions.h"

extern uint8_t CFG_DISPLAY_DPAD;
extern uint8_t CFG_DPAD_ENABLED;

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

uint8_t DPAD_ALT	= 0;
uint16_t DPAD_X		= 0;
uint16_t DPAD_Y		= 0;

void handle_dpad() {
	if (!CAN_USE_DPAD || !DISPLAY_DPAD || z64_game.pause_ctxt.state != 0 || z64_camera_view != 0 || CFG_DPAD_ENABLED == 0) 
		return;
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (CFG_DPAD_ENABLED == 2) {
		if ( (z64_game.common.input[0].raw.pad.l && z64_game.common.input[0].pad_pressed.r) || (z64_game.common.input[0].raw.pad.r && z64_game.common.input[0].pad_pressed.l) )
			DPAD_ALT ^= 1;
	}
	
	if (pad_pressed.du)
		action_dpad_up();
	if (pad_pressed.dr)
		action_dpad_right();
	if (pad_pressed.dd)
		action_dpad_down();
	if (pad_pressed.dl)
		action_dpad_left();
}

void draw_dpad() {
    if (!DISPLAY_DPAD || CFG_DISPLAY_DPAD == 0 || CFG_DPAD_ENABLED == 0)
		return;
	z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
	
	if (CFG_DISPLAY_DPAD != 2) {
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
	
	if (alpha == 0xFF && !CAN_USE_DPAD && z64_game.pause_ctxt.state != 0)
		gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
	
	draw_dpad_up(db, alpha);
	draw_dpad_right(db, alpha);
	draw_dpad_down(db, alpha);
	draw_dpad_left(db, alpha);

	gDPPipeSync(db->p++);
}

