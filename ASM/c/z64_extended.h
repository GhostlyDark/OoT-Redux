#ifndef Z64_EXTENDED_H
#define Z64_EXTENDED_H

#include "z64.h"

/* dram addresses */
#define z64_camera_view_addr		0x801DB0CD
#define z64_has_minimap_addr		0x8018884C		// 0x8011B9B3, 8017643C, 8018884C
#define z64_dungeon_scene_addr		0x801D8BEA
#define z64_b_button_label_x_addr	0x801C7C3A
#define z64_b_button_label_y_addr	0x801C7C3E
#define z64_mask_equipped_addr		0x801DAB7F

/* data */
#define z64_camera_view			(*(uint8_t*)	z64_camera_view_addr)
#define z64_has_minimap			(*(uint16_t*)	z64_has_minimap_addr)
#define z64_dungeon_scene		(*(uint16_t*)	z64_dungeon_scene_addr)
#define z64_b_button_label_x	(*(uint16_t*)	z64_b_button_label_x_addr)
#define z64_b_button_label_y	(*(uint16_t*)	z64_b_button_label_y_addr)
#define z64_mask_equipped		(*(uint8_t*)	z64_mask_equipped_addr)

/* dram addresses & data for 30 FPS */
#define z64_fps_limit					(*(uint8_t*)	0x801C6FA1)
#define z64_is_demo						(*(uint8_t*)	0x801DB09D)
#define z64_control_link				(*(uint16_t*)	0x801DAADE)
#define z64_boomerang_active			(*(uint16_t*)	0x800E8B8E)
#define z64_hookshot_active				(*(uint16_t*)	0x801DAA50)
#define z64_link_animation				(*(uint16_t*)	0x801DABDE)
#define z64_talking_to_npc				(*(uint8_t*)	0x801DAA36)
#define z64_link_animation_parameter	(*(uint32_t*)	0x801DABF0)
#define z64_jump_gravity				(*(uint16_t*)	0x801DAA9C)
#define z64_fps_14						(*(uint16_t*)	0x801DB280)
#define z64_deku_stick_timer			(*(uint16_t*)	0x801DB280)
#define z64_time_of_day_speed			(*(uint16_t*)	0x800F1650)
#define z64_change_scene				(*(uint8_t*)	0x801DB09C)
#define z64_playing_ocarina				(*(uint8_t*)	0x80102208)
#define z64_bottle_action				(*(uint32_t*)	0x801C86B2)
#define z64_hover_boots_length			(*(uint16_t*)	0x8039E612)
#define z64_timer_type					(*(uint8_t*)	0x8011B99F)
#define z64_time_remaining				(*(uint16_t*)	0x8011B9A0)
#define z64_seconds_left				(*(uint8_t*)	0x8011BF31)

#define z64_fps_1						(*(uint16_t*)	0x801C8C40)
#define z64_fps_2						(*(uint8_t*)	0x801C8C41)

#endif