#ifndef DPAD_ACTIONS_H
#define DPAD_ACTIONS_H

#include "z64.h"
#include "z64_extended.h"

void change_sword(uint8_t sword);
void change_shield(uint8_t shield);
void change_tunic(uint8_t tunic);
void change_boots(uint8_t boots);
void change_arrow(uint8_t button, z64_item_t item);
void change_equipment();

void run_dpad_actions(pad_t pad_pressed);
void draw_dpad_actions(z64_disp_buf_t *db, uint8_t alpha);
uint8_t * check_dpad_actions();
void run_action(uint8_t action);
void draw_action(uint8_t action, z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void check_action(uint8_t button, uint8_t action);
void check_action_item(uint8_t button, uint8_t action, uint8_t dpad, z64_slot_t slot, z64_item_t item);

void toggle_sword();
void toggle_shield();
void toggle_tunic();
void toggle_boots();
void toggle_arrow();
void swap_iron_boots();
void swap_hover_boots();
void use_item(z64_slot_t slot, uint8_t usability);

void draw_sword_icon(      z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void draw_shield_icon(     z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void draw_tunic_icon(      z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void draw_boots_icon(      z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void draw_arrow_icon(      z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void draw_iron_boots_icon( z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void draw_hover_boots_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void draw_child_trade_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void draw_adult_trade_icon(z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void draw_ocarina_icon(    z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, uint8_t alpha);
void draw_item_icon(       z64_disp_buf_t *db, uint16_t icon_x, uint16_t icon_y, z64_slot_t slot, z64_item_t item, uint8_t usability, uint8_t alpha);

#endif