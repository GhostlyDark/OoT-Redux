;==================================================================================================
; main.c hooks
;==================================================================================================
.headersize (0x800110A0 - 0xA87000)

; Runs before the game state update function
;   lw      t6, 0x0018 (sp)
;   lui     at, 0x8010
.org 0x8009CAD4
    jal     before_game_state_update_hook
    nop

; Runs after the game state update function
;   jr      ra
;   nop
.org 0x8009CB00
    j       after_game_state_update
    nop

; Runs before skybox init
.org 0x8009CED0
    jal     before_skybox_init
.org 0x8009CDA0
Gameplay_InitSkybox:

; Runs after scene init
;   jr      ra
;   nop
.org 0x8009CEE4
    j       after_scene_init
    nop

.headersize(0)

;==================================================================================================
; File select hash
;==================================================================================================
; Runs after the file select menu is rendered (replaces code that draws the fade-out rectangle on file load)
.orga 0xBAF738 ; In memory: 0x803B3538
.area 0x60, 0
    or      a1, r0, s0   ; menu data
    jal     draw_file_select_hash
    andi    a0, t8, 0xFF ; a0 = alpha channel of fade-out rectangle
    lw      s0, 0x18 (sp)
    lw      ra, 0x1C (sp)
    jr      ra
    addiu   sp, sp, 0x88
.endarea

;==================================================================================================
; Hide file details panel
;==================================================================================================
; Keep death count alpha at 0 instead of using file_detail alpha
.orga 0xBAC064 ; RAM: 0x803AFE64
    move    t7, r0

; Keep hearts alpha at 0 instead of using file_detail alpha
.orga 0xBAC1BC ; RAM: 0x803AFFBC
    move    t7, r0

; Keep stones/medals alpha at 0 instead of using file_detail alpha
.orga 0xBAC3EC ; RAM: 0x803B01EC
    move    t9, r0

; Keep detail panel alpha at 0 instead of using file_detail alpha
.orga 0xBAC94C ; RAM: 0x803B074C
    move    t9, r0

; Keep file tag alpha at 0xC8 instead of subtracting 0x19 each transition frame
.orga 0xBAE5A4 ; RAM: 0x803B23A4
    sh      t3, 0x4A6C (v1)

; Prevent setting file tag alpha to 0x00 when transition is finished
.orga 0xBAE5C8 ; RAM: 0x803B23C8
    nop ; was: sh r0, 0x4A6C (v1)

; Prevent increasing alpha when transitioning away from file
.orga 0xBAE864 ; RAM: 0x803B2664
    nop

; Change file positions in copy menu (ROM: 0xBB05FC, RAM: 0x803B43FC)
;    .word 0x0000FFC0
;    .word 0xFFB0FFB0

; Keep file tag alpha at 0xC8 in copy menu
.orga 0xBA18C4 ; RAM: 0x803A56C4
    ori     t4, r0, 0x00C8
.orga 0xBA1980 ; RAM: 0x803A5780
    ori     t0, r0, 0x00C8
.orga 0xBA19DC ; RAM: 0x803A57DC
    nop ; was: sh r0, 0x4A6C (t2)
.orga 0xBA1E20 ; RAM: 0x803A5C20
    ori     t5, r0, 0x00C8
.orga 0xBA18C4 ; RAM: 0x803A56C4
    ori     t4, r0, 0x00C8

; Keep file tag alpha at 0xC8 in erase menu
.orga 0xBA34DC ; RAM: 0x803A72DC
    ori     t8, r0, 0x00C8
.orga 0xBA3654
    nop
.orga 0xBA39D0
    ori     t5, r0, 0x00C8

;==================================================================================================
; Empty Bomb Fix
;==================================================================================================
.orga 0xC0E77C
    jal     empty_bomb
    sw      r0, 0x0428(v0)

;==================================================================================================
; Talon Cutscene Skip
;==================================================================================================
.orga 0xCC0038
    jal    talon_break_free
    lw     a0, 0x0018(sp)

;==================================================================================================
; Pause menu
;==================================================================================================
; Don't display hover boots in the bullet bag/quiver slot if you haven't gotten a slingshot before becoming adult
.orga 0xBB6CF0
    jal     equipment_menu_fix
    nop

;==================================================================================================
; Easier Fishing
;==================================================================================================
; Make fishing less obnoxious
.orga 0xDBF428
    jal     easier_fishing
    lui     at, 0x4282
    mtc1    at, f8
    mtc1    t8, f18
    swc1    f18, 0x019C(s2)
.orga 0xDBF484
    nop
.orga 0xDBF4A8
    nop

; set adult fish size requirement
;.orga 0xDCBEA8
;    lui     at, 0x4248
;.orga 0xDCBF24
;    lui     at, 0x4248

; set child fish size requirements
;.orga 0xDCBF30
;    lui     at, 0x4230
;.orga 0xDCBF9C
;    lui     at, 0x4230

; Fish bite guaranteed when the hook is stable
.orga 0xDC7090
    jal     fishing_bite_when_stable
    lwc1    f10, 0x0198(s0)

; Remove most fish loss branches
;.orga 0xDC87A0
;    nop
;.orga 0xDC87BC
;    nop
;.orga 0xDC87CC
;    nop

; Prevent RNG fish loss
;.orga 0xDC8828
;    move    at, t5

;==================================================================================================
; DPAD Display
;==================================================================================================
;.orga 0xAEB67C ; In Memory: 0x8007571C
;    jal     dpad_draw
;    nop

;==================================================================================================
; Stone of Agony indicator
;==================================================================================================
.orga 0xBE4A14
    jal     agony_distance_hook
.orga 0xBE4A40
    jal     agony_vibrate_hook
.orga 0xBE4A60
    j       agony_post_hook
    nop

;==================================================================================================
; Cast Fishing Rod without B Item
;==================================================================================================
.orga 0xBCF914 ; RAM: 8038A904
    jal     keep_fishing_rod_equipped
    nop
.orga 0xBCF73C ; RAM: 8038A72C
    sw      ra, 0x0000(sp)
    jal     cast_fishing_rod_if_equipped
    nop
    lw      ra, 0x0000(sp)

;==================================================================================================
; Big Goron Fix
;==================================================================================================
.orga 0xED645C
    jal     bgs_fix
    nop

;==================================================================================================
; Warp song speedup
;==================================================================================================
; Manually set next entrance and fade out type
.orga 0xBEA044 
   jal      warp_speedup
   nop

; Set fade in type after the warp
.orga 0xB10CC0
    jal     set_fade_in
    lui     at, 0x0001

;==================================================================================================
; Dampe Digging Fix
;==================================================================================================
; Leaving without collecting dampe's prize won't lock you out from that prize
.orga 0xCC4038
    jal     dampe_fix
    addiu   t4, r0, 0x0004
.orga 0xCC453C
    .word 0x00000806
    
;==================================================================================================
; Make Bunny Hood like Majora's Mask
;==================================================================================================
.orga 0xBD9A04
    jal bunny_hood
    nop

;==================================================================================================
; Prevent hyrule guards from causing a softlock if they're culled 
;==================================================================================================
.orga 0xE24E7C
    jal guard_catch
    nop

;==================================================================================================
; Never override Heart Colors
;==================================================================================================
.orga 0xADA8A8
    nop
    nop
    nop
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
; Magic Meter Colors
;==================================================================================================
.orga 0xB58320
    sw      ra, 0x0000 (sp)
    jal     magic_colors
    nop
    lw      ra, 0x0000 (sp)
    nop

;==================================================================================================
; HUD Rupee Icon color
;==================================================================================================
.orga 0xAEB764
    addiu   t8, s1, 0x0008
    sw      t8, 0x02B0(s4)
    jal     rupee_hud_color
    sw      t9, 0x0000(s1)
    lhu     t4, 0x0252(s7)
    move    at, v0

; ==================================================================================================
; HUD Button Colors
; ==================================================================================================
; Fix HUD Start Button to allow a value other than 00 for the blue intensity
.orga 0xAE9ED8
    ori     t6, t7, 0x0000 ; Add blue intensity to the start button color (Value Mutated in Cosmetics.py)

; Handle Dynamic Shop Cursor Colors
.orga 0xC6FF30
.area 0x4C, 0
    mul.s   f16, f10, f0
    mfc1    a1, f8          ; Color delta 1 (for extreme colors)
    trunc.w.s f18, f16
    mfc1    a2, f18         ; Color delta 2 (for general colors)
    swc1    f0, 0x023C(a0)  ; Displaced code
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
; Speed Up Gold Gauntlets Rock Throw
;==================================================================================================
; Replace onepointdemo calls for the different cases so the cutscene never plays (for cases 0 and 4 set position so that the rock lands in the right place)

; Case 1: Light Trial (breaks on impact) (0x8006B6FC)
.orga 0xCDF3EC
    nop

; Case 0: Fire Trial (0x8006B6FC)
.orga 0xCDF404
    nop

; Case 4: Outside Ganon's Castle (0x8006B6FC)
.orga 0xCDF420 
    jal     heavy_block_set_switch

; Set links position and angle to the center of the block as its being lifted
.orga 0xBD5C58
    jal      heavy_block_posrot
    or       t9, t8, at

; Set links action to 7 so he can move again
.orga 0xCDF638
    jal     heavy_block_set_link_action
    swc1    f4, 0x34(sp)

; Reduce quake timer for case 1
.orga 0xCDF790
    addiu      a1, r0, 0x1E

; Skip parts of links lifting animation
.orga 0xBE1BC8
    jal    heavy_block_shorten_anim
    sw     a1, 0x34(sp)

; Slightly change rock throw trajectory to land in the right place
.orga 0xBE1C98
    lui    at, 0x4218

;==================================================================================================
; Use Sticks and Masks as Adult
;==================================================================================================
; Deku Stick
.orga 0xAF1814
    jal     stick_as_adult
    nop

; Masks
.orga 0xBE5D8C
    jal     masks_as_adult
    nop

;==================================================================================================
; Bombchu Ticking Color
;==================================================================================================
.orga 0xD5FF94
    jal     bombchu_back_color
    ctc1    t9, $31

;==================================================================================================
; Null Boomerang Pointer in Links Instance
;==================================================================================================
; Clear the boomerang pointer in Links instance when the boomerangs destroy function runs (this fixes an issue where the boomerang trail color hack checks this pointer to write data)
.orga 0xC5A9F0
    jal     clear_boomerang_pointer

;===================================================================================================
;Kill Door of Time collision when the cutscene starts
;===================================================================================================
.orga 0xCCE9A4
    jal     kill_door_of_time_col ; Replaces lui     $at, 0x3F80 
    lw      a0, 0x011C(s0) ; replaces mtc1    $at, $f6 

;==================================================================================================
; Prevent Mask de-equip if not on a C-button
;==================================================================================================
.orga 0xBCF8CC
    jal     mask_check_trade_slot   ; sb      zero, 0x014F(t0)

;===================================================================================================
; Allow ice arrows to melt red ice
;===================================================================================================
.orga 0xDB32C8
    jal blue_fire_arrows ; replaces addiu at, zero, 0x00F0
	
;==================================================================================================
; Speed up armos push
;==================================================================================================
; In EnAm_Statue, subtract 0x1000 from this->unk_258 instead of 0x800, this value is used as the timer for how long to push for (this halves the number of frames that the push will occur over)
.orga 0xC97C68
    addiu t0, a2, 0xF000

; In EnAm_Statue, when calculating the speed, multiply by 1.00973892212 instead of 0.5 
.orga 0xC97D68
    jal     en_am_calculation_1
    nop
.orga 0xC97E20
    jal     en_am_calculation_2
    nop

;==================================================================================================
; Draw Chest Base and Lid
;==================================================================================================
.org 0xC0754C
    j   draw_chest
    nop

; Set chest_base front texture
.org 0xFEB000 + 0x6F0 - 0x3296C0 + 0x3296D8
.word   0xDE000000, 0x09000000
.org 0xFEB000 + 0x6F0 - 0x3296C0 + 0x3297B8
.word   0xDE000000, 0x09000000

; Set chest_base base texture
.org 0xFEB000 + 0x6F0 - 0x3296C0 + 0x329758
.word   0xDE000000, 0x09000010
.org 0xFEB000 + 0x6F0 - 0x3296C0 + 0x329810
.word   0xDE000000, 0x09000010

; Set chest_lid front texture
.org 0xFEB000 + 0x10C0 - 0x32A090 + 0x32A0A8
.word   0xDE000000, 0x09000000
.org 0xFEB000 + 0x10C0 - 0x32A090 + 0x32A1C8
.word   0xDE000000, 0x09000000

; Set chest_lid base texture
.org 0xFEB000 + 0x10C0 - 0x32A090 + 0x32A158
.word   0xDE000000, 0x09000010

;==================================================================================================
; Randomize Enemy HP
;==================================================================================================
; Hook at the end of Actor_SetWorldToHome to zeroize anything we use to store additional flag data
;.orga 0xA96E5C ; RAM: 0x80020EFC
;    j       Actor_SetWorldToHome_Hook

; Hook Actor_UpdateAll when each actor is being initialized. At the call to Actor_SpawnEntry (used to set the flag (z-rotation) of the actor to its position in the actor table)
;.orga 0xA99D48 ; RAM: 0x80023DE8
;    jal     Actor_UpdateAll_Hook

; Hack Actor_SpawnEntry so we can override actors being spawned
.orga 0xA9B524 ; RAM: 0x800255C4
    j       Actor_SpawnEntry_Hack
    nop

;===================================================================================================
; Cancel Volvagia flying form hitbox when her health is already at O
;===================================================================================================
.orga 0xCEA41C
    jal     volvagia_flying_hitbox
    nop

;===================================================================================================
; Connect Interface functions to Redux
;===================================================================================================
.orga 0xBCF814 ; RAM: 0x8038A804
    j       player_process_item_buttons
    nop
.orga 0xAE7ADC ; RAM: 0x80071B7C
    j       player_update_bottle_item
    nop
.orga 0xAE3888
    j       interface_raise_button_alphas
    nop
.orga 0xAE39AC
    j       interface_dim_button_alphas
    nop
.orga 0xAE3A80
    j      interface_update_hud_alphas
    nop
.orga 0xAE3848
    j      interface_change_hud_visibility_mode
    nop
 ;.orga 0xAE42A0
 ;   j      interface_enable_buttons
 ;   nop