#include "gfx.h"
#include "dpad_actions.h"
#include "fps.h"

extern uint8_t CFG_ALLOW_KOKIRI_SWORD;
extern uint8_t CFG_ALLOW_MASTER_SWORD;
extern uint8_t CFG_ALLOW_GIANTS_KNIFE;
extern uint8_t CFG_ALLOW_DEKU_SHIELD;
extern uint8_t CFG_ALLOW_MIRROR_SHIELD;
extern uint8_t CFG_ALLOW_TUNIC;
extern uint8_t CFG_ALLOW_BOOTS;
extern uint8_t CFG_OPTIONS_MENU;

extern uint16_t dpad_x;
extern uint16_t dpad_y;
extern uint16_t play_sfx;
extern uint8_t  compare_frames;
extern uint8_t fps_switch;

static uint8_t DPAD_ACTIVE[4] = {0, 0, 0, 0};

const static int16_t gPositions[4][2] = {
    {  0,  -15 },
    {  16,  0  },
    {  0,   15 },
    { -16,  0  },
};

const static int16_t gPositionsBig[4][2] = {
    { -2,  -17 },
    {  14, -2  },
    { -2,   13 },
    { -18, -2  },
};

const static uint8_t icon_size     = 12;
const static uint8_t icon_size_big = 16;
const static int8_t  dpad_offset   = 2;


void change_sword(uint8_t sword) {
    z64_file.equip_sword            = sword;
    if (z64_file.link_age)
        z64_file.child_equip_sword  = sword;
    else z64_file.adult_equip_sword = sword;
    if (z64_file.equip_sword == 0) {
        z64_file.inf_table[29]   = 1;
        z64_file.button_items[0] = -1;
    }
    else {
        z64_file.inf_table[29] = 0;
        if (z64_file.equip_sword == 3 && z64_file.broken_giants_knife)
            z64_file.button_items[0] = 0x55;
        else z64_file.button_items[0] = z64_file.equip_sword + 0x3A;
    }
    change_equipment();
    z64_UpdateItemButton(&z64_game, 0);
}

void change_shield(uint8_t shield) {
    z64_file.equip_shield            = shield;
    if (z64_file.link_age)
        z64_file.child_equip_shield  = shield;
    else z64_file.adult_equip_shield = shield;
    change_equipment();
}

void change_tunic(uint8_t tunic) {
    z64_file.equip_tunic            = tunic;
    if (z64_file.link_age)
        z64_file.child_equip_tunic  = tunic;
    else z64_file.adult_equip_tunic = tunic;
    change_equipment();
}

void change_boots(uint8_t boots) {
    z64_file.equip_boots            = boots;
    if (z64_file.link_age)
        z64_file.child_equip_boots  = boots;
    else z64_file.adult_equip_boots = boots;
    change_equipment();
}

void change_arrow(uint8_t button, z64_item_t item) {
    z64_file.button_items[button]            = item;
    if (!z64_file.link_age)
        z64_file.adult_button_items[button]  = item;
    else z64_file.child_button_items[button] = item;
    z64_UpdateItemButton(&z64_game, button);
}

void change_equipment() {
    z64_UpdateEquipment(&z64_game, &z64_link);
    play_sfx = 0x835;
}

void run_dpad_actions(pad_t pad_pressed) {
    if (z64_game.common.input[0].raw.pad.l && SAVE_DPAD == 2) {
        if (pad_pressed.du)   toggle_sword();
        if (pad_pressed.dr)   toggle_boots();
        if (pad_pressed.dd)   toggle_shield();
        if (pad_pressed.dl)   toggle_tunic();
        return;
    }
                          
    const uint8_t dpad_array[4] = { pad_pressed.du, pad_pressed.dr, pad_pressed.dd, pad_pressed.dl };                          
    for (uint8_t i=0; i<4; i++)
        if (dpad_array[i])
            run_action(DPAD_SET_BUTTON_INDEX(i));
}

void draw_dpad_actions(z64_disp_buf_t *db, uint8_t alpha) {
    if (z64_game.common.input[0].raw.pad.l && SAVE_DPAD == 2) {
        draw_sword_icon( db, gPositions[0][0], gPositions[0][1], alpha);
        draw_boots_icon( db, gPositions[1][0], gPositions[1][1], alpha);
        draw_shield_icon(db, gPositions[2][0], gPositions[2][1], alpha);
        draw_tunic_icon( db, gPositions[3][0], gPositions[3][1], alpha);
        
        if (OPTION_ACTIVE(1, SAVE_30_FPS, CFG_DEFAULT_30_FPS)) { // FPS Counter Top-Left
            gDPSetPrimColor(db->p++, 0, 0, fps_switch ? 0 : 255, fps_switch ? 255 : 0, 0, 255);
            sprite_load_and_draw(db, &ammo_digit_sprite, compare_frames / 10, dpad_x - 11 - dpad_offset, dpad_y - 8 - dpad_offset, 8, 8);
            sprite_load_and_draw(db, &ammo_digit_sprite, compare_frames % 10, dpad_x - 4  - dpad_offset, dpad_y - 8 - dpad_offset, 8, 8);
        }
        
        return;
    }
                                       
    for (uint8_t i=0; i<4; i++)
        draw_action(db, DPAD_SET_BUTTON_INDEX(i), gPositions[i][0], gPositions[i][1], gPositionsBig[i][0], gPositionsBig[i][1], alpha);
}

uint8_t * check_dpad_actions() {                                
    for (uint8_t i=0; i<4; i++)
        check_action(i, DPAD_SET_BUTTON_INDEX(i));
    
    return DPAD_ACTIVE;
}

void run_action(uint8_t action) {
    if (action == Z64_SLOT_GORON_TUNIC)
        swap_goron_tunic();
    else if (action == Z64_SLOT_ZORA_TUNIC)
        swap_zora_tunic();
    else if (action == Z64_SLOT_IRON_BOOTS)
        swap_iron_boots();
    else if (action == Z64_SLOT_HOVER_BOOTS)
        swap_hover_boots();
    else if (action <= Z64_SLOT_CHILD_TRADE) {
        if (z64_file.items[action] >= Z64_ITEM_WEIRD_EGG && z64_file.items[action] <= Z64_ITEM_SOLD_OUT)
            use_item(action, CAN_USE_CHILD_TRADE);
        else if (z64_file.items[action] >= Z64_ITEM_POCKET_EGG && z64_file.items[action] <= Z64_ITEM_CLAIM_CHECK)
            use_item(action, CAN_USE_ADULT_TRADE);
        else if (z64_file.items[action] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[action] == Z64_ITEM_OCARINA_OF_TIME)
            use_item(action, CAN_USE_OCARINA);
        else if (z64_file.items[action] == Z64_ITEM_LENS)
            use_item(action, CAN_USE_LENS);
        else if (z64_file.items[action] == Z64_ITEM_FARORES_WIND)
            use_item(action, Z64_SLOT_FARORES_WIND);
        else if (z64_file.items[action] == Z64_ITEM_NUT) {
            if (!z64_throwing_nut)
                use_item(Z64_SLOT_NUT, CAN_USE_ITEMS);
        }
        else if (z64_file.items[action] != Z64_ITEM_NULL)
            use_item(action, CAN_USE_ITEMS);
    }
}

void draw_action(z64_disp_buf_t *db, z64_slot_t action, int8_t icon_x, int8_t icon_y, int8_t icon_big_x, int8_t icon_big_y, uint8_t alpha) {
    if (action == Z64_SLOT_GORON_TUNIC)
        draw_equipment_icon(db, icon_x, icon_y, icon_big_x, icon_big_y, alpha, 66, z64_file.goron_tunic, z64_file.equip_tunic == 2);
    else if (action == Z64_SLOT_ZORA_TUNIC)
        draw_equipment_icon(db, icon_x, icon_y, icon_big_x, icon_big_y, alpha, 67, z64_file.zora_tunic,  z64_file.equip_tunic == 3);
    else if (action == Z64_SLOT_IRON_BOOTS)
        draw_equipment_icon(db, icon_x, icon_y, icon_big_x, icon_big_y, alpha, 69, z64_file.iron_boots,  z64_file.equip_boots == 2);
    else if (action == Z64_SLOT_HOVER_BOOTS)
        draw_equipment_icon(db, icon_x, icon_y, icon_big_x, icon_big_y, alpha, 70, z64_file.hover_boots, z64_file.equip_boots == 3);
    else if (action <= Z64_SLOT_CHILD_TRADE) {
        if (z64_file.items[action] >= Z64_ITEM_WEIRD_EGG && z64_file.items[action] <= Z64_ITEM_SOLD_OUT)
            draw_child_trade_icon(db, icon_x, icon_y, icon_big_x, icon_big_y, alpha);
        else if (z64_file.items[action] >= Z64_ITEM_POCKET_EGG && z64_file.items[action] <= Z64_ITEM_CLAIM_CHECK)
            draw_adult_trade_icon(db, icon_x, icon_y, alpha);
        else if (z64_file.items[action] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[action] == Z64_ITEM_OCARINA_OF_TIME)
            draw_ocarina_icon(db, icon_x, icon_y, alpha);
        else if (z64_file.items[action] == Z64_ITEM_LENS)
            draw_item_icon(db, icon_x, icon_y, Z64_SLOT_LENS, Z64_ITEM_LENS, CAN_USE_LENS, alpha);
        else if (z64_file.items[action] == Z64_ITEM_FARORES_WIND)
            draw_item_icon(db, icon_x, icon_y, Z64_SLOT_FARORES_WIND, Z64_ITEM_FARORES_WIND, CAN_USE_FARORES_WIND, alpha);
        else if (z64_file.items[action] != Z64_ITEM_NULL)
            draw_item_icon(db, icon_x, icon_y, action, z64_file.items[action], CAN_USE_ITEMS, alpha);
        draw_ammo(db, z64_file.items[action], dpad_x + icon_x + dpad_offset, dpad_y + icon_y + dpad_offset + 8, 4, -1, alpha);
    }
}

void draw_ammo(z64_disp_buf_t* db, uint8_t item, uint16_t x, uint16_t y, uint8_t size, int8_t spacing, uint8_t alpha) {
    int8_t  ammo     = -1;
    uint8_t capacity = 0;
    
    if (item == z64_file.items[Z64_SLOT_FIRE_ARROW] || item == z64_file.items[Z64_SLOT_ICE_ARROW] || item == z64_file.items[Z64_SLOT_LIGHT_ARROW]) {
        ammo     = z64_file.ammo[Z64_SLOT_BOW];
        capacity = z64_capacity.quiver[z64_file.quiver];
    }
    else if (item == z64_file.items[Z64_SLOT_BOMB]) {
        ammo     = z64_file.ammo[Z64_SLOT_BOMB];
        capacity = z64_capacity.bomb_bag[z64_file.bomb_bag];
    }
    else if (item == z64_file.items[Z64_SLOT_BOMBCHU]) {
        ammo     = z64_file.ammo[Z64_SLOT_BOMBCHU];
        capacity = 50;
    }
    else if (item == z64_file.items[Z64_SLOT_STICK]) {
        ammo     = z64_file.ammo[Z64_SLOT_STICK];
        capacity = z64_capacity.stick_upgrade[z64_file.stick_upgrade];
    }
    else if (item == z64_file.items[Z64_SLOT_NUT]) {
        ammo     = z64_file.ammo[Z64_SLOT_NUT];
        capacity = z64_capacity.nut_upgrade[z64_file.nut_upgrade];
    }
    else if (item == z64_file.items[Z64_SLOT_BEANS]) {
        ammo     = z64_file.ammo[Z64_SLOT_BEANS];
        capacity = 10;
    }
        
    if (ammo >= 0) {
        if (ammo == capacity)
            gDPSetPrimColor(db->p++, 0, 0, 120, 255, 0, alpha);
        else if (ammo == 0)
            gDPSetPrimColor(db->p++, 0, 0, 100, 100, 100, alpha);
        else gDPSetPrimColor(db->p++, 0, 0, 255, 255, 255, alpha);
        
        if (ammo >= 10)
            sprite_load_and_draw(db, &ammo_digit_sprite, ammo / 10, x, y, size, size);
        sprite_load_and_draw(db, &ammo_digit_sprite, ammo % 10, x + size + spacing, y, size, size);
    }
}

void check_action(uint8_t button, z64_slot_t action) {
    if (action == Z64_SLOT_IRON_BOOTS && z64_file.iron_boots)
        DPAD_ACTIVE[button] = 1;
    else if (action == Z64_SLOT_HOVER_BOOTS && z64_file.hover_boots)
        DPAD_ACTIVE[button] = 1;
    else if (action == Z64_SLOT_NULL || action > Z64_SLOT_CHILD_TRADE)
        DPAD_ACTIVE[button] = 0;
    else if (z64_file.items[action] != Z64_ITEM_NULL)
        DPAD_ACTIVE[button] = 1;
    else DPAD_ACTIVE[button] = 0;
}

void toggle_sword() {
    if (!z64_file.kokiri_sword && !z64_file.master_sword && !z64_file.giants_knife)
        return;
    
    uint8_t sword = z64_file.equip_sword;
    sword++;
    
    if (sword > 3) {
        sword = 0;
        if (!OPTION_ACTIVE(2, SAVE_UNEQUIP_GEAR, CFG_DEFAULT_UNEQUIP_GEAR))
            sword++;
    }
    
    if (sword == 1 && (!z64_file.kokiri_sword || (!z64_file.link_age && !CFG_ALLOW_KOKIRI_SWORD) ) )
        sword++;
    if (sword == 2 && (!z64_file.master_sword || ( z64_file.link_age && !CFG_ALLOW_MASTER_SWORD) ) )
        sword++;
    if (sword == 3 && (!z64_file.giants_knife || ( z64_file.link_age && !CFG_ALLOW_GIANTS_KNIFE) ) ) {
        if (OPTION_ACTIVE(2, SAVE_UNEQUIP_GEAR, CFG_DEFAULT_UNEQUIP_GEAR))
            sword = 0;
        else if (z64_file.kokiri_sword && ( z64_file.link_age || CFG_ALLOW_KOKIRI_SWORD) )
            sword = 1;
        else if (z64_file.master_sword && (!z64_file.link_age || CFG_ALLOW_MASTER_SWORD) )
            sword = 2;
        else sword = 0;
    }
            
    if (sword != z64_file.equip_sword)
        change_sword(sword);
}

void toggle_shield() {
    if (!z64_file.deku_shield && !z64_file.hylian_shield && !z64_file.mirror_shield)
        return;
    
    uint8_t shield = z64_file.equip_shield;
    shield++;
    
    if (shield > 3) {
        shield = 0;
        if (!OPTION_ACTIVE(2, SAVE_UNEQUIP_GEAR, CFG_DEFAULT_UNEQUIP_GEAR))
            shield++;
    }
    
    if (shield == 1 && (!z64_file.deku_shield || (!z64_file.link_age && !CFG_ALLOW_DEKU_SHIELD) ) )
        shield++;
    if (shield == 2 && !z64_file.hylian_shield)
        shield++;
    if (shield == 3 && (!z64_file.mirror_shield || (z64_file.link_age && !CFG_ALLOW_MIRROR_SHIELD) ) ) {
        if (OPTION_ACTIVE(2, SAVE_UNEQUIP_GEAR, CFG_DEFAULT_UNEQUIP_GEAR))
            shield = 0;
        else if (z64_file.deku_shield && (z64_file.link_age || CFG_ALLOW_DEKU_SHIELD) )
            shield = 1;
        else if (z64_file.hylian_shield)
            shield = 2;
        else shield = 0;
    }
    
    if (shield != z64_file.equip_shield)
        change_shield(shield);
}

void toggle_tunic() {
    if (!z64_file.kokiri_tunic && !z64_file.goron_tunic && !z64_file.zora_tunic)
        return;
    
    uint8_t not_allowed = z64_file.link_age && !CFG_ALLOW_TUNIC;
    uint8_t can_unequip = OPTION_ACTIVE(2, SAVE_UNEQUIP_GEAR, CFG_DEFAULT_UNEQUIP_GEAR) || OPTION_ACTIVE(2, SAVE_EXTRA_ABILITIES, CFG_DEFAULT_EXTRA_ABILITIES);
    uint8_t tunic       = z64_file.equip_tunic;
    tunic++;
    
    if (tunic > 3) {
        tunic = 0;
        if (!can_unequip)
            tunic++;
    }
    
    if (tunic == 1 && !z64_file.kokiri_tunic)
        tunic++;
    if (tunic == 2 && (!z64_file.goron_tunic || not_allowed) )
        tunic++;
    if (tunic == 3 && (!z64_file.zora_tunic  || not_allowed) ) {
        if (can_unequip)
            tunic = 0;
        else if (z64_file.kokiri_tunic)
            tunic = 1;
        else if (z64_file.goron_tunic && (!z64_file.link_age || !CFG_ALLOW_TUNIC) )
            tunic = 2;
        else tunic = 0;
    }
    
    if (tunic != z64_file.equip_tunic)
        change_tunic(tunic);
}

void toggle_boots() {
    if (z64_file.link_age && !CFG_ALLOW_BOOTS)
        return;
    if (!z64_file.kokiri_boots || (!z64_file.iron_boots && !z64_file.hover_boots) )
        return;
    
    uint8_t boots = z64_file.equip_boots;
    boots++;
    if (boots > 3)
        boots = 1;
    
    if (boots == 2 && !z64_file.iron_boots)
        boots++;
    if (boots == 3 && !z64_file.hover_boots)
        boots = 1;
    
    if (boots != z64_file.equip_boots)
        change_boots(boots);
}

void swap_goron_tunic() {
    if (!z64_file.goron_tunic || (z64_file.link_age && !CFG_ALLOW_BOOTS) )
        return;
    z64_file.equip_tunic = z64_file.equip_tunic == 2 ? 1 : 2;
    change_equipment();
}

void swap_zora_tunic() {
    if (!z64_file.zora_tunic || (z64_file.link_age && !CFG_ALLOW_BOOTS) )
        return;
    z64_file.equip_tunic = z64_file.equip_tunic == 3 ? 1 : 3;
    change_equipment();
}

void swap_iron_boots() {
    if (!z64_file.iron_boots || (z64_file.link_age && !CFG_ALLOW_BOOTS) )
        return;
    z64_file.equip_boots = z64_file.equip_boots == 2 ? 1 : 2;
    change_equipment();
}

void swap_hover_boots() {
    if (!z64_file.hover_boots || (z64_file.link_age && !CFG_ALLOW_BOOTS) )
        return;
    z64_file.equip_boots = z64_file.equip_boots == 3 ? 1 : 3;
    change_equipment();
}

void use_item(z64_slot_t slot, uint8_t usability) {
    if (usability)
        z64_usebutton(&z64_game, &z64_link, z64_file.items[slot], 2);
}

void draw_sword_icon(z64_disp_buf_t *db, int8_t icon_x, int8_t icon_y, uint8_t alpha) {
    if (z64_file.equip_sword == 0)
        return;
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
    draw_dpad_icon(db, &items_sprite, z64_file.equip_sword == 3 && z64_file.broken_giants_knife ? 85 : 58 + z64_file.equip_sword, icon_x, icon_y, icon_size);
}

void draw_shield_icon(z64_disp_buf_t *db, int8_t icon_x, int8_t icon_y, uint8_t alpha) {
    if (z64_file.equip_shield == 0)
        return;
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
    draw_dpad_icon(db, &items_sprite, 61 + z64_file.equip_shield, icon_x, icon_y, icon_size);
}

void draw_tunic_icon(z64_disp_buf_t *db, int8_t icon_x, int8_t icon_y, uint8_t alpha) {
    if (z64_file.equip_tunic == 0)
        return;
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
    draw_dpad_icon(db, &items_sprite, 64 + z64_file.equip_tunic, icon_x, icon_y, icon_size);
}


void draw_boots_icon(z64_disp_buf_t *db, int8_t icon_x, int8_t icon_y, uint8_t alpha) {
    if (z64_file.equip_boots == 0)
        return;
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
    draw_dpad_icon(db, &items_sprite, 67 + z64_file.equip_boots, icon_x, icon_y, icon_size);
}

void draw_equipment_icon(z64_disp_buf_t *db, int8_t icon_x, int8_t icon_y, int8_t icon_big_x, int8_t icon_big_y, uint8_t alpha, uint8_t icon, uint8_t has_item, uint8_t is_equipped) {
    if (has_item) {
        gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, alpha);
        draw_dpad_icon(db, &items_sprite, icon, is_equipped ? icon_big_x : icon_x, is_equipped ? icon_big_y : icon_y, is_equipped ? icon_size_big : icon_size);
    }
}

void draw_child_trade_icon(z64_disp_buf_t *db, int8_t icon_x, int8_t icon_y, int8_t icon_big_x, int8_t icon_big_y, uint8_t alpha) {
    if (z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH) {
        gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, (!CAN_USE_CHILD_TRADE && IS_SEMI_ALPHA) ? 0x46 : alpha);
        draw_dpad_icon(db, &items_sprite, z64_file.items[Z64_SLOT_CHILD_TRADE], z64_link.current_mask > 0 ? icon_big_x : icon_x, z64_link.current_mask > 0 ?  icon_big_y : icon_y, z64_link.current_mask > 0 ? icon_size_big : icon_size);
    }
}

void draw_adult_trade_icon(z64_disp_buf_t *db, int8_t icon_x, int8_t icon_y, uint8_t alpha) {
    if (z64_file.items[Z64_SLOT_ADULT_TRADE] >= Z64_ITEM_POCKET_EGG && z64_file.items[Z64_SLOT_ADULT_TRADE] <= Z64_ITEM_CLAIM_CHECK) {
        gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, (!CAN_USE_ADULT_TRADE && IS_SEMI_ALPHA) ? 0x46 : alpha);
        draw_dpad_icon(db, &items_sprite, z64_file.items[Z64_SLOT_ADULT_TRADE], icon_x, icon_y, icon_size);
    }
}

void draw_ocarina_icon(z64_disp_buf_t *db, int8_t icon_x, int8_t icon_y, uint8_t alpha) {
    if (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) {
        gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, (!CAN_USE_OCARINA && IS_SEMI_ALPHA) ? 0x46 : alpha);
        draw_dpad_icon(db, &items_sprite, z64_file.items[Z64_SLOT_OCARINA], icon_x, icon_y, icon_size);
    }
}

void draw_item_icon(z64_disp_buf_t *db, int8_t icon_x, int8_t icon_y, z64_slot_t slot, z64_item_t item, uint8_t usability, uint8_t alpha) {
    if (z64_file.items[slot] == item) {
        gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, (!usability && IS_SEMI_ALPHA) ? 0x46 : alpha);
        draw_dpad_icon(db, &items_sprite, z64_file.items[slot], icon_x, icon_y, icon_size);
    }
}

void draw_dpad_icon(z64_disp_buf_t *db, sprite_t *sprite, uint8_t icon, int8_t x, int8_t y, uint8_t size) {
    sprite_load_and_draw(db, sprite, icon, dpad_x + x + dpad_offset, dpad_y + y + dpad_offset, size, size);
}