#ifndef FPS_H
#define FPS_H

#include "z64.h"
#include "z64_extended.h"

void handle_fps();
void reset_fps_values();

#define bottle_action                   (*(uint32_t*)           0x801C86B2)
#define link_animation_parameter        (*(uint32_t*)           0x801DABF0)
#define jump_gravity                    (*(uint16_t*)           0x801DAA9C)
#define link_animation                  (*(uint16_t*)           0x801DABDE)
#define deku_stick_timer                (*(uint16_t*)           0x801DB280)
#define hover_boots_length              (*(uint16_t*)           0x8039E612)
#define time_of_day_speed               (*(uint16_t*)           0x800F1650)
#define time_remaining                  (*(uint16_t*)           0x8011B9A0)
#define boomerang_active                (*(uint16_t*)           0x800E8B8E)
#define hookshot_active                 (*(uint16_t*)           0x801DAA50)
#define timer_type                      (*(uint8_t*)            0x8011B99F)
#define seconds_left                    (*(uint8_t*)            0x8011BF31)
#define playing_ocarina                 (*(uint8_t*)            0x80102208)
#define talking_to_npc                  (*(uint8_t*)            0x801DAA36)
#define fishing                         (*(uint8_t*)            0x801C8C41)
#define frogs                           (*(uint8_t*)            0x801C8835)
#define lens_of_truth_start             (*(uint8_t*)            0x80072747)
#define lens_of_truth_interval          (*(uint8_t*)            0x80072E0B) // 0x801D8BC1

#define timer1_1                        (*(uint16_t*)           0x800772FE)
#define timer1_2                        (*(uint16_t*)           0x80077366)
#define timer1_3                        (*(uint16_t*)           0x800775D2)
#define timer1_4                        (*(uint16_t*)           0x800778CE)
#define timer2_1                        (*(uint16_t*)           0x800772EA)
#define timer2_2                        (*(uint16_t*)           0x80077342)
#define timer2_3                        (*(uint16_t*)           0x8007735A)
#define timer2_4                        (*(uint16_t*)           0x800773C2)
#define timer2_5                        (*(uint16_t*)           0x800774FE)
#define timer2_6                        (*(uint16_t*)           0x8007782E)
#define timer2_7                        (*(uint16_t*)           0x800778FE)
#define timer3_1                        (*(uint16_t*)           0x80077956)
#define timer3_2                        (*(uint16_t*)           0x80077A0A)
#define timer3_3                        (*(uint16_t*)           0x80077C4A)
#define timer4_1                        (*(uint16_t*)           0x800770AE)
#define timer4_2                        (*(uint16_t*)           0x80077952)
#define timer4_3                        (*(uint16_t*)           0x800779F6)
#define timer4_4                        (*(uint16_t*)           0x80077A5E)
#define timer4_5                        (*(uint16_t*)           0x80077B86)
#define timer4_6                        (*(uint16_t*)           0x80077C8A)
#define timer4_7                        (*(uint16_t*)           0x80077CF2)

#endif
