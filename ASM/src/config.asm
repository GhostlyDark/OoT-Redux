;==================================================================================================
; Settings and tables which the front-end may write
;==================================================================================================

CONFIGURATION_CONTEXT:

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
CFG_DEFAULT_SHOW_OVERLAY:
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
CFG_DEFAULT_UNEQUIP_GEAR:
.byte 0x01
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

CFG_DEFAULT_ADULT_DPAD_UP:
.byte 0x0D
CFG_DEFAULT_ADULT_DPAD_RIGHT:
.byte 0x27
CFG_DEFAULT_ADULT_DPAD_DOWN:
.byte 0x07
CFG_DEFAULT_ADULT_DPAD_LEFT:
.byte 0x26
CFG_DEFAULT_CHILD_DPAD_UP:
.byte 0x0D
CFG_DEFAULT_CHILD_DPAD_RIGHT:
.byte 0x17
CFG_DEFAULT_CHILD_DPAD_DOWN:
.byte 0x07
CFG_DEFAULT_CHILD_DPAD_LEFT:
.byte 0x01

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
CFG_CUSTOM_MAPS:
.byte 0x00
CFG_TYCOON_WALLET:
.byte 0x00
CFG_PREVENT_GROTTO_SAVING:
.byte 0x00
CFG_OPTIONS_MENU:
.byte 0x03
CFG_SILVER_SWORD:
.byte 0x00

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

BOMBCHUS_IN_LOGIC:
.word 0x00
FAST_CHESTS:
.byte 0x00
FAST_BUNNY_HOOD_ENABLED:
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
OVERWORLD_SHUFFLED:
.byte 0x00

.align 4