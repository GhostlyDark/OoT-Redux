#ifndef ARROW_CYCLE_H
#define ARROW_CYCLE_H

#include "z64.h"
#include "z64_extended.h"

uint8_t actor_helper_does_actor_exist(const z64_actor_t* target, const z64_game_t* ctxt, uint8_t actorCategory);
z64_actor_t* arrow_cycle_find_arrow(z64_link_t* player, z64_game_t* ctxt);
void arrow_cycle_handle(z64_link_t* player, z64_game_t* ctxt);
void* reloc_resolve_actor_overlay(z64_actor_ovl_t* ovl, uint32_t vram);
z64_actor_init_t* reloc_resolve_actor_init(z64_actor_ovl_t* ovl);


#endif
