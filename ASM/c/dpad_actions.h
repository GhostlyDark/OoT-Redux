#ifndef DPAD_ACTIONS_H
#define DPAD_ACTIONS_H

#include "z64.h"
#include "z64_extended.h"

#define BLOCK_ITEMS (0x00800000 | \
                     0x00000800 | \
                     0x00200000 | \
                     0x08000000)

#define CAN_USE_OCARINA     (z64_game.pause_ctxt.state == 0 && (z64_file.items[Z64_SLOT_OCARINA]     == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA]     == Z64_ITEM_OCARINA_OF_TIME) && !z64_game.restriction_flags.ocarina     && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_CHILD_TRADE (z64_game.pause_ctxt.state == 0 && (z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG     && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH)   && !z64_game.restriction_flags.trade_items && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_ADULT_TRADE (z64_game.pause_ctxt.state == 0 && (z64_file.items[Z64_SLOT_ADULT_TRADE] >= Z64_ITEM_POCKET_EGG    && z64_file.items[Z64_SLOT_ADULT_TRADE] <= Z64_ITEM_CLAIM_CHECK)     && !z64_game.restriction_flags.trade_items && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_ITEMS       (z64_game.pause_ctxt.state == 0 && !z64_game.restriction_flags.all && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_LENS        (CAN_USE_ITEMS)

#endif

void change_sword(uint8_t sword);
void change_shield(uint8_t shield);
void change_tunic(uint8_t tunic);
void change_boots(uint8_t boots);
void change_arrow(uint8_t button, z64_item_t item, uint16_t sfx);
void change_equipment();

void run_dpad_actions(pad_t pad_pressed);
void draw_dpad_actions(z64_disp_buf_t *db, uint16_t alpha);
uint8_t * check_dpad_actions();
void run_action(uint8_t action);
void draw_action(uint8_t action, z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void check_action(uint8_t button, uint8_t action);

void toggle_sword();
void toggle_shield();
void toggle_tunic();
void toggle_boots();
void toggle_arrow();
void swap_iron_boots();
void swap_hover_boots();
void use_child_trade();
void use_adult_trade();
void use_ocarina();
void use_lens();
void use_item(z64_slot_t slot);

void draw_sword_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_shield_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_tunic_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_boots_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_arrow_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_iron_boots_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_hover_boots_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_child_trade_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_adult_trade_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_ocarina_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_lens_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_item_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y, z64_slot_t slot, z64_item_t item);