#include "gfx.h"
#include "dpad.h"

extern uint8_t CFG_DISPLAY_DPAD;

//unknown 00 is a pointer to some vector transformation when the sound is tied to an actor. actor + 0x3E, when not tied to an actor (map), always 80104394
//unknown 01 is always 4 in my testing
//unknown 02 is a pointer to some kind of audio configuration Always 801043A0 in my testing
//unknown 03 is always a3 in my testing
//unknown 04 is always a3 + 0x08 in my testing (801043A8)
typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);
typedef void(*usebutton_t)(z64_game_t *game, z64_link_t *link, uint8_t item, uint8_t button);

#define z64_playsfx   ((playsfx_t)      0x800C806C)
#define z64_usebutton ((usebutton_t)    0x8038C9A0)

void handle_dpad() {

    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;

    if (CAN_USE_DPAD && DISPLAY_DPAD) {
		
		if (z64_game.pause_ctxt.state == 6) {
			
			if (pad_pressed.du) {
				if (z64_game.pause_ctxt.screen_idx == 3) {
					if (z64_game.pause_ctxt.equip_cursor == 1 && z64_file.equip_sword == 1) {
						z64_file.equip_sword     = 0;
						z64_file.inf_table[29]   = 1;
						z64_file.button_items[0] = -1;
					}
					if (z64_game.pause_ctxt.equip_cursor == 2 && z64_file.equip_sword == 2) {
						z64_file.equip_sword     = 0;
						z64_file.button_items[0] = -1;
					}
					if (z64_game.pause_ctxt.equip_cursor == 3 && z64_file.equip_sword == 3) {
						z64_file.equip_sword     = 0;
						z64_file.button_items[0] = -1;
					}
					
					if (z64_game.pause_ctxt.equip_cursor == 5 && z64_file.equip_shield == 1) {
						z64_file.equip_shield = 0;
					}
					if (z64_game.pause_ctxt.equip_cursor == 6 && z64_file.equip_shield == 2) {
						z64_file.equip_shield = 0;
					}
					if (z64_game.pause_ctxt.equip_cursor == 7 && z64_file.equip_shield == 3) {
						z64_file.equip_shield = 0;
					}
				}
			}
			
		}
		
		if (z64_game.pause_ctxt.state == 0) {
			
			if (z64_file.link_age == 0) {
				if (pad_pressed.dl && z64_file.iron_boots) {
					if (z64_file.equip_boots == 2) z64_file.equip_boots = 1;
					else z64_file.equip_boots = 2;
					z64_UpdateEquipment(&z64_game, &z64_link);
					z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}

				if (pad_pressed.dr && z64_file.hover_boots) {
					if (z64_file.equip_boots == 3) z64_file.equip_boots = 1;
					else z64_file.equip_boots = 3;
					z64_UpdateEquipment(&z64_game, &z64_link);
					z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				}	
			}

			if (z64_file.link_age == 1) {
				if (pad_pressed.dr && CAN_USE_CHILD_TRADE) {
					z64_usebutton(&z64_game,&z64_link,z64_file.items[Z64_SLOT_CHILD_TRADE], 2);
				}
			}

			if (pad_pressed.dd && CAN_USE_OCARINA) {
				z64_usebutton(&z64_game,&z64_link,z64_file.items[Z64_SLOT_OCARINA], 2);
			}
		
			if (pad_pressed.du) {
				if (z64_game.hud_alpha_channels.hearts_navi != 0) { // Hide
					z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				
					z64_game.hud_alpha_channels.a_button_carots   = 0;
					z64_game.hud_alpha_channels.b_button          = 0;
					z64_game.hud_alpha_channels.cl_button         = 0;
					z64_game.hud_alpha_channels.cd_button         = 0;
					z64_game.hud_alpha_channels.cr_button         = 0;
					z64_game.hud_alpha_channels.a_button_carots   = 0;
					z64_game.hud_alpha_channels.hearts_navi       = 0;
					z64_game.hud_alpha_channels.rupees_keys_magic = 0;
					z64_game.hud_alpha_channels.minimap           = 0;
				}
				else { // Show
					z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
					
					if (z64_game.restriction_flags.b_button) { z64_game.hud_alpha_channels.b_button = 70; } else { z64_game.hud_alpha_channels.b_button = 255; }
					
					// C-Left
					if (z64_file.c_button_slots[0] == Z64_ITEM_NULL) { z64_game.hud_alpha_channels.cl_button = 255; }
					else if (z64_file.c_button_slots[0] >= Z64_ITEM_BOTTLE && z64_file.c_button_slots[0] <= Z64_ITEM_POE) {
						if (z64_game.restriction_flags.bottles) { z64_game.hud_alpha_channels.cl_button = 70; } else { z64_game.hud_alpha_channels.cl_button = 255; }
					}
					else if (z64_file.c_button_slots[0] >= Z64_ITEM_WEIRD_EGG && z64_file.c_button_slots[0] <= Z64_ITEM_CLAIM_CHECK) {
						if (z64_game.restriction_flags.trade_items) { z64_game.hud_alpha_channels.cl_button = 70; } else { z64_game.hud_alpha_channels.cl_button = 255; }
					}
					else if (z64_file.c_button_slots[0] >= Z64_ITEM_HOOKSHOT && z64_file.c_button_slots[0] <= Z64_ITEM_LONGSHOT) {
						if (z64_game.restriction_flags.hookshot) { z64_game.hud_alpha_channels.cl_button = 70; } else { z64_game.hud_alpha_channels.cl_button = 255; }
					}
					else if (z64_file.c_button_slots[0] >= Z64_ITEM_FAIRY_OCARINA && z64_file.c_button_slots[0] <= Z64_ITEM_OCARINA_OF_TIME) {
						if (z64_game.restriction_flags.ocarina) { z64_game.hud_alpha_channels.cl_button = 70; } else { z64_game.hud_alpha_channels.cl_button = 255; }
					}
					else if (z64_game.restriction_flags.all) { z64_game.hud_alpha_channels.cl_button = 70; } else { z64_game.hud_alpha_channels.cl_button = 255; }
				
					// C-Down
					if (z64_file.c_button_slots[1] == Z64_ITEM_NULL) { z64_game.hud_alpha_channels.cd_button = 255; }
					else if (z64_file.c_button_slots[1] >= Z64_ITEM_BOTTLE && z64_file.c_button_slots[1] <= Z64_ITEM_POE) {
						if (z64_game.restriction_flags.bottles) { z64_game.hud_alpha_channels.cd_button = 70; } else { z64_game.hud_alpha_channels.cd_button = 255; }
					}
					else if (z64_file.c_button_slots[1] >= Z64_ITEM_WEIRD_EGG && z64_file.c_button_slots[1] <= Z64_ITEM_CLAIM_CHECK) {
						if (z64_game.restriction_flags.trade_items) { z64_game.hud_alpha_channels.cd_button = 70; } else { z64_game.hud_alpha_channels.cd_button = 255; }
					}
					else if (z64_file.c_button_slots[1] >= Z64_ITEM_HOOKSHOT && z64_file.c_button_slots[1] <= Z64_ITEM_LONGSHOT) {
						if (z64_game.restriction_flags.hookshot) { z64_game.hud_alpha_channels.cd_button = 70; } else { z64_game.hud_alpha_channels.cd_button = 255; }
					}
					else if (z64_file.c_button_slots[1] >= Z64_ITEM_FAIRY_OCARINA && z64_file.c_button_slots[1] <= Z64_ITEM_OCARINA_OF_TIME) {
						if (z64_game.restriction_flags.ocarina) { z64_game.hud_alpha_channels.cd_button = 70; } else { z64_game.hud_alpha_channels.cd_button = 255; }
					}
					else if (z64_game.restriction_flags.all) { z64_game.hud_alpha_channels.cd_button = 70; } else { z64_game.hud_alpha_channels.cd_button = 255; }
				
					// C-Right
					if (z64_file.c_button_slots[2] == Z64_ITEM_NULL) { z64_game.hud_alpha_channels.cr_button = 255; }
					else if (z64_file.c_button_slots[2] >= Z64_ITEM_BOTTLE && z64_file.c_button_slots[2] <= Z64_ITEM_POE) {
						if (z64_game.restriction_flags.bottles) { z64_game.hud_alpha_channels.cr_button = 70; } else { z64_game.hud_alpha_channels.cr_button = 255; }
					}
					else if (z64_file.c_button_slots[2] >= Z64_ITEM_WEIRD_EGG && z64_file.c_button_slots[2] <= Z64_ITEM_CLAIM_CHECK) {
						if (z64_game.restriction_flags.trade_items) { z64_game.hud_alpha_channels.cr_button = 70; } else { z64_game.hud_alpha_channels.cr_button = 255; }
					}
					else if (z64_file.c_button_slots[2] >= Z64_ITEM_HOOKSHOT && z64_file.c_button_slots[2] <= Z64_ITEM_LONGSHOT) {
						if (z64_game.restriction_flags.hookshot) { z64_game.hud_alpha_channels.cr_button = 70; } else { z64_game.hud_alpha_channels.cr_button = 255; }
					}
					else if (z64_file.c_button_slots[2] >= Z64_ITEM_FAIRY_OCARINA && z64_file.c_button_slots[2] <= Z64_ITEM_OCARINA_OF_TIME) {
						if (z64_game.restriction_flags.ocarina) { z64_game.hud_alpha_channels.cr_button = 70; } else { z64_game.hud_alpha_channels.cr_button = 255; }
					}
					else if (z64_game.restriction_flags.all) { z64_game.hud_alpha_channels.cr_button = 70; } else { z64_game.hud_alpha_channels.cr_button = 255; }
					
					z64_game.hud_alpha_channels.a_button_carots   = 255;
					z64_game.hud_alpha_channels.hearts_navi       = 255;
					z64_game.hud_alpha_channels.rupees_keys_magic = 255;
					z64_game.hud_alpha_channels.minimap           = 170;
				}
			}
		}
    }
}
void draw_dpad() {
    z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
    if (DISPLAY_DPAD && CFG_DISPLAY_DPAD) {
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

        if (z64_file.iron_boots && z64_file.link_age==0) {
            sprite_load(db, &items_sprite, 69, 1);
            if (z64_file.equip_boots == 2) {
                sprite_draw(db, &items_sprite, 0, 258, 64, 16, 16);
            }
            else {
                sprite_draw(db, &items_sprite, 0, 260, 66, 12, 12);
            }
        }

        if (z64_file.hover_boots && z64_file.link_age == 0) {
            sprite_load(db, &items_sprite, 70, 1);
            if (z64_file.equip_boots == 3) {
                sprite_draw(db, &items_sprite, 0, 283, 64, 16, 16);
            }
            else {
                sprite_draw(db, &items_sprite, 0, 285, 66, 12, 12);
            }
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

        gDPPipeSync(db->p++);
    }
}

