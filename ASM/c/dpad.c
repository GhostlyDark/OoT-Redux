#include "gfx.h"
#include "text.h"
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
extern uint8_t CFG_TRISWIPE;
extern uint8_t CFG_ABILITIES_ENABLED;

uint8_t dpad_alt			= 0;
uint16_t dpad_x				= 0;
uint16_t dpad_y				= 0;
uint16_t last_mask			= 0;

extern uint8_t CHECKED_LENS;

void handle_dpad() {
	handle_layout();
	handle_hud();
	handle_buttons();
	handle_dpad_ingame();
	handle_dpad_paused();
	handle_fps();
	handle_l_button();
	handle_infinite();
	handle_abilities_tunic_colors();
	
	if (CAN_CONTROL_LINK && !IS_INTRO) {
		handle_rupee_dash();
		handle_power_crouch_stab_fix();
		handle_weaker_swords();
		handle_abilities();
		handle_inverted_axis();
	}
	
	if (CFG_KEEP_MASK) {
		if (z64_change_scene == 0x20000001)
			last_mask = z64_mask_equipped;
		if (z64_change_scene == 0x20000000 && last_mask > 0)
			z64_mask_equipped = last_mask;
	}
	
	if (CFG_TRISWIPE && z64_triswipe == 255)
		z64_triswipe = 1;
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
	z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
	gSPDisplayList(db->p++, &setup_db);
	gDPPipeSync(db->p++);
	
	if (!draw_abilities_info(db))
		draw_dpad_icons(db);

}

void draw_dpad_icons(z64_disp_buf_t *db) {
	if (CFG_DISPLAY_DPAD == 0 || CFG_DPAD_ENABLED == 0 || (z64_game.pause_ctxt.state > 0x7 && z64_game.pause_ctxt.state < 0x12) )
		return;
	
	uint8_t *dpad_active = check_dpad_actions();
	if (z64_game.pause_ctxt.state == 0)
		if (!dpad_active[0] && !dpad_active[1] && !dpad_active[2] && !dpad_active[3])
			return;
	
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

uint8_t draw_abilities_info(z64_disp_buf_t *db) {
	if (!CFG_ABILITIES_ENABLED || !IS_PAUSE_SCREEN_CURSOR || z64_game.pause_ctxt.screen_idx != 2 || !z64_game.common.input[0].raw.pad.a)
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
		uint16_t x = 100;
		uint16_t y = 100;
		
		gDPSetCombineMode(db->p++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
		gDPSetPrimColor(db->p++, 0, 0, 0x00, 0x00, 0x00, 0xD0);
		
		int left   = 10;
		int top    = 10;
		int width  = 240;
		int height = 115;
		
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