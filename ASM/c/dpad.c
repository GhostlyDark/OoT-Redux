#include "gfx.h"
#include "dpad.h"
#include "arrow_toggling.h"

extern uint8_t CFG_DISPLAY_DPAD;
extern uint8_t CFG_DPAD_ENABLED;
extern uint8_t CFG_UNEQUIP_GEAR_ENABLED;

extern uint8_t  CFG_ALLOW_KOKIRI_SWORD;
extern uint8_t  CFG_ALLOW_MASTER_SWORD;
extern uint8_t  CFG_ALLOW_GIANTS_KNIFE;
extern uint8_t  CFG_ALLOW_DEKU_SHIELD;
extern uint8_t  CFG_ALLOW_MIRROR_SHIELD;
extern uint8_t  CFG_ALLOW_TUNIC;
extern uint8_t  CFG_ALLOW_BOOTS;

char DPAD_ALT	= 0;

//unknown 00 is a pointer to some vector transformation when the sound is tied to an actor. actor + 0x3E, when not tied to an actor (map), always 80104394
//unknown 01 is always 4 in my testing
//unknown 02 is a pointer to some kind of audio configuration Always 801043A0 in my testing
//unknown 03 is always a3 in my testing
//unknown 04 is always a3 + 0x08 in my testing (801043A8)
typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);
typedef void(*usebutton_t)(z64_game_t *game, z64_link_t *link, uint8_t item, uint8_t button);

#define z64_playsfx   ((playsfx_t)      0x800C806C)
#define z64_usebutton ((usebutton_t)    0x8038C9A0)

void change_sword(char sword) {
	z64_file.equip_sword = sword;
	change_equipment();
	if (z64_file.equip_sword == 0) {
		z64_file.inf_table[29]   = 1;
		z64_file.button_items[0] = -1;
	}
	else {
		z64_file.inf_table[29]   = 0;
		z64_file.button_items[0] = z64_file.equip_sword + 0x3A;
		z64_UpdateItemButton(&z64_game, 0);
	}
}

void change_boots(char boots) {
	z64_file.equip_boots = boots;
	change_equipment();
}

void change_shield(char shield) {
	z64_file.equip_shield = shield;
	change_equipment();
}

void change_tunic(char tunic) {
	z64_file.equip_tunic = tunic;
	change_equipment();
}

void change_equipment() {
	z64_UpdateEquipment(&z64_game, &z64_link);
	z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
}

void handle_dpad() {
	if (!CAN_USE_DPAD || !DISPLAY_DPAD || z64_game.pause_ctxt.state != 0 || z64_camera_view != 0 || CFG_DPAD_ENABLED == 0) 
		return;
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
	
	if (CFG_DPAD_ENABLED == 2) {
		if ( (z64_game.common.input[0].raw.pad.l && z64_game.common.input[0].pad_pressed.r) || (z64_game.common.input[0].raw.pad.r && z64_game.common.input[0].pad_pressed.l) )
			DPAD_ALT ^= 1;
	}
	
	if (DPAD_ALT)
		handle_dpad_alt(pad_pressed);
	else handle_dpad_normal(pad_pressed);
}

void handle_dpad_normal(pad_t pad_pressed) {
	if (!z64_file.link_age) {
		if (pad_pressed.du)
			handle_arrow_toggling(pad_pressed);
		
		if (pad_pressed.dl && z64_file.iron_boots) {
			if (z64_file.equip_boots == 2)
				z64_file.equip_boots = 1;
			else z64_file.equip_boots = 2;
			z64_UpdateEquipment(&z64_game, &z64_link);
			z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		}
		
		if (pad_pressed.dr && z64_file.hover_boots) {
			if (z64_file.equip_boots == 3)
				z64_file.equip_boots = 1;
			else z64_file.equip_boots = 3;
			z64_UpdateEquipment(&z64_game, &z64_link);
			z64_playsfx(0x835, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		}
	}
	
	if (z64_file.link_age && pad_pressed.dr && CAN_USE_CHILD_TRADE)
		z64_usebutton(&z64_game,&z64_link,z64_file.items[Z64_SLOT_CHILD_TRADE], 2);
	
	if (pad_pressed.dd && CAN_USE_OCARINA)
		z64_usebutton(&z64_game,&z64_link,z64_file.items[Z64_SLOT_OCARINA], 2);
}

void handle_dpad_alt(pad_t pad_pressed) {
	if (pad_pressed.du) {
		if ( (z64_file.kokiri_sword && (z64_file.link_age || CFG_ALLOW_KOKIRI_SWORD) ) || (z64_file.master_sword && (!z64_file.link_age || CFG_ALLOW_MASTER_SWORD) ) || (z64_file.giants_knife && (!z64_file.link_age || CFG_ALLOW_GIANTS_KNIFE) ) ) {
			char sword = z64_file.equip_sword;
			sword++;
			if (sword > 3) {
				sword = 0;
				if (!CFG_UNEQUIP_GEAR_ENABLED)
					sword++;
			}
			
			if (sword == 1 && (!z64_file.link_age && !CFG_ALLOW_KOKIRI_SWORD) || !z64_file.kokiri_sword)
				sword++;
			if (sword == 2 && ( z64_file.link_age && !CFG_ALLOW_MASTER_SWORD) || !z64_file.master_sword)
				sword++;
			if (sword == 3 && ( z64_file.link_age && !CFG_ALLOW_GIANTS_KNIFE) || !z64_file.giants_knife) {
				if (CFG_UNEQUIP_GEAR_ENABLED)
					sword = 0;
				else if (z64_file.kokiri_sword && ( z64_file.link_age || CFG_ALLOW_KOKIRI_SWORD) )
					sword = 1;
				else if (z64_file.master_sword && (!z64_file.link_age || CFG_ALLOW_MASTER_SWORD) )
					sword = 2;
				else sword = 0;
			}
			
			if (sword != z64_file.equip_sword)
				change_sword(sword);
		}
	}
	
	if (pad_pressed.dr && (!z64_file.link_age || CFG_ALLOW_BOOTS) ) {
		if (z64_file.kokiri_boots && (z64_file.iron_boots || z64_file.hover_boots) ) {
			char boots = z64_file.equip_boots;
			boots++;
			if (boots > 3)
				boots = 1;
			
			if (boots == 2 && !z64_file.iron_boots)
				boots++;
			if (boots == 3 && !z64_file.hover_boots)
				boots = 1;
				
			if (boots != z64_file.equip_boots)
				change_boots(boots);
		}
	}
	
	if (pad_pressed.dd) {
		if ( (z64_file.deku_shield && (z64_file.link_age || CFG_ALLOW_DEKU_SHIELD) ) || z64_file.hylian_shield || (z64_file.mirror_shield && (!z64_file.link_age || CFG_ALLOW_MIRROR_SHIELD) ) ) {
			char shield = z64_file.equip_shield;
			shield++;
			if (shield > 3) {
				shield = 0;
				if (!CFG_UNEQUIP_GEAR_ENABLED)
					shield++;
			}
			
			if (shield == 1 && (!z64_file.link_age && !CFG_ALLOW_DEKU_SHIELD)   || !z64_file.deku_shield)
				shield++;
			if (shield == 2 && !z64_file.hylian_shield)
				shield++;
			if (shield == 3 && ( z64_file.link_age && !CFG_ALLOW_MIRROR_SHIELD) || !z64_file.mirror_shield) {
				if (CFG_UNEQUIP_GEAR_ENABLED)
					shield = 0;
				else if (z64_file.deku_shield && (z64_file.link_age || CFG_ALLOW_DEKU_SHIELD) )
					shield = 1;
				else if (z64_file.hylian_shield)
					shield = 2;
				else shield = 0;
			}
			
			if (shield != z64_file.equip_shield)
				change_shield(shield);
		}
	}
	
	if (pad_pressed.dl && (!z64_file.link_age || CFG_ALLOW_TUNIC) ) {
		if (z64_file.kokiri_tunic && (z64_file.goron_tunic || z64_file.zora_tunic) ) {
			char tunic = z64_file.equip_tunic;
			tunic++;
			if (tunic > 3)
				tunic = 1;
		
			if (tunic == 2 && !z64_file.goron_tunic)
				tunic++;
			if (tunic == 3 && !z64_file.zora_tunic)
				tunic = 1;
		
			if (tunic != z64_file.equip_tunic)
				change_tunic(tunic);
		}
	}
}

void draw_dpad() {
    if (!DISPLAY_DPAD || CFG_DISPLAY_DPAD == 0 || CFG_DPAD_ENABLED == 0)
		return;
	z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
	
	uint16_t dpad_x = 21;
	uint16_t dpad_y = 44;
	if (CFG_DISPLAY_DPAD != 2) {
		if (z64_file.magic_acquired && z64_file.energy_capacity > 0xA0)
			dpad_y += 23;
		else if (z64_file.energy_capacity > 0xA0)
			dpad_y += 10;
		else if (z64_file.magic_acquired)
			dpad_y += 14;
	}
	else {
		dpad_x = 271;
		dpad_y = 64;
	}
	
	gSPDisplayList(db->p++, &setup_db);
	gDPPipeSync(db->p++);
	gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
	uint16_t alpha = z64_game.hud_alpha_channels.minimap;
	
	if (alpha == 0xAA)
		alpha = 0xFF;
	gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
	sprite_load(db, &dpad_sprite, 0, 1);
	sprite_draw(db, &dpad_sprite, 0, dpad_x, dpad_y, 16, 16);
	
	if (alpha == 0xFF && !CAN_USE_DPAD && z64_game.pause_ctxt.state != 0)
		gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
	
	if (DPAD_ALT) {
		if (z64_file.equip_sword != 0) {
			sprite_load(db, &items_sprite, (58 + z64_file.equip_sword), 1);
			sprite_draw(db, &items_sprite, 0, (dpad_x + 2), (dpad_y - 10), 12, 12);
		}
		if (z64_file.equip_boots != 0) {
			sprite_load(db, &items_sprite, (67 + z64_file.equip_boots), 1);
			sprite_draw(db, &items_sprite, 0, (dpad_x + 14), (dpad_y + 2), 12, 12);
		}
		if (z64_file.equip_shield != 0) {
			sprite_load(db, &items_sprite, (61 + z64_file.equip_shield), 1);
			sprite_draw(db, &items_sprite, 0, (dpad_x + 2), (dpad_y + 14), 12, 12);
		}
		if (z64_file.equip_tunic != 0) {
			sprite_load(db, &items_sprite, (64 + z64_file.equip_tunic), 1);
			sprite_draw(db, &items_sprite, 0, (dpad_x - 11), (dpad_y + 2), 12, 12);
		}
	}
	else {
		if (!z64_file.link_age) {
			if (z64_file.iron_boots) {
				sprite_load(db, &items_sprite, 69, 1);
				if (z64_file.equip_boots == 2)
					sprite_draw(db, &items_sprite, 0, (dpad_x - 2 - 11), (dpad_y), 16, 16);
				else sprite_draw(db, &items_sprite, 0, (dpad_x - 11), (dpad_y + 2), 12, 12);
			}
			if (z64_file.hover_boots) {
				sprite_load(db, &items_sprite, 70, 1);
				if (z64_file.equip_boots == 3)
					sprite_draw(db, &items_sprite, 0, (dpad_x - 2 + 14), (dpad_y), 16, 16);
				else sprite_draw(db, &items_sprite, 0, (dpad_x + 14), (dpad_y + 2), 12, 12);
			}
		}
	
		if (z64_file.link_age) {
			if (z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH && z64_file.link_age) {
				if(alpha==0xFF && !CAN_USE_CHILD_TRADE)
					gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
				else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
				sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_CHILD_TRADE], 1);
				sprite_draw(db, &items_sprite, 0, (dpad_x + 14), (dpad_y + 2), 12, 12);
			}
		}

		if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) {
			if(alpha==0xFF && !CAN_USE_OCARINA)
				gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0x46);
			else gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
			sprite_load(db, &items_sprite, z64_file.items[Z64_SLOT_OCARINA], 1);
			sprite_draw(db, &items_sprite, 0, (dpad_x + 2), (dpad_y + 13), 12,12);
		}
	}

	gDPPipeSync(db->p++);
}

