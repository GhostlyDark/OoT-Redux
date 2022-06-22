#ifndef DPAD_ACTIONS_H
#define DPAD_ACTIONS_H

#include "z64.h"

#define BLOCK_ITEMS (0x00800000 | \
                     0x00000800 | \
                     0x00200000 | \
                     0x08000000)

#define CAN_USE_OCARINA     (z64_game.pause_ctxt.state == 0 && (z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_FAIRY_OCARINA || z64_file.items[Z64_SLOT_OCARINA] == Z64_ITEM_OCARINA_OF_TIME) && !z64_game.restriction_flags.ocarina && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_CHILD_TRADE (z64_game.pause_ctxt.state == 0 && z64_file.items[Z64_SLOT_CHILD_TRADE] >= Z64_ITEM_WEIRD_EGG && z64_file.items[Z64_SLOT_CHILD_TRADE] <= Z64_ITEM_MASK_OF_TRUTH && !z64_game.restriction_flags.trade_items && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))
#define CAN_USE_ITEMS       (z64_game.pause_ctxt.state == 0 && !z64_game.restriction_flags.all && ((z64_link.state_flags_1 & BLOCK_ITEMS) == 0))

#endif

void change_sword(uint8_t sword);
void change_boots(uint8_t boots);
void change_shield(uint8_t shield);
void change_tunic(uint8_t tunic);
void change_arrow(uint8_t button, z64_item_t item, uint16_t sfx);
void change_equipment();

void action_dpad_up();
void action_dpad_right();
void action_dpad_down();
void action_dpad_left();
void draw_dpad_up(z64_disp_buf_t *db, uint16_t alpha);
void draw_dpad_right(z64_disp_buf_t *db, uint16_t alpha);
void draw_dpad_down(z64_disp_buf_t *db, uint16_t alpha);
void draw_dpad_left(z64_disp_buf_t *db, uint16_t alpha);
void run_action(uint8_t action);
void draw_action(uint8_t action, z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);

void toggle_sword();
void toggle_boots();
void toggle_shield();
void toggle_tunic();
void toggle_arrow();
void swap_iron_boots();
void swap_hover_boots();
void use_child_trade();
void use_ocarina();

void draw_sword_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_boots_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_shield_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_tunic_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_arrow_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_iron_boots_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_hover_boots_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_child_trade_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);
void draw_ocarina_icon(z64_disp_buf_t *db, uint16_t alpha, uint16_t icon_x, uint16_t icon_y);