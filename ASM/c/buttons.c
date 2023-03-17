#include "buttons.h"

extern uint8_t CFG_WS;

uint8_t hud_hide			= 0;
uint8_t hud_hearts_hide		= 1;
uint8_t hud_counter			= 0;
uint8_t block				= 0;
uint8_t pressed_r			= 0;
uint8_t pressed_z			= 0;
uint8_t pressed_x			= 0;
uint8_t pressed_y			= 0;

uint8_t rupee_dash_frames	= 0;
uint8_t rupee_dash_secs		= 0;
uint8_t restore_frames		= 0;
uint8_t restore_secs		= 0;
uint8_t magic_frames		= 0;
uint8_t magic_secs			= 0;
uint8_t restore_health		= 0;
uint16_t last_health		= 0;

void handle_rupee_dash() {
	if (SAVE_RUPEE_DRAIN == 0)
		return;
	
	if (z64_file.energy > 1) {
		rupee_dash_frames++;
		uint8_t compare_frames = 20;
		if (fps_limit == 2)
			compare_frames *= 1.5;
	
		if (rupee_dash_frames >= compare_frames) {
			rupee_dash_frames = 0;
			rupee_dash_secs++;
		}
	
		if (rupee_dash_secs >= SAVE_RUPEE_DRAIN) {
			rupee_dash_secs = 0;
		
			if (z64_file.rupees > 0)
				z64_file.rupees--;
			else {
				if (z64_file.energy > 7)
					z64_file.energy -= 4;
				else z64_file.energy = 1;
				z64_LinkInvincibility(&z64_link, 0x14);
				uint16_t sfx = 0x6805;
				if (z64_file.link_age)
					sfx = 0x6825;
				z64_playsfx(sfx, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
		}
	}
	else rupee_dash_frames = rupee_dash_secs = 0;
}

void handle_power_crouch_stab_fix() {
	if (!SAVE_CROUCH_STAB_FIX || z64_file.equip_sword == 0)
		return;
	
	if (z64_file.equip_sword == 1)
		z64_sword_damage_1 = z64_sword_damage_2 = 1;
	else if (z64_file.equip_sword == 2)
		z64_sword_damage_1 = z64_sword_damage_2 = 2;
	else if (z64_file.equip_sword == 3 && !z64_file.broken_giants_knife)
		z64_sword_damage_1 = z64_sword_damage_2 = 4;
	else if (z64_file.equip_sword == 3 && z64_file.broken_giants_knife)
		z64_sword_damage_1 = z64_sword_damage_2 = 1;
}

void handle_weaker_swords() {
	if (SAVE_WEAKER_SWORDS)
		if (z64_sword_damage_1 > 1 && z64_file.equip_sword > 0)
			z64_sword_damage_2 = z64_sword_damage_1 - 1;
}

void handle_abilities_tunic_colors() {
	if (!SAVE_EXTRA_ABILITIES || z64_file.game_mode != 0)
		return;
	
	if (z64_file.forest_medallion && z64_file.equip_tunic == 1)
		z64_tunic_color = COLOR_FOREST;
	else if (z64_file.fire_medallion && z64_file.equip_tunic == 2)
		z64_tunic_color = COLOR_FIRE;
	else if (z64_file.water_medallion && z64_file.equip_tunic == 3)
		z64_tunic_color = COLOR_WATER;
	else if (z64_file.shadow_medallion && z64_file.equip_tunic == 0)
		z64_tunic_color = COLOR_SHADOW;
}

void handle_abilities() {
	if (!SAVE_EXTRA_ABILITIES)
		return;
	
	if (restore_health == 0 && z64_damage_frames == 0)
		last_health = z64_file.energy;
	
	uint8_t compare_frames = 20;
	if (fps_limit == 2)
		compare_frames *= 1.5;
	
	if (z64_file.light_medallion && (z64_file.equip_boots == 1 || (z64_file.spirit_medallion && z64_file.equip_boots == 3) ) && z64_game.common.input[0].raw.pad.l) {
		if (z64_link_animation_parameter == 0x3FA0)
			z64_link_a_action = 0x40FF;
	}
	
	if (z64_file.forest_medallion && z64_file.equip_tunic == 1 && HAS_MAGIC) {
		if ( (z64_file.magic < 0x30 && !z64_file.magic_capacity) || (z64_file.magic < 0x60 && z64_file.magic_capacity) ) {
			if (z64_damage_frames == 14 && restore_health == 0)
				restore_health = 1;
			else if (z64_damage_frames == 0)
				restore_health = 0;
			
			if (restore_health == 1) {
				restore_health = 2;
				z64_file.magic += 4;
				z64_playsfx(0x480B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				
				if (z64_file.magic > 0x60 && z64_file.magic_capacity)
					z64_file.magic = 0x60;
				else if (z64_file.magic > 0x30)
					z64_file.magic = 0x30;
			}
		}
	}
		
	if (z64_file.fire_medallion && z64_file.equip_tunic == 2 && HAS_MAGIC && z64_file.magic > 0) {
		if (z64_damage_frames == 14 && restore_health == 0)
			restore_health = 1;
		else if (z64_damage_frames == 0)
			restore_health = 0;
		
		if (restore_health == 1) {
			restore_health = 2;
			z64_file.energy += (last_health - z64_file.energy) / 2;
			z64_playsfx(0x481E, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
				
			if (z64_file.magic >= 4)
				z64_file.magic -= 4;
			else z64_file.magic = 0;
			
			if (z64_file.energy > z64_file.energy_capacity)
				z64_file.energy = z64_file.energy_capacity;
		}
	}
		
	if (z64_file.water_medallion && z64_file.equip_tunic == 3 && z64_sword_damage_1 > 0 && z64_file.equip_sword > 0) {
		if (SAVE_WEAKER_SWORDS)
			z64_sword_damage_2 = z64_sword_damage_1;
		else z64_sword_damage_2 = z64_sword_damage_1 + 1;
	}
		
	if (z64_file.shadow_medallion && z64_file.equip_tunic == 0) {
		if (z64_file.energy < z64_file.energy_capacity && z64_file.magic > 0 && HAS_MAGIC) {
			restore_frames++;
		
			if (restore_frames >= compare_frames) {
				restore_frames = 0;
				restore_secs++;
			}
		
			if (restore_secs >= 3) {
				restore_secs = 0;
				z64_file.energy += 4;
				if (z64_file.magic >= 4)
					z64_file.magic -= 4;
				else z64_file.magic = 0;
				z64_playsfx(0x480B, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
			}
		}
		else restore_frames = restore_secs = 0;
	}
		
	if (z64_file.spirit_medallion ) {
		z64_inner_red_trail_r = z64_outer_red_trail_r = 0xFF;
		z64_inner_red_trail_g = z64_outer_red_trail_g = 0xD7;
		z64_inner_red_trail_b = z64_outer_red_trail_b = 0x00;
	}
	
	if (z64_file.kokiris_emerald && (z64_file.equip_boots == 1 || (z64_file.spirit_medallion && z64_file.equip_boots == 3) ) && z64_game.common.input[0].raw.pad.l && HAS_MAGIC && z64_file.magic > 0) {
		uint16_t extra_speed = 0x50;
		uint16_t magic_cost  = 2;
		if (z64_file.gorons_ruby)
			extra_speed = 0xA0;
		if (z64_file.zoras_sapphire)
			magic_cost = 1;
		
		uint16_t speed = 0x40B0;
		if (!z64_file.link_age)
			speed = 0x40C0;
		if (z64_move_speed >= speed - 0x1A) {
			magic_frames++;
				
			if (magic_frames >= compare_frames/2) {
				magic_frames = 0;
				magic_secs++;
			}
				
			if (magic_secs >= 1) {
				magic_secs = 0;
				if (z64_file.magic >= magic_cost)
					z64_file.magic -= magic_cost;
				else z64_file.magic = 0;
			}
				
			z64_move_speed = speed + extra_speed;
		}
	}
	else magic_frames = magic_secs = 0;
}

void handle_l_button() {
	if (z64_game.pause_ctxt.state != 0 && z64_game.pause_ctxt.state != 6)
		return;
	
	pad_t pad_released = z64_game.common.input[0].pad_released;
	
	uint8_t toggle = 0;
	
	if (z64_game.common.input[0].raw.pad.r && SAVE_DPAD == 2)
		pressed_r = 1;
	if (z64_game.common.input[0].raw.pad.z && SAVE_30_FPS)
		pressed_z = 1;
	if (z64_game.common.input[0].raw.x != 0 && SAVE_EXTRA_ABILITIES)
		pressed_x = 1;
	if (z64_game.common.input[0].raw.y != 0 && SAVE_EXTRA_ABILITIES)
		pressed_y = 1;
	if (pad_released.l && !pressed_r && !pressed_z && !pressed_x && !pressed_y) {
		toggle = 1;
		settings_menu();
		
		if (z64_game.pause_ctxt.unk_02_[1] == 2) {
			z64_game.pause_ctxt.unk_02_[1] = 3;
			z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		}
	}
	if (!z64_game.common.input[0].raw.pad.l)
		pressed_r = pressed_z = pressed_x = pressed_y = 0;
	
	if (z64_game.common.input[0].pad_pressed.l && !z64_game.common.input[0].raw.pad.r)
		block = 1;
	else if (z64_game.common.input[0].pad_pressed.l && !z64_game.common.input[0].raw.pad.z)
		block = 1;
	if (!z64_game.common.input[0].raw.pad.l)
		block = 0;
	
	if (block) {
		z64_game.common.input[0].raw.pad.r = z64_game.common.input[0].pad_pressed.r = 0;
		z64_game.common.input[0].raw.pad.z = z64_game.common.input[0].pad_pressed.z = 0;
	}
	
	z64_game.common.input[0].pad_pressed.l = 0;	
	
	if (toggle && z64_game.pause_ctxt.state == 0 && !hud_hide)
		z64_button_input |= 0x20;
}

void handle_layout() {
	if (z64_game.pause_ctxt.state == 6) {
		if (SAVE_HUD_LAYOUT <= 1) { // Vanilla & Majora's Mask
			if (!CFG_WS) {
				z64_c_left_x_set_item	= 0x294;
				z64_c_down_x_set_item	= 0x384;
				z64_c_right_x_set_item	= 0x474;
			}
			else {
				z64_c_left_x_set_item	= 0x49C;
				z64_c_down_x_set_item	= 0x59C;
				z64_c_right_x_set_item	= 0x67C;
			}
			z64_c_left_y_set_item	= 0x44C;
			z64_c_down_y_set_item	= 0x398;
			z64_c_right_y_set_item	= 0x44C;
		}
		else if (SAVE_HUD_LAYOUT == 2) { // Nintendo
			if (!CFG_WS) {
				z64_c_left_x_set_item	= 0x343;
				z64_c_down_x_set_item	= 0x4FB;
				z64_c_right_x_set_item	= 0x1D1;
			}
			else {
				z64_c_left_x_set_item	= 0x54B;
				z64_c_down_x_set_item	= 0x703;
				z64_c_right_x_set_item	= 0x3D9;
			}
			z64_c_left_y_set_item	= 0x44C;
			z64_c_down_y_set_item	= 0x492;
			z64_c_right_y_set_item	= 0x352;
		}
		else if (SAVE_HUD_LAYOUT == 3) { // Modern
			if (!CFG_WS) {
				z64_c_left_x_set_item	= 0x217;
				z64_c_down_x_set_item	= 0x4FB;
				z64_c_right_x_set_item	= 0x2FD;
			}
			else {
				z64_c_left_x_set_item	= 0x41F;
				z64_c_down_x_set_item	= 0x703;
				z64_c_right_x_set_item	= 0x505;
			}
			z64_c_left_y_set_item	= 0x352;
			z64_c_down_y_set_item	= 0x492;
			z64_c_right_y_set_item	= 0x44C;
		}
		else if (SAVE_HUD_LAYOUT == 4) { // GameCube (Original)
			if (!CFG_WS) {	
				z64_c_left_x_set_item	= 0x3C0;
				z64_c_down_x_set_item	= 0x4FB;
				z64_c_right_x_set_item	= 0x4FE;
			}
			else {
				z64_c_left_x_set_item	= 0x5C8;
				z64_c_down_x_set_item	= 0x703;
				z64_c_right_x_set_item	= 0x706;
			}
			z64_c_left_y_set_item	= 0x4C9;
			z64_c_down_y_set_item	= 0x492;
			z64_c_right_y_set_item	= 0x314;
		}
		else if (SAVE_HUD_LAYOUT == 5) { // GameCube (Modern)
			if (!CFG_WS) {	
				z64_c_left_x_set_item	= 0x544;
				z64_c_down_x_set_item	= 0x4FB;
				z64_c_right_x_set_item	= 0x37A;
			}
			else {
				z64_c_left_x_set_item	= 0x74C;
				z64_c_down_x_set_item	= 0x703;
				z64_c_right_x_set_item	= 0x582;
			}
			z64_c_left_y_set_item	= 0x314;
			z64_c_down_y_set_item	= 0x492;
			z64_c_right_y_set_item	= 0x4C9;
		}
	}
	
	if (SAVE_HUD_LAYOUT == 0 || !CAN_DRAW_HUD || z64_gameinfo.a_button_y != 0x09)
		return;
	
	uint16_t a_x = 0, a_y = 0, b_x = 0, b_y = 0, c_left_x = 0, c_left_y = 0, c_down_x = 0, c_down_y = 0, c_right_x = 0, c_right_y = 0, c_up_x = 0, c_up_y = 0;
	
	if (SAVE_HUD_LAYOUT == 1) { // Majora's Mask
		a_x			= 4;	// 186	->	190
		a_y			= 14;	// 9	->	23
		b_x			= 7;	// 160	->	167
	}
	else if (SAVE_HUD_LAYOUT == 2) { // Nintendo
		a_x			= 70;	// 186	->	256
		a_y			= 23;	// 9	->	32
		b_x			= 80;	// 160	->	240
		b_y			= 45;	// 11	->	56
		c_left_x	= 14;	// 227	->	241
		c_left_y	= 0;	// 18	->	18
		c_down_x	= 30;	// 249	->	279
		c_down_y	= -20;	// 34	->	14
		c_right_x	= -54;	// 271	->	217
		c_right_y	= 20;	// 18	->	38
		c_up_x		= 10;	// 254	->	264
		c_up_y		= -10;	// 16	->	6
	}
	else if (SAVE_HUD_LAYOUT == 3) { // Modern
		a_x			= 46;	// 186	->	234
		a_y			= 45;	// 9	->	54
		b_x			= 104;	// 160	->	264
		b_y			= 23;	// 11	->	33
		c_left_x	= -10;	// 227	->	217
		c_left_y	= 20;	// 18	->	38
		c_down_x	= 30;	// 249	->	279		
		c_down_y	= -20;	// 34	->	14		
		c_right_x	= -30;	// 271	->	241
		c_right_y	= 0;	// 18	->	18
		c_up_x		= 10;	// 254	->	264		
		c_up_y		= -10;	// 16	->	6		
	}
	else if (SAVE_HUD_LAYOUT == 4) { // GameCube (Original)
		a_x			= 55;	// 186	->	241
		a_y			= 20;	// 9	->	29
		b_x			= 65;	// 160	->	225
		b_y			= 40;	// 11	->	51
		c_left_x	= 24;	// 227	->	251
		c_left_y	= -10;	// 18	->	8
		c_down_x	= 30;	// 249	->	279
		c_down_y	= -20;	// 34	->	14
		c_right_x	= 11;	// 271	->	282
		c_right_y	= 25;	// 18	->	43
		c_up_x		= -20;	// 254	->	234
	}
	else if (SAVE_HUD_LAYOUT == 5) { // GameCube (Modern)
		a_x			= 55;	// 186	->	241
		a_y			= 20;	// 9	->	29
		b_x			= 65;	// 160	->	225
		b_y			= 40;	// 11	->	51
		c_left_x	= 55;	// 227	->	282
		c_left_y	= 25;	// 18	->	43
		c_down_x	= 30;	// 249	->	279
		c_down_y	= -20;	// 34	->	14
		c_right_x	= -20;	// 271	->	251
		c_right_y	= -10;	// 18	->	8
		c_up_x		= -20;	// 254	->	234
	}
	
	z64_gameinfo.a_button_x			+= a_x;
	z64_gameinfo.a_button_y			+= a_y;
	z64_gameinfo.a_button_icon_x	+= a_x;
	z64_gameinfo.a_button_icon_y	+= a_y;
	z64_gameinfo.item_button_x[0]	+= b_x;
	z64_gameinfo.item_button_y[0]	+= b_y;
	z64_gameinfo.item_icon_x[0]		+= b_x;
	z64_gameinfo.item_icon_y[0]		+= b_y;
	z64_gameinfo.item_ammo_x[0]		+= b_x;
	z64_gameinfo.item_ammo_y[0]		+= b_y;
	z64_b_button_label_x			+= b_x;
	z64_b_button_label_y			+= b_y;
	z64_gameinfo.item_button_x[1]	+= c_left_x;
	z64_gameinfo.item_button_y[1]	+= c_left_y;
	z64_gameinfo.item_icon_x[1]		+= c_left_x;
	z64_gameinfo.item_icon_y[1]		+= c_left_y;
	z64_gameinfo.item_ammo_x[1]		+= c_left_x;
	z64_gameinfo.item_ammo_y[1]		+= c_left_y;
	z64_gameinfo.item_button_x[2]	+= c_down_x;
	z64_gameinfo.item_button_y[2]	+= c_down_y;
	z64_gameinfo.item_icon_x[2]		+= c_down_x;
	z64_gameinfo.item_icon_y[2]		+= c_down_y;
	z64_gameinfo.item_ammo_x[2]		+= c_down_x;
	z64_gameinfo.item_ammo_y[2]		+= c_down_y;
	z64_gameinfo.item_button_x[3]	+= c_right_x;
	z64_gameinfo.item_button_y[3]	+= c_right_y;
	z64_gameinfo.item_icon_x[3]		+= c_right_x;
	z64_gameinfo.item_icon_y[3]		+= c_right_y;
	z64_gameinfo.item_ammo_x[3]		+= c_right_x;
	z64_gameinfo.item_ammo_y[3]		+= c_right_y;
	z64_gameinfo.c_up_button_x		+= c_up_x;
	z64_gameinfo.c_up_button_y		+= c_up_y;
	z64_gameinfo.c_up_icon_x		+= c_up_x;
	z64_gameinfo.c_up_icon_y		+= c_up_y;
}

void reset_layout() {
	z64_gameinfo.a_button_y			= z64_gameinfo.a_button_icon_y = 0x9;
	z64_gameinfo.item_button_y[0]	= z64_gameinfo.item_icon_y[0]  = 0x11;
	z64_gameinfo.item_ammo_y[0]		= 0x23;
	z64_b_button_label_y			= 0x16;
	z64_gameinfo.item_button_y[1]	= z64_gameinfo.item_icon_y[1] = 0x12;
	z64_gameinfo.item_ammo_y[1]		= 0x23;
	z64_gameinfo.item_button_y[2]	= z64_gameinfo.item_icon_y[2] = 0x22;
	z64_gameinfo.item_ammo_y[2]		= 0x33;
	z64_gameinfo.item_button_y[3]	= z64_gameinfo.item_icon_y[3] = 0x12;
	z64_gameinfo.item_ammo_y[3]		= 0x23;
	z64_gameinfo.c_up_button_y		= 0x10;
	z64_gameinfo.c_up_icon_y		= 0x14;
	
	if (!CFG_WS) {
		z64_gameinfo.a_button_x			= z64_gameinfo.a_button_icon_x = 0xBA;
		z64_gameinfo.item_button_x[0]	= z64_gameinfo.item_icon_x[0]  = 0xA2;
		z64_gameinfo.item_ammo_x[0]		= 0xA2;
		z64_b_button_label_x			= 0x94;
		z64_gameinfo.item_ammo_x[1]		= 0xE4;
		z64_gameinfo.item_button_x[1]	= z64_gameinfo.item_icon_x[1] = 0xE3;
		z64_gameinfo.item_button_x[2]	= z64_gameinfo.item_icon_x[2] = 0xF9;
		z64_gameinfo.item_ammo_x[2]		= 0xFA;
		z64_gameinfo.item_button_x[3]	= z64_gameinfo.item_icon_x[3] = 0x10F;
		z64_gameinfo.item_ammo_x[3]		= 0x110;
		z64_gameinfo.c_up_button_x		= 0xFE;
		z64_gameinfo.c_up_icon_x		= 0xF7;
	}
	else {
		z64_gameinfo.a_button_x			= z64_gameinfo.a_button_icon_x = 0x122;
		z64_gameinfo.item_button_x[0]	= z64_gameinfo.item_icon_x[0]  = 0x108;
		z64_gameinfo.item_ammo_x[0]		= 0x10A;
		z64_b_button_label_x			= 0xFC;
		z64_gameinfo.item_ammo_x[1]		= 0x14C;
		z64_gameinfo.item_button_x[1]	= z64_gameinfo.item_icon_x[1] = 0x14B;
		z64_gameinfo.item_button_x[2]	= z64_gameinfo.item_icon_x[2] = 0x161;
		z64_gameinfo.item_ammo_x[2]		= 0x161;
		z64_gameinfo.item_button_x[3]	= z64_gameinfo.item_icon_x[3] = 0x177;
		z64_gameinfo.item_ammo_x[3]		= 0x178;
		z64_gameinfo.c_up_button_x		= 0x166;
		z64_gameinfo.c_up_icon_x		= 0x15F;
	}
}

void handle_hud() {
		if (SAVE_HIDE_HUD == 0) {
			if (z64_file.unk_15_[13] == 1)
				z64_file.unk_15_[13] = 50;
		}
		else if (SAVE_HIDE_HUD == 1) {
			if (z64_file.unk_15_[13] == 50)
				z64_file.unk_15_[13] = 6;
			if (z64_file.unk_15_[7] == 50)
				z64_file.unk_15_[7] = 6;
		}
		else if (SAVE_HIDE_HUD == 2) {
			if (z64_file.unk_15_[13] == 6)
				z64_file.unk_15_[13] = 9;
			if (z64_file.unk_15_[7] == 50)
				z64_file.unk_15_[7] = 9;
		}
		else if (SAVE_HIDE_HUD == 3) {
			if (z64_file.unk_15_[13] == 9)
				z64_file.unk_15_[13] = 11;
			if (z64_file.unk_15_[7] == 50)
				z64_file.unk_15_[7] = 11;
		}
		else if (SAVE_HIDE_HUD == 4) {
			if (z64_file.unk_15_[13] == 11)
				z64_file.unk_15_[13] = 1;
			if (z64_file.unk_15_[7] == 50)
				z64_file.unk_15_[7] = 1;
		}
}

void set_b_button(pad_t pad_pressed) {
	if (!SAVE_ITEM_ON_B || !z64_game.common.input[0].pad_pressed.a || z64_game.pause_ctxt.screen_idx != 0 || z64_game.pause_ctxt.unk_02_[1] != 0 || z64_game.pause_ctxt.cursor_pos == 0x0A || z64_game.pause_ctxt.cursor_pos == 0x0B || z64_game.pause_ctxt.item_cursor <= Z64_SLOT_STICK || z64_game.pause_ctxt.item_cursor >= Z64_SLOT_CHILD_TRADE)
		return;
	
	z64_item_t item = 0xFF;
	for (uint8_t i=0; i<18; i++)
		if (z64_game.pause_ctxt.item_cursor == i) {
			if ( (!z64_file.link_age && z64_usability.item[i] == 1) || (z64_file.link_age && z64_usability.item[i] == 0) )
				item = 0xFF;
			else item = z64_file.items[i];
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

void settings_menu() {
	if (z64_game.pause_ctxt.state != 6)
		return;
	
	if (z64_game.pause_ctxt.unk_02_[1] == 0) {
		z64_game.pause_ctxt.unk_02_[1] = 3;
		z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
	else if (z64_game.pause_ctxt.unk_02_[1] == 3) {
		z64_game.pause_ctxt.unk_02_[1] = 0;
		z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
}

void handle_infinite() {
	if (SAVE_INFINITE_HP) {
		if (z64_file.energy < z64_file.energy_capacity)
			z64_file.energy = z64_file.energy_capacity;
	}
	
	if (SAVE_INFINITE_MP) {
		if (HAS_MAGIC) {
			if (z64_file.magic_capacity)
				z64_file.magic = 0x60;
			else z64_file.magic = 0x30;
		}
	}
	
	if (SAVE_INFINITE_AMMO) {
		if (z64_file.nut_upgrade == 1)
			z64_file.ammo[0x01] = z64_capacity.nut_upgrade[1];
		else if (z64_file.nut_upgrade == 2)
			z64_file.ammo[0x01] = z64_capacity.nut_upgrade[2];
		else if (z64_file.nut_upgrade == 3)
			z64_file.ammo[0x01] = z64_capacity.nut_upgrade[3];
		
		if (z64_file.stick_upgrade == 1)
			z64_file.ammo[0x00] = z64_capacity.stick_upgrade[1];
		else if (z64_file.stick_upgrade == 2)
			z64_file.ammo[0x00] = z64_capacity.stick_upgrade[2];
		else if (z64_file.stick_upgrade == 3)
			z64_file.ammo[0x00] = z64_capacity.stick_upgrade[3];
		
		if (z64_file.bullet_bag == 1)
			z64_file.ammo[0x06] = z64_capacity.bullet_bag[1];
		else if (z64_file.bullet_bag == 2)
			z64_file.ammo[0x06] = z64_capacity.bullet_bag[1];
		else if (z64_file.bullet_bag == 3)
			z64_file.ammo[0x06] = z64_capacity.bullet_bag[1];
		
		if (z64_file.quiver == 1)
			z64_file.ammo[0x03] = z64_capacity.quiver[1];
		else if (z64_file.quiver == 2)
			z64_file.ammo[0x03] = z64_capacity.quiver[2];
		else if (z64_file.quiver == 3)
			z64_file.ammo[0x03] = z64_capacity.quiver[3];
		
		if (z64_file.bomb_bag == 1)
			z64_file.ammo[0x02] = z64_capacity.bomb_bag[1];
		else if (z64_file.bomb_bag == 2)
			z64_file.ammo[0x02] = z64_capacity.bomb_bag[2];
		else if (z64_file.bomb_bag == 3)
			z64_file.ammo[0x02] = z64_capacity.bomb_bag[3];
		
		if (z64_file.items[Z64_SLOT_BOMBCHU] == Z64_ITEM_BOMBCHU)
			z64_file.ammo[0x08] = 50;
	}
	
	if (SAVE_INFINITE_RUPEES) {
		if (z64_file.wallet == 0)
			z64_file.rupees = z64_capacity.wallet[0];
		else if (z64_file.wallet == 1)
			z64_file.rupees = z64_capacity.wallet[1];
		else if (z64_file.wallet == 2)
			z64_file.rupees = z64_capacity.wallet[2];
		else if (z64_file.wallet == 3)
			z64_file.rupees = z64_capacity.wallet[3];
	}
}