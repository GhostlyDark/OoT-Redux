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
	DPAD_NULL			= 0x00,
	DPAD_SWORD			= 0x01,
	DPAD_SHIELD			= 0x02,
	DPAD_TUNIC			= 0x03,
	DPAD_BOOTS			= 0x04,
	DPAD_IRON_BOOTS		= 0x05,
	DPAD_HOVER_BOOTS	= 0x06,
	DPAD_ARROWS			= 0x07,
	DPAD_NUT			= 0x08,
	DPAD_LENS			= 0x09,
	DPAD_OCARINA		= 0x0A,
	DPAD_CHILD_TRADE	= 0x0B,
	DPAD_ADULT_TRADE	= 0x0C,
	DPAD_DINS_FIRE		= 0x0D,
	DPAD_FARORES_WIND	= 0x0E,
	DPAD_NAYRUS_LOVE	= 0x0F,
} dpad_action_t;

typedef enum {
	Z64_AMMO_STICK			= 0x00,		// Used
	Z64_AMMO_NUT			= 0x01,		// Used
	Z64_AMMO_BOMB			= 0x02,		// Used
	Z64_AMMO_BOW			= 0x03,		// Used
	Z64_AMMO_FIRE_ARROW		= 0x04,		// Unused 1
	Z64_AMMO_DINS_FIRE		= 0x05,		// Unused 2
	Z64_AMMO_SLINGSHOT		= 0x06,		// Used
	Z64_AMMO_OCARINA		= 0x07,		// Unused 3
	Z64_AMMO_BOMBCHU		= 0x08,		// Used
	Z64_AMMO_HOOKSHOT		= 0x09,		// Unused 4
	Z64_AMMO_ICE_ARROW		= 0x0A,		// Unused 5
	Z64_AMMO_FARORES_WIND	= 0x0B,		// Unused 6
	Z64_AMMO_BOOMERANG		= 0x0C,		// Unused 7
	Z64_AMMO_LENS			= 0x0D,		// Unused 8
	Z64_AMMO_BEANS			= 0x0E,		// Used
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

#define DPAD_ADULT_UP		(z64_file.ammo[0x04])
#define DPAD_CHILD_UP		(z64_file.ammo[0x05])
#define DPAD_ADULT_RIGHT	(z64_file.ammo[0x07])
#define DPAD_CHILD_RIGHT	(z64_file.ammo[0x09])
#define DPAD_ADULT_DOWN		(z64_file.ammo[0x0A])
#define DPAD_CHILD_DOWN		(z64_file.ammo[0x0B])
#define DPAD_ADULT_LEFT		(z64_file.ammo[0x0C])
#define DPAD_CHILD_LEFT		(z64_file.ammo[0x0D])

#define DPAD_ADULT_SET1_UP		( (DPAD_ADULT_UP >> 4) & 0xF)
#define DPAD_ADULT_SET2_UP		(DPAD_ADULT_UP & 0xF)
#define DPAD_CHILD_SET1_UP		( (DPAD_CHILD_UP >> 4) & 0xF)
#define DPAD_CHILD_SET2_UP		(DPAD_CHILD_UP & 0xF)
#define DPAD_ADULT_SET1_RIGHT	( (DPAD_ADULT_RIGHT >> 4) & 0xF)
#define DPAD_ADULT_SET2_RIGHT	(DPAD_ADULT_RIGHT & 0xF)
#define DPAD_CHILD_SET1_RIGHT	( (DPAD_CHILD_RIGHT >> 4) & 0xF)
#define DPAD_CHILD_SET2_RIGHT	(DPAD_CHILD_RIGHT & 0xF)
#define DPAD_ADULT_SET1_DOWN	( (DPAD_ADULT_DOWN >> 4) & 0xF)
#define DPAD_ADULT_SET2_DOWN	(DPAD_ADULT_DOWN & 0xF)
#define DPAD_CHILD_SET1_DOWN	( (DPAD_CHILD_DOWN >> 4) & 0xF)
#define DPAD_CHILD_SET2_DOWN	(DPAD_CHILD_DOWN & 0xF)
#define DPAD_ADULT_SET1_LEFT	( (DPAD_ADULT_LEFT >> 4) & 0xF)
#define DPAD_ADULT_SET2_LEFT	(DPAD_ADULT_LEFT & 0xF)
#define DPAD_CHILD_SET1_LEFT	( (DPAD_CHILD_LEFT >> 4) & 0xF)
#define DPAD_CHILD_SET2_LEFT	(DPAD_CHILD_LEFT & 0xF)

#define EXTRA_SRAM				(z64_file.unk_08_[0])
#define DPAD_INIT_SETUP			(EXTRA_SRAM & (1 << 0) )
#define DOWNGRADE_GIANTS_KNIFE	(EXTRA_SRAM & (1 << 1) )
#define DOWNGRADE_OCARINA		(EXTRA_SRAM & (1 << 2) )
#define DOWNGRADE_HOOKSHOT		(EXTRA_SRAM & (1 << 3) )

#endif