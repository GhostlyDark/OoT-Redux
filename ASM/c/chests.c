#include "chests.h"
#include "n64.h"
#include "gfx.h"

#define BROWN_FRONT_TEXTURE 0x06001798
#define BROWN_BASE_TEXTURE 0x06002798
#define GOLD_FRONT_TEXTURE 0x06002F98
#define GOLD_BASE_TEXTURE 0x06003798

#define CHEST_BASE 1
#define CHEST_LID 3

#define LARGE_CHEST 0
#define LARGE_CHEST_APPEARS_CLEAR 1
#define BOSS_CHEST 2
#define LARGE_CHEST_FALLING_SWITCH 3
#define LARGE_CHEST_INVISIBLE 4
#define SMALL_CHEST 5
#define SMALL_CHEST_INVISIBLE 6
#define SMALL_CHEST_APPEARS_CLEAR 7
#define SMALL_CLEAR_FALLING_SWITCH 8
#define LARGE_CHEST_ZELDAS_LULLABY 9
#define LARGE_CHEST_SUNS_SONG 10
#define LARGE_CHEST_APPEARS_SWITCH 11

//#define box_obj_idx(actor) ((int8_t *)actor)[0x015A] // forest_hallway_actor->box_obj_idx
//#define OBJECT_BOX 0x000E

#define IS_LARGE_CHEST (chest_type == LARGE_CHEST || chest_type == LARGE_CHEST_APPEARS_CLEAR || chest_type == LARGE_CHEST_FALLING_SWITCH || chest_type == LARGE_CHEST_INVISIBLE || chest_type == LARGE_CHEST_ZELDAS_LULLABY || chest_type == LARGE_CHEST_SUNS_SONG || chest_type == LARGE_CHEST_APPEARS_SWITCH)

/*extern uint8_t SHUFFLE_CHEST_GAME
uint32_t CHEST_TEXTURE_MATCH_CONTENTS = 0;
uint32_t CHEST_SIZE_MATCH_CONTENTS = 0;
uint32_t CHEST_SIZE_TEXTURE = 0;*/

extern void* LEATHER_CHEST_FRONT_TEXTURE;
extern void* LEATHER_CHEST_BASE_TEXTURE;
extern void* LEATHER_SYEO_CHEST_FRONT_TEXTURE;
extern void* LEATHER_SYEO_CHEST_BASE_TEXTURE;
extern void* KEY_CHEST_FRONT_TEXTURE;
extern void* KEY_CHEST_BASE_TEXTURE;

extern Mtx_t* write_matrix_stack_top(z64_gfx_t* gfx);
asm(".equ write_matrix_stack_top, 0x800AB900");

/*void get_chest_override(z64_actor_t *actor) {
    Chest *chest = (Chest *)actor;
    uint8_t size = chest->en_box.type;
    uint8_t color = size;
    
    if (CHEST_SIZE_MATCH_CONTENTS || CHEST_SIZE_TEXTURE || CHEST_TEXTURE_MATCH_CONTENTS) {
        uint8_t scene = z64_game.scene_index;
        uint8_t item_id = (actor->variable & 0x0FE0) >> 5;

        override_t override = lookup_override(actor, scene, item_id);
        if (override.value.base.item_id != 0) {
            item_row_t *item_row = get_item_row(override.value.looks_like_item_id);
            if (item_row == NULL)
                item_row = get_item_row(override.value.base.item_id);
            if (CHEST_SIZE_MATCH_CONTENTS || CHEST_SIZE_TEXTURE) {
                if (item_row->chest_type == BROWN_CHEST || item_row->chest_type == SILVER_CHEST || item_row->chest_type == SKULL_CHEST_SMALL || item_row->chest_type == HEART_CHEST_SMALL)
                    size = (scene == 0x10 && actor->variable != 0x4ECA && !SHUFFLE_CHEST_GAME) ? BROWN_CHEST : SMALL_CHEST; // Ensure vanilla chest size in Chest Game when not shuffled
                else size = BROWN_CHEST; // These chest_types are big by default
            }
            color = item_row->chest_type;
        }
    }

    chest->size = size;
    chest->color = color;
    if (CHEST_LENS_ONLY)
        actor->flags |= 0x80; //Actor flag 7 makes actors invisible, usually only applies to chest types 4 and 6
}*/

uint8_t get_chest_type(z64_actor_t *actor) {
    //uint8_t chest_type = ((Chest *)actor)->color;
    //if (CHEST_SIZE_MATCH_CONTENTS && chest_type == SILVER_CHEST)
    //    chest_type = GOLD_CHEST;
    
    uint8_t chest_type = ((EnBox *)actor)->type;
    return chest_type;
}

void set_chest_texture(z64_gfx_t *gfx, uint8_t chest_type, uint8_t chest_item, Gfx **opa_ptr) {
    //set texture type
    void* frontTexture = (void*)BROWN_FRONT_TEXTURE;
    void* baseTexture = (void*)BROWN_BASE_TEXTURE;
    uint8_t id        = chest_item;

    if (OPTION_ACTIVE(1, SAVE_CHEST_CONTENTS, CFG_DEFAULT_CHEST_CONTENTS)) {
        if (IS_LARGE_CHEST) {
            if (id == CHEST_DUNGEON_MAP || id == CHEST_COMPASS) {
                frontTexture = &LEATHER_CHEST_FRONT_TEXTURE;
                baseTexture = &LEATHER_CHEST_BASE_TEXTURE;
            }
            else if ( (id >= CHEST_FAIRY_BOW && id <= CHEST_BOOMERANG) || (id >= CHEST_HOOKSHOT && id <= CHEST_LENS_OF_TRUTH) || (id >= CHEST_IRON_BOOTS && id <= CHEST_GOLDEN_SCALE) || (id >= CHEST_FIRE_ARROW && id <= CHEST_LIGHT_ARROW) || (id >= CHEST_DINS_FIRE && id <= CHEST_NAYRUS_LOVE)
                || id == CHEST_MEGATON_HAMMER || id == CHEST_FAIRY_OCARINA || id == CHEST_MIRROR_SHIELD ) {
                frontTexture = &LEATHER_SYEO_CHEST_FRONT_TEXTURE;
                baseTexture = &LEATHER_SYEO_CHEST_BASE_TEXTURE;
            }
        }
        else {
            if (id == CHEST_SMALL_KEY) { 
                frontTexture = &KEY_CHEST_FRONT_TEXTURE;
                baseTexture = &KEY_CHEST_BASE_TEXTURE;
            }
        }
    }

    /*if (CHEST_SIZE_TEXTURE || CHEST_TEXTURE_MATCH_CONTENTS) {
        switch (chest_type) {
            case GILDED_CHEST:
                frontTexture = get_texture(TEXTURE_ID_CHEST_FRONT_GILDED);
                baseTexture = get_texture(TEXTURE_ID_CHEST_BASE_GILDED);
                break;

            case SILVER_CHEST:
                frontTexture = get_texture(TEXTURE_ID_CHEST_FRONT_SILVER);
                baseTexture = get_texture(TEXTURE_ID_CHEST_BASE_SILVER);
                break;

            case SKULL_CHEST_SMALL:
            case SKULL_CHEST_BIG:
                frontTexture = get_texture(TEXTURE_ID_CHEST_FRONT_SKULL);
                baseTexture = get_texture(TEXTURE_ID_CHEST_BASE_SKULL);
                break;

            case HEART_CHEST_SMALL:
            case HEART_CHEST_BIG:
                frontTexture = get_texture(TEXTURE_ID_CHEST_FRONT_HEART);
                baseTexture = get_texture(TEXTURE_ID_CHEST_BASE_HEART);
                break;

            default:
                break;
        }
    }*/

    //the brown chest's base and lid dlist has been modified to jump to
    //segment 09 in order to dynamically set the chest front and base textures
    gfx->poly_opa.d -= 4;
    gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, frontTexture);
    gSPEndDisplayList(gfx->poly_opa.d + 1);
    gDPSetTextureImage(gfx->poly_opa.d + 2, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, baseTexture);
    gSPEndDisplayList(gfx->poly_opa.d + 3);

    //push custom dlist (that sets the texture) to segment 09
    gSPSegment((*opa_ptr)++, 0x09, gfx->poly_opa.d);
}

void draw_chest_base(z64_game_t *game, z64_actor_t *actor, Gfx **opa_ptr) {
    z64_gfx_t *gfx = game->common.gfx;
    uint8_t chest_type = get_chest_type(actor);
    //int chest_type = ((uint8_t*)actor)[0x01ED];
    uint8_t chest_item = (actor->variable & 0x0FE0) >> 5;
    gSPMatrix((*opa_ptr)++, write_matrix_stack_top(gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (chest_type != BOSS_CHEST) {
        set_chest_texture(gfx, chest_type, chest_item, opa_ptr);
        gSPDisplayList((*opa_ptr)++, 0x060006F0);
    }
    else gSPDisplayList((*opa_ptr)++, 0x06000AE8);
}

void draw_chest_lid(z64_game_t *game, z64_actor_t *actor, Gfx **opa_ptr) {
    z64_gfx_t *gfx = game->common.gfx;
    uint8_t chest_type = get_chest_type(actor);
    //int chest_type = ((uint8_t*)actor)[0x01ED];
    uint8_t chest_item = (actor->variable & 0x0FE0) >> 5;
    gSPMatrix((*opa_ptr)++, write_matrix_stack_top(gfx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (chest_type != BOSS_CHEST) {
        set_chest_texture(gfx, chest_type, chest_item, opa_ptr);
        gSPDisplayList((*opa_ptr)++, 0x060010C0);
    }
    else gSPDisplayList((*opa_ptr)++, 0x06001678);
}

void draw_chest(z64_game_t *game, int32_t part, void *unk, void *unk2, z64_actor_t *actor, Gfx **opa_ptr) {
    if (part == CHEST_BASE)
        draw_chest_base(game, actor, opa_ptr);
    else if (part == CHEST_LID) draw_chest_lid(game, actor, opa_ptr);
}

/*void draw_chest(z64_game_t* game, int part, void* unk, void* unk2, z64_actor_t *actor, Gfx **opa_ptr) {
    if (part != CHEST_BASE && part != CHEST_LID)
        return;

    z64_gfx_t *gfx = game->common.gfx;
    int chest_type = ((uint8_t*)actor)[0x01ED];

    //write matrix
    Mtx_t *mtx = write_matrix_stack_top(gfx);
    gSPMatrix((*opa_ptr)++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    
    int dlist;
    
    if (part == CHEST_BASE) {
        if (chest_type == BOSS_CHEST)
            dlist = 0x06000AE8;
        else dlist = 0x060006F0;

    }
    else { // (part == CHEST_LID)
        if (chest_type == BOSS_CHEST)
            dlist = 0x06001678;
        else dlist = 0x060010C0;
    }
    
    if (chest_type != BOSS_CHEST) {
        //set texture type
        void* frontTexture = (void*)BROWN_FRONT_TEXTURE;
        void* baseTexture = (void*)BROWN_BASE_TEXTURE;
        uint8_t id        = (actor->variable & 0x0FE0) >> 5;
        
        if (SAVE_CHEST_CONTENTS) {
            if (id == CHEST_DUNGEON_MAP || id == CHEST_COMPASS) {
                frontTexture = &LEATHER_CHEST_FRONT_TEXTURE;
                baseTexture = &LEATHER_CHEST_BASE_TEXTURE;
            }
            else if ( (id >= CHEST_FAIRY_BOW && id <= CHEST_BOOMERANG) || (id >= CHEST_HOOKSHOT && id <= CHEST_LENS_OF_TRUTH) || (id >= CHEST_IRON_BOOTS && id <= CHEST_GOLDEN_SCALE) || (id >= CHEST_FIRE_ARROW && id <= CHEST_LIGHT_ARROW) || (id >= CHEST_DINS_FIRE && id <= CHEST_NAYRUS_LOVE)
                || id == CHEST_MEGATON_HAMMER || id == CHEST_FAIRY_OCARINA || id == CHEST_MIRROR_SHIELD ) {
                if (IS_LARGE_CHEST) {
                    frontTexture = &LEATHER_SYEO_CHEST_FRONT_TEXTURE;
                    baseTexture = &LEATHER_SYEO_CHEST_BASE_TEXTURE;
                }
            }
            else if (id == CHEST_SMALL_KEY) { 
                frontTexture = &KEY_CHEST_FRONT_TEXTURE;
                baseTexture = &KEY_CHEST_BASE_TEXTURE;
            }
        }
        
        // The brown chest's base and lid dlist has been modified to jump to
        // segment 09 in order to dynamically set the chest front and base textures
        gfx->poly_opa.d -= 4;
        gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, frontTexture);
        gSPEndDisplayList(gfx->poly_opa.d + 1);
        gDPSetTextureImage(gfx->poly_opa.d + 2, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, baseTexture);
        gSPEndDisplayList(gfx->poly_opa.d + 3);
        
        gMoveWd((*opa_ptr)++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);
    }
    gSPDisplayList((*opa_ptr)++, dlist);
}*/