;==================================================================================================
; Settings and tables which the front-end may write
;==================================================================================================

; This is used to determine if and how the cosmetics can be patched
; It this moves then the version will no longer be valid, so it is important that this does not move
COSMETIC_CONTEXT:

COSMETIC_FORMAT_VERSION:
.word 0x1F073FD8

; HUDC string
.byte 0x48
.byte 0x55
.byte 0x44
.byte 0x43
CFG_MAGIC_COLOR:
.halfword 0x0000, 0x00C8, 0x0000
CFG_HEART_COLOR:
.halfword 0x00FF, 0x0046, 0x0032
CFG_A_BUTTON_COLOR:
.halfword 0x005A, 0x005A, 0x00FF
CFG_B_BUTTON_COLOR:
.halfword 0x0000, 0x0096, 0x0000
CFG_C_BUTTON_COLOR:
.halfword 0x00FF, 0x00A0, 0x0000
CFG_TEXT_CURSOR_COLOR:
.halfword 0x0000, 0x0050, 0x00C8
CFG_SHOP_CURSOR_COLOR:
.halfword 0x0000, 0x0050, 0x00FF
CFG_A_NOTE_COLOR:
.halfword 0x0050, 0x0096, 0x00FF
CFG_C_NOTE_COLOR:
.halfword 0x00FF, 0x00FF, 0x0032
CFG_BOOM_TRAIL_INNER_COLOR:
.byte 0xFF, 0xFF, 0x64
CFG_BOOM_TRAIL_OUTER_COLOR:
.byte 0xFF, 0xFF, 0x64
CFG_BOMBCHU_TRAIL_INNER_COLOR:
.byte 0xFA, 0x00, 0x00
CFG_BOMBCHU_TRAIL_OUTER_COLOR:
.byte 0xFA, 0x00, 0x00
CFG_RAINBOW_SWORD_INNER_ENABLED:
.byte 0x00
CFG_RAINBOW_SWORD_OUTER_ENABLED:
.byte 0x00
CFG_RAINBOW_BOOM_TRAIL_INNER_ENABLED:
.byte 0x00
CFG_RAINBOW_BOOM_TRAIL_OUTER_ENABLED:
.byte 0x00
CFG_RAINBOW_BOMBCHU_TRAIL_INNER_ENABLED:
.byte 0x00
CFG_RAINBOW_BOMBCHU_TRAIL_OUTER_ENABLED:
.byte 0x00
CFG_RAINBOW_NAVI_IDLE_INNER_ENABLED:
.byte 0x00
CFG_RAINBOW_NAVI_IDLE_OUTER_ENABLED:
.byte 0x00
CFG_RAINBOW_NAVI_ENEMY_INNER_ENABLED:
.byte 0x00
CFG_RAINBOW_NAVI_ENEMY_OUTER_ENABLED:
.byte 0x00
CFG_RAINBOW_NAVI_NPC_INNER_ENABLED:
.byte 0x00
CFG_RAINBOW_NAVI_NPC_OUTER_ENABLED:
.byte 0x00
CFG_RAINBOW_NAVI_PROP_INNER_ENABLED:
.byte 0x00
CFG_RAINBOW_NAVI_PROP_OUTER_ENABLED:
.byte 0x00
CFG_SHOW_SETTING_INFO:
.byte 0x00

; DEFS string
.byte 0x44
.byte 0x45
.byte 0x46
.byte 0x53
CFG_DEFAULT_30_FPS:
.byte 0x00
CFG_DEFAULT_ARROW_TOGGLE:
.byte 0x01
CFG_DEFAULT_DPAD:
.byte 0x01
CFG_DEFAULT_SHOW_DPAD:
.byte 0x01
CFG_DEFAULT_HIDE_HUD:
.byte 0x00
CFG_DEFAULT_HUD_LAYOUT:
.byte 0x00
CFG_DEFAULT_SHOW_HEALTH:
.byte 0x00
CFG_DEFAULT_CHEST_CONTENTS:
.byte 0x00
CFG_DEFAULT_A_BUTTON_SCALE:
.byte 0x00
CFG_DEFAULT_B_BUTTON_SCALE:
.byte 0x00
CFG_DEFAULT_C_LEFT_BUTTON_SCALE:
.byte 0x00
CFG_DEFAULT_C_DOWN_BUTTON_SCALE:
.byte 0x00
CFG_DEFAULT_C_RIGHT_BUTTON_SCALE:
.byte 0x00
CFG_DEFAULT_INVERSE_AIM:
.byte 0x00
CFG_DEFAULT_NO_IDLE_CAMERA:
.byte 0x00
CFG_DEFAULT_KEEP_MASK:
.byte 0x01
CFG_DEFAULT_TRISWIPE:
.byte 0x00
CFG_DEFAULT_SWAP_ITEM:
.byte 0x01
CFG_DEFAULT_UNEQUIP_ITEM:
.byte 0x00
CFG_DEFAULT_UNEQUIP_GEAR:
.byte 0x00
CFG_DEFAULT_ITEM_ON_B:
.byte 0x00
CFG_DEFAULT_MASK_ABILITIES:
.byte 0x01
CFG_DEFAULT_EXTRA_ABILITIES:
.byte 0x00
CFG_DEFAULT_DAMAGE_TAKEN:
.byte 0x00
CFG_DEFAULT_RANDOM_ENEMIES:
.byte 0x00
CFG_DEFAULT_CROUCH_STAB_FIX:
.byte 0x00
CFG_DEFAULT_WEAKER_SWORDS:
.byte 0x00
CFG_DEFAULT_RUPEE_DRAIN:
.byte 0x00
CFG_DEFAULT_FOG:
.byte 0x00
CFG_DEFAULT_LEVITATION:
.byte 0x00
CFG_DEFAULT_INFINITE_HP:
.byte 0x00
CFG_DEFAULT_INFINITE_MP:
.byte 0x00
CFG_DEFAULT_INFINITE_RUPEES:
.byte 0x00
CFG_DEFAULT_INFINITE_AMMO:
.byte 0x00

; MISC string
.byte 0x4D
.byte 0x49
.byte 0x53
.byte 0x43
CFG_ALLOW_KOKIRI_SWORD:
.byte 0x00
CFG_ALLOW_MASTER_SWORD:
.byte 0x00
CFG_ALLOW_GIANTS_KNIFE:
.byte 0x00
CFG_ALLOW_DEKU_SHIELD:
.byte 0x00
CFG_ALLOW_MIRROR_SHIELD:
.byte 0x00
CFG_ALLOW_TUNIC:
.byte 0x00
CFG_ALLOW_BOOTS:
.byte 0x00
CFG_WS:
.byte 0x00
CFG_TYCOON_WALLET:
.byte 0x00
CFG_PREVENT_GROTTO_SAVING:
.byte 0x00
CFG_OPTIONS_MENU:
.byte 0x03

CFG_TUNIC_MAGICIAN:
.byte 0x00, 0x91, 0x3C
CFG_TUNIC_GUARDIAN:
.byte 0xA5, 0xAF, 0xBE
CFG_TUNIC_HERO:
.byte 0xC8, 0xAF, 0x32
CFG_TUNIC_NONE:
.byte 0x64, 0xB4, 0xFF
CFG_TUNIC_SHADOW:
.byte 0x50, 0x3C, 0xAA

.area 0x20, 0
CFG_CUSTOM_MESSAGE_1:
.endarea
.area 0x20, 0
CFG_CUSTOM_MESSAGE_2:
.endarea

.align 4

; Version string
.area 0x24, 0
VERSION_STRING_TXT:
.endarea

; World string (max length "255 of 255" = 10 chars)
.area 0x10, 0
WORLD_STRING_TXT:
.endarea

; Time string
.area 0x24, 0
TIME_STRING_TXT:
.endarea

; Initial Save Data table:
;
; This table describes what extra data should be written when a new save file is created. It must be terminated with
; four 0x00 bytes (which will happen by default as long as you don't fill the allotted space).
;
; Row format (4 bytes):
; AAAATTVV
; AAAA = Offset from the start of the save data
; TT = Type (0x00 = or value with current value, 0x01 = set the byte to the given value)
; VV = Value to write to the save

.area 0x400, 0
INITIAL_SAVE_DATA:
.endarea

.area 0x20, 0
EXTENDED_OBJECT_TABLE:
.endarea

BOMBCHUS_IN_LOGIC:
.word 0x00

GOSSIP_HINT_CONDITION:
.word 0x00
; 0 = Mask of Truth
; 1 = Stone of Agony
; 2 = No Requirements

FREE_SCARECROW_ENABLED:
.word 0x00

JABU_ELEVATOR_ENABLE:
.byte 0x00
OCARINAS_SHUFFLED:
.byte 0x00
FAST_CHESTS:
.byte 0x00
SHUFFLE_COWS:
.byte 0x00
SONGS_AS_ITEMS:
.byte 0x00
WINDMILL_SONG_ID:
.byte 0x00
WINDMILL_TEXT_ID:
.byte 0x00
MALON_TEXT_ID:
.byte 0x00
DISABLE_TIMERS:
.byte 0x00
DUNGEONS_SHUFFLED:
.byte 0x00
OVERWORLD_SHUFFLED:
.byte 0x00
FAST_BUNNY_HOOD_ENABLED:
.byte 0x00
SPOILER_AVAILABLE:
.byte 0x00
PLANDOMIZER_USED:
.byte 0x00
.align 4

; These configuration values are given fixed addresses to aid auto-trackers.
; Any changes made here should be documented in Notes/auto-tracker-ctx.md
AUTO_TRACKER_CONTEXT:
AUTO_TRACKER_VERSION:
.word 2 ; Increment this if the auto-tracker context layout changes

CFG_DUNGEON_INFO_ENABLE:
.word 0
CFG_DUNGEON_INFO_MQ_ENABLE:
.word 0
CFG_DUNGEON_INFO_MQ_NEED_MAP:
.word 0
CFG_DUNGEON_INFO_REWARD_ENABLE:
.word 0
CFG_DUNGEON_INFO_REWARD_NEED_COMPASS:
.word 0
CFG_DUNGEON_INFO_REWARD_NEED_ALTAR:
.word 0
.area 14, 0xff
CFG_DUNGEON_REWARDS:
.endarea
.area 14, 0x00
CFG_DUNGEON_IS_MQ:
.endarea

RAINBOW_BRIDGE_CONDITION:
.word 0x04
; 0 = Open
; 1 = Medallions
; 2 = Dungeons
; 3 = Stones
; 4 = Vanilla
; 5 = Tokens
; 6 = Hearts

LACS_CONDITION:
.word 0x00
; 0 = Vanilla
; 1 = Medallions
; 2 = Dungeons
; 3 = Stones
; 4 = Tokens
; 5 = Hearts

RAINBOW_BRIDGE_COUNT:
.halfword 0x0064

LACS_CONDITION_COUNT:
.halfword 0x0000

TRIFORCE_HUNT_ENABLED:
.halfword 0x0000

TRIFORCE_PIECES_REQUIRED:
.halfword 0xffff

.area 8, 0x00
SPECIAL_DEAL_COUNTS:
.endarea

.align 4
