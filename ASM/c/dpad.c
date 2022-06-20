#include "gfx.h"
#include "dpad.h"

extern uint8_t CFG_DISPLAY_DPAD;
extern uint8_t CFG_DPAD_ENABLED;
extern uint8_t CFG_DPAD_SHIELD_ENABLED;
extern uint8_t CFG_DPAD_TUNIC_ENABLED;
extern uint8_t CFG_UNEQUIP_GEAR_ENABLED;

//unknown 00 is a pointer to some vector transformation when the sound is tied to an actor. actor + 0x3E, when not tied to an actor (map), always 80104394
//unknown 01 is always 4 in my testing
//unknown 02 is a pointer to some kind of audio configuration Always 801043A0 in my testing
//unknown 03 is always a3 in my testing
//unknown 04 is always a3 + 0x08 in my testing (801043A8)
typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);
typedef void(*usebutton_t)(z64_game_t *game, z64_link_t *link, uint8_t item, uint8_t button);

#define z64_playsfx   ((playsfx_t)      0x800C806C)
#define z64_usebutton ((usebutton_t)    0x8038C9A0)

void change_shield(char shield) {
	z64_file.equip_shield = shield;
	change_equipment();
}

void change_tunic(char tunic) {
	z64_file.equip_tunic = tunic;
	change_equipment();
}

void change_boots(char boots) {
	z64_file.equip_boots = boots;
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
	
	if (!z64_file.link_age) {
		if (CFG_DPAD_TUNIC_ENABLED == 0) {
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
		else if (pad_pressed.dr) {
			char boots = z64_file.equip_boots;
			if (z64_file.kokiri_boots && (z64_file.iron_boots || z64_file.hover_boots) )
				boots++;
			if (boots > 3)
				boots = 1;
			
			if (boots == 1 && !z64_file.kokiri_boots)
				boots = 0;
			if (boots == 2 && !z64_file.iron_boots)
				boots++;
			if (boots == 3 && !z64_file.hover_boots)
				boots = 1;
			
			if (boots != z64_file.equip_boots)
				change_boots(boots);
		}
	}
	
	if (z64_file.link_age && pad_pressed.dr && CAN_USE_CHILD_TRADE)
		z64_usebutton(&z64_game,&z64_link,z64_file.items[Z64_SLOT_CHILD_TRADE], 2);
	
	if (pad_pressed.du && CFG_DPAD_SHIELD_ENABLED > 0) {
		char shield = z64_file.equip_shield;
		if ( (z64_file.deku_shield && (z64_file.link_age || CFG_DPAD_SHIELD_ENABLED == 3 || CFG_DPAD_SHIELD_ENABLED == 4) ) || (z64_file.hylian_shield) || (z64_file.mirror_shield && (!z64_file.link_age ||  CFG_DPAD_SHIELD_ENABLED == 2 || CFG_DPAD_SHIELD_ENABLED == 4)) )
			shield++;
		if (shield > 3) {
			shield = 0;
			if (!CFG_UNEQUIP_GEAR_ENABLED)
				shield++;
		}
		
		if (shield == 1 && ( (!z64_file.link_age && (CFG_DPAD_SHIELD_ENABLED == 1 || CFG_DPAD_SHIELD_ENABLED == 2) ) || !z64_file.deku_shield) )
			shield++;
		if (shield == 2 && !z64_file.hylian_shield)
			shield++;
		if (shield == 3 && ( (z64_file.link_age && (CFG_DPAD_SHIELD_ENABLED == 1 || CFG_DPAD_SHIELD_ENABLED == 3) ) || !z64_file.mirror_shield) ) {
			if (CFG_UNEQUIP_GEAR_ENABLED)
				shield = 0;
			else if (z64_file.deku_shield && (z64_file.link_age || CFG_DPAD_SHIELD_ENABLED == 3 || CFG_DPAD_SHIELD_ENABLED == 4))
				shield = 1;
			else if (z64_file.hylian_shield)
				shield = 2;
			else shield = 0;
		}
		
		if (shield != z64_file.equip_shield)
			change_shield(shield);
	}
	
	if (pad_pressed.dl && CFG_DPAD_TUNIC_ENABLED > 0) {
		char tunic = z64_file.equip_tunic;
		if (z64_file.kokiri_tunic || (z64_file.goron_tunic && (!z64_file.link_age || CFG_DPAD_TUNIC_ENABLED == 2) ) || (z64_file.zora_tunic && (!z64_file.link_age || CFG_DPAD_TUNIC_ENABLED == 2) ) )
			tunic++;
		if (tunic > 3)
			tunic = 1;
		
		if (tunic == 1 && !z64_file.kokiri_tunic)
			tunic++;
		if (tunic == 2 && ( (z64_file.link_age && CFG_DPAD_TUNIC_ENABLED != 2) || !z64_file.goron_tunic) )
			tunic++;
		if (tunic == 3 && ( (z64_file.link_age && CFG_DPAD_TUNIC_ENABLED != 2) || !z64_file.zora_tunic) ) {
			if (z64_file.kokiri_tunic)
				tunic = 1;
			else tunic = 0;
		}
		
		if (tunic != z64_file.equip_tunic)
			change_tunic(tunic);
	}
	
	if (pad_pressed.dd && CAN_USE_OCARINA)
		z64_usebutton(&z64_game,&z64_link,z64_file.items[Z64_SLOT_OCARINA], 2);
}

void draw_dpad() {
    if (!DISPLAY_DPAD || !CFG_DISPLAY_DPAD || CFG_DPAD_ENABLED == 0)
		return;
	z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
	
	uint16_t dpad_x = 21;
	uint16_t dpad_y = 68;
	if (CFG_DPAD_ENABLED == 2) {
		dpad_x = 271;
		uint16_t dpad_y = 64;
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
	
	if (!z64_file.link_age) {
		if (CFG_DPAD_TUNIC_ENABLED == 0) {
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
		else if (z64_file.equip_boots != 0) {
			sprite_load(db, &items_sprite, (67 + z64_file.equip_boots), 1);
			sprite_draw(db, &items_sprite, 0, (dpad_x + 14), (dpad_y + 2), 12, 12);
		}
	}
	
	if (z64_file.equip_tunic != 0 && ( (CFG_DPAD_TUNIC_ENABLED == 1 && !z64_file.link_age) || CFG_DPAD_TUNIC_ENABLED == 2) ) {
		sprite_load(db, &items_sprite, (64 + z64_file.equip_tunic), 1);
		sprite_draw(db, &items_sprite, 0, (dpad_x - 11), (dpad_y + 2), 12, 12);
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
	
	if (CFG_DPAD_SHIELD_ENABLED && z64_file.equip_shield != 0) {
		sprite_load(db, &items_sprite, (61 + z64_file.equip_shield), 1);
		sprite_draw(db, &items_sprite, 0, (dpad_x + 2), (dpad_y - 11), 12, 12);
	}

	gDPPipeSync(db->p++);
}

