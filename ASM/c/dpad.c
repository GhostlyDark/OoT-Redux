#include "gfx.h"
#include "dpad.h"

extern uint8_t CFG_DISPLAY_DPAD;
extern uint8_t CFG_DPAD_ENABLED;
extern uint8_t CFG_HIDE_HUD_ENABLED;
extern uint8_t CFG_UNEQUIP_GEAR_ENABLED;
extern uint8_t CFG_SWAP_ITEMS_ENABLED;
extern uint8_t CFG_B_BUTTON_ITEMS_ENABLED;
extern uint8_t CFG_ARROW_TOGGLING_ENABLED;

//unknown 00 is a pointer to some vector transformation when the sound is tied to an actor. actor + 0x3E, when not tied to an actor (map), always 80104394
//unknown 01 is always 4 in my testing
//unknown 02 is a pointer to some kind of audio configuration Always 801043A0 in my testing
//unknown 03 is always a3 in my testing
//unknown 04 is always a3 + 0x08 in my testing (801043A8)
typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);
typedef void(*usebutton_t)(z64_game_t *game, z64_link_t *link, uint8_t item, uint8_t button);

char HUD_HIDE        = 0;
char HUD_HEARTS_HIDE = 1;
char HUD_COUNTER     = 0;

#define z64_playsfx   ((playsfx_t)      0x800C806C)
#define z64_usebutton ((usebutton_t)    0x8038C9A0)

void handle_hud() {
	
	if (HUD_HIDE == 1 && (z64_game.pause_ctxt.state == 0 || z64_game.pause_ctxt.state == 0x1A || z64_game.pause_ctxt.state == 0x1B) ) {
		if (z64_game.hud_alpha_channels.hearts_navi != 0) {
			if (HUD_COUNTER < 8) {
				HUD_COUNTER++;
				return;
			}
		}
		else { HUD_COUNTER = 0; }
		
		if (z64_game.hud_alpha_channels.b_button              > 40)		z64_game.hud_alpha_channels.b_button          -= 40; else z64_game.hud_alpha_channels.b_button          = 0;
		if (z64_game.hud_alpha_channels.cl_button             > 40)		z64_game.hud_alpha_channels.cl_button         -= 40; else z64_game.hud_alpha_channels.cl_button         = 0;
		if (z64_game.hud_alpha_channels.cd_button             > 40)		z64_game.hud_alpha_channels.cd_button         -= 40; else z64_game.hud_alpha_channels.cd_button         = 0;
		if (z64_game.hud_alpha_channels.cr_button             > 40)		z64_game.hud_alpha_channels.cr_button         -= 40; else z64_game.hud_alpha_channels.cr_button         = 0;
		if (z64_game.hud_alpha_channels.a_button_carots       > 40)		z64_game.hud_alpha_channels.a_button_carots   -= 40; else z64_game.hud_alpha_channels.a_button_carots   = 0;
		if (z64_game.hud_alpha_channels.minimap               > 40)		z64_game.hud_alpha_channels.minimap           -= 40; else z64_game.hud_alpha_channels.minimap           = 0;
		
		if (HUD_HEARTS_HIDE == 1) {
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
	
	if (!CAN_USE_DPAD)
		return;
	pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (z64_game.pause_ctxt.state == 6 && pad_pressed.a && z64_game.pause_ctxt.screen_idx == 0 && z64_game.pause_ctxt.cursor_pos != 0x0A && z64_game.pause_ctxt.cursor_pos != 0x0B) {
		if (z64_game.pause_ctxt.item_cursor >= Z64_SLOT_STICK && z64_game.pause_ctxt.item_cursor <= Z64_SLOT_CHILD_TRADE && CFG_B_BUTTON_ITEMS_ENABLED) {
			z64_item_t item = 0xFF;
			
			for (char i=0; i<18; i++)
				if (z64_game.pause_ctxt.item_cursor == i)
					item = z64_file.items[i];
			
			if (z64_file.link_age == 0) {
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
			
			if (item != Z64_ITEM_SLINGSHOT && item != Z64_ITEM_BOW && item != Z64_ITEM_BOMBCHU && item <= Z64_ITEM_NAYRUS_LOVE) {
				z64_file.button_items[0] = item;
				if (z64_file.link_age == 0)
					z64_file.adult_button_items[0] = item;
				else if	(z64_file.link_age == 1)
					z64_file.child_button_items[0] = item;
				z64_UpdateItemButton(&z64_game, 0);
				z64_playsfx(0x4808, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
		}
	}
	
	if (z64_game.pause_ctxt.state == 6 && pad_pressed.l && CFG_HIDE_HUD_ENABLED) {
		if (HUD_HIDE == 0) {
			HUD_HIDE = 1;
			z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		}
		else {
			HUD_HIDE = 0;
			z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		}
	}
	
	if (z64_game.pause_ctxt.state == 0 && pad_pressed.l && HUD_HIDE == 1) {
		if (HUD_HEARTS_HIDE == 0)
			HUD_HEARTS_HIDE = 1;
		else HUD_HEARTS_HIDE = 0;
	}
	
	if (z64_game.pause_ctxt.state == 0 && pad_pressed.r && z64_camera_view == 2 && z64_file.items[Z64_SLOT_BOW] == Z64_ITEM_BOW && CFG_ARROW_TOGGLING_ENABLED) {
		for (char i=0; i<3; i++) {
			if (z64_file.c_button_slots[i] == Z64_ITEM_BOW || z64_file.c_button_slots[i] == Z64_ITEM_FIRE_ARROW || z64_file.c_button_slots[i] == Z64_ITEM_ICE_ARROW || z64_file.c_button_slots[i] == Z64_ITEM_LIGHT_ARROW) {
				if (z64_file.c_button_slots[i] == Z64_ITEM_BOW && z64_file.items[Z64_SLOT_FIRE_ARROW] == Z64_ITEM_FIRE_ARROW) { // Regular -> Fire
					z64_file.c_button_slots[i] = Z64_ITEM_FIRE_ARROW;
					z64_file.button_items[i+1] = Z64_ITEM_BOW_FIRE_ARROW;
					z64_playsfx(0x483E, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
				else if (z64_file.c_button_slots[i] == Z64_ITEM_FIRE_ARROW && z64_file.items[Z64_SLOT_ICE_ARROW] == Z64_ITEM_ICE_ARROW) { // Fire -> Ice
					z64_file.c_button_slots[i] = Z64_ITEM_ICE_ARROW;
					z64_file.button_items[i+1]   = Z64_ITEM_BOW_ICE_ARROW;
					z64_playsfx(0x483F, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
				else if (z64_file.c_button_slots[i] == Z64_ITEM_ICE_ARROW && z64_file.items[Z64_SLOT_LIGHT_ARROW] == Z64_ITEM_LIGHT_ARROW) { // Ice -> Light
					z64_file.c_button_slots[i] = Z64_ITEM_LIGHT_ARROW;
					z64_file.button_items[i+1] = Z64_ITEM_BOW_LIGHT_ARROW;
					z64_playsfx(0x4840, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
				else { // Back to regular
					z64_file.c_button_slots[i] = Z64_ITEM_BOW;
					z64_file.button_items[i+1] = Z64_ITEM_BOW;
					z64_playsfx(0x4808, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
				z64_UpdateItemButton(&z64_game, i);
			}
		}
	}
	
}

void handle_dpad() {
	
	if (!CAN_USE_DPAD || !DISPLAY_DPAD) 
		return;
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (z64_game.pause_ctxt.state == 6) {
		
		if (pad_pressed.du && z64_game.pause_ctxt.cursor_pos != 0x0A && z64_game.pause_ctxt.cursor_pos != 0x0B && CFG_UNEQUIP_GEAR_ENABLED) {
			if (z64_game.pause_ctxt.screen_idx == 3) {
				if (z64_game.pause_ctxt.equip_cursor == 1 && z64_file.equip_sword == 1) {
					z64_file.equip_sword     = 0;
					z64_file.inf_table[29]   = 1;
					z64_file.button_items[0] = -1;
					z64_UpdateEquipment(&z64_game, &z64_link);
					z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
				if (z64_game.pause_ctxt.equip_cursor == 2 && z64_file.equip_sword == 2) {
					z64_file.equip_sword     = 0;
					z64_file.inf_table[29]   = 1;
					z64_file.button_items[0] = -1;
					z64_UpdateEquipment(&z64_game, &z64_link);
					z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
				if (z64_game.pause_ctxt.equip_cursor == 3 && z64_file.equip_sword == 3) {
					z64_file.equip_sword     = 0;
					z64_file.inf_table[29]   = 1;
					z64_file.button_items[0] = -1;
					z64_UpdateEquipment(&z64_game, &z64_link);
					z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
					
				if (z64_game.pause_ctxt.equip_cursor == 5 && z64_file.equip_shield == 1) {
					z64_file.equip_shield = 0;
					z64_UpdateEquipment(&z64_game, &z64_link);
					z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
				if (z64_game.pause_ctxt.equip_cursor == 6 && z64_file.equip_shield == 2) {
					z64_file.equip_shield = 0;
					z64_UpdateEquipment(&z64_game, &z64_link);
					z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
				if (z64_game.pause_ctxt.equip_cursor == 7 && z64_file.equip_shield == 3) {
					z64_file.equip_shield = 0;
					z64_UpdateEquipment(&z64_game, &z64_link);
					z64_playsfx(0x480A, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
			}
		}
		
		if (pad_pressed.dd && z64_game.pause_ctxt.cursor_pos != 0x0A && z64_game.pause_ctxt.cursor_pos != 0x0B && CFG_SWAP_ITEMS_ENABLED) {
			if (z64_game.pause_ctxt.screen_idx == 3) {
				if (z64_game.pause_ctxt.equip_cursor == 3 && (z64_file.ammo[4] == 1 || z64_file.bgs_flag == 1) ) {
					z64_file.ammo[4] = 1;
					if (z64_file.bgs_flag == 1)
						z64_file.bgs_flag = 0;
					else z64_file.bgs_flag = 1;
					z64_playsfx(0x4808, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
			}
			
			if (z64_game.pause_ctxt.screen_idx == 0) {
				if (z64_game.pause_ctxt.item_cursor == 7 && (z64_file.ammo[5] == 1 || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) ) {
					z64_file.ammo[5] = 1;
					if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) {
						for (char i=0; i<4; i++) {
							if (z64_file.button_items[i] == Z64_ITEM_OCARINA_OF_TIME) {
															z64_file.button_items[i]			= Z64_ITEM_FAIRY_OCARINA;
								if		(i > 0)				z64_file.c_button_slots[i-1]		= Z64_SLOT_OCARINA;
								if		(z64_file.link_age == 0) {
															z64_file.adult_button_items[i]		= Z64_ITEM_FAIRY_OCARINA;
									if	(i > 0)				z64_file.adult_c_button_slots[i-1]	= Z64_SLOT_OCARINA;
								}
								else if	(z64_file.link_age == 1) {
															z64_file.child_button_items[i]		= Z64_ITEM_FAIRY_OCARINA;
									if	(i > 0)				z64_file.child_c_button_slots[i-1]	= Z64_SLOT_OCARINA;
								}
								z64_UpdateItemButton(&z64_game, i);
							}
						}
						z64_file.items[Z64_SLOT_OCARINA] = Z64_ITEM_FAIRY_OCARINA;
					}
					else if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA) {
						for (char i=0; i<4; i++) {
							if (z64_file.button_items[i] == Z64_ITEM_FAIRY_OCARINA) {
															z64_file.button_items[i]			= Z64_ITEM_OCARINA_OF_TIME;
								if		(i > 0)				z64_file.c_button_slots[i-1]		= Z64_SLOT_OCARINA;
								if		(z64_file.link_age == 0) {
															z64_file.adult_button_items[i]		= Z64_ITEM_OCARINA_OF_TIME;
									if	(i > 0)				z64_file.adult_c_button_slots[i-1]	= Z64_SLOT_OCARINA;
								}
								else if	(z64_file.link_age == 1) {
															z64_file.child_button_items[i]		= Z64_ITEM_OCARINA_OF_TIME;
									if	(i > 0)				z64_file.child_c_button_slots[i-1]	= Z64_SLOT_OCARINA;
								}
								z64_UpdateItemButton(&z64_game, i);
							}
						}
						z64_file.items[Z64_SLOT_OCARINA] = Z64_ITEM_OCARINA_OF_TIME;
					}
					z64_playsfx(0x4808, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
				
				if (z64_game.pause_ctxt.item_cursor == 9 && (z64_file.ammo[7] == 1 || z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_LONGSHOT) ) {
					z64_file.ammo[7] = 1;
					if (z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_LONGSHOT) {
						for (char i=0; i<4; i++) {
							if (z64_file.button_items[i] == Z64_ITEM_LONGSHOT) {
															z64_file.button_items[i]			= Z64_ITEM_HOOKSHOT;
								if		(i > 0)				z64_file.c_button_slots[i-1]		= Z64_SLOT_HOOKSHOT;
								if		(z64_file.link_age == 0) {
															z64_file.adult_button_items[i]		= Z64_ITEM_HOOKSHOT;
									if	(i > 0)				z64_file.adult_c_button_slots[i-1]	= Z64_SLOT_HOOKSHOT;
								}
								else if	(z64_file.link_age == 1) {
															z64_file.child_button_items[i]		= Z64_ITEM_HOOKSHOT;
									if	(i > 0)				z64_file.child_c_button_slots[i-1]	= Z64_SLOT_HOOKSHOT;
								}
								z64_UpdateItemButton(&z64_game, i);
							}
						}
						z64_file.items[Z64_SLOT_HOOKSHOT] = Z64_ITEM_HOOKSHOT;
					}
					else if (z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_HOOKSHOT) {
						for (char i=0; i<4; i++) {
							if (z64_file.button_items[i] == Z64_ITEM_HOOKSHOT) {
															z64_file.button_items[i]			= Z64_ITEM_LONGSHOT;
								if		(i > 0)				z64_file.c_button_slots[i-1]		= Z64_SLOT_HOOKSHOT;
								if		(z64_file.link_age == 0) {
															z64_file.adult_button_items[i]		= Z64_ITEM_LONGSHOT;
									if	(i > 0)  			z64_file.adult_c_button_slots[i-1]	= Z64_SLOT_HOOKSHOT;
								}
								else if	(z64_file.link_age == 1) {
															z64_file.child_button_items[i]		= Z64_ITEM_LONGSHOT;
									if	(i > 0)  			z64_file.child_c_button_slots[i-1]	= Z64_SLOT_HOOKSHOT;
								}
								z64_UpdateItemButton(&z64_game, i);
							}
						}
						z64_file.items[Z64_SLOT_HOOKSHOT] = Z64_ITEM_LONGSHOT;
					}
					z64_playsfx(0x4808, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}
			}
		}
		
	}
	
	if (z64_game.pause_ctxt.state == 0 && CFG_DPAD_ENABLED) {
		
		if (z64_file.link_age == 0) {
			if (pad_pressed.dl && z64_file.iron_boots && z64_camera_view == 0) {
				if (z64_file.equip_boots == 2)
					z64_file.equip_boots = 1;
				else z64_file.equip_boots = 2;
				z64_UpdateEquipment(&z64_game, &z64_link);
				z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
			
			if (pad_pressed.dr && z64_file.hover_boots && z64_camera_view == 0) {
				if (z64_file.equip_boots == 3)
					z64_file.equip_boots = 1;
				else z64_file.equip_boots = 3;
				z64_UpdateEquipment(&z64_game, &z64_link);
				z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
			
			if (pad_pressed.du && z64_file.hylian_shield && z64_file.mirror_shield && z64_camera_view == 0) {
				if (z64_file.equip_shield == 2)
					z64_file.equip_shield = 3;
				else z64_file.equip_shield = 2;
				z64_UpdateEquipment(&z64_game, &z64_link);
				z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
		}
			
		if (z64_file.link_age == 1) {
			if (pad_pressed.dr && CAN_USE_CHILD_TRADE && z64_camera_view == 0)
				z64_usebutton(&z64_game,&z64_link,z64_file.items[Z64_SLOT_CHILD_TRADE], 2);
				
			if (pad_pressed.du && z64_file.deku_shield && z64_file.hylian_shield && z64_camera_view == 0) {
				if (z64_file.equip_shield == 2)
					z64_file.equip_shield = 1;
				else z64_file.equip_shield = 2;
				z64_UpdateEquipment(&z64_game, &z64_link);
				z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
		}
		
		if (pad_pressed.dd && CAN_USE_OCARINA && z64_camera_view == 0)
			z64_usebutton(&z64_game,&z64_link,z64_file.items[Z64_SLOT_OCARINA], 2);
    }
}
void draw_dpad() {
    if (!DISPLAY_DPAD || !CFG_DISPLAY_DPAD || !CFG_DPAD_ENABLED)
		return;
	z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
	
	gSPDisplayList(db->p++, &setup_db);
	gDPPipeSync(db->p++);
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	uint16_t alpha = z64_game.hud_alpha_channels.minimap;
	
	if (alpha == 0xAA) alpha = 0xFF;
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
	sprite_load(db, &dpad_sprite, 0, 1);
	sprite_draw(db, &dpad_sprite, 0, 271, 64, 16, 16);
	
	if (alpha == 0xFF && !CAN_USE_DPAD)
		gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
	
	if (z64_file.iron_boots && z64_file.link_age == 0) {
		sprite_load(db, &items_sprite, 69, 1);
		if (z64_file.equip_boots == 2)
			sprite_draw(db, &items_sprite, 0, 258, 64, 16, 16);
		else sprite_draw(db, &items_sprite, 0, 260, 66, 12, 12);
	}
	
	if (z64_file.hover_boots && z64_file.link_age == 0) {
		sprite_load(db, &items_sprite, 70, 1);
		if (z64_file.equip_boots == 3)
			sprite_draw(db, &items_sprite, 0, 283, 64, 16, 16);
		else sprite_draw(db, &items_sprite, 0, 285, 66, 12, 12);
	}

	if (z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH && z64_file.link_age == 1) {
		if(alpha==0xFF && !CAN_USE_CHILD_TRADE) gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
		else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
		sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_CHILD_TRADE], 1);
		sprite_draw(db, &items_sprite, 0, 285, 66, 12, 12);
	}

	if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) {
		if(alpha==0xFF && !CAN_USE_OCARINA) gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
		else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
		sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_OCARINA], 1);
		sprite_draw(db, &items_sprite, 0, 273, 77, 12,12);
	}
		
	if (z64_file.equip_shield == 1) {
		sprite_load(db, &items_sprite, 62, 1);
		sprite_draw(db, &items_sprite, 0, 273, 53, 12, 12);
	}
	else if (z64_file.equip_shield == 2) {
		sprite_load(db, &items_sprite, 63, 1);
		sprite_draw(db, &items_sprite, 0, 273, 53, 12, 12);
	}
	else if (z64_file.equip_shield == 3) {
		sprite_load(db, &items_sprite, 64, 1);
		sprite_draw(db, &items_sprite, 0, 273, 53, 12, 12);
	}

	gDPPipeSync(db->p++);
}

