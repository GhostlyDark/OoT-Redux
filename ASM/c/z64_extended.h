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

#endif