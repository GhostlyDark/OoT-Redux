#include "fps.h"

extern uint8_t CFG_FPS_ENABLED;
extern uint8_t FPS_SWITCH;

typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);

uint16_t DEKU_STICK_TIMER_SWITCH	= 0;
uint16_t LAST_TIME					= 0;
uint16_t STARTED_TIMER				= 0;

#define z64_playsfx   ((playsfx_t)      0x800C806C)

void handle_fps() {
	if (!CFG_FPS_ENABLED || z64_game.pause_ctxt.state != 0)
		return;
	
	if ( (z64_game.common.input[0].raw.pad.l && z64_game.common.input[0].pad_pressed.z) || (z64_game.common.input[0].raw.pad.z && z64_game.common.input[0].pad_pressed.l) ) {
		FPS_SWITCH ^= 1;
		if (FPS_SWITCH)
			z64_playsfx(0x4814, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
		else z64_playsfx(0x4813, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
	}
	
	if (z64_is_demo == 0xFB)
		z64_fps_limit = 1;
	else if (!FPS_SWITCH)
		z64_fps_limit = 3;
	else if (z64_playing_ocarina || z64_change_scene == 0x20)
		z64_fps_limit = 2;
	else if (z64_is_demo == 0x80 || z64_control_link == 0 || z64_control_link == 0x4200 || (z64_control_link == 0xFF00 && !z64_talking_to_npc) || z64_hookshot_active == 0x100B || z64_link_animation == 0x2708 || z64_bottle_action == 0x00010005)
		z64_fps_limit = 3;
	else if (FPS_SWITCH)
		z64_fps_limit = 2;
	
	if (z64_fps_limit == 2) {
		if (z64_deku_stick_timer == 100 && !DEKU_STICK_TIMER_SWITCH) {
			z64_deku_stick_timer += 100;
			DEKU_STICK_TIMER_SWITCH = 1;
		}
		else if (z64_deku_stick_timer == 0)
			DEKU_STICK_TIMER_SWITCH = 0;
		
		if (z64_link_animation == 0x2968 || z64_link_animation == 0x2970 || z64_link_animation == 0x2A80 || z64_link_animation == 0x2A90)
			z64_link_animation_parameter = 0x3F4CCCCD;
		
		if (z64_jump_gravity != 0)
			z64_jump_gravity = 0xBF34;
		
		if (z64_time_of_day_speed == 10)
			z64_time_of_day_speed = 7;
		
		if ( (z64_timer_type == 0x4 || z64_timer_type == 0x8 || z64_timer_type == 0xE) && !STARTED_TIMER) {
			STARTED_TIMER	= 1;
			LAST_TIME		= z64_time_remaining;
		}
		else if (z64_timer_type == 0 && STARTED_TIMER)
			STARTED_TIMER	= 0;
		
		if (z64_timer_type == 0x4 || z64_timer_type == 0x8) { // Decreasing
			if (LAST_TIME == z64_time_remaining + 3) {
				z64_time_remaining++;
				z64_seconds_left++;
				LAST_TIME = z64_time_remaining;
			}
		}
		else if (z64_timer_type == 0xE) { // Increasing
			if (LAST_TIME == z64_time_remaining - 3) {
				z64_time_remaining--;
				z64_seconds_left--;
				LAST_TIME = z64_time_remaining;
			}
		}
		
		if (z64_hover_boots_length == 19)
			z64_hover_boots_length	= 30;
	}
	else if (z64_fps_limit == 3) {
		if (z64_hover_boots_length == 30)
			z64_hover_boots_length	= 19;
	}
}