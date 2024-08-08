#ifndef Z64_EXTENDED_H
#define Z64_EXTENDED_H

#include "z64.h"

typedef void(*playsfx_t)(uint16_t sfx, z64_xyzf_t *unk_00_, int8_t unk_01_ , float *unk_02_, float *unk_03_, float *unk_04_);
typedef void(*usebutton_t)(z64_game_t *game, z64_link_t *link, uint8_t item, uint8_t button);

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* Important CFGs */
extern uint8_t CFG_WS;
extern uint8_t CFG_OPTIONS_MENU;

/* Default Options */
extern uint8_t CFG_DEFAULT_30_FPS;
extern uint8_t CFG_DEFAULT_ARROW_TOGGLE;
extern uint8_t CFG_DEFAULT_DPAD;
extern uint8_t CFG_DEFAULT_SHOW_DPAD;
extern uint8_t CFG_DEFAULT_HIDE_HUD;
extern uint8_t CFG_DEFAULT_HUD_LAYOUT;
extern uint8_t CFG_DEFAULT_SHOW_HEALTH;
extern uint8_t CFG_DEFAULT_CHEST_CONTENTS;
extern uint8_t CFG_DEFAULT_A_BUTTON_SCALE;
extern uint8_t CFG_DEFAULT_B_BUTTON_SCALE;
extern uint8_t CFG_DEFAULT_C_LEFT_BUTTON_SCALE;
extern uint8_t CFG_DEFAULT_C_DOWN_BUTTON_SCALE;
extern uint8_t CFG_DEFAULT_C_RIGHT_BUTTON_SCALE;
extern uint8_t CFG_DEFAULT_INVERSE_AIM;
extern uint8_t CFG_DEFAULT_NO_IDLE_CAMERA;
extern uint8_t CFG_DEFAULT_KEEP_MASK;
extern uint8_t CFG_DEFAULT_TRISWIPE;
extern uint8_t CFG_DEFAULT_SWAP_ITEM;
extern uint8_t CFG_DEFAULT_UNEQUIP_ITEM;
extern uint8_t CFG_DEFAULT_UNEQUIP_GEAR;
extern uint8_t CFG_DEFAULT_ITEM_ON_B;
extern uint8_t CFG_DEFAULT_MASK_ABILITIES;
extern uint8_t CFG_DEFAULT_EXTRA_ABILITIES;
extern uint8_t CFG_DEFAULT_DAMAGE_TAKEN;
extern uint8_t CFG_DEFAULT_RANDOM_ENEMIES;
extern uint8_t CFG_DEFAULT_CROUCH_STAB_FIX;
extern uint8_t CFG_DEFAULT_WEAKER_SWORDS;
extern uint8_t CFG_DEFAULT_RUPEE_DRAIN;
extern uint8_t CFG_DEFAULT_FOG;
extern uint8_t CFG_DEFAULT_LEVITATION;
extern uint8_t CFG_DEFAULT_INFINITE_HP;
extern uint8_t CFG_DEFAULT_INFINITE_MP;
extern uint8_t CFG_DEFAULT_INFINITE_RUPEES;
extern uint8_t CFG_DEFAULT_INFINITE_AMMO;

typedef enum {
    OPTIONS_SIZE_CORE  = 17,
    OPTIONS_SIZE_MAIN  = 29,
    OPTIONS_SIZE_ALL   = 35,
    
    OPTION_30_FPS = 0,
    OPTION_ARROW_TOGGLE,
    OPTION_DPAD,
    OPTION_SHOW_DPAD,
    OPTION_HIDE_HUD,
    OPTION_HUD_LAYOUT,
    OPTION_SHOW_HEALTH,
    OPTION_CHEST_CONTENTS,
    OPTION_A_BUTTON_SCALE,
    OPTION_B_BUTTON_SCALE,
    OPTION_C_LEFT_BUTTON_SCALE,
    OPTION_C_DOWN_BUTTON_SCALE,
    OPTION_C_RIGHT_BUTTON_SCALE,
    OPTION_INVERSE_AIM,
    OPTION_NO_IDLE_CAMERA,
    OPTION_KEEP_MASK,
    OPTION_TRISWIPE,
    
    OPTION_SWAP_ITEM,
    OPTION_UNEQUIP_ITEM,
    OPTION_UNEQUIP_GEAR,
    OPTION_ITEM_ON_B,
    OPTION_MASK_ABILITIES,
    OPTION_EXTRA_ABILITIES,
    OPTION_DAMAGE_TAKEN,
    OPTION_RANDOM_ENEMIES,
    OPTION_CROUCH_STAB_FIX,
    OPTION_WEAKER_SWORDS,
    OPTION_RUPEE_DRAIN,
    
    OPTION_FOG,
    OPTION_INVENTORY_EDITOR,
    OPTION_LEVITATION,
    OPTION_INFINITE_HP,
    OPTION_INFINITE_MP,
    OPTION_INFINITE_RUPEES,
    OPTION_INFINITE_AMMO,
} option_t;

typedef enum {
    /* 0x00 */ PLAYER_MASK_NONE,
    /* 0x01 */ PLAYER_MASK_KEATON,
    /* 0x02 */ PLAYER_MASK_SKULL,
    /* 0x03 */ PLAYER_MASK_SPOOKY,
    /* 0x04 */ PLAYER_MASK_BUNNY,
    /* 0x05 */ PLAYER_MASK_GORON,
    /* 0x06 */ PLAYER_MASK_ZORA,
    /* 0x07 */ PLAYER_MASK_GERUDO,
    /* 0x08 */ PLAYER_MASK_TRUTH,
    /* 0x09 */ PLAYER_MASK_MAX
} player_mask_t;

typedef enum {
    /* 0x01 */ CHEST_BOMBS_5 = 1,
    /* 0x02 */ CHEST_DEKU_NUTS,
    /* 0x03 */ CHEST_BOMBCHU_10,
    /* 0x04 */ CHEST_FAIRY_BOW,
    /* 0x05 */ CHEST_FAIRY_SLINGSHOT,
    /* 0x06 */ CHEST_BOOMERANG,
    /* 0x07 */ CHEST_DEKU_STICK_1,
    /* 0x08 */ CHEST_HOOKSHOT,
    /* 0x09 */ CHEST_LONGSHOT,
    /* 0x0A */ CHEST_LENS_OF_TRUTH,
    /* 0x0B */ CHEST_ZELDAS_LETTER,
    /* 0x0C */ CHEST_OCARINA_OF_TIME,
    /* 0x0D */ CHEST_MEGATON_HAMMER,
    /* 0x0E */ CHEST_COJIRO,
    /* 0x0F */ CHEST_EMPTY_BOTTLE,
    /* 0x10 */ CHEST_RED_POTION,
    /* 0x11 */ CHEST_GREEN_POTION,
    /* 0x12 */ CHEST_BLUE_POTION,
    /* 0x13 */ CHEST_FAIRY,
    /* 0x14 */ CHEST_LON_LON_MILK_FULL,
    /* 0x15 */ CHEST_RUTOS_LETTER,
    /* 0x16 */ CHEST_MAGIC_BEAN,
    /* 0x17 */ CHEST_SKULL_MASK,
    /* 0x18 */ CHEST_SPOOKY_MASK,
    /* 0x19 */ CHEST_CUCCO,
    /* 0x1A */ CHEST_KEATON_MASK,
    /* 0x1B */ CHEST_BUNNY_HOOD,
    /* 0x1C */ CHEST_MASK_OF_TRUTH,
    /* 0x1D */ CHEST_POCKET_EGG,
    /* 0x1E */ CHEST_POCKET_CUCCO,
    /* 0x1F */ CHEST_ODD_MUSHROOM,
    /* 0x20 */ CHEST_ODD_POTION,
    /* 0x21 */ CHEST_POACHERS_SAW,
    /* 0x22 */ CHEST_BROKEN_GORONS_SWORD,
    /* 0x23 */ CHEST_PRESCRIPTION,
    /* 0x24 */ CHEST_EYEBALL_FROG,
    /* 0x25 */ CHEST_EYE_DROPS,
    /* 0x26 */ CHEST_CLAIM_CHECK,
    /* 0x27 */ CHEST_KOKIRI_SWORD,
    /* 0x28 */ CHEST_GIANTS_KNIFE,
    /* 0x29 */ CHEST_DEKU_SHIELD,
    /* 0x2A */ CHEST_HYLIAN_SHIELD,
    /* 0x2B */ CHEST_MIRROR_SHIELD,
    /* 0x2C */ CHEST_GORON_TUNIC,
    /* 0x2D */ CHEST_ZORA_TUNIC,
    /* 0x2E */ CHEST_IRON_BOOTS,
    /* 0x2F */ CHEST_HOVER_BOOTS,
    /* 0x30 */ CHEST_QUIVER_40,
    /* 0x31 */ CHEST_QUIVER_50,
    /* 0x32 */ CHEST_BOMB_BAG_20,
    /* 0x33 */ CHEST_BOMB_BAG_30,
    /* 0x34 */ CHEST_BOMB_BAG_40,
    /* 0x35 */ CHEST_SILVER_GAUNTLETS,
    /* 0x36 */ CHEST_GOLDEN_GAUNTLETS,
    /* 0x37 */ CHEST_SILVER_SCALE,
    /* 0x38 */ CHEST_GOLDEN_SCALE,
    /* 0x39 */ CHEST_STONE_OF_AGONY,
    /* 0x3A */ CHEST_GERUDO_MEMBERSHIP_CARD,
    /* 0x3B */ CHEST_FAIRY_OCARINA,
    /* 0x3C */ CHEST_DEKU_SEEDS_5,
    /* 0x3D */ CHEST_HEART_CONTAINER,
    /* 0x3E */ CHEST_PIECE_OF_HEART,
    /* 0x3F */ CHEST_BOSS_KEY,
    /* 0x40 */ CHEST_COMPASS,
    /* 0x41 */ CHEST_DUNGEON_MAP,
    /* 0x42 */ CHEST_SMALL_KEY,
    /* 0x43 */ CHEST_SMALL_MAGIC_JAR,
    /* 0x44 */ CHEST_LARGE_MAGIC_JAR,
    /* 0x45 */ CHEST_ADULT_WALLET,
    /* 0x46 */ CHEST_GIANT_WALLET,
    /* 0x47 */ CHEST_WEIRD_EGG,
    /* 0x48 */ CHEST_RECOVERY_HEART,
    /* 0x49 */ CHEST_ARROWS_5,
    /* 0x4A */ CHEST_ARROWS_10,
    /* 0x4B */ CHEST_ARROWS_30,
    /* 0x4C */ CHEST_GREEN_RUPEE,
    /* 0x4D */ CHEST_BLUE_RUPEE,
    /* 0x4E */ CHEST_RED_RUPEE,
    /* 0x4F */ CHEST_HEART_CONTAINER_2,
    /* 0x50 */ CHEST_LON_LON_MILK,
    /* 0x51 */ CHEST_GORON_MASK,
    /* 0x52 */ CHEST_ZORA_MASK,
    /* 0x53 */ CHEST_GERUDO_MASK,
    /* 0x54 */ CHEST_GORONS_BRACELET,
    /* 0x55 */ CHEST_PURPLE_RUPEE,
    /* 0x56 */ CHEST_GOLD_RUPEE,
    /* 0x57 */ CHEST_BIGGORON_SWORD,
    /* 0x58 */ CHEST_FIRE_ARROW,
    /* 0x59 */ CHEST_ICE_ARROW,
    /* 0x5A */ CHEST_LIGHT_ARROW,
    /* 0x5B */ CHEST_GOLD_SKULLTULA_TOKEN,
    /* 0x5C */ CHEST_DINS_FIRE,
    /* 0x5D */ CHEST_FARORES_WIND,
    /* 0x5E */ CHEST_NAYRUS_LOVE,
    /* 0x5F */ CHEST_BULLET_BAG_30,
    /* 0x60 */ CHEST_BULLET_BAG_40,
    /* 0x61 */ CHEST_DEKU_STICKS_5,
    /* 0x62 */ CHEST_DEKU_STICK_10,
    /* 0x63 */ CHEST_DEKU_NUTS_5,
    /* 0x64 */ CHEST_DEKU_NUTS_10,
    /* 0x65 */ CHEST_BOMB,
    /* 0x66 */ CHEST_BOMBS_10,
    /* 0x67 */ CHEST_BOMBS_20,
    /* 0x68 */ CHEST_BOMBS_30,
    /* 0x69 */ CHEST_DEKU_SEEDS_30,
    /* 0x6A */ CHEST_BOMBCHU_5,
    /* 0x6B */ CHEST_BOMBCHU_20,
    /* 0x6C */ CHEST_FISH,
    /* 0x6D */ CHEST_BUG,
    /* 0x6E */ CHEST_BLUE_FIRE,
    /* 0x6F */ CHEST_POE,
    /* 0x70 */ CHEST_BIG_POE,
    /* 0x71 */ CHEST_DOOR_KEY,
    /* 0x72 */ CHEST_LOSER_GREEN_RUPEE,
    /* 0x73 */ CHEST_LOSER_BLUE_RUPEE,
    /* 0x74 */ CHEST_LOSER_RED_RUPEE,
    /* 0x75 */ CHEST_WINNER_PURPLE_RUPEE,
    /* 0x76 */ CHEST_WINNER_PIECE_OF_HEART,
    /* 0x77 */ CHEST_DEKU_STICK_UPGRADE_20,
    /* 0x78 */ CHEST_DEKU_STICK_UPGRADE_30,
    /* 0x79 */ CHEST_DEKU_NUT_UPGRADE_30,
    /* 0x7A */ CHEST_DEKU_NUT_UPGRADE_40,
    /* 0x7B */ CHEST_BULLET_BAG_50,
    /* 0x7C */ CHEST_ICE_TRAP,
} chest_item_id_t;

typedef enum {
    /* 0x01 */ SCENE_GROTTO = 0x3E,
} scene_id_t;

typedef enum {
    A_BUTTON = -1,
    B_BUTTON,
    C_LEFT_BUTTON,
    C_DOWN_BUTTON,
    C_RIGHT_BUTTON,
} button;

typedef struct {
    char item[0x002C];
} z64_usability_t;

typedef struct {
    uint16_t quiver[4];        // 0,  30,  40,  50
    uint16_t bomb_bag[4];      // 0,  20,  30,  40
    uint16_t u1[8];            // Unused
    uint16_t wallet[4];        // 99, 200, 500, 500
    uint16_t bullet_bag[4];    // 0,  30,  40,  50
    uint16_t stick_upgrade[4]; // 0,  20,  30,  40
    uint16_t nut_upgrade[4];   // 0,  10,  20,  30
} z64_capacity_t;

typedef enum {
    HUD_VISIBILITY_NO_CHANGE       = 0,
    HUD_VISIBILITY_NOTHING,
    HUD_VISIBILITY_NOTHING_ALT,
    HUD_VISIBILITY_HEARTS_FORCE,
    HUD_VISIBILITY_A,
    HUD_VISIBILITY_A_HEARTS_MAGIC_FORCE,
    HUD_VISIBILITY_A_HEARTS_MAGIC_MINIMAP_FORCE,
    HUD_VISIBILITY_ALL_NO_MINIMAP_BY_BTN_STATUS,
    HUD_VISIBILITY_B,
    HUD_VISIBILITY_HEARTS_MAGIC,
    HUD_VISIBILITY_B_ALT,
    HUD_VISIBILITY_HEARTS,
    HUD_VISIBILITY_A_B_MINIMAP,
    HUD_VISIBILITY_HEARTS_MAGIC_FORCE,
    HUD_VISIBILITY_ALL             = 50,
    HUD_VISIBILITY_NOTHING_INSTANT = 52
} z64_hud_visibility_mode_t;



/* Functions */
#define z64_usebutton                   ((usebutton_t)          0x8038C9A0)

/* DRAM addresses & data */
#define z64_usability                   (*(z64_usability_t*)    0x8039F114)
#define z64_capacity                    (*(z64_capacity_t*)     0x800F8CCC)
//#define z64_change_scene              (*(uint32_t*)           0x801DB09C)
#define z64_has_minimap                 (*(uint16_t*)           0x8018884C) // 0x8011B9B3, 8017643C, 8018884C
#define z64_dungeon_scene               (*(uint16_t*)           0x801D8BEA)
#define z64_scene                       (*(uint16_t*)           0x801C8544)
#define z64_camera_view                 (*(uint8_t*)            0x801DB0CD)
//#define z64_mask_equipped             (*(uint8_t*)            0x801DAB7F)
#define z64_throwing_nut                (*(uint8_t*)            0x80124696)
#define z64_triswipe                    (*(uint8_t*)            0x8011B9ED)
#define z64_x_axis_input                (*(int8_t*)             0x801C84C8)
#define z64_y_axis_input                (*(int8_t*)             0x801C84C9)
#define z64_button_input                (*(int16_t*)            0x801C84C0) // 801C84B4, 801C84BA, 801C84C6
#define z64_link_animation_parameter    (*(uint16_t*)           0x801DABF0)
#define z64_link_a_action               (*(uint16_t*)           0x801DAA90)
#define z64_idle_camera_counter         (*(uint8_t*)            0x801C86CF)
#define z64_inventory_editor_index      (*(uint8_t*)            0x8039EA59)
#define z64_damage_taken_modifier_1     (*(uint32_t*)           0x8038E5D0)
#define z64_damage_taken_modifier_2     (*(uint32_t*)           0x8038E5D8)
#define z64_damage_taken_modifier_3     (*(uint16_t*)           0x8038E5EA)
#define z64_textbox                     (*(uint16_t*)           0x801D8870)
#define z64_check_lava_floor            (*(uint16_t*)           0x8038EBE4)
#define z64_check_lava_trail_1          (*(uint8_t*)            0x8007A6F7)
#define z64_check_lava_trail_2          (*(uint8_t*)            0x8007A6FF)
#define z64_check_wall_damage           (*(uint32_t*)           0x800347EC)
#define z64_check_freeze                (*(uint16_t*)           0x8038E760)
#define z64_check_shock                 (*(uint16_t*)           0x8038E7C8)

/* DRAM addresses & data for HUD */
#define z64_b_button_label_x            (*(uint16_t*)           0x801C7C3A)
#define z64_b_button_label_y            (*(uint16_t*)           0x801C7C3E)
#define z64_c_left_x_set_item           (*(uint16_t*)           0x8039EAF8)
#define z64_c_left_y_set_item           (*(uint16_t*)           0x8039EB00)
#define z64_c_down_x_set_item           (*(uint16_t*)           0x8039EAFA)
#define z64_c_down_y_set_item           (*(uint16_t*)           0x8039EB02)
#define z64_c_right_x_set_item          (*(uint16_t*)           0x8039EAFC)
#define z64_c_right_y_set_item          (*(uint16_t*)           0x8039EB04)

/* DRAM addresses & data for items and Lens of Truth on D-Pad */
#define z64_dpad_lens_1                 (*(uint16_t*)           0x80072D40)
#define z64_dpad_lens_2                 (*(uint16_t*)           0x80072D4C)
#define z64_dpad_lens_3                 (*(uint16_t*)           0x80072D58)

#define z64_dpad_item_b                 (*(uint16_t*)           0x8038A948)
#define z64_dpad_item_1                 (*(uint16_t*)           0x8038A978)
#define z64_dpad_item_2                 (*(uint16_t*)           0x8038A9A8)
#define z64_dpad_item_3                 (*(uint16_t*)           0x8038A9D4)

/* DRAM addresses & data for FPS */
#define fps_limit                       (*(uint8_t*)            0x801C6FA1)
#define control_link                    (*(uint16_t*)           0x801DAADE)

/* DRAM addresses & data for Medallion Abilities */
#define z64_tunic_color                 (*(uint8_t*)            0x801DAB6C)
#define z64_move_speed                  (*(uint16_t*)           0x801DB258)
#define z64_max_move_speed              (*(uint16_t*)           0x801DB2A0)
#define z64_damage_frames               (*(uint8_t*)            0x801DB498)
#define z64_damage_flag_1               (*(uint32_t*)           0x801DAF1C)
#define z64_damage_flag_2               (*(uint32_t*)           0x801DAF9C)
#define z64_inner_red_trail_r           (*(uint8_t*)            0x80273794)
#define z64_inner_red_trail_g           (*(uint8_t*)            0x80273795)
#define z64_inner_red_trail_b           (*(uint8_t*)            0x80273796)
#define z64_outer_red_trail_r           (*(uint8_t*)            0x802738B4)
#define z64_outer_red_trail_g           (*(uint8_t*)            0x802738B5)
#define z64_outer_red_trail_b           (*(uint8_t*)            0x802738B6)

/* Options */
#define OPTION_ACTIVE(menu, save, default)       ( (CFG_OPTIONS_MENU >= menu && save)          || (CFG_OPTIONS_MENU < menu && default)          )
#define OPTION_VALUE(menu, value, save, default) ( (CFG_OPTIONS_MENU >= menu && save == value) || (CFG_OPTIONS_MENU < menu && default == value) )
#define IS_MASK_COMPLETED(paid_back, sold)       (GET_EVENTCHKINF(paid_back) && GET_ITEMGETINF(sold) )

/* Availability */
#define HAS_MAGIC                       (z64_file.magic_acquired && z64_file.magic_capacity_set)
#define CAN_CONTROL_LINK                (z64_game.pause_ctxt.state == 0 && (uint32_t)z64_ctxt.state_dtor == z64_state_ovl_tab[3].vram_dtor && z64_file.game_mode == 0 && (z64_event_state_1 & 0x20) == 0)
#define IS_PAUSE_SCREEN_CURSOR          (z64_game.pause_ctxt.state == 6 && z64_game.pause_ctxt.unk_02_[1] == 0 && z64_game.pause_ctxt.cursor_pos != 0xA && z64_game.pause_ctxt.cursor_pos != 0xB)
#define TYCOON_WALLET                   (z64_file.event_chk_inf[13] & (1 << 13) )

/* D-Pad & Controls Availability */
#define BLOCK_DPAD                      (0x00000001 | 0x00000002 | 0x00000080 | 0x00000400 | 0x10000000 | 0x20000000)
#define CAN_USE_DPAD                    ( (z64_link.state_flags_1 & BLOCK_DPAD) == 0 && (uint32_t)z64_ctxt.state_dtor == z64_state_ovl_tab[3].vram_dtor && z64_file.game_mode == 0 && (z64_event_state_1 & 0x20) == 0)
#define CAN_DRAW_HUD                    ( ( (uint32_t)z64_ctxt.state_dtor==z64_state_ovl_tab[3].vram_dtor) && (z64_file.game_mode == 0) && ( (z64_event_state_1 & 0x20) == 0) )
#define IS_SEMI_ALPHA                   (z64_game.pause_ctxt.state == 0 && alpha >= 0x46)
#define CAN_USE_SWAP(index)             (IS_PAUSE_SCREEN_CURSOR && (z64_game.pause_ctxt.changing == 0 || z64_game.pause_ctxt.changing == 3) && z64_game.pause_ctxt.screen_idx == index)
#define CAN_USE_MASK_SWAP               (CAN_USE_SWAP(0) && z64_game.pause_ctxt.item_cursor == Z64_SLOT_CHILD_TRADE && OPTION_ACTIVE(1, SAVE_SWAP_ITEM, CFG_DEFAULT_SWAP_ITEM) && GET_INFTABLE(INFTABLE_GIVEN_ZELDAS_LETTER) && \
                                        z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_KEATON_MASK && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH) 
#define CAN_USE_OCARINA_SWAP            (CAN_USE_SWAP(0) && z64_game.pause_ctxt.item_cursor  == Z64_SLOT_OCARINA  && OPTION_ACTIVE(2, SAVE_SWAP_ITEM, CFG_DEFAULT_SWAP_ITEM) && DOWNGRADE_OCARINA)
#define CAN_USE_HOOKSHOT_SWAP           (CAN_USE_SWAP(0) && z64_game.pause_ctxt.item_cursor  == Z64_SLOT_HOOKSHOT && OPTION_ACTIVE(2, SAVE_SWAP_ITEM, CFG_DEFAULT_SWAP_ITEM) && DOWNGRADE_HOOKSHOT)
#define CAN_USE_GIANTS_KNIFE_SWAP       (CAN_USE_SWAP(3) && z64_game.pause_ctxt.equip_cursor == 3                 && OPTION_ACTIVE(2, SAVE_SWAP_ITEM, CFG_DEFAULT_SWAP_ITEM) && DOWNGRADE_GIANTS_KNIFE)
                                        

/* D-Pad Usability for Items */
#define BLOCK_ITEMS                     (0x00800000 | 0x00000800 | 0x00200000 | 0x08000000)
#define CAN_USE_OCARINA                 (z64_game.pause_ctxt.state == 0 && (z64_file.items[Z64_SLOT_OCARINA]     == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA]     == Z64_ITEM_OCARINA_OF_TIME) && !z64_game.restriction_flags.ocarina     && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_CHILD_TRADE             (z64_game.pause_ctxt.state == 0 && (z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG     && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH)   && !z64_game.restriction_flags.trade_items && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_ADULT_TRADE             (z64_game.pause_ctxt.state == 0 && (z64_file.items[Z64_SLOT_ADULT_TRADE] >= Z64_ITEM_POCKET_EGG    && z64_file.items[Z64_SLOT_ADULT_TRADE] <= Z64_ITEM_CLAIM_CHECK)     && !z64_game.restriction_flags.trade_items && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_ITEMS                   (z64_game.pause_ctxt.state == 0 && !z64_game.restriction_flags.all && ( (z64_link.state_flags_1 & BLOCK_ITEMS) == 0) )
#define CAN_USE_LENS                    (z64_game.pause_ctxt.state == 0 && (!z64_game.restriction_flags.all || z64_game.scene_index == 0x0010) && ( (z64_link.state_flags_1 & BLOCK_ITEMS) == 0) )
#define CAN_USE_FARORES_WIND            (z64_game.pause_ctxt.state == 0 && !z64_game.restriction_flags.farores_wind && ( (z64_link.state_flags_1 & BLOCK_ITEMS) == 0) )

/* D-Pad SRAM locations & Button Mappings */
#define DPAD_CHILD_SET1_UP              (z64_file.unk_E8C[0x0])
#define DPAD_CHILD_SET1_RIGHT           (z64_file.unk_E8C[0x1])
#define DPAD_CHILD_SET1_DOWN            (z64_file.unk_E8C[0x2])
#define DPAD_CHILD_SET1_LEFT            (z64_file.unk_E8C[0x3])
#define DPAD_CHILD_SET2_UP              (z64_file.unk_E8C[0x4])
#define DPAD_CHILD_SET2_RIGHT           (z64_file.unk_E8C[0x5])
#define DPAD_CHILD_SET2_DOWN            (z64_file.unk_E8C[0x6])
#define DPAD_CHILD_SET2_LEFT            (z64_file.unk_E8C[0x7])
#define DPAD_ADULT_SET1_UP              (z64_file.unk_E8C[0x8])
#define DPAD_ADULT_SET1_RIGHT           (z64_file.unk_E8C[0x9])
#define DPAD_ADULT_SET1_DOWN            (z64_file.unk_E8C[0xA])
#define DPAD_ADULT_SET1_LEFT            (z64_file.unk_E8C[0xB])
#define DPAD_ADULT_SET2_UP              (z64_file.unk_E8C[0xC])
#define DPAD_ADULT_SET2_RIGHT           (z64_file.unk_E8C[0xD])
#define DPAD_ADULT_SET2_DOWN            (z64_file.unk_E8C[0xE])
#define DPAD_ADULT_SET2_LEFT            (z64_file.unk_E8C[0xF])
#define DPAD_BUTTON_INDEX(index)        ( (z64_file.link_age ? 0 : 8) + (GET_DPAD_ALT ? 4 : 0) + index)
#define DPAD_SET_BUTTON(index)          (z64_file.unk_E8C[index])
#define DPAD_SET_BUTTON_INDEX(index)    (z64_file.unk_E8C[DPAD_BUTTON_INDEX(index)])

/* Extra saving for Redux Options (8011B4C8) */
#define DPAD_INIT_SETUP                 ( (z64_file.inf_table[0x15] & 0x0001) >> 0)
#define DOWNGRADE_GIANTS_KNIFE          ( (z64_file.inf_table[0x15] & 0x0002) >> 1)
#define DOWNGRADE_OCARINA               ( (z64_file.inf_table[0x15] & 0x0004) >> 2)
#define DOWNGRADE_HOOKSHOT              ( (z64_file.inf_table[0x15] & 0x0008) >> 3)

#define SAVE_MASK_ABILITIES             ( (z64_file.inf_table[0x13] & 0x0001) >> 0)

#define SAVE_CROUCH_STAB_FIX            ( (z64_file.inf_table[0x14] & 0x0100) >> 8)
#define SAVE_WEAKER_SWORDS              ( (z64_file.inf_table[0x14] & 0x0200) >> 9)
#define SAVE_EXTRA_ABILITIES            ( (z64_file.inf_table[0x14] & 0x0400) >> 10)
#define SAVE_LEVITATION                 ( (z64_file.inf_table[0x14] & 0x0800) >> 11)
#define SAVE_INFINITE_HP                ( (z64_file.inf_table[0x14] & 0x1000) >> 12)
#define SAVE_INFINITE_MP                ( (z64_file.inf_table[0x14] & 0x2000) >> 13)
#define SAVE_INFINITE_RUPEES            ( (z64_file.inf_table[0x14] & 0x4000) >> 14)
#define SAVE_INFINITE_AMMO              ( (z64_file.inf_table[0x14] & 0x8000) >> 15)

#define SAVE_30_FPS                     ( (z64_file.inf_table[0x15] & 0x0010) >> 4)
#define SAVE_ARROW_TOGGLE               ( (z64_file.inf_table[0x15] & 0x0020) >> 5)
#define SAVE_SHOW_HEALTH                ( (z64_file.inf_table[0x15] & 0x0040) >> 6)
#define SAVE_CHEST_CONTENTS             ( (z64_file.inf_table[0x15] & 0x0080) >> 7)
#define SAVE_INVERSE_AIM                ( (z64_file.inf_table[0x15] & 0x0100) >> 8)
#define SAVE_NO_IDLE_CAMERA             ( (z64_file.inf_table[0x15] & 0x0200) >> 9)
#define SAVE_KEEP_MASK                  ( (z64_file.inf_table[0x15] & 0x0400) >> 10)
#define SAVE_TRISWIPE                   ( (z64_file.inf_table[0x15] & 0x0800) >> 11)
#define SAVE_UNEQUIP_ITEM               ( (z64_file.inf_table[0x15] & 0x1000) >> 12)
#define SAVE_UNEQUIP_GEAR               ( (z64_file.inf_table[0x15] & 0x2000) >> 13)
#define SAVE_ITEM_ON_B                  ( (z64_file.inf_table[0x15] & 0x4000) >> 14)
#define SAVE_SWAP_ITEM                  ( (z64_file.inf_table[0x15] & 0x8000) >> 15)

#define SAVE_RUPEE_DRAIN                ( (z64_file.inf_table[0x18] & 0x000F) >> 0)  // Options: 15
#define SAVE_FOG                        ( (z64_file.inf_table[0x18] & 0x00F0) >> 4)  // Options: 15

#define SAVE_DPAD                       ( (z64_file.inf_table[0x1B] & 0x0003) >> 0)  // Options: 2
#define SAVE_SHOW_DPAD                  ( (z64_file.inf_table[0x1B] & 0x000C) >> 2)  // Options: 3
#define SAVE_HIDE_HUD                   ( (z64_file.inf_table[0x1B] & 0x0070) >> 4)  // Options: 4
#define SAVE_HUD_LAYOUT                 ( (z64_file.inf_table[0x1B] & 0x0380) >> 7)  // Options: 6
#define SAVE_A_BUTTON_SCALE             ( (z64_file.inf_table[0x1B] & 0x0C00) >> 10) // Options: 2
#define SAVE_B_BUTTON_SCALE             ( (z64_file.inf_table[0x1B] & 0x7000) >> 12) // Options: 7
#define SAVE_RANDOM_ENEMIES             ( (z64_file.inf_table[0x1B] & 0x8000) >> 15) // Options: 1

#define SAVE_C_LEFT_BUTTON_SCALE        ( (z64_file.inf_table[0x1C] & 0x0007) >> 0)  // Options: 7
#define SAVE_C_DOWN_BUTTON_SCALE        ( (z64_file.inf_table[0x1C] & 0x0070) >> 4)  // Options: 7
#define SAVE_C_RIGHT_BUTTON_SCALE       ( (z64_file.inf_table[0x1C] & 0x0700) >> 8)  // Options: 7
#define SAVE_DAMAGE_TAKEN               ( (z64_file.inf_table[0x1C] & 0x7000) >> 12) // Options: 7

/* Extra Saving for Redux */
#define LAST_MASK                       ( z64_file.unk_06_[z64_file.link_age] )
#define SET_DPAD_ALT_                   ( SET_INFTABLE(INFTABLE_02) )
#define CLEAR_DPAD_ALT                  ( CLEAR_INFTABLE(INFTABLE_02) )
#define GET_DPAD_ALT                    ( (OPTION_VALUE(1, 2, SAVE_DPAD, CFG_DEFAULT_DPAD) ) ? GET_INFTABLE(INFTABLE_02) : 0)
#define TOGGLE_DPAD_ALT                 ( GET_INFTABLE(INFTABLE_02) ?  CLEAR_INFTABLE(INFTABLE_02) : SET_INFTABLE(INFTABLE_02) )

/* Keeping track of values */
#define A_BUTTON_SCALE                  (*(uint16_t*)           0x80074F76)
#define A_BUTTON_TEXT_SCALE             (*(uint16_t*)           0x8007650E)
#define MAX_SWORD_HEALTH                (*(uint8_t*)            0x8038E1A3)
#define SWORD_HEALTH                    (*(uint8_t*)            0x8011B4F8) // 0x30

/* Extra Ability Tunic Colors */
#define TUNIC_KOKIRI_R                  (*(uint8_t*)            0x800F7AD8)
#define TUNIC_KOKIRI_G                  (*(uint8_t*)            0x800F7AD9)
#define TUNIC_KOKIRI_B                  (*(uint8_t*)            0x800F7ADA)
#define TUNIC_GORON_R                   (*(uint8_t*)            0x800F7ADB)
#define TUNIC_GORON_G                   (*(uint8_t*)            0x800F7ADC)
#define TUNIC_GORON_B                   (*(uint8_t*)            0x800F7ADD)
#define TUNIC_ZORA_R                    (*(uint8_t*)            0x800F7ADE)
#define TUNIC_ZORA_G                    (*(uint8_t*)            0x800F7ADF)
#define TUNIC_ZORA_B                    (*(uint8_t*)            0x800F7AE0)
#define TUNIC_UNUSED_R                  (*(uint8_t*)            0x800F7AE1)
#define TUNIC_UNUSED_G                  (*(uint8_t*)            0x800F7AE2)
#define TUNIC_UNUSED_B                  (*(uint8_t*)            0x800F7AE3)

#define MP_FIRE_ARROW                   (*(uint8_t*)            0x803AAC00)
#define MP_ICE_ARROW                    (*(uint8_t*)            0x803AAC01)
#define MP_LIGHT_ARROW                  (*(uint8_t*)            0x803AAC02)

/* Player States */
#define PLAYER_STATE1_EXITING                  (1 << 0)
#define PLAYER_STATE1_SWINGING_BOTTLE          (1 << 1)
#define PLAYER_STATE1_2                        (1 << 2)
#define PLAYER_STATE1_HOLDING_RANGED           (1 << 3)
#define PLAYER_STATE1_HAS_TARGET               (1 << 4)
#define PLAYER_STATE1_5                        (1 << 5)
#define PLAYER_STATE1_TALKING_1                (1 << 6)
#define PLAYER_STATE1_7                        (1 << 7)
#define PLAYER_STATE1_START_CHANGING_HELD_ITEM (1 << 8)
#define PLAYER_STATE1_AIMING                   (1 << 9)
#define PLAYER_STATE1_OPENING_CHEST            (1 << 10)
#define PLAYER_STATE1_HOLDING_ABOVE_HEAD       (1 << 11)
#define PLAYER_STATE1_12                       (1 << 12)
#define PLAYER_STATE1_HANGING                  (1 << 13)
#define PLAYER_STATE1_FINISHED_CLIMBING_1      (1 << 14)
#define PLAYER_STATE1_HOLDING_Z                (1 << 15)
#define PLAYER_STATE1_TALKING_2                (1 << 16)
#define PLAYER_STATE1_FIXED_MOVEMENT           (1 << 17)
#define PLAYER_STATE1_JUMPING                  (1 << 18)
#define PLAYER_STATE1_FINISHED_CLIMBING_2      (1 << 19)
#define PLAYER_STATE1_FIRST_PERSON             (1 << 20)
#define PLAYER_STATE1_CLIMBING                 (1 << 21)
#define PLAYER_STATE1_SHIELDING                (1 << 22)
#define PLAYER_STATE1_RIDING                   (1 << 23)
#define PLAYER_STATE1_HOLDING_BOOMERANG        (1 << 24)
#define PLAYER_STATE1_25                       (1 << 25)
#define PLAYER_STATE1_DAMAGED                  (1 << 26)
#define PLAYER_STATE1_SWIMMING                 (1 << 27)
#define PLAYER_STATE1_PLAYING_OCARINA          (1 << 28)
#define PLAYER_STATE1_NO_CONTROL               (1 << 29)
#define PLAYER_STATE1_30                       (1 << 30)
#define PLAYER_STATE1_31                       (1 << 31)

#define PLAYER_STATE2_0                         (1 << 0)
#define PLAYER_STATE2_1                         (1 << 1)
#define PLAYER_STATE2_2                         (1 << 2)
#define PLAYER_STATE2_PULLING_ITEM              (1 << 3)
#define PLAYER_STATE2_4                         (1 << 4)
#define PLAYER_STATE2_MOVING                    (1 << 5)
#define PLAYER_STATE2_HANGING                   (1 << 6)
#define PLAYER_STATE2_7                         (1 << 7)
#define PLAYER_STATE2_8                         (1 << 8)
#define PLAYER_STATE2_FORCE_SAND_FLOOR_SOUND    (1 << 9)
#define PLAYER_STATE2_10                        (1 << 10)
#define PLAYER_STATE2_11                        (1 << 11)
#define PLAYER_STATE2_IDLE_ON_LADDER            (1 << 12)
#define PLAYER_STATE2_13                        (1 << 13)
#define PLAYER_STATE2_14                        (1 << 14)
#define PLAYER_STATE2_15                        (1 << 15)
#define PLAYER_STATE2_DO_ACTION_ENTER           (1 << 16) // Turns on the "Enter On A" DoAction
#define PLAYER_STATE2_17                        (1 << 17)
#define PLAYER_STATE2_CRAWLING                  (1 << 18) // Crawling through a crawlspace
#define PLAYER_STATE2_JUMPING                   (1 << 19)
#define PLAYER_STATE2_NAVI_PRESENT              (1 << 20)
#define PLAYER_STATE2_21                        (1 << 21)
#define PLAYER_STATE2_22                        (1 << 22)
#define PLAYER_STATE2_23                        (1 << 23)
#define PLAYER_STATE2_24                        (1 << 24)
#define PLAYER_STATE2_25                        (1 << 25)
#define PLAYER_STATE2_26                        (1 << 26)
#define PLAYER_STATE2_PLAYING_OCARINA           (1 << 27)
#define PLAYER_STATE2_IDLE                      (1 << 28)
#define PLAYER_STATE2_29                        (1 << 29)
#define PLAYER_STATE2_30                        (1 << 30)
#define PLAYER_STATE2_31                        (1 << 31)

/* Damage Flags */
#define DMG_DEKU_NUT     (1 << 0)
#define DMG_DEKU_STICK   (1 << 1)
#define DMG_SLINGSHOT    (1 << 2)
#define DMG_EXPLOSIVE    (1 << 3)
#define DMG_BOOMERANG    (1 << 4)
#define DMG_ARROW_NORMAL (1 << 5)
#define DMG_HAMMER_SWING (1 << 6)
#define DMG_HOOKSHOT     (1 << 7)
#define DMG_SLASH_KOKIRI (1 << 8)
#define DMG_SLASH_MASTER (1 << 9)
#define DMG_SLASH_GIANT  (1 << 10)
#define DMG_ARROW_FIRE   (1 << 11)
#define DMG_ARROW_ICE    (1 << 12)
#define DMG_ARROW_LIGHT  (1 << 13)
#define DMG_ARROW_UNK1   (1 << 14)
#define DMG_ARROW_UNK2   (1 << 15)
#define DMG_ARROW_UNK3   (1 << 16)
#define DMG_MAGIC_FIRE   (1 << 17)
#define DMG_MAGIC_ICE    (1 << 18)
#define DMG_MAGIC_LIGHT  (1 << 19)
#define DMG_SHIELD       (1 << 20)
#define DMG_MIR_RAY      (1 << 21)
#define DMG_SPIN_KOKIRI  (1 << 22)
#define DMG_SPIN_GIANT   (1 << 23)
#define DMG_SPIN_MASTER  (1 << 24)
#define DMG_JUMP_KOKIRI  (1 << 25)
#define DMG_JUMP_GIANT   (1 << 26)
#define DMG_JUMP_MASTER  (1 << 27)
#define DMG_UNKNOWN_1    (1 << 28)
#define DMG_UNBLOCKABLE  (1 << 29)
#define DMG_HAMMER_JUMP  (1 << 30)
#define DMG_UNKNOWN_2    (1 << 31)

#define DMG_SLASH (DMG_SLASH_KOKIRI | DMG_SLASH_MASTER | DMG_SLASH_GIANT)
#define DMG_SPIN_ATTACK (DMG_SPIN_KOKIRI | DMG_SPIN_MASTER | DMG_SPIN_GIANT)
#define DMG_JUMP_SLASH (DMG_JUMP_KOKIRI | DMG_JUMP_MASTER | DMG_JUMP_GIANT)
#define DMG_SWORD (DMG_SLASH | DMG_SPIN_ATTACK | DMG_JUMP_SLASH)
#define DMG_HAMMER (DMG_HAMMER_SWING | DMG_HAMMER_JUMP)
#define DMG_FIRE (DMG_ARROW_FIRE | DMG_MAGIC_FIRE)
#define DMG_ARROW (DMG_ARROW_NORMAL | DMG_ARROW_FIRE | DMG_ARROW_ICE | DMG_ARROW_LIGHT | DMG_ARROW_UNK1 | DMG_ARROW_UNK2 | DMG_ARROW_UNK3)
#define DMG_RANGED (DMG_ARROW | DMG_HOOKSHOT | DMG_SLINGSHOT)
#define DMG_DEFAULT ~(DMG_SHIELD | DMG_MIR_RAY)

#endif