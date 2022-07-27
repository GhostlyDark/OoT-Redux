#include "l_button.h"

typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);

extern uint8_t CFG_HIDE_HUD_ENABLED;
extern uint8_t CFG_DPAD_ENABLED;
extern uint8_t CFG_FPS_ENABLED;
extern uint8_t CFG_INVENTORY_EDITOR_ENABLED;

extern uint8_t FPS_SWITCH;
extern uint8_t HUD_HIDE;
extern uint8_t HUD_HEARTS_HIDE;
extern uint8_t EXITED_EDITOR;

uint8_t BLOCK		= 0;
uint8_t PRESSED_R	= 0;
uint8_t PRESSED_Z	= 0;

void toggle_minimap() {
	if (z64_dungeon_scene != 0xFFFF) {
		if (!z64_file.dungeon_items[z64_dungeon_scene].map)
			return;
	}		
	else if (z64_has_minimap == 0xFFFF)
		return;
	
	z64_gameinfo.minimap_disabled ^= 1;
	if (z64_gameinfo.minimap_disabled)
		z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	else z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}

void handle_l_button() {
	if (z64_game.pause_ctxt.state != 0 && z64_game.pause_ctxt.state != 6)
		return;
	if (CFG_DPAD_ENABLED != 2 && !CFG_FPS_ENABLED && !CFG_INVENTORY_EDITOR_ENABLED)
		return;
	
	pad_t pad_released = z64_game.common.input[0].pad_released;
	
	if (CFG_DPAD_ENABLED == 2) {
		if (z64_game.common.input[0].raw.pad.r)
			PRESSED_R = 1;
		if (CFG_FPS_ENABLED && z64_game.common.input[0].raw.pad.z)
			PRESSED_Z = 1;
		if (pad_released.l && !PRESSED_R && !PRESSED_Z) {
			if (z64_game.pause_ctxt.state == 0 && !HUD_HIDE)
				toggle_minimap();
			else if (z64_game.pause_ctxt.state == 0 && HUD_HIDE)
				HUD_HEARTS_HIDE ^= 1;
			else if (z64_game.pause_ctxt.state == 6 && CFG_HIDE_HUD_ENABLED && z64_game.pause_ctxt.unk_02_[1] == 0) {
				HUD_HIDE ^= 1;
				if (HUD_HIDE)
					z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				else z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
		}
		if (!z64_game.common.input[0].raw.pad.l)
			PRESSED_R = PRESSED_Z = 0;
	}
	
	if (z64_game.common.input[0].pad_pressed.l && !z64_game.common.input[0].raw.pad.r && CFG_DPAD_ENABLED == 2)
		BLOCK = 1;
	else if (z64_game.common.input[0].pad_pressed.l && !z64_game.common.input[0].raw.pad.z && (CFG_FPS_ENABLED || CFG_INVENTORY_EDITOR_ENABLED) )
		BLOCK = 1;
	if (!z64_game.common.input[0].raw.pad.l)
		BLOCK = 0;
	
	if (BLOCK) {
		if (CFG_DPAD_ENABLED == 2)
			z64_game.common.input[0].raw.pad.r = z64_game.common.input[0].pad_pressed.r = 0;
		if (CFG_FPS_ENABLED || CFG_INVENTORY_EDITOR_ENABLED)
			z64_game.common.input[0].raw.pad.z = z64_game.common.input[0].pad_pressed.z = 0;
	}
	
	z64_game.common.input[0].pad_pressed.l = 0;
	
}

