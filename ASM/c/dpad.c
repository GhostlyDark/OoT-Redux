#include "gfx.h"
#include "dpad.h"
#include "trade_quests.h"
#include "dpad_paused.h"
#include "dpad_actions.h"
#include "options.h"
#include "buttons.h"
#include "fps.h"

extern uint8_t CFG_WS;
extern uint8_t CFG_TYCOON_WALLET;
extern uint8_t CFG_OPTIONS_MENU;

uint8_t  dpad_alt		 = 0;
uint16_t dpad_x			 = 0;
uint16_t dpad_y			 = 0;
uint8_t last_mask		 = 0;
int8_t  last_mask_age 	 = -1;
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
		handle_layout();
		handle_hud();
		handle_fps();
		
		if (SAVE_DOWNGRADE_ITEM)
			if (!z64_file.bgs_flag && z64_file.equip_sword == 3)
				SWORD_HEALTH = z64_file.bgs_hits_left;
		
		if (SAVE_KEEP_MASK) {
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
				if (z64_camera_view == 1 || z64_camera_view == 2)
					z64_y_axis_input *= -1;
		
			if (SAVE_NO_IDLE_CAMERA)
				if (z64_idle_camera_counter < 10)
					z64_idle_camera_counter = 10;
		}
		
		if (CFG_OPTIONS_MENU >= 2) {
			handle_rupee_dash();
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
		
		if (!DPAD_INIT_SETUP) {
			EXTRA_SRAM_1 |= 1;      // Init
			EXTRA_SRAM_2 |= 1 << 6; // Keep Mask
			EXTRA_SRAM_4 |= 2;      // D-Pad
			EXTRA_SRAM_4 |= 1 << 2; // Show D-Pad
	
			DPAD_ADULT_UP		= DPAD_ARROWS		* 16 + DPAD_SWORD;
			DPAD_ADULT_RIGHT	= DPAD_HOVER_BOOTS  * 16 + DPAD_BOOTS;
			DPAD_ADULT_DOWN		= DPAD_OCARINA		* 16 + DPAD_SHIELD;
			DPAD_ADULT_LEFT		= DPAD_IRON_BOOTS	* 16 + DPAD_TUNIC;
			DPAD_CHILD_UP		= DPAD_LENS			* 16 + DPAD_SWORD;
			DPAD_CHILD_RIGHT	= DPAD_CHILD_TRADE	* 16 + DPAD_BOOTS;
			DPAD_CHILD_DOWN		= DPAD_OCARINA		* 16 + DPAD_SHIELD;
			DPAD_CHILD_LEFT		= DPAD_NULL			* 16 + DPAD_TUNIC;
		}
	}
}

void handle_dpad_ingame() {
	if (!CAN_USE_DPAD || z64_camera_view != 0 || SAVE_DPAD == 0) 
		return;
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
//<<<<<<< HEAD
    pad_t pad_held = z64_ctxt.input[0].raw.pad;

    if (CAN_USE_TRADE_DPAD) {
        uint8_t current_trade_item = z64_file.items[z64_game.pause_ctxt.item_cursor];
        if (IsTradeItem(current_trade_item)) {
            uint8_t potential_trade_item = current_trade_item;

            if (pad_pressed.dl) {
                potential_trade_item = SaveFile_PrevOwnedTradeItem(current_trade_item);
            }

            if (pad_pressed.dr) {
                potential_trade_item = SaveFile_NextOwnedTradeItem(current_trade_item);
            }

            if (current_trade_item != potential_trade_item) {
                UpdateTradeEquips(potential_trade_item, z64_game.pause_ctxt.item_cursor);
                PlaySFX(0x4809); // cursor move sound effect NA_SE_SY_CURSOR
            }
        }
    } else if (CAN_USE_DPAD && DISPLAY_DPAD && (!pad_held.a || !CAN_DRAW_DUNGEON_INFO)) {
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
    }
}

void draw_dpad() {
    z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
    if (CAN_DRAW_DUNGEON_INFO || (DISPLAY_DPAD && CFG_DISPLAY_DPAD) || CAN_DRAW_TRADE_DPAD) {
        gSPDisplayList(db->p++, &setup_db);
        gDPPipeSync(db->p++);
        gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        uint16_t alpha = z64_game.hud_alpha_channels.rupees_keys_magic;

        gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
        sprite_load(db, &dpad_sprite, 0, 1);
        int left = 271;
        if (CAN_DRAW_TRADE_DPAD) {
            uint8_t current_trade_item = z64_file.items[z64_game.pause_ctxt.item_cursor];
            if (IsTradeItem(current_trade_item)) {
                uint8_t prev_trade_item = SaveFile_PrevOwnedTradeItem(current_trade_item);
                uint8_t next_trade_item = SaveFile_NextOwnedTradeItem(current_trade_item);

                if (current_trade_item != next_trade_item) {
                    // D-pad under selected trade item slot, if more than one trade item
                    left = (z64_game.pause_ctxt.item_cursor == Z64_SLOT_ADULT_TRADE) ? 197 : 230;
                    sprite_draw(db, &dpad_sprite, 0, left, 190, 24, 24);

                    // Previous trade quest item
                    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
                    sprite_load(db, &items_sprite, prev_trade_item, 1);
                    sprite_draw(db, &items_sprite, 0, left - 16, 194, 16, 16);

                    // Next trade quest item
                    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
                    sprite_load(db, &items_sprite, next_trade_item, 1);
                    sprite_draw(db, &items_sprite, 0, left + 24, 194, 16, 16);
                }
            }
        }
        if (left == 271) {
            // D-pad under C buttons, if trade slot selector not drawn
            sprite_draw(db, &dpad_sprite, 0, left, 64, 16, 16);
        }

        if (CAN_DRAW_DUNGEON_INFO && CFG_DPAD_DUNGEON_INFO_ENABLE && left == 271) {
            // Zora sapphire on D-down
            sprite_load(db, &stones_sprite, 2, 1);
            sprite_draw(db, &stones_sprite, 0, 273, 77, 12, 12);

            // small key on D-right
            sprite_load(db, &quest_items_sprite, 17, 1);
            sprite_draw(db, &quest_items_sprite, 0, 285, 66, 12, 12);

            // map on D-left
            sprite_load(db, &quest_items_sprite, 16, 1);
            sprite_draw(db, &quest_items_sprite, 0, 260, 66, 12, 12);
        } else if (left == 271) {
            if (!CAN_USE_DPAD)
                gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha * 0x46 / 0xFF);

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
                if(!CAN_USE_DPAD || !CAN_USE_CHILD_TRADE) gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha * 0x46 / 0xFF);
                else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
                sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_CHILD_TRADE], 1);
                if (z64_link.current_mask >= 1 && z64_link.current_mask <= 9) {
                    sprite_draw(db, &items_sprite, 0, 283, 64, 16, 16);
                }
                else {
                    sprite_draw(db, &items_sprite, 0, 285, 66, 12, 12);
                }
            }

            if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) {
                if(!CAN_USE_DPAD || !CAN_USE_OCARINA) gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha * 0x46 / 0xFF);
                else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
                sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_OCARINA], 1);
                sprite_draw(db, &items_sprite, 0, 273, 77, 12,12);
            }
        }

        gDPPipeSync(db->p++);
    }
//=======
	
	if (SAVE_DPAD == 2) {
		if ( (z64_game.common.input[0].raw.pad.l && pad_pressed.r) || (z64_game.common.input[0].raw.pad.r && pad_pressed.l) ) {
			dpad_alt ^= 1;
			CHECKED_LENS = 0;
			if (dpad_alt)
				play_sfx = 0x4813;
			else 0x4814;
		}
	}
	
	if (z64_game.pause_ctxt.state == 0 && SAVE_DPAD > 0) 
		run_dpad_actions(pad_pressed);
}

void draw_dpad() {
	z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
	gSPDisplayList(db->p++, &setup_db);
	gDPPipeSync(db->p++);
	
	if (!draw_settings_menu(db))
		if (!draw_abilities_info(db))
			draw_dpad_icons(db);
//>>>>>>> a22ed401 (Redux (2023-04-04))
}

void draw_dpad_icons(z64_disp_buf_t *db) {
	if (!CAN_DRAW_HUD || SAVE_DPAD == 0 || (SAVE_SHOW_DPAD == 0 && z64_game.pause_ctxt.state != 6) || (z64_game.pause_ctxt.state > 0x7 && z64_game.pause_ctxt.state < 0x12) )
		return;
	
	uint8_t *dpad_active = check_dpad_actions();
	if (z64_game.pause_ctxt.state == 0)
		if (!dpad_active[0] && !dpad_active[1] && !dpad_active[2] && !dpad_active[3])
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
		if (z64_file.timer_1_state > 0 || z64_file.timer_2_state > 0)
			dpad_y += 18;
	}
	
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 255);
	sprite_load(db, &dpad_sprite, 0, 1);
	sprite_draw(db, &dpad_sprite, 0, dpad_x, dpad_y, 16, 16);
	
	draw_dpad_actions(db);
}