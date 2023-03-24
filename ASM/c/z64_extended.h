#ifndef Z64_EXTENDED_H
#define Z64_EXTENDED_H

#include "z64.h"

typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);
typedef void(*usebutton_t)(z64_game_t *game, z64_link_t *link, uint8_t item, uint8_t button);

typedef enum {
	DPAD_NULL			= 0x0,
	DPAD_SWORD			= 0x1,
	DPAD_SHIELD			= 0x2,
	DPAD_TUNIC			= 0x3,
	DPAD_BOOTS			= 0x4,
	DPAD_IRON_BOOTS		= 0x5,
	DPAD_HOVER_BOOTS	= 0x6,
	DPAD_ARROWS			= 0x7,
	DPAD_NUT			= 0x8,
	DPAD_LENS			= 0x9,
	DPAD_OCARINA		= 0xA,
	DPAD_CHILD_TRADE	= 0xB,
	DPAD_ADULT_TRADE	= 0xC,
	DPAD_DINS_FIRE		= 0xD,
	DPAD_FARORES_WIND	= 0xE,
	DPAD_NAYRUS_LOVE	= 0xF,
} dpad_action_t;

typedef enum {
	LIMIT_STICK			= 0x0,
	LIMIT_NUT,
	LIMIT_BOMB,
	LIMIT_BOW,
	LIMIT_FIRE_ARROW,
	LIMIT_DINS_FIRE,
	LIMIT_SLINGSHOT,
	LIMIT_OCARINA,
	LIMIT_BOMBCHU,
	LIMIT_HOOKSHOT,
	LIMIT_ICE_ARROW,
	LIMIT_FARORES_WIND,
	LIMIT_BOOMERANG,
	LIMIT_LENS,
	LIMIT_BEANS,
	LIMIT_HAMMER,
	LIMIT_LIGHT_ARROW,
	LIMIT_NAYRUS_LOVE,
	LIMIT_BOTTLE_1,
	LIMIT_BOTTLE_2,
	LIMIT_BOTTLE_3,
	LIMIT_BOTTLE_4,
	LIMIT_ADULT_TRADE,
	LIMIT_CHILD_TRADE,
	LIMIT_QUIVER,
	LIMIT_KOKIRI_SWORD,
	LIMIT_MASTER_SWORD,
	LIMIT_GIANTS_KNIFE,
	LIMIT_BOMB_BAG,
	LIMIT_DEKU_SHIELD,
	LIMIT_HYLIAN_SHIELD,
	LIMIT_MIRROR_SHIELD,
	LIMIT_STRENGTH,
	LIMIT_KOKIRI_TUNIC,
	LIMIT_GORON_TUNIC,
	LIMIT_ZORA_TUNIC,
	LIMIT_SCALE,
	LIMIT_KOKIRI_BOOTS,
	LIMIT_IRON_BOOTS,
	LIMIT_HOVER_BOOTS,
} limit_item_t;

typedef enum {
	COLOR_KOKIRI		= 0x0,
	COLOR_GORON			= 0x1,
	COLOR_ZORA			= 0x2,
	COLOR_FOREST		= 0x10,
	COLOR_FIRE			= 0x4,
	COLOR_WATER			= 0x5,
	COLOR_SHADOW		= 0xC0,
} tunic_color_t;

typedef enum {
	OPTIONS_SIZE_CORE	= 9,
	OPTIONS_SIZE_MAIN	= 17,
	OPTIONS_SIZE_ALL	= 24,
	OPTIONS_LENGTH		= 17,
	OPTIONS_ROWS		= 13,
	
	OPTION_30_FPS		= 0,
	OPTION_DPAD,
	OPTION_SHOW_DPAD,
	OPTION_HIDE_HUD,
	OPTION_HUD_LAYOUT,
	OPTION_INVERSE_AIM,
	OPTION_NO_IDLE_CAMERA,
	OPTION_KEEP_MASK,
	OPTION_TRISWIPE,
	
	OPTION_UNEQUIP_ITEM,
	OPTION_UNEQUIP_GEAR,
	OPTION_ITEM_ON_B,
	OPTION_DOWNGRADE_ITEM,
	OPTION_CROUCH_STAB_FIX,
	OPTION_WEAKER_SWORDS,
	OPTION_EXTRA_ABILITIES,
	OPTION_RUPEE_DRAIN,
	
	OPTION_FOG,
	OPTION_INVENTORY_EDITOR,
	OPTION_LEVITATION,
	OPTION_INFINITE_HP,
	OPTION_INFINITE_MP,
	OPTION_INFINITE_RUPEES,
	OPTION_INFINITE_AMMO,
} option;

typedef struct {
	char		item[0x002C];
} z64_usability_t;

typedef struct {
	uint16_t	quiver[4];			// 0,  30,  40,  50
	uint16_t	bomb_bag[4];		// 0,  20,  30,  40
	uint16_t	u1[8];				// Unused
	uint16_t	wallet[4];			// 99, 200, 500, 500
	uint16_t	bullet_bag[4];		// 0,  30,  40,  50
	uint16_t	stick_upgrade[4];	// 0,  20,  30,  40
	uint16_t	nut_upgrade[4];		// 0,  10,  20,  30
	
} z64_capacity_t;

/* Functions */
#define z64_playsfx						((playsfx_t)			0x800C806C)
#define z64_usebutton					((usebutton_t)			0x8038C9A0)

/* DRAM addresses & data */
#define z64_usability					(*(z64_usability_t*)	0x8039F114)
#define z64_capacity					(*(z64_capacity_t*)		0x800F8CCC)
#define z64_change_scene				(*(uint32_t*)			0x801DB09C)
#define z64_has_minimap					(*(uint16_t*)			0x8018884C)	// 0x8011B9B3, 8017643C, 8018884C
#define z64_dungeon_scene				(*(uint16_t*)			0x801D8BEA)
#define z64_scene						(*(uint16_t*)			0x801C8544)
#define z64_camera_view					(*(uint8_t*)			0x801DB0CD)
#define z64_mask_equipped				(*(uint8_t*)			0x801DAB7F)
#define z64_throwing_nut				(*(uint8_t*)			0x80124696)
#define z64_triswipe					(*(uint8_t*)			0x8011B9ED)
#define z64_x_axis_input				(*(int8_t*)				0x801C84C8)
#define z64_y_axis_input				(*(int8_t*)				0x801C84C9)
#define z64_button_input				(*(int16_t*)			0x801C84C0) // 801C84B4, 801C84BA, 801C84C6
#define z64_link_animation_parameter	(*(uint16_t*)			0x801DABF0)
#define z64_link_a_action				(*(uint16_t*)			0x801DAA90)
#define z64_idle_camera_counter			(*(uint8_t*)			0x801C86CF)
#define z64_inventory_editor_index		(*(uint8_t*)			0x8039EA59)

/* DRAM addresses & data for HUD */
#define z64_b_button_label_x			(*(uint16_t*)			0x801C7C3A)
#define z64_b_button_label_y			(*(uint16_t*)			0x801C7C3E)
#define z64_c_left_x_set_item			(*(uint16_t*)			0x8039EAF8)
#define z64_c_left_y_set_item			(*(uint16_t*)			0x8039EB00)
#define z64_c_down_x_set_item			(*(uint16_t*)			0x8039EAFA)
#define z64_c_down_y_set_item			(*(uint16_t*)			0x8039EB02)
#define z64_c_right_x_set_item			(*(uint16_t*)			0x8039EAFC)
#define z64_c_right_y_set_item			(*(uint16_t*)			0x8039EB04)

/* DRAM addresses & data for Lens of Truth on D-Pad */
#define z64_dpad_lens_1					(*(uint16_t*)			0x80072D40)
#define z64_dpad_lens_2					(*(uint16_t*)			0x80072D4C)
#define z64_dpad_lens_3					(*(uint16_t*)			0x80072D58)

/* DRAM addresses & data for FPS */
#define fps_limit						(*(uint8_t*)			0x801C6FA1)
#define control_link					(*(uint16_t*)			0x801DAADE)

/* DRAM addresses & data for Medallion Abilities */
#define z64_tunic_color					(*(uint8_t*)			0x801DAB6C)
#define z64_move_speed					(*(uint16_t*)			0x801DB258)
#define z64_damage_frames				(*(uint8_t*)			0x801DB498)
#define z64_sword_damage_1				(*(uint8_t*)			0x801DAF1E)
#define z64_sword_damage_2				(*(uint8_t*)			0x801DAF9E)
#define z64_inner_red_trail_r			(*(uint8_t*)			0x80273794)
#define z64_inner_red_trail_g			(*(uint8_t*)			0x80273795)
#define z64_inner_red_trail_b			(*(uint8_t*)			0x80273796)
#define z64_outer_red_trail_r			(*(uint8_t*)			0x802738B4)
#define z64_outer_red_trail_g			(*(uint8_t*)			0x802738B5)
#define z64_outer_red_trail_b			(*(uint8_t*)			0x802738B6)

/* Availability */
#define HAS_MAGIC						(z64_file.magic_acquired && z64_file.magic_capacity_set)
#define CAN_CONTROL_LINK				(z64_game.pause_ctxt.state == 0 && (uint32_t)z64_ctxt.state_dtor == z64_state_ovl_tab[3].vram_dtor && z64_file.game_mode == 0 && (z64_event_state_1 & 0x20) == 0)
#define IS_PAUSE_SCREEN_CURSOR			(z64_game.pause_ctxt.state == 6 && z64_game.pause_ctxt.unk_02_[1] == 0 && z64_game.pause_ctxt.cursor_pos != 0xA && z64_game.pause_ctxt.cursor_pos != 0xB)
#define TYCOON_WALLET					(z64_file.event_chk_inf[13] & (1 << 13) )

/* D-Pad & Controls Availability */
#define BLOCK_DPAD						(0x00000001 | 0x00000002 | 0x00000080 | 0x00000400 | 0x10000000 | 0x20000000)
#define CAN_USE_DPAD					( (z64_link.state_flags_1 & BLOCK_DPAD) == 0 && (uint32_t)z64_ctxt.state_dtor == z64_state_ovl_tab[3].vram_dtor && z64_file.game_mode == 0 && (z64_event_state_1 & 0x20) == 0)
#define CAN_DRAW_HUD					( ( (uint32_t)z64_ctxt.state_dtor==z64_state_ovl_tab[3].vram_dtor) && (z64_file.game_mode == 0) && ( (z64_event_state_1 & 0x20) == 0) )

/* D-Pad Usability for Items */
#define BLOCK_ITEMS						(0x00800000 | 0x00000800 | 0x00200000 | 0x08000000)
#define CAN_USE_OCARINA					(z64_game.pause_ctxt.state == 0 && (z64_file.items[Z64_SLOT_OCARINA]     == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA]     == Z64_ITEM_OCARINA_OF_TIME) && !z64_game.restriction_flags.ocarina     && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_CHILD_TRADE				(z64_game.pause_ctxt.state == 0 && (z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG     && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH)   && !z64_game.restriction_flags.trade_items && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_ADULT_TRADE				(z64_game.pause_ctxt.state == 0 && (z64_file.items[Z64_SLOT_ADULT_TRADE] >= Z64_ITEM_POCKET_EGG    && z64_file.items[Z64_SLOT_ADULT_TRADE] <= Z64_ITEM_CLAIM_CHECK)     && !z64_game.restriction_flags.trade_items && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_ITEMS					(z64_game.pause_ctxt.state == 0 && !z64_game.restriction_flags.all && ( (z64_link.state_flags_1 & BLOCK_ITEMS) == 0) )
#define CAN_USE_LENS					(z64_game.pause_ctxt.state == 0 && (!z64_game.restriction_flags.all || z64_game.scene_index == 0x0010) && ( (z64_link.state_flags_1 & BLOCK_ITEMS) == 0) )
#define CAN_USE_FARORES_WIND			(z64_game.pause_ctxt.state == 0 && !z64_game.restriction_flags.farores_wind && ( (z64_link.state_flags_1 & BLOCK_ITEMS) == 0) )

/* D-Pad SRAM locations */
#define DPAD_ADULT_UP					(z64_file.ammo[0x04])
#define DPAD_CHILD_UP					(z64_file.ammo[0x05])
#define DPAD_ADULT_RIGHT				(z64_file.ammo[0x07])
#define DPAD_CHILD_RIGHT				(z64_file.ammo[0x09])
#define DPAD_ADULT_DOWN					(z64_file.ammo[0x0A])
#define DPAD_CHILD_DOWN					(z64_file.ammo[0x0B])
#define DPAD_ADULT_LEFT					(z64_file.ammo[0x0C])
#define DPAD_CHILD_LEFT					(z64_file.ammo[0x0D])

/* D-Pad Button Mappings */
#define DPAD_ADULT_SET1_UP				( (DPAD_ADULT_UP >> 4) & 0xF)
#define DPAD_ADULT_SET2_UP				(DPAD_ADULT_UP & 0xF)
#define DPAD_CHILD_SET1_UP				( (DPAD_CHILD_UP >> 4) & 0xF)
#define DPAD_CHILD_SET2_UP				(DPAD_CHILD_UP & 0xF)
#define DPAD_ADULT_SET1_RIGHT			( (DPAD_ADULT_RIGHT >> 4) & 0xF)
#define DPAD_ADULT_SET2_RIGHT			(DPAD_ADULT_RIGHT & 0xF)
#define DPAD_CHILD_SET1_RIGHT			( (DPAD_CHILD_RIGHT >> 4) & 0xF)
#define DPAD_CHILD_SET2_RIGHT			(DPAD_CHILD_RIGHT & 0xF)
#define DPAD_ADULT_SET1_DOWN			( (DPAD_ADULT_DOWN >> 4) & 0xF)
#define DPAD_ADULT_SET2_DOWN			(DPAD_ADULT_DOWN & 0xF)
#define DPAD_CHILD_SET1_DOWN			( (DPAD_CHILD_DOWN >> 4) & 0xF)
#define DPAD_CHILD_SET2_DOWN			(DPAD_CHILD_DOWN & 0xF)
#define DPAD_ADULT_SET1_LEFT			( (DPAD_ADULT_LEFT >> 4) & 0xF)
#define DPAD_ADULT_SET2_LEFT			(DPAD_ADULT_LEFT & 0xF)
#define DPAD_CHILD_SET1_LEFT			( (DPAD_CHILD_LEFT >> 4) & 0xF)
#define DPAD_CHILD_SET2_LEFT			(DPAD_CHILD_LEFT & 0xF)

/* Extra saving for Redux */
#define EXTRA_SRAM_1					(*(uint8_t*)			0x8011B4FE)
#define DPAD_INIT_SETUP					  (EXTRA_SRAM_1 & 1)
#define DOWNGRADE_GIANTS_KNIFE			( (EXTRA_SRAM_1 & 2)    >> 1)
#define DOWNGRADE_OCARINA				( (EXTRA_SRAM_1 & 4)    >> 2)
#define DOWNGRADE_HOOKSHOT				( (EXTRA_SRAM_1 & 8)    >> 3)
#define SAVE_30_FPS						( (EXTRA_SRAM_1 & 0x10) >> 4)
#define SAVE_INVERSE_AIM				( (EXTRA_SRAM_1 & 0x20) >> 5)
#define SAVE_NO_IDLE_CAMERA				( (EXTRA_SRAM_1 & 0x40) >> 6)
#define SAVE_EXTRA_ABILITIES			( (EXTRA_SRAM_1 & 0x80) >> 7)

#define EXTRA_SRAM_2					(*(uint8_t*)			0x8011B4FF)
#define SAVE_UNEQUIP_GEAR				  (EXTRA_SRAM_2 & 1)
#define SAVE_UNEQUIP_ITEM				( (EXTRA_SRAM_2 & 2)    >> 1)
#define SAVE_ITEM_ON_B					( (EXTRA_SRAM_2 & 4)    >> 2)
#define SAVE_WEAKER_SWORDS				( (EXTRA_SRAM_2 & 8)    >> 3)
#define SAVE_DOWNGRADE_ITEM				( (EXTRA_SRAM_2 & 0x10) >> 4)
#define SAVE_CROUCH_STAB_FIX			( (EXTRA_SRAM_2 & 0x20) >> 5)
#define SAVE_KEEP_MASK					( (EXTRA_SRAM_2 & 0x40) >> 6)
#define SAVE_TRISWIPE					( (EXTRA_SRAM_2 & 0x80) >> 7)

#define EXTRA_SRAM_3					(*(uint8_t*)			0x8011B500)
#define SAVE_RUPEE_DRAIN				  (EXTRA_SRAM_3 & 15)
#define SAVE_HIDE_HUD					( (EXTRA_SRAM_3 & 0x70) >> 4)
#define SAVE_INFINITE_HP				( (EXTRA_SRAM_3 & 0x80) >> 7)

#define EXTRA_SRAM_4					(*(uint8_t*)			0x8011B501)
#define SAVE_DPAD						  (EXTRA_SRAM_4 & 0x3)
#define SAVE_SHOW_DPAD					( (EXTRA_SRAM_4 & 0xC)  >> 2)
#define SAVE_HUD_LAYOUT					( (EXTRA_SRAM_4 & 0x70) >> 4)
#define SAVE_INFINITE_MP				( (EXTRA_SRAM_4 & 0x80) >> 7)

#define EXTRA_SRAM_5					(*(uint8_t*)			0x8011B4F2)
#define SAVE_FOG						  (EXTRA_SRAM_5 & 15)
#define SAVE_LEVITATION					( (EXTRA_SRAM_5 & 0x10) >> 4)
#define SAVE_INFINITE_RUPEES			( (EXTRA_SRAM_5 & 0x20) >> 5)
#define SAVE_INFINITE_AMMO				( (EXTRA_SRAM_5 & 0x40) >> 6)

#endif