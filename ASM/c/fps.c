#include "fps.h"

extern uint8_t CFG_FPS_ENABLED;

uint8_t  fps_switch					= 1;
uint8_t deku_stick_timer_switch		= 0;
uint8_t nayrus_love_timer_switch	= 0;
uint16_t last_time					= 0;
uint16_t started_timer				= 0;

void handle_fps() {
	if (!CFG_FPS_ENABLED || z64_game.pause_ctxt.state != 0)
		return;
	
	if ( (z64_game.common.input[0].raw.pad.l && z64_game.common.input[0].pad_pressed.z) || (z64_game.common.input[0].raw.pad.z && z64_game.common.input[0].pad_pressed.l) ) {
		fps_switch ^= 1;
		if (fps_switch)
			z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		else z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
	
	if (is_demo == 0xFB)
		fps_limit = 1;
	else if (!fps_switch)
		fps_limit = 3;
	else if (playing_ocarina || change_scene == 0x20)
		fps_limit = 2;
	else if (is_demo == 0x80 || control_link == 0 || control_link == 0x4200 || (control_link == 0xFF00 && !talking_to_npc) || hookshot_active == 0x100B || link_animation == 0x2708 || bottle_action == 0x00010005 || frogs > 1)
		fps_limit = 3;
	else if (fps_switch)
		fps_limit = 2;
	
	if (fps_limit == 2) {
		if (deku_stick_timer == 100 && !deku_stick_timer_switch) {
			deku_stick_timer += 100;
			deku_stick_timer_switch = 1;
		}
		else if (deku_stick_timer == 0)
			deku_stick_timer_switch = 0;
		
		if (z64_file.nayrus_love_timer == 601 && !nayrus_love_timer_switch) {
			z64_file.nayrus_love_timer -= 600;
			nayrus_love_timer_switch = 1;
		}
		else if (z64_file.nayrus_love_timer == 0 || z64_file.nayrus_love_timer > 601)
			nayrus_love_timer_switch = 0;
		
		if (link_animation == 0x2968 || link_animation == 0x2970 || link_animation == 0x2A80 || link_animation == 0x2A90)
			link_animation_parameter = 0x3F4CCCCD;
		
		if (jump_gravity != 0)
			jump_gravity = 0xBF34;
		
		if (time_of_day_speed == 10)
			time_of_day_speed = 7;
		if (hover_boots_length == 19)
			hover_boots_length = 30;
		if (lens_of_truth_start == 80)
			lens_of_truth_start	= lens_of_truth_interval = 120;
		
		// Timers
		timer1_1 = timer1_2 = timer1_3 = timer1_4 = 0x1E;
		timer2_1 = timer2_2 = timer2_3 = timer2_4 = timer2_5 = timer2_6 = 0x1E;
		timer2_7 = 0x3C;
		timer3_1 = timer3_2 = timer3_3 = 0x1E;
		timer4_2 = timer4_3 = timer4_4 = timer4_5 = 0x1E;
		timer4_1 = timer4_6 = timer4_7 = 0x3C;
			
	}
	else if (fps_limit == 3) {
		if (time_of_day_speed == 7)
			time_of_day_speed = 10;
		if (hover_boots_length == 30)
			hover_boots_length	= 19;
		if (lens_of_truth_start == 120)
			lens_of_truth_start	= lens_of_truth_interval = 80;
		
		// Timers
		timer1_1 = timer1_2 = timer1_3 = timer1_4 = 0x14;
		timer2_1 = timer2_2 = timer2_3 = timer2_4 = timer2_5 = timer2_6 = 0x14;
		timer2_7 = 0x28;
		timer3_1 = timer3_2 = timer3_3 = 0x14;
		timer4_2 = timer4_3 = timer4_4 = timer4_5 = 0x14;
		timer4_1 = timer4_6 = timer4_7 = 0x28;
	}
}