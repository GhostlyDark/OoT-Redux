#ifndef Z64_EXTENDED_H
#define Z64_EXTENDED_H

#include "z64.h"

/* dram addresses */
#define z64_camera_view_addr	0x801DB0CD
#define z64_has_minimap_addr	0x8018884C		// 0x8011B9B3, 8017643C, 8018884C
#define z64_dungeon_scene_addr	0x801D8BEA

/* data */
#define z64_camera_view			(*(uint8_t*)	z64_camera_view_addr)
#define z64_has_minimap			(*(uint16_t*)	z64_has_minimap_addr)
#define z64_dungeon_scene		(*(uint16_t*)	z64_dungeon_scene_addr)

#endif