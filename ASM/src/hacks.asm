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
;.org 0x8009CB00
;    j       after_game_state_update
;    nop

; 
;.org 0x8009CED0
;    jal     before_skybox_init

;.org 0x8009CDA0
;Gameplay_InitSkybox:

; Runs after scene init
; Replaces:
;   jr      ra
;   nop
;.org 0x8009CEE4
;    j       after_scene_init
;    nop
