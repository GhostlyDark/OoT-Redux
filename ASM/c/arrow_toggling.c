#include "arrow_toggling.h"

extern uint8_t CFG_ARROW_TOGGLING_ENABLED;

typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);

#define z64_playsfx   ((playsfx_t)      0x800C806C)

void handle_arrow_toggling(pad_t pad_pressed) {
	/*if (z64_game.pause_ctxt.state != 0 || !pad_pressed.r || z64_camera_view != 2 || z64_file.items[Z64_SLOT_BOW] != Z64_ITEM_BOW || !CFG_ARROW_TOGGLING_ENABLED)
		return;
	
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
	}*/
}

