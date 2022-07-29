#ifndef Z64_EXTENDED_H
#define Z64_EXTENDED_H

#include "z64.h"

#define BLOCK_DPAD (0x00000001 | \
	                0x00000002 | \
                    0x00000080 | \
                    0x00000400 | \
                    0x10000000 | \
                    0x20000000)

#define CAN_USE_DPAD        (((z64_link.state_flags_1 & BLOCK_DPAD) == 0) && \
                            ((uint32_t)z64_ctxt.state_dtor==z64_state_ovl_tab[3].vram_dtor) && \
                            (z64_file.game_mode == 0) && \
                            ((z64_event_state_1 & 0x20) == 0))

typedef enum {
	DPAD_NULL = -1,
	DPAD_SWORD = 1,
	DPAD_SHIELD,
	DPAD_TUNIC,
	DPAD_BOOTS,
	DPAD_IRON_BOOTS,
	DPAD_HOVER_BOOTS,
	DPAD_ARROWS,
	DPAD_NUT,
	DPAD_LENS,
	DPAD_OCARINA,
	DPAD_CHILD_TRADE,
	DPAD_ADULT_TRADE,
	DPAD_DINS_FIRE,
	DPAD_FARORES_WIND,
	DPAD_NAYRUS_LOVE,
} dpad_action_t;

typedef enum {
	DPAD_ADULT_SET1_UP = 0,
	DPAD_ADULT_SET1_RIGHT,
	DPAD_ADULT_SET1_DOWN,
	DPAD_ADULT_SET1_LEFT,
	DPAD_ADULT_SET2_UP,
	DPAD_ADULT_SET2_RIGHT,
	DPAD_ADULT_SET2_DOWN,
	DPAD_ADULT_SET2_LEFT,
	DPAD_CHILD_SET1_UP,
	DPAD_CHILD_SET1_RIGHT,
	DPAD_CHILD_SET1_DOWN,
	DPAD_CHILD_SET1_LEFT,
	DPAD_CHILD_SET2_UP,
	DPAD_CHILD_SET2_RIGHT,
	DPAD_CHILD_SET2_DOWN,
	DPAD_CHILD_SET2_LEFT,
	DOWNGRADE_OCARINA,
	DOWNGRADE_HOOKSHOT,
	DOWNGRADE_GIANTS_KNIFE,
} unused_sram_t;

typedef enum {
	Z64_AMMO_STICK = 0,		// Used
	Z64_AMMO_NUT,				// Used
	Z64_AMMO_BOMB,			// Used
	Z64_AMMO_FIRE_ARROW,		// Unused 1
	Z64_AMMO_DINS_FIRE,		// Unused 2
	Z64_AMMO_SLINGSHOT,		// Unused 3
	Z64_AMMO_OCARINA,			// Unused 4
	Z64_AMMO_BOMBCHU,			// Used
	Z64_AMMO_HOOKSHOT,		// Unused 5
	Z64_AMMO_ICE_ARROW,		// Unused 6
	Z64_AMMO_FARORES_WIND,	// Unused 7
	Z64_AMMO_BOOMERANG,		// Unused 8
	Z64_AMMO_LENS,			// Unused 9
	Z64_AMMO_BEANS,			// Used
} z64_ammo_t;

/* dram addresses & data*/
#define z64_playsfx   					((playsfx_t)    0x800C806C)
#define z64_camera_view					(*(uint8_t*)	0x801DB0CD)
#define z64_has_minimap					(*(uint16_t*)	0x8018884C)	// 0x8011B9B3, 8017643C, 8018884C
#define z64_dungeon_scene				(*(uint16_t*)	0x801D8BEA)
#define z64_scene						(*(uint16_t*)	0x801C8544)
#define z64_b_button_label_x			(*(uint16_t*)	0x801C7C3A)
#define z64_b_button_label_y			(*(uint16_t*)	0x801C7C3E)
#define z64_mask_equipped				(*(uint8_t*)	0x801DAB7F)

/* dram addresses & data for D-Pad */
#define z64_dpad_lens_1					(*(uint16_t*)	0x80072D40)
#define z64_dpad_lens_2					(*(uint16_t*)	0x80072D4C)
#define z64_dpad_lens_3					(*(uint16_t*)	0x80072D58)

/* dram addresses & data for 30 FPS */
#define z64_fps_limit					(*(uint8_t*)	0x801C6FA1)
#define z64_jump_gravity				(*(uint16_t*)	0x801DAA9C)
#define z64_link_animation				(*(uint16_t*)	0x801DABDE)
#define z64_link_animation_parameter	(*(uint32_t*)	0x801DABF0)
#define z64_control_link				(*(uint16_t*)	0x801DAADE)
#define z64_deku_stick_timer			(*(uint16_t*)	0x801DB280)
#define z64_hover_boots_length			(*(uint16_t*)	0x8039E612)
#define z64_time_of_day_speed			(*(uint16_t*)	0x800F1650)
#define z64_timer_type					(*(uint8_t*)	0x8011B99F)
#define z64_time_remaining				(*(uint16_t*)	0x8011B9A0)
#define z64_seconds_left				(*(uint8_t*)	0x8011BF31)
#define z64_is_demo						(*(uint8_t*)	0x801DB09D)
#define z64_boomerang_active			(*(uint16_t*)	0x800E8B8E)
#define z64_hookshot_active				(*(uint16_t*)	0x801DAA50)
#define z64_talking_to_npc				(*(uint8_t*)	0x801DAA36)
#define z64_change_scene				(*(uint8_t*)	0x801DB09C)
#define z64_playing_ocarina				(*(uint8_t*)	0x80102208)
#define z64_bottle_action				(*(uint32_t*)	0x801C86B2)
#define z64_fishing						(*(uint8_t*)	0x801C8C41)
#define z64_frogs						(*(uint8_t*)	0x801C8835)

#endif