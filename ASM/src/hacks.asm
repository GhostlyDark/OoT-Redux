;==================================================================================================
; main.c hooks
;==================================================================================================
.headersize (0x800110A0 - 0xA87000)

; Runs before the game state update function
; Replaces:
;   lw      t6, 0x0018 (sp)
;   lui     at, 0x8010
.org 0x8009CAD4
    jal     before_game_state_update_hook
    nop

; Runs after the game state update function
; Replaces:
;   jr      ra
;   nop
.org 0x8009CB00
    j       after_game_state_update
    nop

; 
.org 0x8009CED0
    jal     before_skybox_init

.org 0x8009CDA0
Gameplay_InitSkybox:

; Runs after scene init
; Replaces:
;   jr      ra
;   nop
.org 0x8009CEE4
    j       after_scene_init
    nop

;==================================================================================================
; Never override Heart Colors
;==================================================================================================

; Replaces:
;   SH A2, 0x020E (V0)
;   SH T9, 0x0212 (V0)
;   SH A0, 0x0216 (V0)
.orga 0xADA8A8
    nop
    nop
    nop

; Replaces:
;   SH T5, 0x0202 (V0)
.orga 0xADA97C
    nop

.orga 0xADA9A8
    nop

.orga 0xADA9BC
    nop


.orga 0xADAA64
    nop

.orga 0xADAA74
    nop
    nop


.orga 0xADABA8
    nop

.orga 0xADABCC
    nop

.orga 0xADABE4
    nop

;==================================================================================================
; Pause menu
;==================================================================================================

; Don't display hover boots in the bullet bag/quiver slot if you haven't gotten a slingshot before becoming adult
; Replaces:
;   lbu     t4, 0x0000 (t7)
;   and     t6, v1, t5
.orga 0xBB6CF0
    jal     equipment_menu_fix
    nop

;==================================================================================================
; DPAD Display
;==================================================================================================
;
; Replaces lw    t6, 0x1C44(s6)
;          lui   t8, 0xDB06
.orga 0xAEB67C ; In Memory: 0x8007571C
    jal     dpad_draw
    nop

;==================================================================================================
; Cast Fishing Rod without B Item
;==================================================================================================

.orga 0xBCF914 ; 8038A904
    jal     keep_fishing_rod_equipped
    nop

.orga 0xBCF73C ; 8038A72C
    sw      ra, 0x0000(sp)
    jal     cast_fishing_rod_if_equipped
    nop
    lw      ra, 0x0000(sp)
    
;==================================================================================================
; Make Bunny Hood like Majora's Mask
;==================================================================================================

; Replaces: mfc1    a1, f12
;           mtc1    t7, f4
.orga 0xBD9A04
    jal bunny_hood
    nop

;==================================================================================================
; Magic Meter Colors
;==================================================================================================
; Replaces: sh  r0, 0x0794 (t6)
;           lw  t7, 0x0000 (v0)
;           sh  r0, 0x0796 (t7)
;           lw  t7, 0x0000 (v0)
;           sh  r0, 0x0798 (t8)
.orga 0xB58320
    sw      ra, 0x0000 (sp)
    jal     magic_colors
    nop
    lw      ra, 0x0000 (sp)
    nop

; ==================================================================================================
; HUD Button Colors
; ==================================================================================================
; Fix HUD Start Button to allow a value other than 00 for the blue intensity
; Replaces: andi    t6, t7, 0x00FF
.orga 0xAE9ED8
    ori     t6, t7, 0x0000 ; add blue intensity to the start button color (Value Mutated in Cosmetics.py)

; Handle Dynamic Shop Cursor Colors
.orga 0xC6FF30
.area 0x4C, 0
    mul.s   f16, f10, f0
    mfc1    a1, f8          ; color delta 1 (for extreme colors)
    trunc.w.s f18, f16
    mfc1    a2, f18         ; color delta 2 (for general colors)
    swc1    f0, 0x023C(a0)  ; displaced code

    addiu   sp, sp, -0x18
    sw      ra, 0x04(sp)
    jal     shop_cursor_colors
    nop
    lw      ra, 0x04(sp)
    addiu   sp, sp, 0x18

    jr      ra
    nop
.endarea

;==================================================================================================
; Use Sticks and Masks as Adult
;==================================================================================================
; Deku Stick
; Replaces: addiu   t8, v1, 0x0008
;           sw      t8, 0x02C0(t7)
.orga 0xAF1814
    jal     stick_as_adult
    nop

; Masks
; Replaces: sw      t6, 0x0004(v0)
;           lb      t7, 0x013F(s0)
.orga 0xBE5D8C
    jal     masks_as_adult
    nop

;==================================================================================================
; Bombchu Ticking Color
;==================================================================================================
; Replaces: ctc1    t9, $31
;           ori     t5, t4, 0x00FF
.orga 0xD5FF94
    jal     bombchu_back_color
    ctc1    t9, $31

;==================================================================================================
; Null Boomerang Pointer in Links Instance
;==================================================================================================
;Clear the boomerang pointer in Links instance when the boomerangs destroy function runs.
;This fixes an issue where the boomerang trail color hack checks this pointer to write data.
; Replaces: sw      a0, 0x18(sp)
.orga 0xC5A9F0
    jal     clear_boomerang_pointer

;==================================================================================================
; Prevent Mask de-equip if not on a C-button
;==================================================================================================
.orga 0xBCF8CC
    jal     mask_check_trade_slot   ; sb      zero, 0x014F(t0)
