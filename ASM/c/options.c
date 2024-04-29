#include "gfx.h"
#include "text.h"
#include "options.h"
#include "buttons.h"
#include "dpad_paused.h"
#include "fps.h"

extern uint16_t play_sfx;
extern uint8_t checked_lens;
extern uint8_t block_r;
extern uint8_t block_z;
extern uint8_t pressed_r;
extern uint8_t pressed_z;

uint8_t moved_axis_option = 0;
uint8_t options_frames    = 0;
uint8_t holding_stick     = 0;

char medallion_item[9][17]                 = { "Light Medallion", "Forest Medallion", "Fire Medallion", "Water Medallion", "Shadow Medallion", "Spirit Medallion", "Kokiri's Emerald", "Goron's Ruby", "Zora's Sapphire" };
char medallion_ability[9][16]              = { "Long Jump",       "Magician Tunic",   "Guardian Tunic", "Hero Tunic",      "Shadow Tunic",     "Hover Dash Jump",  "Dash",             "Faster Dash",  "Cheaper Dash"    };
char options[OPTIONS_SIZE_ALL][17]         = { "30 FPS", "Arrow Toggle", "D-Pad Config", "D-Pad Layout", "Hide HUD", "HUD Layout", "Show Health", "Chest Contents", "A Button Scale", "B Button Scale", "C-Left Scale", "C-Down Scale", "C-Right Scale", "Inverse Aim", "No Idle Camera", "Keep Mask", "Tri-Swipe", "Swap Item", "Unequip Item", "Unequip Gear", "Item on B", "Mask Abilities", "Extra Abilities", "Damage Taken", "Random Enemies", "Crouch Stab Fix", "Weaker Swords", "Rupee Drain", "Fog", "Inventory Editor", "Levitation", "Infinite Health", "Infinite Magic", "Infinite Rupees", "Infinite Ammo" };
uint8_t options_max[OPTIONS_SIZE_ALL]      = { 0,        0,              2,              3,              4,          6,            0,             0,                2,                7,                7,              7,              7,               0,             0,                0,           0,           0,           0,              0,              0,           0,                0,                 7,              0,                0,                 0,               15,            15,    0,                  0,             0,                0,                0,                 0               };
int8_t  options_recenter[OPTIONS_SIZE_ALL] = { 40,       15,             15,             15,             30,         22,           17,            8,                6,                6,                11,             11,             10,              17,            5,                25,          25,          25,          15,             15,             27,          5,                0,                 15,             8,                0,                 10,              20,            50,    -5,                 20,            0,                5,                0,                 10              };
uint8_t options_cursor                     = 0;

void handle_options_menu() {
    if (z64_game.pause_ctxt.state != 6 || z64_game.pause_ctxt.unk_02_[1] != 3)
        return;
    
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
    
    uint8_t size;
    if (CFG_OPTIONS_MENU == 1)
        size = OPTIONS_SIZE_CORE;
    else if (CFG_OPTIONS_MENU == 2)
        size = OPTIONS_SIZE_MAIN;
    else size = OPTIONS_SIZE_ALL;
    
    if (moved_axis_option)
        if (z64_x_axis_input > -10 && z64_x_axis_input < 10)
            moved_axis_option = 0;
    
    if (pad_pressed.dl || (z64_x_axis_input < -50 && !moved_axis_option) ) {
        if (options_cursor == 0)
            options_cursor = size - 1;
        else options_cursor--;
        play_sfx = 0x4839;
        
        if (z64_x_axis_input < -50)
            moved_axis_option = 1;
    }
    else if (pad_pressed.dr || (z64_x_axis_input > 50 && !moved_axis_option) ) {
        if (options_cursor == size - 1)
            options_cursor = 0;
        else options_cursor++;
        play_sfx = 0x4839;
        
        if (z64_x_axis_input > 50)
            moved_axis_option = 1;
    }
    else if (pad_pressed.a  || pad_pressed.b) {
        play_sfx = 0x483B;
        handle_options_menu_input(pad_pressed);
    }
    
    z64_x_axis_input = z64_y_axis_input = 0;
    z64_game.common.input[0].raw.pad.b = z64_game.common.input[0].pad_pressed.b = 0;
    z64_game.common.input[0].raw.pad.r = z64_game.common.input[0].pad_pressed.r = 0;
    z64_game.common.input[0].raw.pad.z = z64_game.common.input[0].pad_pressed.z = 0;
}

void handle_options_menu_input(pad_t pad_pressed) {
    switch (options_cursor) {
        case OPTION_ARROW_TOGGLE:       z64_file.inf_table[0x15] ^= 1 << 5;  return;
        case OPTION_SHOW_HEALTH:        z64_file.inf_table[0x15] ^= 1 << 6;  return;
        case OPTION_CHEST_CONTENTS:     z64_file.inf_table[0x15] ^= 1 << 7;  return;
        case OPTION_INVERSE_AIM:        z64_file.inf_table[0x15] ^= 1 << 8;  return;
        case OPTION_NO_IDLE_CAMERA:     z64_file.inf_table[0x15] ^= 1 << 9;  return;
        case OPTION_KEEP_MASK:          z64_file.inf_table[0x15] ^= 1 << 10; return;
        case OPTION_TRISWIPE:           z64_file.inf_table[0x15] ^= 1 << 11; return;
        case OPTION_SWAP_ITEM:          z64_file.inf_table[0x15] ^= 1 << 15; return;
        case OPTION_UNEQUIP_ITEM:       z64_file.inf_table[0x15] ^= 1 << 12; return;
        case OPTION_UNEQUIP_GEAR:       z64_file.inf_table[0x15] ^= 1 << 13; return;
        case OPTION_ITEM_ON_B:          z64_file.inf_table[0x15] ^= 1 << 14; return;
        case OPTION_MASK_ABILITIES:     z64_file.inf_table[0x13] ^= 1 << 0;  return;
        case OPTION_EXTRA_ABILITIES:    z64_file.inf_table[0x14] ^= 1 << 10; return;
        case OPTION_RANDOM_ENEMIES:     z64_file.inf_table[0x1B] ^= 1 << 15; return;
        case OPTION_CROUCH_STAB_FIX:    z64_file.inf_table[0x14] ^= 1 << 8;  return;
        case OPTION_WEAKER_SWORDS:      z64_file.inf_table[0x14] ^= 1 << 9;  return;
        case OPTION_LEVITATION:         z64_file.inf_table[0x14] ^= 1 << 11; return;
        case OPTION_INFINITE_HP:        z64_file.inf_table[0x14] ^= 1 << 12; return;
        case OPTION_INFINITE_MP:        z64_file.inf_table[0x14] ^= 1 << 13; return;
        case OPTION_INFINITE_RUPEES:    z64_file.inf_table[0x14] ^= 1 << 14; return;
        case OPTION_INFINITE_AMMO:      z64_file.inf_table[0x14] ^= 1 << 15; return;
        
        case OPTION_30_FPS:
            z64_file.inf_table[0x15] ^= 1 << 4;
            if (!SAVE_30_FPS)
                reset_fps_values();
            return;
        
        case OPTION_DPAD:
            write_option(0x1B, 0,  pad_pressed, SAVE_DPAD);
            return;
        
        case OPTION_SHOW_DPAD:
            write_option(0x1B, 2,  pad_pressed, SAVE_SHOW_DPAD);
            return;
        
        case OPTION_HIDE_HUD:
            write_option(0x1B, 4,  pad_pressed, SAVE_HIDE_HUD);
            return;
        
        case OPTION_HUD_LAYOUT:
            write_option(0x1B, 7,  pad_pressed, SAVE_HUD_LAYOUT);
            reset_layout();
            return;
        
        case OPTION_A_BUTTON_SCALE:
            write_option(0x1B, 10, pad_pressed, SAVE_A_BUTTON_SCALE);
            reset_layout();
            return;
        
        case OPTION_B_BUTTON_SCALE:
            write_option(0x1B, 12, pad_pressed, SAVE_B_BUTTON_SCALE);
            reset_layout();
            return;
        
        case OPTION_C_LEFT_BUTTON_SCALE:
            write_option(0x1C, 0,  pad_pressed, SAVE_C_LEFT_BUTTON_SCALE);
            reset_layout();
            return;
        
        case OPTION_C_DOWN_BUTTON_SCALE:
            write_option(0x1C, 4,  pad_pressed, SAVE_C_DOWN_BUTTON_SCALE);
            reset_layout();
            return;
        
        case OPTION_C_RIGHT_BUTTON_SCALE:
            write_option(0x1C, 8,  pad_pressed, SAVE_C_RIGHT_BUTTON_SCALE);
            reset_layout();
            return;
        
        case OPTION_DAMAGE_TAKEN:
            write_option(0x1C, 12, pad_pressed, SAVE_DAMAGE_TAKEN);
            return;

        
        case OPTION_RUPEE_DRAIN:
            write_option(0x18, 0,  pad_pressed, SAVE_RUPEE_DRAIN);
            return;
        
        case OPTION_FOG:
            write_option(0x18, 4,  pad_pressed, SAVE_FOG);
            if (SAVE_FOG == 0)
                z64_game.fog_distance = 10.0f;
            return;
        
        case OPTION_INVENTORY_EDITOR:
            z64_game.pause_ctxt.unk_02_[1] = 2;
            z64_game.common.input[0].raw.pad.a = z64_game.common.input[0].pad_pressed.a = 0;
            z64_game.common.input[0].raw.pad.b = z64_game.common.input[0].pad_pressed.b = 0;
            return;
    }
}

uint8_t write_option(uint8_t index, uint8_t shift, pad_t pad_pressed, uint16_t save) {
    uint16_t max = options_max[options_cursor] << shift;
    uint16_t one = 1 << shift;
    
    if (pad_pressed.a) {
        if (save < options_max[options_cursor])
            z64_file.inf_table[index] += one;
        else z64_file.inf_table[index] -= max;
    }
    else {
        if (save > 0)
            z64_file.inf_table[index] -= one;
        else z64_file.inf_table[index] += max;
    }
}

uint8_t draw_settings_menu(z64_disp_buf_t *db) {
    if (z64_game.pause_ctxt.state != 6 || z64_game.pause_ctxt.unk_02_[1] != 3)
        return 0;
    
    uint8_t x = 40;
    if (CFG_WS)
        x += 52;
    
    uint8_t y                = 40;
    uint8_t top              = 75;
    uint8_t left             = x + 58;
    uint8_t setting             = 0;
    uint8_t tooltipLeft      = left - 45;
    uint8_t width            = 80;
    uint8_t height           = 32;
    
    gDPSetPrimColor(db->p++, 0, 0, 0xA0, 0xA0, 0xA0, 0xFF);
    sprite_load_and_draw(db, &subscreen_sprite, 4,  x,         y, width, height);
    sprite_load_and_draw(db, &subscreen_sprite, 66, x + width, y, width, height);
    sprite_load_and_draw(db, &subscreen_sprite, 5,  x + 160,   y, width, height);
    
    for (uint8_t i=0; i<3; i++)
        for (uint8_t j=1; j<=4; j++)
            sprite_load_and_draw(db, &subscreen_sprite, (i + 3) + (15 * j), x + (width * i), y + (height * j), width, height);
    
    gDPSetPrimColor(db->p++, 0, 0, 0xD0, 0xD0, 0xD0, 0xFF);
    sprite_load_and_draw(db, &button_sprite, 2, left - 35,  top - 10, 32, 32);
    sprite_load_and_draw(db, &button_sprite, 4, left + 120, top - 10, 32, 32);
    
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    sprite_load_and_draw(db, &title_sprite, 9, x + 96, y + 3, 128, 16);
    
    // Options Text
    switch (options_cursor) {
        case OPTION_30_FPS:
            setting = SAVE_30_FPS;
            text_print("Allows 30 FPS, press L and", tooltipLeft, top + 50);
            text_print("Z to toggle between 30 FPS", tooltipLeft, top + 70);
            text_print("and 20 FPS mode",            tooltipLeft, top + 90);
            break;
        
        case OPTION_DPAD:
            setting = SAVE_DPAD;
            text_print("0. Disabled",                tooltipLeft, top + 50);
            text_print("1. Enabled",                 tooltipLeft, top + 70);
            text_print("2. Dual Set",                tooltipLeft, top + 90);
            break;
            
        case OPTION_ARROW_TOGGLE:
            setting = SAVE_ARROW_TOGGLE;
            text_print("Enables Arrow Toggle",       tooltipLeft, top + 50);
            text_print("Press R while aiming to",    tooltipLeft, top + 70);
            text_print("toggle arrow types ",        tooltipLeft, top + 90);
            break;
        
        case OPTION_SHOW_DPAD:
            setting = SAVE_SHOW_DPAD;
            text_print("0. Hidden, 1. Left",         tooltipLeft, top + 50);
            text_print("2. Right,  3. Bottom",       tooltipLeft, top + 70);
            break;
        
        case OPTION_HIDE_HUD:
            setting = SAVE_HIDE_HUD;
            text_print("Choose from four different", tooltipLeft, top + 50);
            text_print("options to hide the HUD",    tooltipLeft, top + 70);
            break;
        
        case OPTION_HUD_LAYOUT:
            setting = SAVE_HUD_LAYOUT;
            text_print("Choose from seven",          tooltipLeft, top + 50);
            text_print("different layouts to ",      tooltipLeft, top + 70);
            text_print("change the HUD",             tooltipLeft, top + 90);
            break;
        
        case OPTION_SHOW_HEALTH:
            setting = SAVE_SHOW_HEALTH;
            text_print("Show the amount of health", tooltipLeft, top + 50);
            text_print("enemies have left in the",  tooltipLeft, top + 70);
            text_print("HUD",                       tooltipLeft, top + 90);           
            break;
        
        case OPTION_CHEST_CONTENTS:
            setting = SAVE_CHEST_CONTENTS;
            text_print("Adjusts the appearance of", tooltipLeft, top + 50);
            text_print("the chest according to",    tooltipLeft, top + 70);
            text_print("the contents it contains",  tooltipLeft, top + 90);           
            break;
        
        case OPTION_A_BUTTON_SCALE:
            setting = SAVE_A_BUTTON_SCALE;
            text_print("Set the scale for the",      tooltipLeft, top + 50);
            text_print("A button",                   tooltipLeft, top + 70);
            break;
        
        case OPTION_B_BUTTON_SCALE:
            setting = SAVE_B_BUTTON_SCALE;
            text_print("Set the scale for the",      tooltipLeft, top + 50);
            text_print("B button",                   tooltipLeft, top + 70);
            break;
            
        case OPTION_C_LEFT_BUTTON_SCALE:
            setting = SAVE_C_LEFT_BUTTON_SCALE;
            text_print("Set the scale for the",      tooltipLeft, top + 50);
            text_print("C-Left button",              tooltipLeft, top + 70);
            break;
            
        case OPTION_C_DOWN_BUTTON_SCALE:
            setting = SAVE_C_DOWN_BUTTON_SCALE;
            text_print("Set the scale for the",      tooltipLeft, top + 50);
            text_print("C-Down button",              tooltipLeft, top + 70);
            break;
            
        case OPTION_C_RIGHT_BUTTON_SCALE:
            setting = SAVE_C_RIGHT_BUTTON_SCALE;
            text_print("Set the scale for the",      tooltipLeft, top + 50);
            text_print("C-Right button",             tooltipLeft, top + 70);
            break;
        
        case OPTION_INVERSE_AIM:
            setting = SAVE_INVERSE_AIM;
            text_print("Inverse y-axis for analog",  tooltipLeft, top + 50);
            text_print("controls when aiming in",    tooltipLeft, top + 70);
            text_print("first-person view",          tooltipLeft, top + 90);
            break;
        
        case OPTION_NO_IDLE_CAMERA:
            setting = SAVE_NO_IDLE_CAMERA;
            text_print("The camera no longer moves", tooltipLeft, top + 50);
            text_print("behind Link during idle",    tooltipLeft, top + 70);
            text_print("stance",                     tooltipLeft, top + 90);
            break;
        
        case OPTION_KEEP_MASK:
            setting = SAVE_KEEP_MASK;
            text_print("Mask remains equipped upon", tooltipLeft, top + 50);
            text_print("entering another area",      tooltipLeft, top + 70);
            break;
        
        case OPTION_TRISWIPE:
            setting = SAVE_TRISWIPE;
            text_print("Replaces the transition",    tooltipLeft, top + 50);
            text_print("effect with a twirling",     tooltipLeft, top + 70);
            text_print("Triforce animation",         tooltipLeft, top + 90);
            break;
            
        case OPTION_SWAP_ITEM:
            setting = SAVE_SWAP_ITEM;
            text_print("Cycle with C-Up between",    tooltipLeft, top + 50);
            text_print("obtained items you own",     tooltipLeft, top + 70);
            break;
            
        case OPTION_UNEQUIP_ITEM:
            setting = SAVE_UNEQUIP_ITEM;
            text_print("Unassign items using the",   tooltipLeft, top + 50);
            text_print("respective C button",        tooltipLeft, top + 70);
            break;
        
        case OPTION_UNEQUIP_GEAR:
            setting = SAVE_UNEQUIP_GEAR;
            text_print("Unassign equipment by",      tooltipLeft, top + 50);
            text_print("pressing A",                 tooltipLeft, top + 70);
            break;
        
        case OPTION_ITEM_ON_B:
            setting = SAVE_ITEM_ON_B;
            text_print("Assign items to the B",      tooltipLeft, top + 50);
            text_print("button by pressing A",       tooltipLeft, top + 70);
            break;
        
        case OPTION_MASK_ABILITIES:
            setting = SAVE_MASK_ABILITIES;
            text_print("Equiping masks grant Link",  tooltipLeft, top + 50);
            text_print("with abilities such as",     tooltipLeft, top + 70);
            text_print("faster running or swimming", tooltipLeft, top + 90);
            break;
        
        case OPTION_EXTRA_ABILITIES:
            setting = SAVE_EXTRA_ABILITIES;
            text_print("Obtain Spiritual Stones",    tooltipLeft, top + 50);
            text_print("and Medallions to earn new", tooltipLeft, top + 70);
            text_print("abilities",                  tooltipLeft, top + 90);
            break;
            
        case OPTION_DAMAGE_TAKEN:
            if (SAVE_DAMAGE_TAKEN > 0)
                setting = 1 << (SAVE_DAMAGE_TAKEN-1);
            text_print("Multiplies taken damage",    tooltipLeft, top + 50);
            text_print("by the factor shown below",  tooltipLeft, top + 70);
            text_print("0 = Off",                    tooltipLeft, top + 90);
            break;
        
        case OPTION_RANDOM_ENEMIES:
            setting = SAVE_RANDOM_ENEMIES;
            text_print("Multiplies the health and",  tooltipLeft, top + 50);
            text_print("changes the subtype for",    tooltipLeft, top + 70);
            text_print("regular enemies randomly",   tooltipLeft, top + 90);
            break;
        
        case OPTION_CROUCH_STAB_FIX:
            setting = SAVE_CROUCH_STAB_FIX;
            text_print("The Crouch Stab move no",    tooltipLeft, top + 50);
            text_print("longer keeps the last",      tooltipLeft, top + 70);
            text_print("dealt damage",               tooltipLeft, top + 90);
            break;
        
        case OPTION_WEAKER_SWORDS:
            setting = SAVE_WEAKER_SWORDS;
            text_print("Sword slashes now deal one", tooltipLeft, top + 50);
            text_print("less point of damage",       tooltipLeft, top + 70);
            break;
        
        case OPTION_RUPEE_DRAIN:
            setting = SAVE_RUPEE_DRAIN;
            text_print("First drains rupees, then",  tooltipLeft, top + 50);
            text_print("health based on set",        tooltipLeft, top + 70);
            text_print("seconds",                    tooltipLeft, top + 90);
            break;
        
        case OPTION_FOG:
            setting = SAVE_FOG;
            text_print("Adjust the level of fog",    tooltipLeft, top + 50);
            text_print("applied, with higher",       tooltipLeft, top + 70);
            text_print("values decreasing the fog",  tooltipLeft, top + 90);
            break;
        
        case OPTION_INVENTORY_EDITOR:
            text_print("Open the Inventory Editor",  tooltipLeft, top + 50);
            text_print("subscreen and adjust all",   tooltipLeft, top + 70);
            text_print("of your items",              tooltipLeft, top + 90);
            break;
        
        case OPTION_LEVITATION:
            setting = SAVE_LEVITATION;
            text_print("Hold the L button to",       tooltipLeft, top + 50);
            text_print("levitate",                   tooltipLeft, top + 70);
            break;
        
        case OPTION_INFINITE_HP:
            setting = SAVE_INFINITE_HP;
            text_print("Your Health is kept at",     tooltipLeft, top + 50);
            break;
        
        case OPTION_INFINITE_MP:
            setting = SAVE_INFINITE_MP;
            text_print("Your Magic is kept at",       tooltipLeft, top + 50);
            break;
        
        case OPTION_INFINITE_RUPEES:
            setting = SAVE_INFINITE_RUPEES;
            text_print("Your Rupees are kept at",     tooltipLeft, top + 50);
            break;
        
        case OPTION_INFINITE_AMMO:
            setting = SAVE_INFINITE_AMMO;
            text_print("Your Ammo is kept at",        tooltipLeft, top + 50);
            break;
    }
    if (options_cursor >= OPTION_INFINITE_HP && options_cursor <= OPTION_INFINITE_AMMO)
        text_print("the current maximum limit", tooltipLeft, top + 70);
    
    text_flush(db);
    
    // Options Value & Title
    if (setting == 0)
        gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    else gDPSetPrimColor(db->p++, 0, 0, 0x00, 0xFF, 0x00, 0xFF);
    
    if (options_cursor != OPTION_INVENTORY_EDITOR) {
        sprite_load_and_draw(db, &stones_sprite, 0, -10, -10, 4, 4); // Fake Texture
        
        uint8_t adjust = 8;
        if (setting >= 10) {
            sprite_load_and_draw(db, &ammo_digit_sprite, (setting / 10), left + 48, top + 20, 16, 16);
            adjust = 0;
        }
        sprite_load_and_draw(db, &ammo_digit_sprite, (setting % 10), left + 64 - adjust, top + 20, 16, 16);
    }
    
    text_print(options[options_cursor],  left + options_recenter[options_cursor], top);
    text_flush(db);
    
    return 1;
}

uint8_t draw_abilities_info(z64_disp_buf_t *db) {
    if (!OPTION_ACTIVE(2, SAVE_EXTRA_ABILITIES, CFG_DEFAULT_EXTRA_ABILITIES) || !IS_PAUSE_SCREEN_CURSOR || z64_game.pause_ctxt.screen_idx != 2 || !z64_game.common.input[0].raw.pad.a)
        return 0;
    
    uint8_t show;
    
    if      (z64_game.pause_ctxt.quest_cursor == 5    && z64_file.light_medallion)  { show = 0; } // Light Medallion
    else if (z64_game.pause_ctxt.quest_cursor == 0    && z64_file.forest_medallion) { show = 1; } // Forest Medallion
    else if (z64_game.pause_ctxt.quest_cursor == 1    && z64_file.fire_medallion)   { show = 2; } // Fire Medallion
    else if (z64_game.pause_ctxt.quest_cursor == 2    && z64_file.water_medallion)  { show = 3; } // Water Medallion
    else if (z64_game.pause_ctxt.quest_cursor == 4    && z64_file.shadow_medallion) { show = 4; } // Shadow Medallion
    else if (z64_game.pause_ctxt.quest_cursor == 3    && z64_file.spirit_medallion) { show = 5; } // Spirit Medallion
    else if (z64_game.pause_ctxt.quest_cursor == 0x12 && z64_file.kokiris_emerald)  { show = 6; } // Kokiri's Emerald
    else if (z64_game.pause_ctxt.quest_cursor == 0x13 && z64_file.gorons_ruby)      { show = 7; } // Goron's Ruby
    else if (z64_game.pause_ctxt.quest_cursor == 0x14 && z64_file.zoras_sapphire)   { show = 8; } // Zora's Sapphire
    else return 0;
    
    uint8_t left     = 15;
    uint8_t leftInfo = 85;
    uint8_t info1    = 55;
    uint8_t info2    = 72;
    uint8_t info3    = 89;
    uint8_t info4    = 106;
    
    gDPSetCombineMode(db->p++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(db->p++, 0, 0, 0x00, 0x00, 0x00, 0xD0);
    gSPTextureRectangle(db->p++,
        10<<2,  10<<2,
        250<<2, 125<<2,
        0,
        0, 0,
        1<<10, 1<<10);
        
    gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    switch (show) {
        case 0: // Light Medallion
            text_print("Run, hold L and",               left, info1);
            text_print("press A to jump",               left, info2);
            text_print("Requires the Kokiri Boots",     left, info3);
            text_print("for it to be effective",        left, info4);
            break;
        
        case 1: // Forest Medallion
            text_print("Restores some magic when hit",  left, info1);
            text_print("Replaces the Kokiri Tunic and", left, info2);
            text_print("requires to be equipped",       left, info3);
            break;
                
        case 2: // Fire Medallion
            text_print("Reduces damage taken by half",  left, info1);
            text_print("Replaces the Goron Tunic and",  left, info2);
            text_print("requires to be equipped",       left, info3);
            break;
        
        case 3: // Water Medallion
            text_print("Increases damage done by",      left, info1);
            text_print("sword slashes by +1",           left, info2);
            text_print("Replaces the Zora Tunic and",   left, info3);
            text_print("requires to be equipped",       left, info4);
            break;
        
        case 4: // Shadow Medallion
            text_print("Restores some health over",     left, info1);
            text_print("time at the cost of magic",     left, info2);
            text_print("Unequip the tunic with C-Up",   left, info3);
            text_print("for it to be effective",        left, info4);
            break;
        
        case 5: // Spirit Medallion
            text_print("The Jump and Dash abilities",   left, info1);
            text_print("can now be used with the",      left, info2);
            text_print("Hover Boots as well",           left, info3);
            break;
        
        case 6: // Kokiri's Emerald
            text_print("Hold L when running to dash",   left, info1);
            text_print("Consumes magic",                left, info2);
            text_print("Requires the Kokiri Boots",     left, info3);
            text_print("for it to be effective",        left, info4);
            break;
        
        case 7: // Goron's Ruby
            text_print("Upgrades the Dash Ability",     left, info1);
            text_print("to be faster",                  left, info2);
            break;
        
        case 8: // Zora's Sapphire
            text_print("Lower the magic cost of",       left, info1);
            text_print("the dash ability",              left, info2);
            break;
    }
    text_flush(db);
    
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0x00, 0x00, 0xFF);
    text_print("Item:",                 left,     15);
    text_print("Ability:",              left,     35);
    text_print(medallion_item[show],    leftInfo, 15);
    text_print(medallion_ability[show], leftInfo, 35);
    
    text_flush(db);
    gDPFullSync(db->p++);
    gSPEndDisplayList(db->p++);
        
    return 1;
}

void handle_inventory_editor() {
    if (z64_game.pause_ctxt.unk_02_[1] != 2 || CFG_OPTIONS_MENU < 3)
        return;
    
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
    
    if (pad_pressed.a || pad_pressed.b) {
        z64_game.pause_ctxt.unk_02_[1] = 3;
        play_sfx = 0x4814;
    }
    
    if (z64_x_axis_input > -10 && z64_x_axis_input < 10 && z64_y_axis_input > -10 && z64_y_axis_input < 10)
        moved_axis_option = holding_stick = 0;
    if (moved_axis_option) {
        options_frames++;
        uint8_t compare_frames = 15;
        if (holding_stick)
            compare_frames = 1;
        if (options_frames > compare_frames) {
            options_frames = moved_axis_option = 0;
            holding_stick  = 1;
        }
    }
    else options_frames = 0;
    
    if (!moved_axis_option) {
        if (z64_x_axis_input < -50) {
            moved_axis_option = 1;
            if (z64_inventory_editor_index == 0)
                z64_inventory_editor_index = 0x5C;
            else z64_inventory_editor_index--;
        }
        else if (z64_x_axis_input > 50) {
            moved_axis_option = 1;
            if (z64_inventory_editor_index == 0x5C)
                z64_inventory_editor_index = 0;
            else z64_inventory_editor_index++;
        }
        else if (z64_y_axis_input > 50) {
            moved_axis_option = 1;
            
            if (     z64_inventory_editor_index <= 0x5C && z64_inventory_editor_index > 0x59)
                z64_inventory_editor_index = 0x59;
            else if (z64_inventory_editor_index <= 0x59 && z64_inventory_editor_index > 0x56)
                z64_inventory_editor_index = 0x56;
            else if (z64_inventory_editor_index <= 0x56 && z64_inventory_editor_index > 0x4A)
                z64_inventory_editor_index = 0x4A;
            else if (z64_inventory_editor_index <= 0x4A && z64_inventory_editor_index > 0x44)
                z64_inventory_editor_index = 0x44;
            else if (z64_inventory_editor_index <= 0x44 && z64_inventory_editor_index > 0x38)
                z64_inventory_editor_index = 0x38;
            else if (z64_inventory_editor_index <= 0x38 && z64_inventory_editor_index > 0x34)
                z64_inventory_editor_index = 0x34;
            else if (z64_inventory_editor_index <= 0x34 && z64_inventory_editor_index > 0x2C)
                z64_inventory_editor_index = 0x2C;
            else if (z64_inventory_editor_index <= 0x2C && z64_inventory_editor_index > 0x1B)
                z64_inventory_editor_index = 0x1B;
            else if (z64_inventory_editor_index <= 0x1B && z64_inventory_editor_index > 3)
                z64_inventory_editor_index = 3;
            else if (z64_inventory_editor_index <= 3    && z64_inventory_editor_index > 0)
                z64_inventory_editor_index = 0;
            else if (z64_inventory_editor_index <= 0)
                z64_inventory_editor_index = 0x5C;
        }
        else if (z64_y_axis_input < -50) {
            moved_axis_option = 1;
            
            if (     z64_inventory_editor_index >= 0    && z64_inventory_editor_index < 3)
                z64_inventory_editor_index = 3;
            else if (z64_inventory_editor_index >= 3    && z64_inventory_editor_index < 0x1B)
                z64_inventory_editor_index = 0x1B;
            else if (z64_inventory_editor_index >= 0x1B && z64_inventory_editor_index < 0x2C)
                z64_inventory_editor_index = 0x2C;
            else if (z64_inventory_editor_index >= 0x2C && z64_inventory_editor_index < 0x34)
                z64_inventory_editor_index = 0x34;
            else if (z64_inventory_editor_index >= 0x34 && z64_inventory_editor_index < 0x38)
                z64_inventory_editor_index = 0x38;
            else if (z64_inventory_editor_index >= 0x38 && z64_inventory_editor_index < 0x44)
                z64_inventory_editor_index = 0x44;
            else if (z64_inventory_editor_index >= 0x44 && z64_inventory_editor_index < 0x4A)
                z64_inventory_editor_index = 0x4A;
            else if (z64_inventory_editor_index >= 0x4A && z64_inventory_editor_index < 0x56)
                z64_inventory_editor_index = 0x56;
            else if (z64_inventory_editor_index >= 0x56 && z64_inventory_editor_index < 0x59)
                z64_inventory_editor_index = 0x59;
            else if (z64_inventory_editor_index >= 0x59 && z64_inventory_editor_index < 0x5C)
                z64_inventory_editor_index = 0x5C;
            else if (z64_inventory_editor_index >= 0x5C)
                z64_inventory_editor_index = 0;
        }
    }
    
    z64_x_axis_input = z64_y_axis_input = 0;
    z64_game.common.input[0].raw.pad.b = z64_game.common.input[0].pad_pressed.b = 0;
    z64_game.common.input[0].raw.pad.r = z64_game.common.input[0].pad_pressed.r = 0;
    z64_game.common.input[0].raw.pad.z = z64_game.common.input[0].pad_pressed.z = 0;
}