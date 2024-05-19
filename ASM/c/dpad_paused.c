#include "dpad_paused.h"

extern uint8_t CFG_PREVENT_GROTTO_SAVING;

extern uint16_t play_sfx;

uint8_t had_fairy_ocarina   = 0;
uint8_t had_ocarina_of_time = 0;
uint8_t had_hookshot        = 0;
uint8_t had_longshot        = 0;

const static int16_t sold_mask_check[7] = { ITEMGETINF_SOLD_KEATON_MASK, ITEMGETINF_SOLD_SKULL_MASK, ITEMGETINF_SOLD_SPOOKY_MASK, ITEMGETINF_OTHER_MASKS_AVAILABLE, ITEMGETINF_OTHER_MASKS_AVAILABLE, ITEMGETINF_OTHER_MASKS_AVAILABLE, (ITEMGETINF_OTHER_MASKS_AVAILABLE && ITEMGETINF_MASK_OF_TRUTH_LOANED) };

void handle_dpad_paused() {
    if (!IS_PAUSE_SCREEN_CURSOR)
        return;
    
    if (CFG_PREVENT_GROTTO_SAVING && z64_game.scene_index == SCENE_GROTTO)
        z64_game.common.input[0].raw.pad.b = z64_game.common.input[0].pad_pressed.b = 0;
    
    pad_t pad_pressed = z64_game.common.input[0].pad_pressed;
    if (CAN_USE_DPAD && SAVE_DPAD > 0)
        handle_dpad_slots(pad_pressed);
    handle_unequipping(pad_pressed);
    handle_downgrading(pad_pressed);
    handle_switch_mask(pad_pressed);
}

void handle_dpad_slots(pad_t pad_pressed) {
    if (OPTION_VALUE(1, 0, SAVE_DPAD, CFG_DEFAULT_DPAD))
        return;
    
    if (z64_game.pause_ctxt.screen_idx == 3) {
        uint8_t cursor = z64_game.pause_ctxt.equip_cursor + Z64_SLOT_QUIVER;
        if (cursor >= Z64_SLOT_KOKIRI_SWORD && cursor <= Z64_SLOT_HOVER_BOOTS && cursor != Z64_SLOT_BOMB_BAG && cursor != Z64_SLOT_STRENGTH && cursor != Z64_SLOT_SCALE)
            set_dpad_action(pad_pressed, cursor);
        
    }
    else if (z64_game.pause_ctxt.screen_idx == 0) {
        uint8_t cursor = z64_game.pause_ctxt.item_cursor;
        if (cursor == Z64_SLOT_NUT || cursor == Z64_SLOT_DINS_FIRE || cursor == Z64_SLOT_OCARINA || cursor == Z64_SLOT_FARORES_WIND || cursor == Z64_SLOT_LENS  || cursor == Z64_SLOT_NAYRUS_LOVE || cursor == Z64_SLOT_ADULT_TRADE || cursor == Z64_SLOT_CHILD_TRADE)
            set_dpad_action(pad_pressed, cursor);
    }
}

void set_dpad_action(pad_t pad_pressed, z64_slot_t slot) {
    if (z64_usability.item[slot] != z64_file.link_age && z64_usability.item[slot] != 9)
        return;
    
    const uint8_t dpad_array[4] = { pad_pressed.du, pad_pressed.dr, pad_pressed.dd, pad_pressed.dl };
                                                                     
    for (uint8_t i=0; i<4; i++)
        if (dpad_array[i]) {
            const uint8_t index = DPAD_BUTTON_INDEX(i);
            DPAD_SET_BUTTON(index) = run_set_dpad_action(slot, DPAD_SET_BUTTON(index));
        }
}

uint8_t run_set_dpad_action(z64_slot_t slot, uint8_t button) {
    if (button == slot) {
        play_sfx = 0x480A;
        return Z64_SLOT_NULL;
    }
    play_sfx = 0x4808;
    return slot;
    
}

void handle_unequipping(pad_t pad_pressed) {
    if (z64_game.pause_ctxt.screen_idx == 3 && pad_pressed.a) {
        if (OPTION_ACTIVE(2, SAVE_UNEQUIP_GEAR, CFG_DEFAULT_UNEQUIP_GEAR)) { // Unequip gear
            if (z64_game.pause_ctxt.equip_cursor     == z64_file.equip_sword)
                unequip_sword(1);
            if (z64_game.pause_ctxt.equip_cursor - 4 == z64_file.equip_shield)
                unequip_shield();
        }
        if (OPTION_ACTIVE(2, SAVE_EXTRA_ABILITIES, CFG_DEFAULT_EXTRA_ABILITIES) || OPTION_ACTIVE(2, SAVE_UNEQUIP_GEAR, CFG_DEFAULT_UNEQUIP_GEAR))
            if (z64_game.pause_ctxt.equip_cursor - 8 == z64_file.equip_tunic)
                unequip_tunic();
    }
    
    if (z64_game.pause_ctxt.screen_idx == 0 && OPTION_ACTIVE(2, SAVE_UNEQUIP_ITEM, CFG_DEFAULT_UNEQUIP_ITEM)) { // Unset item from C button
        uint8_t button;
        if (pad_pressed.cl)
            button = 1;
        else if (pad_pressed.cd)
            button = 2;
        else if (pad_pressed.cr)
            button = 3;
        else return;
    
        for (uint8_t cursor=0; cursor<24; cursor++)
            if (z64_game.pause_ctxt.item_cursor == cursor)
                for (uint8_t item=0; item<=0x3A; item++) {
                    uint8_t compare_cursor = cursor;
                    uint8_t compare_item   = item;
                    if (cursor == 0x4) {
                        compare_cursor = 0x3;
                        compare_item   = 0x38;
                    }
                    else if (cursor == 0xA) {
                        compare_cursor = 0x3;
                        compare_item   = 0x39;
                    }
                    else if (cursor == 0x10) {
                        compare_cursor = 0x3;
                        compare_item   = 0x3A;
                    }
                    
                    if (z64_file.button_items[button] == compare_item && z64_file.c_button_slots[button - 1] == compare_cursor) {
                        z64_game.common.input[0].pad_pressed.cl = z64_game.common.input[0].pad_pressed.cd = z64_game.common.input[0].pad_pressed.cr = 0;
                        z64_file.button_items[button] = z64_file.c_button_slots[button - 1] = 0xFF;
                        if (!z64_file.link_age)
                            z64_file.adult_button_items[button]    = z64_file.adult_c_button_slots[button - 1] = 0xFF;
                        else z64_file.child_button_items[button] = z64_file.child_c_button_slots[button - 1] = 0xFF;
                        play_sfx = 0x480A;
                        break;
                    }
                }
    }
}

void handle_switch_mask(pad_t pad_pressed) {
    if (!CAN_USE_MASK_SWAP || !pad_pressed.cu)
        return;
    
    uint8_t next_mask = get_next_mask();
    
    if (z64_file.items[Z64_SLOT_CHILD_TRADE] != next_mask) {
        update_trade_equips(next_mask, z64_game.pause_ctxt.item_cursor);
        if (z64_link.current_mask > 0 && next_mask >= Z64_ITEM_KEATON_MASK && next_mask <= Z64_ITEM_MASK_OF_TRUTH)
            z64_link.current_mask = next_mask - Z64_ITEM_ZELDAS_LETTER;
        play_sfx = 0x4808;
    }
}

uint8_t get_next_mask() {
    uint8_t next_mask = z64_file.items[Z64_SLOT_CHILD_TRADE] < Z64_ITEM_MASK_OF_TRUTH ? z64_file.items[Z64_SLOT_CHILD_TRADE] + 1 : Z64_ITEM_KEATON_MASK;
    for (uint8_t i=0; i<7; i++)
        if (!GET_ITEMGETINF(sold_mask_check[i]) && z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_KEATON_MASK + i)
            return Z64_ITEM_KEATON_MASK;
    return next_mask;
}

void update_trade_equips(uint16_t item, int16_t slot) {
    z64_file.items[Z64_SLOT_CHILD_TRADE] = item;
    for (int i = 0; i < 3; i++) {
        if (z64_file.c_button_slots[i] == slot) {
            z64_file.button_items[i + 1] = item;
            Interface_LoadItemIcon1(&z64_game, i + 1);
        }
        if (z64_file.child_c_button_slots[i] == slot)
            z64_file.child_button_items[i + 1] = item;
        if (z64_file.adult_c_button_slots[i] == slot)
            z64_file.adult_button_items[i + 1] = item;
    }
}

void handle_downgrading(pad_t pad_pressed) {
    // Check if both ocarinas were in the same slot at some point
    if (had_fairy_ocarina = 2 && had_ocarina_of_time == 2)
        z64_file.inf_table[0x15] |= 1 << 2;
    else if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA)
        had_fairy_ocarina = 1;
    else if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME)
        had_ocarina_of_time = 1;
    else if (had_fairy_ocarina == 1 && had_ocarina_of_time == 1) {
        had_fairy_ocarina = had_ocarina_of_time = 2;
        for (z64_slot_t i=0; i<24; i++) {
            if (i == Z64_SLOT_OCARINA)
                continue;
            if (z64_file.items[i] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[i] == Z64_ITEM_OCARINA_OF_TIME) {
                had_fairy_ocarina = had_ocarina_of_time = 2;
                break;
            }
        }
    }
    
    // Check if both hookshots were in the same slot at some point
    if (had_hookshot == 2 && had_longshot == 2)
        z64_file.inf_table[0x15] |= 1 << 3;
    else if (z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_HOOKSHOT)
        had_hookshot = 1;
    else if (z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_LONGSHOT)
        had_longshot = 1;
    else if (had_hookshot == 1 && had_longshot == 1) {
        had_hookshot = had_longshot = 2;
        for (z64_slot_t i=0; i<24; i++) {
            if (i == Z64_SLOT_HOOKSHOT)
                continue;
            if (z64_file.items[i] == Z64_ITEM_HOOKSHOT || z64_file.items[i] == Z64_ITEM_LONGSHOT) {
                had_hookshot = had_longshot = 2;
                break;
            }
        }
    }
    
    // Check if Biggoron Sword was in your possession at some point
    if (z64_file.bgs_flag)
        z64_file.inf_table[0x15] |= 1 << 1;;
    if (z64_file.giants_knife)
        if ( (z64_file.broken_giants_knife && z64_file.bgs_hits_left > 0) ||  (!z64_file.broken_giants_knife && z64_file.bgs_hits_left == 0) )
            z64_file.broken_giants_knife ^= 1;
    
    if (!pad_pressed.cu)
        return;
    
    if (CAN_USE_GIANTS_KNIFE_SWAP) {
        z64_file.bgs_flag ^= 1;
        if (!z64_file.bgs_flag)
            z64_file.bgs_hits_left = z64_file.unk_06_[2];
        
        if (z64_file.equip_sword == 3)
                unequip_sword(0);
            z64_UpdateEquipment(&z64_game, &z64_link);
            play_sfx = 0x4808;
    }
            
    if (CAN_USE_OCARINA_SWAP) { // Downgrade / Upgrade ocarina
        if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME)
            swap_item(Z64_SLOT_OCARINA, Z64_ITEM_OCARINA_OF_TIME, Z64_ITEM_FAIRY_OCARINA);
        else if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA)
            swap_item(Z64_SLOT_OCARINA, Z64_ITEM_FAIRY_OCARINA, Z64_ITEM_OCARINA_OF_TIME);
    }
    
    if (CAN_USE_HOOKSHOT_SWAP) { // Downgrade / Upgrade hookshot
        if (z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_LONGSHOT)
            swap_item(Z64_SLOT_HOOKSHOT, Z64_ITEM_LONGSHOT, Z64_ITEM_HOOKSHOT);
        else if (z64_file.items[Z64_SLOT_HOOKSHOT] == Z64_ITEM_HOOKSHOT)
            swap_item(Z64_SLOT_HOOKSHOT, Z64_ITEM_HOOKSHOT, Z64_ITEM_LONGSHOT);
    }
}

void swap_item(z64_slot_t slot, z64_item_t item, z64_item_t swap) {
    if (z64_file.items[slot] == item) {
        for (uint8_t i=0; i<4; i++) {
            if (z64_file.button_items[i] == item) {
                z64_file.button_items[i]            = swap;
                if (!z64_file.link_age)
                    z64_file.adult_button_items[i]  = swap;
                else z64_file.child_button_items[i] = swap;
                z64_UpdateItemButton(&z64_game, i);
            }
        }
        z64_file.items[slot] = swap;
        play_sfx = 0x4808;
    }
}

void unequip_gear(uint8_t play) {
    z64_game.common.input[0].raw.pad.a = z64_game.common.input[0].pad_pressed.a = 0;
    z64_UpdateEquipment(&z64_game, &z64_link);
    if (play)
        play_sfx = 0x480A;
}

void unequip_sword(uint8_t play) {
    z64_file.equip_sword            = 0;
    if (z64_file.link_age)
        z64_file.child_equip_sword  = 0;
    else z64_file.adult_equip_sword = 0;
    z64_file.sword_flag             = 1;
    z64_file.button_items[0]        = -1;
    unequip_gear(play);
}

void unequip_shield() {
    z64_file.equip_shield            = 0;
    if (z64_file.link_age)
        z64_file.child_equip_shield  = 0;
    else z64_file.adult_equip_shield = 0;
    unequip_gear(1);
}

void unequip_tunic() {
    z64_file.equip_tunic            = 0;
    if (z64_file.link_age)
        z64_file.child_equip_tunic  = 0;
    else z64_file.adult_equip_tunic = 0;
    unequip_gear(1);
}