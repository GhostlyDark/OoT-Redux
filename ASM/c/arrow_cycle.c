#include "arrow_cycle.h"
#include "actor.h"

extern uint8_t CFG_OPTIONS_MENU;

extern uint16_t play_sfx;

typedef struct {
    uint16_t     frameDelay;
    int8_t       magicCost;
    z64_actor_t* arrow;
} arrow_cycle_state;

static arrow_cycle_state g_arrow_cycle_state;

typedef struct {
    uint8_t  item;
    uint8_t  slot;
    uint8_t  icon;
    uint8_t  action;
    uint16_t var;
} arrow_info;

static const arrow_info g_arrows[] = {
    { Z64_ITEM_BOW,         Z64_SLOT_BOW,         Z64_ITEM_BOW,             0x8, 0x2, },
    { Z64_ITEM_FIRE_ARROW,  Z64_SLOT_FIRE_ARROW,  Z64_ITEM_BOW_FIRE_ARROW,  0x9, 0x3, },
    { Z64_ITEM_ICE_ARROW,   Z64_SLOT_ICE_ARROW,   Z64_ITEM_BOW_ICE_ARROW,   0xA, 0x4, },
    { Z64_ITEM_LIGHT_ARROW, Z64_SLOT_LIGHT_ARROW, Z64_ITEM_BOW_LIGHT_ARROW, 0xB, 0x5, },
};

static const arrow_info* get_info(uint16_t variable) {
    for (int i=0; i<ARRAY_SIZE(g_arrows); i++) {
        if (g_arrows[i].var == variable)
            return &g_arrows[i];
    }
    return NULL;
}

static uint16_t get_next_arrow_variable(uint16_t variable) {
    switch (variable) {
        case 2:  return 3; // Normal -> Fire
        case 3:  return 4; // Fire   -> Ice
        case 4:  return 5; // Ice    -> Light
        case 5:  return 2; // Light  -> Normal
        default: return variable;
    }
}

static int8_t get_magic_cost_by_info(const arrow_info *info) {
    switch (info->item) {
        case Z64_ITEM_FIRE_ARROW:  return MP_FIRE_ARROW;
        case Z64_ITEM_ICE_ARROW:   return MP_ICE_ARROW;
        case Z64_ITEM_LIGHT_ARROW: return MP_LIGHT_ARROW;
        default:                   return 0;
    }
}

static const arrow_info* get_next_info(uint16_t variable) {
    int8_t   magicCost = get_magic_cost_by_info(get_info(variable));
    uint16_t current   = variable;
    const    arrow_info* info;
    for (int i=0; i<4; i++) {
        current = get_next_arrow_variable(current);
        info    = get_info(current);
        if (info != NULL && info->item == z64_file.items[info->slot] && z64_file.magic >= (get_magic_cost_by_info(info) - magicCost))
            return info;
    }
    return NULL;
}

static uint8_t call_arrow_actor_ctor(z64_actor_t* arrow, z64_game_t* ctxt) {
    z64_actor_ovl_t*  ovl  = &z64_actor_ovl_table[EN_ARROW];
    z64_actor_init_t* init = reloc_resolve_actor_init(ovl);
    if (init != NULL && init->init != NULL) {
        init->destroy(arrow, ctxt);
        init->init(arrow, ctxt);
        return 1;
    }
    return 0;
}

static uint8_t is_arrow_item(uint8_t item) {
    switch (item) {
        case Z64_ITEM_BOW:
        case Z64_ITEM_BOW_FIRE_ARROW:
        case Z64_ITEM_BOW_ICE_ARROW:
        case Z64_ITEM_BOW_LIGHT_ARROW:
            return 1;
        default:
            return 0;
    }
}

static void update_c_button(z64_link_t* player, z64_game_t* ctxt, const arrow_info* info) {
    z64_file.button_items[player->item_button] = info->icon;
    z64_UpdateItemButton(ctxt, player->item_button);
    player->item_action_param      = info->action;
    player->held_item_action_param = info->action;
}

uint8_t actor_helper_does_actor_exist(const z64_actor_t* target, const z64_game_t* ctxt, uint8_t actorCategory) {
    const z64_actor_t* actor = ctxt->actor_list[actorCategory].first;
    while (actor != NULL) {
        if (actor == target)
            return 1;
        actor = actor->next;
    }
    return 0;
}


static void handle_frame_delay(z64_link_t* player, z64_game_t* ctxt, z64_actor_t* arrow) {
    if (!actor_helper_does_actor_exist(arrow, ctxt, ACTORTYPE_ITEMACTION))
        return;

    const arrow_info* curInfo = get_info(arrow->variable);
    if (arrow != NULL && curInfo != NULL) {
        z64_actor_t* special = arrow->child;
        if (special != NULL) {
            z64_RemoveActor(&ctxt->actor_ctxt, special, ctxt);
            arrow->child = NULL;
        }
        
        if (curInfo->item != Z64_ITEM_BOW)
            z64_file.magic_consume_state = 3;
        else z64_file.magic_consume_state = 5;
        z64_file.magic += g_arrow_cycle_state.magicCost;
        z64_file.magic -= get_magic_cost_by_info(curInfo);
    }
}

z64_actor_t* arrow_cycle_find_arrow(z64_link_t* player, z64_game_t* ctxt) {
    z64_actor_t* attached = player->common.child;
    if (attached != NULL && attached->actor_id == EN_ARROW && attached->parent == &player->common)
        return attached;
    else return NULL;
}

void arrow_cycle_handle(z64_link_t* player, z64_game_t* ctxt) {
    if (CFG_OPTIONS_MENU < 1 || !SAVE_ARROW_TOGGLE)
        return;
    
    if (g_arrow_cycle_state.frameDelay >= 1) {
        handle_frame_delay(player, ctxt, g_arrow_cycle_state.arrow);
        g_arrow_cycle_state.arrow      = NULL;
        g_arrow_cycle_state.frameDelay = 0;
        g_arrow_cycle_state.magicCost  = 0;
        return;
    }
    
    if (z64_file.hud_visibility_mode != HUD_VISIBILITY_ALL && z64_file.button_items[0] == Z64_ITEM_BOW)
        return;

    z64_actor_t* arrow = arrow_cycle_find_arrow(player, ctxt);
    if (arrow == NULL)
        return;
    
    if (!(2 <= arrow->variable && arrow->variable < 6))
        return;
    
    if (!is_arrow_item(z64_file.button_items[player->item_button]))
        return;
    
    if (!z64_game.common.input[0].pad_pressed.r)
        return;
    z64_game.common.input[0].raw.pad.r = z64_game.common.input[0].pad_pressed.r = 0;
    
    const arrow_info *curInfo, *nextInfo;
    curInfo  = get_info(arrow->variable);
    nextInfo = get_next_info(arrow->variable);

    if (curInfo == NULL || nextInfo == NULL || curInfo->var == nextInfo->var) {
        if (curInfo->var == 2 && z64_file.button_items[player->item_button] != Z64_ITEM_BOW && z64_file.items[Z64_SLOT_BOW] == Z64_ITEM_BOW)
            update_c_button(player, ctxt, &g_arrows[0]);
        play_sfx = 0x4806;
        return;
    }

    if (curInfo->item == Z64_ITEM_BOW && z64_file.magic < get_magic_cost_by_info(nextInfo)) {
        play_sfx = 0x4806;
        return;
    }
    
    arrow->variable = nextInfo->var;
    call_arrow_actor_ctor(arrow, ctxt);
    z64_actor_t* special = arrow->child;
    if (special != NULL)
        special->draw_proc = NULL;
    
    update_c_button(player, ctxt, nextInfo);

    g_arrow_cycle_state.arrow = arrow;
    g_arrow_cycle_state.frameDelay++;
    g_arrow_cycle_state.magicCost = get_magic_cost_by_info(curInfo);
    
    if (curInfo->item == Z64_ITEM_BOW)
        z64_file.magic_consume_state = 5;
}

struct resolve_info {
    void* ram;
    uint32_t virtStart;
    uint32_t virtEnd;
};

#define create_info(Ram, Start, End) { .ram = (Ram), .virtStart = (uint32_t)(Start), .virtEnd = (uint32_t)(End), }

static void* resolve(struct resolve_info info, uint32_t vram) {
    if (info.ram && info.virtStart <= vram && vram < info.virtEnd) {
        uint32_t offset = vram - info.virtStart;
        return (void*)((char*)info.ram + offset);
    } else {
        return NULL;
    }
}

void* reloc_resolve_actor_overlay(z64_actor_ovl_t* ovl, uint32_t vram) {
    struct resolve_info info = create_info(ovl->loaded_ram_addr, ovl->vram_start, ovl->vram_end);
    return resolve(info, vram);
}

z64_actor_init_t* reloc_resolve_actor_init(z64_actor_ovl_t* ovl) {
    return reloc_resolve_actor_overlay(ovl, (uint32_t)ovl->init_info);
}