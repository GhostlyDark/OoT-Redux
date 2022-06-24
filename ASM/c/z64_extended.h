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

/* dram addresses & data for Boss Rushing */
#define z64_boss_rush_1_1			(*(uint16_t*)	0x800FA4D4)
#define z64_boss_rush_1_2			(*(uint16_t*)	0x800FA4D8)
#define z64_boss_rush_1_3			(*(uint16_t*)	0x800FA4DC)
#define z64_boss_rush_1_4			(*(uint16_t*)	0x800FA450)
#define z64_boss_rush_2_1			(*(uint16_t*)	0x800FADEC)
#define z64_boss_rush_2_2			(*(uint16_t*)	0x800FADF0)
#define z64_boss_rush_2_3			(*(uint16_t*)	0x800FADF4)
#define z64_boss_rush_2_4			(*(uint16_t*)	0x800FADF8)
#define z64_boss_rush_3_1			(*(uint16_t*)	0x800FAE7C)
#define z64_boss_rush_3_2			(*(uint16_t*)	0x800FAE80)
#define z64_boss_rush_3_3			(*(uint16_t*)	0x800FAE84)
#define z64_boss_rush_3_4			(*(uint16_t*)	0x800FAE88)
#define z64_boss_rush_4_1			(*(uint16_t*)	0x800FA0CC)
#define z64_boss_rush_4_2			(*(uint16_t*)	0x800FA0D0)
#define z64_boss_rush_4_3			(*(uint16_t*)	0x800FA0D4)
#define z64_boss_rush_4_4			(*(uint16_t*)	0x800FA0D8)
#define z64_boss_rush_5_1			(*(uint16_t*)	0x800FB430)
#define z64_boss_rush_5_2			(*(uint16_t*)	0x800FB434)
#define z64_boss_rush_5_3			(*(uint16_t*)	0x800FB438)
#define z64_boss_rush_5_4			(*(uint16_t*)	0x800FB43C)
#define z64_boss_rush_6_1			(*(uint16_t*)	0x800FB220)
#define z64_boss_rush_6_2			(*(uint16_t*)	0x800FB224)
#define z64_boss_rush_6_3			(*(uint16_t*)	0x800FB228)
#define z64_boss_rush_6_4			(*(uint16_t*)	0x800FB22C)
#define z64_boss_rush_7_1			(*(uint16_t*)	0x800FA254)
#define z64_boss_rush_7_2			(*(uint16_t*)	0x800FA258)
#define z64_boss_rush_8_1			(*(uint16_t*)	0x800FAE60)
#define z64_boss_rush_8_2			(*(uint16_t*)	0x800FAE64)

/* data */
#define z64_camera_view			(*(uint8_t*)	z64_camera_view_addr)
#define z64_has_minimap			(*(uint16_t*)	z64_has_minimap_addr)
#define z64_dungeon_scene		(*(uint16_t*)	z64_dungeon_scene_addr)
#define z64_b_button_label_x	(*(uint16_t*)	z64_b_button_label_x_addr)
#define z64_b_button_label_y	(*(uint16_t*)	z64_b_button_label_y_addr)
#define z64_mask_equipped		(*(uint8_t*)	z64_mask_equipped_addr)

#endif