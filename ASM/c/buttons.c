#include "buttons.h"

extern uint8_t CFG_BUTTONS_ENABLED;
extern uint8_t CFG_HIDE_HUD_ENABLED;
extern uint8_t CFG_B_BUTTON_ITEM_ENABLED;

typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);

uint8_t HUD_HIDE		= 0;
uint8_t HUD_HEARTS_HIDE	= 1;
uint8_t HUD_COUNTER		= 0;

#define z64_playsfx   ((playsfx_t)      0x800C806C)

void handle_hud() {
	if (HUD_HIDE && (z64_game.pause_ctxt.state == 0 || z64_game.pause_ctxt.state == 0x1A || z64_game.pause_ctxt.state == 0x1B) ) {
		if (z64_game.hud_alpha_channels.minimap != 0) {
			if (HUD_COUNTER < 8) {
				HUD_COUNTER++;
				return;
			}
		}
		else HUD_COUNTER = 0;
		
		if (z64_game.hud_alpha_channels.b_button              > 40)		z64_game.hud_alpha_channels.b_button          -= 40; else z64_game.hud_alpha_channels.b_button          = 0;
		if (z64_game.hud_alpha_channels.cl_button             > 40)		z64_game.hud_alpha_channels.cl_button         -= 40; else z64_game.hud_alpha_channels.cl_button         = 0;
		if (z64_game.hud_alpha_channels.cd_button             > 40)		z64_game.hud_alpha_channels.cd_button         -= 40; else z64_game.hud_alpha_channels.cd_button         = 0;
		if (z64_game.hud_alpha_channels.cr_button             > 40)		z64_game.hud_alpha_channels.cr_button         -= 40; else z64_game.hud_alpha_channels.cr_button         = 0;
		if (z64_game.hud_alpha_channels.a_button_carots       > 40)		z64_game.hud_alpha_channels.a_button_carots   -= 40; else z64_game.hud_alpha_channels.a_button_carots   = 0;
		if (z64_game.hud_alpha_channels.minimap               > 40)		z64_game.hud_alpha_channels.minimap           -= 40; else z64_game.hud_alpha_channels.minimap           = 0;
		
		if (HUD_HEARTS_HIDE) {
			if (z64_game.hud_alpha_channels.hearts_navi       > 40)		z64_game.hud_alpha_channels.hearts_navi       -= 40; else z64_game.hud_alpha_channels.hearts_navi       = 0;
			if (z64_game.hud_alpha_channels.rupees_keys_magic > 40)		z64_game.hud_alpha_channels.rupees_keys_magic -= 40; else z64_game.hud_alpha_channels.rupees_keys_magic = 0;
		}
		else {
			if (z64_game.hud_alpha_channels.hearts_navi       < 215)	z64_game.hud_alpha_channels.hearts_navi       += 40; else z64_game.hud_alpha_channels.hearts_navi       = 255;
			if (z64_game.hud_alpha_channels.rupees_keys_magic < 215)	z64_game.hud_alpha_channels.rupees_keys_magic += 40; else z64_game.hud_alpha_channels.rupees_keys_magic = 255;
		}
	}
}

void handle_buttons() {
	if (!CFG_BUTTONS_ENABLED || z64_game.pause_ctxt.unk_02_[1] != 0)
		return;
	pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (z64_game.pause_ctxt.state == 6 && pad_pressed.a && z64_game.pause_ctxt.screen_idx == 0 && z64_game.pause_ctxt.cursor_pos != 0x0A && z64_game.pause_ctxt.cursor_pos != 0x0B) {
		if (z64_game.pause_ctxt.item_cursor >= Z64_SLOT_STICK && z64_game.pause_ctxt.item_cursor <= Z64_SLOT_CHILD_TRADE && CFG_B_BUTTON_ITEM_ENABLED) {
			z64_item_t item = 0xFF;
			
			for (uint8_t i=0; i<18; i++)
				if (z64_game.pause_ctxt.item_cursor == i)
					item = z64_file.items[i];
			
			if (!z64_file.link_age) {
				if		(item == Z64_ITEM_STICK)		item = 0xFF;
				else if (item == Z64_ITEM_SLINGSHOT)	item = 0xFF;
				else if (item == Z64_ITEM_BOOMERANG)	item = 0xFF;
				else if (item == Z64_ITEM_BEANS)		item = 0xFF;
			}
			else {
				if		(item == Z64_ITEM_BOW)			item = 0xFF;
				else if (item == Z64_ITEM_FIRE_ARROW)	item = 0xFF;
				else if (item == Z64_ITEM_ICE_ARROW)	item = 0xFF;
				else if (item == Z64_ITEM_LIGHT_ARROW)	item = 0xFF;
				else if (item == Z64_ITEM_HOOKSHOT)		item = 0xFF;
				else if (item == Z64_ITEM_LONGSHOT)		item = 0xFF;
				else if (item == Z64_ITEM_HAMMER)		item = 0xFF;
			}
			
			if (item != Z64_ITEM_SLINGSHOT && item != Z64_ITEM_BOW && item != Z64_ITEM_BOMBCHU && item != Z64_ITEM_LENS && item <= Z64_ITEM_NAYRUS_LOVE) {
				z64_file.button_items[0] = item;
				if (!z64_file.link_age)
					z64_file.adult_button_items[0] = item;
				else if	(z64_file.link_age)
					z64_file.child_button_items[0] = item;
				z64_UpdateItemButton(&z64_game, 0);
				z64_playsfx(0x4808, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
		}
	}
	
	if (pad_pressed.l) {
		if (z64_game.pause_ctxt.state == 6 && CFG_HIDE_HUD_ENABLED) {
			HUD_HIDE ^= 1;
			if (HUD_HIDE)
				z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			else z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		}
	
		if (z64_game.pause_ctxt.state == 0 && HUD_HIDE)
			HUD_HEARTS_HIDE ^= 1;
	}
}
