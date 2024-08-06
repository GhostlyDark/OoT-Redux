#include "gfx.h"
#include "dpad.h"
#include "dpad_paused.h"
#include "dpad_actions.h"
#include "options.h"
#include "buttons.h"
#include "fps.h"
#include "actor.h"

extern uint8_t CFG_TYCOON_WALLET;
extern uint8_t FAST_BUNNY_HOOD_ENABLED;
extern int8_t last_scene;

const static uint16_t pause_screen_hud_offsets[2] = { 230, 210 };

uint16_t dpad_x          = 0;
uint16_t dpad_y          = 0;
uint16_t original_damage = 0;
uint16_t play_sfx        = 0;
uint8_t options_alpha    = 0;
uint8_t compare_frames;

void handle_dpad() {
    if (z64_file.game_mode != 0)
        return;
    
    compare_frames = 60 / fps_limit;
    
    if (play_sfx != 0) {
        z64_playsfx(play_sfx, (z64_xyzf_t*)0x80104394, 0x04, (float*)0x801043A0, (float*)0x801043A0, (float*)0x801043A8);
        play_sfx = 0;
    }
    
    FAST_BUNNY_HOOD_ENABLED = OPTION_ACTIVE(1, SAVE_MASK_ABILITIES, CFG_DEFAULT_MASK_ABILITIES);
    restore_spawner_backup(&z64_game);
    //set_redead_freeze();
    if (FAST_BUNNY_HOOD_ENABLED && CAN_CONTROL_LINK) {
        z64_check_lava_floor    = z64_link.current_mask == PLAYER_MASK_GORON  ? 0x1000     : 0x0440;
        z64_check_lava_trail_1  = z64_link.current_mask == PLAYER_MASK_SPOOKY ? 0x0        : 0x2;
        z64_check_lava_trail_2  = z64_link.current_mask == PLAYER_MASK_SPOOKY ? 0xC        : 0x3;
      //z64_check_wall_damage   = z64_link.current_mask == PLAYER_MASK_SKULL  ? 0x24030001 : 0x00001825;
        z64_check_freeze        = z64_link.current_mask == PLAYER_MASK_KEATON ? 0x1000     : 0x1461;
        z64_check_shock         = z64_link.current_mask == PLAYER_MASK_GERUDO ? 0x1000     : 0x1461;
        remove_enemy_spawner();
        remove_falling_rocks_spawner();
        
        if (z64_link.current_mask == PLAYER_MASK_ZORA && (z64_link.state_flags_1 & PLAYER_STATE1_SWIMMING) && z64_move_speed > 0x4060)
            z64_move_speed += 0x50;
    }
    
    handle_dpad_ingame();
    handle_dpad_paused();
    handle_options_menu();
    handle_button_scaling();
    handle_layout();
    handle_hud();
    handle_fps();
    check_lens();
        
    if (OPTION_ACTIVE(2, SAVE_SWAP_ITEM, CFG_DEFAULT_SWAP_ITEM) && !z64_file.bgs_flag && z64_file.equip_sword == 3)
        z64_file.unk_06_[2] = z64_file.bgs_hits_left;
    
    if (OPTION_ACTIVE(1, SAVE_TRISWIPE, CFG_DEFAULT_TRISWIPE) && z64_triswipe == 255)
        z64_triswipe = 1;
    
    set_b_button();
    handle_abilities_tunic_colors();
    handle_inventory_editor();
    handle_l_button();
    handle_l_button_paused();
    handle_infinite();        
        
    if (OPTION_ACTIVE(3, SAVE_FOG > 0, CFG_DEFAULT_FOG > 0)) {
        uint8_t fog = CFG_OPTIONS_MENU >= 3 ? SAVE_FOG : CFG_DEFAULT_FOG;
        if (fog < 15)
            z64_game.fog_distance = 0.65f * fog;
        else z64_game.fog_distance = 15.0f;
        if (z64_camera_view == 1 && z64_game.fog_distance < 5.0f)
            z64_game.fog_distance = 5.0f;
        if (z64_camera_view == 2 && z64_game.fog_distance < 10.0f)
            z64_game.fog_distance = 10.0f;
    }
    
    if (CAN_CONTROL_LINK) {
        if (OPTION_ACTIVE(1, SAVE_INVERSE_AIM, CFG_DEFAULT_INVERSE_AIM))
            if (z64_MessageGetState(((uint8_t *)(&z64_game)) + 0x20D8) != 4 && (z64_game.camera_mode == CAM_MODE_FIRST_PERSON || z64_game.camera_mode == CAM_MODE_AIM_ADULT || z64_game.camera_mode == CAM_MODE_AIM_BOOMERANG || z64_game.camera_mode == CAM_MODE_AIM_CHILD) ) {
                z64_game.common.input[0].raw.y      = -z64_game.common.input[0].raw.y;
                z64_game.common.input[0].y_diff     = -z64_game.common.input[0].y_diff;
                z64_game.common.input[0].adjusted_y = -z64_game.common.input[0].adjusted_y;
            }
        
        if (OPTION_ACTIVE(1, SAVE_NO_IDLE_CAMERA, CFG_DEFAULT_NO_IDLE_CAMERA) && z64_idle_camera_counter < 10)
            z64_idle_camera_counter = 10;
        
        handle_rupee_drain();
        handle_power_crouch_stab_fix();
        handle_weaker_swords();
        handle_abilities();
        
        if (original_damage == 0)
            original_damage = z64_damage_taken_modifier_3;
        else if (OPTION_ACTIVE(2, SAVE_DAMAGE_TAKEN == 0, CFG_DEFAULT_DAMAGE_TAKEN == 0))
             z64_damage_taken_modifier_3 = original_damage;
        else z64_damage_taken_modifier_3 = 0x2C43 - 0x40 * (CFG_OPTIONS_MENU >= 2 ? SAVE_DAMAGE_TAKEN : CFG_DEFAULT_DAMAGE_TAKEN);
        
        if (OPTION_ACTIVE(3, SAVE_LEVITATION, CFG_DEFAULT_LEVITATION) && z64_game.common.input[0].raw.pad.l)
            z64_link_a_action = 0x40CB;
        
        if (CFG_TYCOON_WALLET && z64_file.gs_tokens >= 40 && z64_file.wallet == 2 && TYCOON_WALLET)
            z64_file.wallet = 3;
        
        if (!DPAD_INIT_SETUP)
            run_default_options_setup();
    }
}

void handle_restore_mask() {
    if (!OPTION_ACTIVE(1, SAVE_KEEP_MASK, CFG_DEFAULT_KEEP_MASK))
        return;
    
    if (last_scene == z64_game.scene_index) {
        last_scene            = -1;
        z64_link.current_mask = LAST_MASK;
    }
    
    if (CAN_CONTROL_LINK)
        LAST_MASK = LAST_MASK > 0 && z64_file.items[Z64_SLOT_CHILD_TRADE] != LAST_MASK + Z64_ITEM_ZELDAS_LETTER ? 0 : z64_link.current_mask;
}

void run_default_options_setup() {
    z64_file.inf_table[0x15] |= 1 << 0; // Init
    
    z64_file.inf_table[0x13] |= CFG_DEFAULT_MASK_ABILITIES       << 0;
    
    z64_file.inf_table[0x15] |= CFG_DEFAULT_30_FPS               << 4;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_ARROW_TOGGLE         << 5;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_SHOW_HEALTH          << 6;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_CHEST_CONTENTS       << 7;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_INVERSE_AIM          << 8;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_NO_IDLE_CAMERA       << 9;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_KEEP_MASK            << 10;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_TRISWIPE             << 11;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_UNEQUIP_ITEM         << 12;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_UNEQUIP_GEAR         << 13;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_ITEM_ON_B            << 14;
    z64_file.inf_table[0x15] |= CFG_DEFAULT_SWAP_ITEM            << 15;
    
    z64_file.inf_table[0x14] |= CFG_DEFAULT_CROUCH_STAB_FIX      << 8;
    z64_file.inf_table[0x14] |= CFG_DEFAULT_WEAKER_SWORDS        << 9;
    z64_file.inf_table[0x14] |= CFG_DEFAULT_EXTRA_ABILITIES      << 10;
    z64_file.inf_table[0x14] |= CFG_DEFAULT_LEVITATION           << 11;
    z64_file.inf_table[0x14] |= CFG_DEFAULT_INFINITE_HP          << 12;
    z64_file.inf_table[0x14] |= CFG_DEFAULT_INFINITE_MP          << 13;
    z64_file.inf_table[0x14] |= CFG_DEFAULT_INFINITE_RUPEES      << 14;
    z64_file.inf_table[0x14] |= CFG_DEFAULT_INFINITE_AMMO        << 15;
    
    z64_file.inf_table[0x18] |= CFG_DEFAULT_RUPEE_DRAIN          << 0;
    z64_file.inf_table[0x18] |= CFG_DEFAULT_FOG                  << 4;
    
    z64_file.inf_table[0x1B] |= CFG_DEFAULT_DPAD                 << 0;
    z64_file.inf_table[0x1B] |= CFG_DEFAULT_SHOW_DPAD            << 2;
    z64_file.inf_table[0x1B] |= CFG_DEFAULT_HIDE_HUD             << 4;
    z64_file.inf_table[0x1B] |= CFG_DEFAULT_HUD_LAYOUT           << 7;
    z64_file.inf_table[0x1B] |= CFG_DEFAULT_A_BUTTON_SCALE       << 10;
    z64_file.inf_table[0x1B] |= CFG_DEFAULT_B_BUTTON_SCALE       << 12;
    z64_file.inf_table[0x1B] |= CFG_DEFAULT_RANDOM_ENEMIES       << 15;
    
    z64_file.inf_table[0x1C] |= CFG_DEFAULT_C_LEFT_BUTTON_SCALE  << 0;
    z64_file.inf_table[0x1C] |= CFG_DEFAULT_C_DOWN_BUTTON_SCALE  << 4;
    z64_file.inf_table[0x1C] |= CFG_DEFAULT_C_RIGHT_BUTTON_SCALE << 8;
    z64_file.inf_table[0x1C] |= CFG_DEFAULT_DAMAGE_TAKEN         << 12;
    
    DPAD_ADULT_SET1_RIGHT = Z64_SLOT_HOVER_BOOTS;
    DPAD_ADULT_SET1_LEFT  = Z64_SLOT_IRON_BOOTS;
    DPAD_CHILD_SET1_RIGHT = Z64_SLOT_CHILD_TRADE;
    DPAD_CHILD_SET1_LEFT  = Z64_SLOT_NUT;
    DPAD_ADULT_SET1_UP    = DPAD_CHILD_SET1_UP    = Z64_SLOT_LENS;
    DPAD_ADULT_SET1_DOWN  = DPAD_CHILD_SET1_DOWN  = Z64_SLOT_OCARINA;
    
    DPAD_ADULT_SET2_UP    = DPAD_CHILD_SET2_UP    = Z64_SLOT_KOKIRI_SWORD;
    DPAD_ADULT_SET2_RIGHT = DPAD_CHILD_SET2_RIGHT = Z64_SLOT_KOKIRI_BOOTS;
    DPAD_ADULT_SET2_DOWN  = DPAD_CHILD_SET2_DOWN  = Z64_SLOT_DEKU_SHIELD;
    DPAD_ADULT_SET2_LEFT  = DPAD_CHILD_SET2_LEFT  = Z64_SLOT_KOKIRI_TUNIC;
}

void handle_dpad_ingame() {
    if (!CAN_USE_DPAD || z64_camera_view != 0 || OPTION_VALUE(1, 0, SAVE_DPAD, CFG_DEFAULT_DPAD))
        return;
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
    
    if (OPTION_VALUE(1, 2, SAVE_DPAD, CFG_DEFAULT_DPAD)) {
        if ( (z64_game.common.input[0].raw.pad.l && pad_pressed.r) || (z64_game.common.input[0].raw.pad.r && pad_pressed.l) ) {
            TOGGLE_DPAD_ALT;
            play_sfx = GET_DPAD_ALT ? 0x4813 : 0x4814;
        }
    }
    
    if (z64_game.pause_ctxt.state == 0)
        run_dpad_actions(pad_pressed);
}

void draw_dpad() {
    z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
    gSPDisplayList(db->p++, &setup_db);
    gDPPipeSync(db->p++);
    gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    
    if (z64_game.pause_ctxt.state != 0) {
        if (CFG_OPTIONS_MENU > 0 && z64_game.pause_ctxt.state == 6) {
            sprite_load_and_draw(db, &stones_sprite, 0, -10, -10, 4, 4); // Fake Texture
            draw_settings_label(db);
        }
        
        if (CAN_USE_MASK_SWAP)
            draw_swap_item_icons(db, z64_file.items[Z64_SLOT_CHILD_TRADE], get_next_mask());
        else if (CAN_USE_OCARINA_SWAP)
            draw_swap_item_icons(db, z64_file.items[Z64_SLOT_OCARINA], z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA ? Z64_ITEM_OCARINA_OF_TIME : Z64_ITEM_FAIRY_OCARINA);
        else if (CAN_USE_HOOKSHOT_SWAP)
            draw_swap_item_icons(db, z64_file.items[Z64_SLOT_HOOKSHOT], z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_HOOKSHOT ? Z64_ITEM_LONGSHOT : Z64_ITEM_HOOKSHOT);
        else if (CAN_USE_GIANTS_KNIFE_SWAP)
            draw_swap_item_icons(db, z64_file.bgs_flag ? Z64_ITEM_BIGGORON_SWORD : Z64_ITEM_BROKEN_GORONS_SWORD , z64_file.bgs_flag ? Z64_ITEM_BROKEN_GORONS_SWORD : Z64_ITEM_BIGGORON_SWORD);
        
        if (z64_game.pause_ctxt.state != 6)
            options_alpha = 0;
        else options_alpha = options_alpha < 235 ? options_alpha + 20 : 255;
    }
    
    draw_health(db);
    if (!draw_settings_menu(db))
        if (!draw_abilities_info(db))
            draw_dpad_icons(db);
}

void draw_name_panel(z64_disp_buf_t *db) {
    gDPSetPrimColor(db->p++, 0, 0, 90, 100, 130, options_alpha);
    uint16_t x = CFG_WS ? pause_screen_hud_offsets[0] + 104 : pause_screen_hud_offsets[0];
    sprite_load_and_draw(db, &name_panel_sprite, 0, x,      pause_screen_hud_offsets[1], 40, 20);
    sprite_load_and_draw(db, &name_panel_sprite, 1, x + 40, pause_screen_hud_offsets[1], 40, 20);
}

void draw_settings_label(z64_disp_buf_t *db) {
    draw_name_panel(db);
    uint16_t x = CFG_WS ? pause_screen_hud_offsets[0] + 104 : pause_screen_hud_offsets[0];
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, options_alpha);
    sprite_load_and_draw(db, &l_button_pause_screen_sprite, 0, x + 58, pause_screen_hud_offsets[1] + 3, 14, 18);
    sprite_load_and_draw(db, &title_text_sprite,            8, x + 10, pause_screen_hud_offsets[1] + 4, 96, 12);
}

void draw_swap_item_icons(z64_disp_buf_t *db, z64_item_t curr_item, z64_item_t next_item) {
    if (curr_item == next_item) 
        return;
    
    uint16_t x = CFG_WS ? pause_screen_hud_offsets[0] + 104 : pause_screen_hud_offsets[0];
    draw_name_panel(db);
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, options_alpha);
    sprite_load_and_draw(db, &items_sprite,  curr_item, x + 16, pause_screen_hud_offsets[1] + 3, 14, 14);
    sprite_load_and_draw(db, &button_sprite, 4,         x + 28, pause_screen_hud_offsets[1] - 2, 24, 24);
    sprite_load_and_draw(db, &items_sprite,  next_item, x + 52, pause_screen_hud_offsets[1] + 3, 14, 14);
}

void draw_ammo_on_b_button() {
    if (OPTION_ACTIVE(2, SAVE_ITEM_ON_B, CFG_DEFAULT_ITEM_ON_B) && CAN_CONTROL_LINK && z64_game.pause_ctxt.state <= 5) {
        z64_disp_buf_t *db = &(z64_ctxt.gfx->overlay);
        gSPDisplayList(db->p++, &setup_db);
        gDPPipeSync(db->p++);
        draw_ammo(db, z64_file.button_items[0], z64_gameinfo.item_ammo_x[0], z64_gameinfo.item_ammo_y[0], 8, -2, z64_game.hud_alpha_channels.b_button);
    }
}

void draw_dpad_icons(z64_disp_buf_t *db) {
    if (!CAN_DRAW_HUD || OPTION_VALUE(1, 0, SAVE_DPAD, CFG_DEFAULT_DPAD) || (OPTION_VALUE(1, 0, SAVE_SHOW_DPAD, CFG_DEFAULT_SHOW_DPAD) && z64_game.pause_ctxt.state != 6) || (z64_game.pause_ctxt.state > 0x7 && z64_game.pause_ctxt.state < 0x12) )
        return;
    
    uint8_t *dpad_active = check_dpad_actions();
    if (z64_game.pause_ctxt.state == 0 && !dpad_active[0] && !dpad_active[1] && !dpad_active[2] && !dpad_active[ 3])
        return;
    
    if (OPTION_VALUE(1, 2, SAVE_SHOW_DPAD, CFG_DEFAULT_SHOW_DPAD)) {
        dpad_x = 271;
        dpad_y = 64;
        if (OPTION_VALUE(1, 2, SAVE_HUD_LAYOUT, CFG_DEFAULT_HUD_LAYOUT) || OPTION_VALUE(1, 3, SAVE_HUD_LAYOUT, CFG_DEFAULT_HUD_LAYOUT)) {
            dpad_x += 10;
            dpad_y += 14;
        }
        else if (OPTION_VALUE(1, 4, SAVE_HUD_LAYOUT, CFG_DEFAULT_HUD_LAYOUT) || OPTION_VALUE(1, 5, SAVE_HUD_LAYOUT, CFG_DEFAULT_HUD_LAYOUT))
            dpad_y += 15;
        if (CFG_WS)
            dpad_x += 104;
    }
    else if (OPTION_VALUE(1, 3, SAVE_SHOW_DPAD, CFG_DEFAULT_SHOW_DPAD)) {
        dpad_x = 35;
        dpad_y = 175;
        if (z64_dungeon_scene != 0xFF)
            if (z64_file.dungeon_keys[z64_dungeon_scene] > 0)
                dpad_y = 158;
    }
    else {
        dpad_x = 21;
        dpad_y = 45;
        if (z64_file.magic_acquired && z64_file.energy_capacity > 0xA0)
            dpad_y += 23;
        else if (z64_file.energy_capacity > 0xA0)
            dpad_y += 10;
        else if (z64_file.magic_acquired)
            dpad_y += 14;
        if (z64_game.pause_ctxt.state == 0 && (z64_file.timer_1_state != 0 || z64_file.timer_2_state != 0) ) {
            dpad_y += 18;
            if (!z64_file.magic_acquired)
                dpad_y += 14;
        }
    }
    
    uint8_t alpha = 255;
    if (alpha > z64_game.hud_alpha_channels.hearts_navi)
        alpha = z64_game.hud_alpha_channels.hearts_navi;
    
    uint8_t rgb = !GET_DPAD_ALT ? 0x90 : 0xCC;
    gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(db->p++, 0, 0, rgb, rgb, rgb, alpha);
    sprite_load_and_draw(db, &dpad_sprite, 0, dpad_x, dpad_y, 16, 16);
    draw_dpad_actions(db, alpha);
}

void check_lens() {
    if (!CAN_CONTROL_LINK || !CAN_USE_DPAD || OPTION_VALUE(1, 0, SAVE_DPAD, CFG_DEFAULT_DPAD) || has_lens_on_button() || !z64_game.actor_ctxt.lens_active) {
        z64_dpad_lens_1 = 0x504E;
        z64_dpad_lens_2 = 0x504F;
        z64_dpad_lens_3 = 0x5458;
    }
    else z64_dpad_lens_1 = z64_dpad_lens_2 = z64_dpad_lens_3 = 0x1000;
}

uint8_t has_lens_on_button() {
    for (uint8_t i=0; i<4; i++)
        if (z64_file.button_items[i] == Z64_ITEM_LENS)
            return 1;
    return 0;
}