#include "l_button.h"

extern uint8_t CFG_DPAD_ENABLED;
extern char HUD_HIDE;

typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);

#define z64_playsfx   ((playsfx_t)      0x800C806C)

char BLOCK_R	= 0;
char PRESSED_R	= 0;

void toggle_minimap() {
	z64_gameinfo.minimap_disabled ^= 1;
	if (z64_gameinfo.minimap_disabled)
		z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	else z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}

void handle_l_button() {
	if (!CAN_USE_DPAD || z64_game.pause_ctxt.state != 0)
		return;
	
	if (CFG_DPAD_ENABLED == 2) {
		pad_t pad_released = z64_game.common.input[0].pad_released;
		
		if (!HUD_HIDE) {
			if (z64_game.common.input[0].raw.pad.r)
				PRESSED_R = 1;
			if (pad_released.l && !PRESSED_R)
				toggle_minimap();
			if (!z64_game.common.input[0].raw.pad.l)
				PRESSED_R = 0;
		}
		
		if (z64_game.common.input[0].pad_pressed.l && !z64_game.common.input[0].raw.pad.r)
			BLOCK_R = 1;
		if (!z64_game.common.input[0].raw.pad.l)
			BLOCK_R = 0;
		
		if (BLOCK_R) {
			z64_game.common.input[0].raw.pad.r		= 0;
			z64_game.common.input[0].pad_pressed.r	= 0;
		}
	
		z64_game.common.input[0].pad_pressed.l = 0;
	}
	
	if (HUD_HIDE)
		z64_game.common.input[0].pad_pressed.l = 0;
}

