#ifndef FPS_H
#define FPS_H

#include "z64.h"
#include "z64_extended.h"

void handle_fps();

/* DRAM addresses & data for 30 FPS */

#define z64_fps_limit					(*(uint8_t*)			0x801C6FA1)
#define z64_jump_gravity				(*(uint16_t*)			0x801DAA9C)
#define z64_link_animation				(*(uint16_t*)			0x801DABDE)
#define z64_link_animation_parameter	(*(uint32_t*)			0x801DABF0)
#define z64_control_link				(*(uint16_t*)			0x801DAADE)
#define z64_deku_stick_timer			(*(uint16_t*)			0x801DB280)
#define z64_hover_boots_length			(*(uint16_t*)			0x8039E612)
#define z64_time_of_day_speed			(*(uint16_t*)			0x800F1650)
#define z64_timer_type					(*(uint8_t*)			0x8011B99F)
#define z64_time_remaining				(*(uint16_t*)			0x8011B9A0)
#define z64_seconds_left				(*(uint8_t*)			0x8011BF31)
#define z64_is_demo						(*(uint8_t*)			0x801DB09D)
#define z64_boomerang_active			(*(uint16_t*)			0x800E8B8E)
#define z64_hookshot_active				(*(uint16_t*)			0x801DAA50)
#define z64_talking_to_npc				(*(uint8_t*)			0x801DAA36)
#define z64_change_scene				(*(uint8_t*)			0x801DB09C)
#define z64_playing_ocarina				(*(uint8_t*)			0x80102208)
#define z64_bottle_action				(*(uint32_t*)			0x801C86B2)
#define z64_fishing						(*(uint8_t*)			0x801C8C41)
#define z64_frogs						(*(uint8_t*)			0x801C8835)

#define timer1_1						(*(uint16_t*)			0x800772FE)
#define timer1_2						(*(uint16_t*)			0x80077366)
#define timer1_3						(*(uint16_t*)			0x800775D2)
#define timer1_4						(*(uint16_t*)			0x800778CE)
#define timer2_1						(*(uint16_t*)			0x800772EA)
#define timer2_2						(*(uint16_t*)			0x80077342)
#define timer2_3						(*(uint16_t*)			0x8007735A)
#define timer2_4						(*(uint16_t*)			0x800773C2)
#define timer2_5						(*(uint16_t*)			0x800774FE)
#define timer2_6						(*(uint16_t*)			0x8007782E)
#define timer2_7						(*(uint16_t*)			0x800778FE)
#define timer3_1						(*(uint16_t*)			0x80077956)
#define timer3_2						(*(uint16_t*)			0x80077A0A)
#define timer3_3						(*(uint16_t*)			0x80077C4A)
#define timer4_1						(*(uint16_t*)			0x800770AE)
#define timer4_2						(*(uint16_t*)			0x80077952)
#define timer4_3						(*(uint16_t*)			0x800779F6)
#define timer4_4						(*(uint16_t*)			0x80077A5E)
#define timer4_5						(*(uint16_t*)			0x80077B86)
#define timer4_6						(*(uint16_t*)			0x80077C8A)
#define timer4_7						(*(uint16_t*)			0x80077CF2)

#endif
